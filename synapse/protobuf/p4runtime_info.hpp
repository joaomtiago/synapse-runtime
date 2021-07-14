#ifndef SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_INFO_HPP_
#define SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_INFO_HPP_

#include "synapse/utils/typedefs.hpp"

#ifdef __cplusplus
#include <stdexcept>

#include "google/protobuf/text_format.h"
#endif  // __cplusplus

#ifdef __cplusplus
namespace synapse::p4runtime {

class P4RuntimeInfoHelper {
 public:
  P4RuntimeInfoHelper(const std::string& p4InfoFilepath);

  // todo
  p4_config_extern_t* externInfo();

  // todo
  p4_config_extern_instance_t* externInstanceInfo();

  p4_config_match_field_t* matchFieldInfo(p4_config_table_t table,
                                          std::string matchFieldName);

  p4_config_table_t* tableInfo(std::string tableName);

  // todo
  p4_config_action_ref_t* actionRefInfo();

  p4_config_action_t* actionInfo(std::string actionName);

  p4_config_action_param_t* actionParamInfo(p4_config_action_t action,
                                            std::string paramName);

  // todo
  p4_config_action_profile_t* actionProfileInfo();

  // todo
  p4_config_counter_spec_t* counterSpecInfo();

  // todo
  p4_config_counter_t* counterInfo();

  // todo
  p4_config_direct_counter_t* directCounterInfo();

  // todo
  p4_config_meter_spec_t* meterSpecInfo();

  // todo
  p4_config_direct_meter_t* directMeterInfo();

  p4_config_controller_packet_metadata_t* controllerPacketMetadataInfo(
      std::string controllerPacketMetadataName);

  p4::config::v1::ControllerPacketMetadata_Metadata*
  controllerPacketMetadataMetadataInfo(
      p4_config_controller_packet_metadata_t controllerPacketMetadata,
      std::string metadataName);

  // todo
  p4_config_value_set_t* valueSetInfo();

  // todo
  p4_config_register_t* externRegister();

  // todo
  p4_config_digest_t* digestInfo();

  p4::config::v1::P4Info* p4Info();

 private:
  std::shared_ptr<p4::config::v1::P4Info> p4Info_;
};

typedef P4RuntimeInfoHelper info_helper_t;
typedef info_helper_t* info_helper_ptr_t;

}  // namespace synapse::p4runtime
#endif  // __cplusplus

#endif  // SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_INFO_HPP_