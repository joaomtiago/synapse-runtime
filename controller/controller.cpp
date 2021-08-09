#include "synapse/runtime/connector.hpp"
#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"
#include "synapse/runtime/wrapper/p4runtime/stream/handler/environment.hpp"
#include "synapse/runtime/wrapper/utils/wrappers.hpp"

#include <string>

#define PRIORITY 1
#define IDLE_TIMEOUT_NS 5000000000

#define TAG(tag_name, tag_value)                                               \
  new pair_t(new string_t(tag_name), new p4_uint32_t(tag_value))
#define DETAG(tag) (new p4_uint32_t(tag))->raw->toStdString()
#define NOT_NULL(exp) assert(nullptr != (exp))

namespace synapse::runtime {

uint32_t globalTag = 0;
uint32_t macSrcExactTag = globalTag;
uint32_t macDstExactTag = globalTag;

using std::pair;
using std::string;
using std::vector;

bool insertTableEntry(env_ptr_t env, const string &tableName,
                      vector<pair<string, void *>> *matchFields,
                      const string &actionName,
                      vector<pair<string, string>> *actionParams,
                      int32_t priority, uint64_t idleTimeoutNs) {
  auto helper = env->helper;

  auto tableInfo = helper->p4InfoTable(tableName);
  auto tableId = tableInfo->mutable_preamble()->id();

  auto actionInfo = helper->p4InfoAction(actionName);
  auto actionId = actionInfo->mutable_preamble()->id();

  auto match = new vector<p4_field_match_ptr_t>();
  for (auto it = matchFields->begin(); it != matchFields->end(); ++it) {
    auto fieldInfo = helper->p4InfoMatchField(tableInfo, it->first);
    auto fieldId = fieldInfo->id();

    if (fieldInfo->has_match_type()) {
      p4_field_match_ptr_t fieldMatch;

      switch (fieldInfo->match_type()) {
      case p4_info_match_field_match_type_t::MatchField_MatchType_EXACT: {
        auto value = *S_CAST(string *, it->second);
        fieldMatch =
            helper->p4FieldMatch(fieldId, helper->p4FieldMatchExact(value));

      } break;

      case p4_info_match_field_match_type_t::MatchField_MatchType_RANGE: {
        auto value = *static_cast<pair<string, string> *>(it->second);
        fieldMatch = helper->p4FieldMatch(
            fieldId, helper->p4FieldMatchRange(value.first, value.second));

      } break;

      default:
        SYNAPSE_DEBUG("Unsupported match type");
        return false;
      }

      match->push_back(fieldMatch);

    } else {
      SYNAPSE_ERROR("Expected information about a match type");
      return false;
    }
  }

  auto params = new vector<p4_action_param_ptr_t>();
  for (auto it = actionParams->begin(); it != actionParams->end(); ++it) {
    auto paramInfo = helper->p4InfoActionParam(actionInfo, it->first);
    auto paramId = paramInfo->id();

    params->push_back(helper->p4ActionParam(paramId, it->second));
  }

  auto action = helper->p4Action(actionId, params);

  auto tableAction = helper->p4TableAction(action);

  auto tableEntry = helper->p4TableEntry(tableId, match, tableAction, priority,
                                         idleTimeoutNs);

  auto entity = helper->p4Entity(tableEntry);

  auto update = helper->p4Update(p4_update_type_t::Update_Type_INSERT, entity);

  return env->update_buffer->buffer(update);
}

bool learnMACAdress(env_ptr_t env, mac_addr_ptr_t addr, port_ptr_t port) {
  auto addrStr = addr->raw->toStdString();
  auto higherTag = DETAG(UINT32_MAX);
  globalTag++;

  // Table `mac_src_exact`
  auto srcMatches = vector<pair<string, void *>>();
  srcMatches.push_back(pair<string, pair<string, string> *>(
      "meta.mac_src_exact_tag",
      new pair<string, string>(DETAG(++macSrcExactTag), higherTag)));
  srcMatches.push_back(
      pair<string, string *>("hdr.ethernet.srcAddr", &addrStr));

  auto srcActionParams = vector<pair<string, string>>();

  // Table `mac_dst_exact`
  auto dstMatches = vector<pair<string, void *>>();
  dstMatches.push_back(pair<string, pair<string, string> *>(
      "meta.mac_dst_exact_tag",
      new pair<string, string>(DETAG(++macDstExactTag), higherTag)));
  dstMatches.push_back(
      pair<string, string *>("hdr.ethernet.dstAddr", &addrStr));

  auto dstActionParams = vector<pair<string, string>>();
  dstActionParams.push_back(
      pair<string, string>("egress_port", port->raw->toStdString()));

  // Populate table
  return insertTableEntry(env, "MyIngress.mac_src_exact", &srcMatches,
                          "NoAction", &srcActionParams, PRIORITY,
                          IDLE_TIMEOUT_NS) &&
         insertTableEntry(env, "MyIngress.mac_dst_exact", &dstMatches,
                          "MyIngress.forward", &dstActionParams, PRIORITY,
                          IDLE_TIMEOUT_NS);
}

void pushTagsToStack(env_ptr_t env) {
  auto tagsSize = new size_t(3);

  auto tags = (pair_ptr_t *)malloc(3 * sizeof(pair_ptr_t));
  tags[0] = TAG("global_tag", globalTag);
  tags[1] = TAG("mac_src_exact_tag", macSrcExactTag);
  tags[2] = TAG("mac_dst_exact_tag", macDstExactTag);

  env->stack->push(tags);
  env->stack->push(tagsSize);
}

bool installMulticastGroup(env_ptr_t env) {
  auto helper = env->helper;

  auto replicas = new std::vector<p4_replica_ptr_t>();

  for (uint32_t i = 0; i < 4; i++) {
    replicas->push_back(helper->p4Replica(i, i));
  }

  return env->update_buffer->buffer(
      helper->p4Update(p4_update_type_t::Update_Type_INSERT,
                       helper->p4Entity(helper->p4PacketReplicationEngineEntry(
                           helper->p4MulticastGroupEntry(1, replicas)))));
}

bool synapse_runtime_handle_pre_configure(env_ptr_t env) {
  if (!installMulticastGroup(env)) {
    SYNAPSE_ERROR("Could not install multicast group");
    return false;
  }

  env->stack->clear();
  pushTagsToStack(env);

  return true;
}

bool synapse_runtime_handle_packet_received(env_ptr_t env) {
  auto stack = env->stack;
  assert(3 == stack->size());

  string_ptr_t payloadIn = S_CAST(string_ptr_t, stack->pop());
  NOT_NULL(payloadIn);

  size_t metaInSize = *S_CAST(size_t *, stack->pop());

  pair_ptr_t *metaIn = S_CAST(pair_ptr_t *, stack->pop());
  NOT_NULL(metaIn);

  string_ptr_t globalTagEncoded = nullptr;
  string_ptr_t ingressPortEncoded = nullptr;

  for (size_t i = 0; i < metaInSize; i++) {
    auto metaName = S_CAST(string_ptr_t, metaIn[i]->left)->toStdString();
    if (nullptr == globalTagEncoded && metaName == "global_tag") {
      globalTagEncoded = S_CAST(string_ptr_t, metaIn[i]->right);

    } else if (nullptr == ingressPortEncoded && metaName == "ingress_port") {
      ingressPortEncoded = S_CAST(string_ptr_t, metaIn[i]->right);
    }
  }

  if (nullptr == globalTagEncoded || nullptr == ingressPortEncoded) {
    SYNAPSE_ERROR("Packet in must contain a global tag, and the ingress port");
    return false;
  }

  if (synapse_runtime_wrappers_decode_p4_uint32(globalTagEncoded) !=
      globalTag) {
    SYNAPSE_INFO("The packet contains an outdated global tag");
    stack->clear();
    return true;
  }

  // Skip the destination address (first 6 bytes)
  payloadIn->str += 6;
  mac_addr_ptr_t addr = synapse_runtime_wrappers_decode_mac_address(payloadIn);
  payloadIn->str -= 6;

  port_ptr_t port = synapse_runtime_wrappers_decode_port(ingressPortEncoded);

  if (!learnMACAdress(env, addr, port)) {
    SYNAPSE_ERROR("Could not learn the MAC address");
    return false;
  }

  // Make sure there's nothing left in the stack
  stack->clear();

  // Push metadata (pair_ptr_t*)
  auto meta = (pair_ptr_t *)malloc(sizeof(pair_ptr_t));
  *meta = new pair_t(new string_t("ingress_port"), ingressPortEncoded);
  stack->push(meta);

  // Push metadata size (size_t*)
  stack->push(new size_t(1));

  // Push packet payload (string_ptr_t)
  stack->push(payloadIn);

  // Finally, update tags (populate the top of the stack)
  pushTagsToStack(env);

  return true;
}

bool synapse_runtime_handle_idle_timeout_notification_received(env_ptr_t env) {
  printf("Received an idle timeout notification\n");
  fflush(stdout);

  // No tags need to be updated, nor a packet needs to be sent to the switch
  env->stack->clear();
  return true;
}

} // namespace synapse::runtime

using synapse::runtime::conn_ptr_t;
using synapse::runtime::conn_t;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "usage: ./cpp_controller <grpcAddr> <p4InfoFilepath> "
                 "<bmv2JsonFilepath>"
              << std::endl;
    exit(1);
  }

  conn_ptr_t connector = new conn_t(argv[1]);
  return connector->configure(argv[3], argv[2]) && connector->startAndWait();
}