#include "synapse/protobuf/p4runtime_info.hpp"

#include <stdexcept>

#include "synapse/utils/file_utils.hpp"

namespace synapse::p4runtime {

P4RuntimeInfoHelper::P4RuntimeInfoHelper(const std::string &p4InfoFilepath) {
  std::string _file = readFile(p4InfoFilepath);
  auto _obj = new p4::config::v1::P4Info();

  google::protobuf::TextFormat::MergeFromString(_file, _obj);
  p4Info_.reset(_obj);
}

p4::config::v1::MatchField *P4RuntimeInfoHelper::matchFieldInfo(
    p4::config::v1::Table table, std::string matchFieldName) {
  for (auto matchField : table.match_fields()) {
    if (matchField.name() == matchFieldName) {
      return new p4::config::v1::MatchField(matchField);
    }
  }

  throw std::invalid_argument("Unknown match field `" + matchFieldName +
                              "` for table `" + table.preamble().name() + "`");
}

p4::config::v1::Table *P4RuntimeInfoHelper::tableInfo(std::string tableName) {
  for (auto table : p4Info_->tables()) {
    if (table.preamble().name() == tableName) {
      return new p4::config::v1::Table(table);
    }
  }

  throw std::invalid_argument("Unknown table `" + tableName + "`");
}

p4::config::v1::Action *P4RuntimeInfoHelper::actionInfo(
    std::string actionName) {
  for (auto action : p4Info_->actions()) {
    if (action.preamble().name() == actionName) {
      return new p4::config::v1::Action(action);
    }
  }

  throw std::invalid_argument("Unknown action `" + actionName + "`");
}

p4::config::v1::Action_Param *P4RuntimeInfoHelper::actionParamInfo(
    p4::config::v1::Action action, std::string paramName) {
  for (auto param : action.params()) {
    if (param.name() == paramName) {
      return new p4::config::v1::Action_Param(param);
    }
  }

  throw std::invalid_argument("Unknown parameter `" + paramName +
                              "` for action `" + action.preamble().name() +
                              "`");
}

p4::config::v1::ControllerPacketMetadata *
P4RuntimeInfoHelper::controllerPacketMetadataInfo(
    std::string controllerPacketMetadataName) {
  for (auto controllerPacketMetadata : p4Info_->controller_packet_metadata()) {
    if (controllerPacketMetadata.preamble().name() ==
        controllerPacketMetadataName) {
      return new p4::config::v1::ControllerPacketMetadata(
          controllerPacketMetadata);
    }
  }

  throw std::invalid_argument("Unknown controller packet metadata `" +
                              controllerPacketMetadataName + "`");
}

p4::config::v1::ControllerPacketMetadata_Metadata *
P4RuntimeInfoHelper::controllerPacketMetadataMetadataInfo(
    p4::config::v1::ControllerPacketMetadata controllerPacketMetadata,
    std::string metadataName) {
  for (auto metadata : controllerPacketMetadata.metadata()) {
    if (metadata.name() == metadataName) {
      return new p4::config::v1::ControllerPacketMetadata_Metadata(metadata);
    }
  }

  throw std::invalid_argument("Unknown controller packet metadata metadata `" +
                              metadataName + "`");
}

p4::config::v1::P4Info *P4RuntimeInfoHelper::p4Info() { return p4Info_.get(); }

}  // namespace synapse::p4runtime