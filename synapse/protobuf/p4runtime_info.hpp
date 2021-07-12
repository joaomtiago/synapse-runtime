#ifndef SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_INFO_HPP_
#define SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_INFO_HPP_

#include <memory>
#include <stdexcept>

#include "google/protobuf/text_format.h"
#include "p4/config/v1/p4info.pb.h"

namespace synapse::p4runtime {

class P4RuntimeInfoHelper {
 public:
  P4RuntimeInfoHelper(const std::string& p4InfoFilepath);

  // todo
  p4::config::v1::Extern* externInfo();

  // todo
  p4::config::v1::ExternInstance* externInstanceInfo();

  p4::config::v1::MatchField* matchFieldInfo(p4::config::v1::Table table,
                                             std::string matchFieldName);

  p4::config::v1::Table* tableInfo(std::string tableName);

  // todo
  p4::config::v1::ActionRef* actionRefInfo();

  p4::config::v1::Action* actionInfo(std::string actionName);

  p4::config::v1::Action_Param* actionParamInfo(p4::config::v1::Action action,
                                                std::string paramName);

  // todo
  p4::config::v1::ActionProfile* actionProfileInfo();

  // todo
  p4::config::v1::CounterSpec* counterSpecInfo();

  // todo
  p4::config::v1::Counter* counterInfo();

  // todo
  p4::config::v1::DirectCounter* directCounterInfo();

  // todo
  p4::config::v1::MeterSpec* meterSpecInfo();

  // todo
  p4::config::v1::DirectMeter* directMeterInfo();

  p4::config::v1::ControllerPacketMetadata* controllerPacketMetadataInfo(
      std::string controllerPacketMetadataName);

  p4::config::v1::ControllerPacketMetadata_Metadata*
  controllerPacketMetadataMetadataInfo(
      p4::config::v1::ControllerPacketMetadata controllerPacketMetadata,
      std::string metadataName);

  // todo
  p4::config::v1::ValueSet* valueSetInfo();

  // todo
  p4::config::v1::Register* externRegister();

  // todo
  p4::config::v1::Digest* digestInfo();

  p4::config::v1::P4Info* p4Info();

 private:
  std::shared_ptr<p4::config::v1::P4Info> p4Info_;
};

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_INFO_HPP_