#include "clientConsole.hpp"

ClientConsole::ClientConsole(): CLI("ClientConsole", "A command line interface for communicating with a server.")
{
  _addCommand("test", "Test connection to server", [&](CLI *cli, std::vector<std::string> args) {
    try {
      std::string ipAddress = args.at(0);
      int portNumber = std::stoi(args.at(1));
      _handler.setIpAddress(ipAddress);
      _handler.setPortNumber(portNumber);
      if (_handler.connect()) {
        std::cout << "Server is reachable" << std::endl;
        _handler.disconnect();
      } else {
        std::cout << "Server offline or misconfigured" << std::endl;
      }
    } catch (std::exception &e) {
      cli->error("Invalid arguments");
    }
  });

  _addCommand("connect", "Connect to server", [&](CLI *cli, std::vector<std::string> args) {
    try {
      std::string ipAddress = args.at(0);
      int portNumber = std::stoi(args.at(1));
      _handler.setIpAddress(ipAddress);
      _handler.setPortNumber(portNumber);
      if (_handler.connect()) {
        std::cout << "Connection successful" << std::endl;
        cli->setPrompt("Remote@" + ipAddress + ":" + std::to_string(portNumber));
      } else {
        std::cout << "Connection failed" << std::endl;
      }
    } catch (std::exception &e) {
      cli->error("Invalid arguments");
    }
  });

  _addCommand("disconnect", "Disconnect from server", [&](CLI *cli, std::vector<std::string> args) {
    try {
      if (_handler.isConnected()) {
        if (_handler.disconnect()) {
          std::cout << "Disconnected from server" << std::endl;
          cli->resetPrompt();
        } else {
          std::cout << "Failed to disconnect from server" << std::endl;
        } 
      } else {
        std::cout << "Not connected to server" << std::endl;
      }
    } catch (std::exception &e) {
      cli->error("Invalid arguments");
    }
  });

  _addCommand("send", "Send message to server", [&](CLI *cli, std::vector<std::string> args) {
    try {
      if (!_handler.isConnected()) {
        cli->error("Not connected to server");
        return;
      }
      std::string message = args.at(0);
      std::string response = _handler.send(message);
      std::cout << "Response: " << response << std::endl;
    } catch (std::exception &e) {
      cli->error("Invalid arguments");
    }
  });
}

void ClientConsole::exit()
{
  if (_handler.isConnected()) {
    _handler.disconnect();
  }
  CLI::exit();
}