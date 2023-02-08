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
        std::string userId = args.at(0);
        login(userId);
        _userId = userId;
        cli->setPrompt(_userId + "@" + _handler.getIpAddress() + ":" + std::to_string(_handler.getPortNumber()));
      } else {
        std::cout << "Not connected to server" << std::endl;
      }
    } catch (std::exception &e) {
      cli->error("Invalid arguments");
    }
  });

  _addCommand("logout", "Logout from server", [&](CLI *cli, std::vector<std::string> args) {
    try {
      if (_handler.isConnected()) {
        if (_userId != "") {
          logout();
          _userId = "";
          cli->resetPrompt();
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

  _addCommand("list", "List channels and users", [&](CLI *cli, std::vector<std::string> args) {
    try {
      if (_handler.isConnected()) {
        if (_userId != "") {
          list();
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
  auto lastActivity = std::chrono::system_clock::now();

  // Notify the other users in the channel that we joined
  _handler.send(Packet("send", {
    {"channel_id", channel},
    {"message", "joined the channel"}
  }));

  // Create a thread to kill the connection if the idle timeout is reached
  std::thread idleThread([&]() {
    while (joined) {
      auto now = std::chrono::system_clock::now();
      if (std::chrono::duration_cast<std::chrono::seconds>(now - lastActivity).count() > 60) {
        std::cout << "Idle timeout reached, disconnecting" << std::endl;
        joined = false;
      }
      sleep(1);
    }
  });

  // Create a thread to receive messages from the server
  std::thread receiveThread([&]() {
    while (joined) {
      Packet packet = _handler.receive();
      std::string action = packet.getAction();
      auto payload = packet.getArgs();
      if (action == "chat" && payload["channel_id"] == channel && payload["user_id"] != _userId) {
        std::cout << GREEN(payload["user_id"]) << ": " << payload["message"] << std::endl;
      } else if (action == "response") {
        std::string status = payload["status"];
        std::string message = payload["message"];
        std::cout << BLUE("Response: " << status << " " << message) << std::endl;
      }
    }
  });

  // allow the user to type messages into the console, if the message is /exit, leave the channel
  std::string message = "";
  do {
    lastActivity = std::chrono::system_clock::now();
    std::getline(std::cin, message);
    if (message != "" && message != "/exit") {
      Packet packet = Packet("send", {
        {"channel_id", channel},
        {"message", message}
      });
      _handler.send(packet);
    }
  } while (message != "/exit" && joined);
  
  // Notify the other users in the channel that we left
  _handler.send(Packet("send", {
    {"channel_id", channel},
    {"message", "left the channel"}
  }));

  // kill the receive thread
  joined = false;

  // wait for the receive thread to finish
  receiveThread.join();
  idleThread.join();
}

void ClientConsole::login(std::string userId)
{
  Packet packet = Packet("login", {
    {"user_id", userId}
  });
  _handler.send(packet);

  Packet response = _handler.receive();
  std::string action = response.getAction();
  auto payload = response.getArgs();

  if (action == "response") {
    if (payload["status"] == "success") {
      std::cout << "Logged in as " << userId << std::endl;
      std::cout << "Message: " << payload["message"] << std::endl;
    } else {
      std::cout << "Failed to login as " << userId << std::endl;
      std::cout << "Message: " << payload["message"] << std::endl;
    }
  }
}

void ClientConsole::logout()
{
  login("");
}

void ClientConsole::list()
{
  Packet packet = Packet("list", {});
  _handler.send(packet);

  Packet response = _handler.receive();
  std::string action = response.getAction();
  auto payload = response.getArgs();

  if (action == "response") {
    if (payload["status"] == "success") {
      std::cout << "Channels: " << payload["channels"] << std::endl;
      std::cout << "Users: " << payload["users"] << std::endl;
    } else {
      std::cout << "Failed to run command list" << std::endl;
      std::cout << "Message: " << payload["message"] << std::endl;
    }
  }
}