#include "synapse/connector/connector.hpp"
#include "synapse/stream/handler/user_handlers.hpp"

bool synapse::p4runtime::preConfigure(user_env_t *env) {
  std::cout << "[*] preconfiguring the environment" << std::endl;
  return true;
}

bool synapse::p4runtime::handlePacketReceived(user_env_t *env) {
  std::cout << "[*] received a packet" << std::endl;
  return true;
}

bool synapse::p4runtime::handleIdleTimeoutNotificationReceived(
    user_env_t *env) {
  std::cout << "[*] received an idle timeout notification" << std::endl;
  return true;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout
        << "usage: ./controller <grpcAddr> <p4InfoFilepath> <bmv2JsonFilepath>"
        << std::endl;
    exit(1);
  }

  // global connector
  auto _conn = new synapse::p4runtime::Connector(argv[1], argv[2]);
  return _conn->startAndWait(argv[3]);
}