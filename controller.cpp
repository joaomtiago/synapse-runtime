#include "synapse/connector/connector.hpp"
#include "synapse/stream/handler/user_handlers.hpp"

namespace synapse::p4runtime {

bool installMulticastGroup(user_env_ptr_t env) {
  std::vector<p4_replica_t *> *replicas = new std::vector<p4_replica_t *>();
  replicas->push_back(env->helper->replica(1, 1));
  replicas->push_back(env->helper->replica(2, 2));
  replicas->push_back(env->helper->replica(3, 3));

  p4_multicast_group_entry_t *multicastGroupEntry =
      env->helper->multicastGroupEntry(1, replicas);
  p4_packet_replication_engine_entry_t *packetReplicationEngineEntry =
      env->helper->packetReplicationEngineEntry(multicastGroupEntry);
  p4_entity_t *entity = env->helper->entity(packetReplicationEngineEntry);
  p4_update_t *update =
      env->helper->update(p4_update_type_t::Update_Type_INSERT, entity);

  env->update_buffer->buffer(update);
  return true;
}

bool synapse_user_handle_pre_configure(user_env_ptr_t env) {
  return installMulticastGroup(env);
}

bool synapse_user_handle_packet_received(user_env_ptr_t env) {
  std::cout << "[*] received a packet" << std::endl;
  return true;
}

bool synapse_user_handle_idle_timeout_notification_received(
    user_env_ptr_t env) {
  std::cout << "[*] received an idle timeout notification" << std::endl;
  return true;
}

}  // namespace synapse::p4runtime

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout
        << "usage: ./controller <grpcAddr> <p4InfoFilepath> <bmv2JsonFilepath>"
        << std::endl;
    exit(1);
  }

  auto _conn = new synapse::p4runtime::Connector(argv[1], argv[2]);
  return _conn->startAndWait(argv[3]);
}