#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <map>
#include <vector>
#include <functional>
#include <sstream>
#include <exception>
#include <bitset>
#include <climits>
#include <cstring>

#include "storage.hpp"
#include "request.hpp"

const int BUF_SIZE = 1024;

class Server;

typedef std::function<void(args_t)> command_t;

struct Command
{
  std::string name;
  command_t command;
};

struct Client
{
  std::string id;
  std::string ip;
  int port;
  int socket;
};

/**
 * Server class
 */
class Server
{
private:
  Storage _storage;
  std::vector<Client> _clients;

  std::string _ipAddress;
  int _portNumber;
  int _socket;
  
  std::vector<Command> _commands = {
    {"send", [this](args_t args) {
        std::string channel_id = args["channel_id"];
        std::string user_id = args["user_id"];
        std::string message = args["message"];

        auto channel = _storage.getChannel(channel_id);
        auto user = _storage.getUserName(user_id);
        if (channel == nullptr)
        {
          return "ERROR: Channel not found";
        }
        if (user == "")
        {
          return "ERROR: User not found";
        }

        channel->addMessage(user_id, message);

        Request request("message", {
          {"channel_id", channel_id},
          {"user_id", user_id},
          {"message", message}
        });

        notifyClients(request);
      }
    },

  };

  void connectionHandler(int sock);
  void keepAlive(int sock);

  void notifyClients(Request request);

public:
  Server(std::string ipAddress, int portNumber);
  ~Server();

  /**
   * Run the server and listen for incoming connections.
  */
  void run();

  /**
   * Get the storage object.
   */
  Storage &storage();
};

#endif // SERVER_HPP