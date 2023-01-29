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
        cli->setPrompt(ipAddress + ":" + std::to_string(portNumber));
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

  _addCommand("login", "Login to server", [&](CLI *cli, std::vector<std::string> args) {
    try {
      if (_handler.isConnected()) {
        _userId = args.at(0);
        cli->setPrompt(_userId + "@" + _handler.getIpAddress() + ":" + std::to_string(_handler.getPortNumber()));
      } else {
        std::cout << "Not connected to server" << std::endl;
      }
    } catch (std::exception &e) {
      cli->error("Invalid arguments");
    }
  });

  _addCommand("join", "Join a channel", [&](CLI *cli, std::vector<std::string> args) {
    try {
      if (_handler.isConnected()) {
        if (_userId != "") {
          std::string channel = args.at(0);
          cli->setPrompt(_userId + "@" + channel + "@" + _handler.getIpAddress() + ":" + std::to_string(_handler.getPortNumber()));
          join(channel);
          cli->setPrompt(_userId + "@" + _handler.getIpAddress() + ":" + std::to_string(_handler.getPortNumber()));
        } else {
          std::cout << "Not logged in" << std::endl;
        }
      } else {
        std::cout << "Not connected to server" << std::endl;
      }
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

void ClientConsole::join(std::string channel)
{
  bool joined = true;
  
  // Create a thread to receive messages from the server
  std::thread receiveThread([&]() {
    while (joined) {
      Request request = _handler.receive();
      std::string action = request.getAction();
      auto payload = request.getArgs();
      if (action == "message" && payload["user_id"] != _userId && payload["channel_id"] == channel) {
        std::cout << payload["user_id"] << ": " << payload["message"] << std::endl;
      } else if (action == "error") {
        std::cout << "Error: " << payload["message"] << std::endl;
      } else if (action == "info") {
        std::cout << "Info: " << payload["message"] << std::endl;
      }
    }
  });

  // allow the user to type messages into the console, if the message is /exit, leave the channel
  std::string message = "";
  do {
    std::getline(std::cin, message);
    if (message != "") {
      Request request = Request("send", {
        {"channel_id", channel},
        {"user_id", _userId},
        {"message", message}
      });
      _handler.send(request);
    }
  } while (message != "/exit" && joined);

  // kill the receive thread
  joined = false;

  // wait for the receive thread to finish
  receiveThread.join();
  
}