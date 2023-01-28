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

#include "runners.hpp"
#include "storage.hpp"

const int BUF_SIZE = 1024;

class Server;

typedef std::vector<std::string> args_t;
typedef std::function<std::string(Server &, args_t)> command_t;
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
      {"sendMessage", sendMessage},
      {"listChannels", listChannels},
      {"listMessages", listMessages},
      {"createChannel", createChannel},
      {"deleteChannel", deleteChannel}
  };

  void connectionHandler(int sock);
  void keepAlive(int sock);

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