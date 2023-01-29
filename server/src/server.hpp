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
#include "packet.hpp"

class Server;
class Client;

typedef std::function<Packet(Client, args_t)> command_t;

struct Command
{
  std::string name;
  command_t command;
};

struct Client
{
  std::string ip;
  int port;
  int socket;
  std::string user_id;

  bool operator==(const Client& other) const
  {
    return ip == other.ip && port == other.port && socket == other.socket;
  }
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
    {"login", [this](Client source, args_t args) {
      std::string user_id = args["user_id"];
      
      Packet response = SUCCESS("OK");

      // Check if the user exists
      if (_storage.getUsers().find(user_id) == _storage.getUsers().end())
      {
        response = ERROR("User not found");
        return response;
      }

      // Check if the user is already logged in
      for (auto client: _clients)
      {
        if (client.user_id == user_id)
        {
          response = ERROR("User already logged in");
          return response;
        }
      }

      // Update the user_id of the client
      getClient(source.socket).user_id = user_id;
      return response;
    }},
    {"send", [this](Client source, args_t args) {
        std::string channel_id = args["channel_id"];
        std::string message = args["message"];

        // Get the user_id associated to the client
        std::string user_id = getClient(source.socket).user_id;

        Packet response = SUCCESS("OK");

        // Check if the channel exists
        auto channel = _storage.getChannel(channel_id);
        if (channel == nullptr)
        {
          response = ERROR("Channel not found");
          return response;
        }

        // Check if the user is in the channel
        if (channel->getUsers().find(user_id) == channel->getUsers().end())
        {
          response = ERROR("User not in channel");
          return response;
        }

        // Add the message to the channel
        channel->addMessage(user_id, message);

        // Broadcast the message to the channel
        Packet packet("chat", {
          {"channel_id", channel_id},
          {"user_id", user_id},
          {"message", message}
        });
        broadcast(packet, *channel);

        return response;
      }
    },

  };

  void connectionHandler(int sock);
  void keepAlive(Client client);

  void broadcast(Packet packet);
  void broadcast(Packet packet, Channel channel);
  void send(Client client, Packet packet);

  Client &getClient(int socket);

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