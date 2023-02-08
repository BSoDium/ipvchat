#ifndef CLIENTCONSOLE_HPP
#define CLIENTCONSOLE_HPP

#include <clifford.hpp>
#include <thread>
#include <termios.h>

#include "client.hpp"
#include "packet.hpp"

#define IDLE_TIMEOUT 5 // minutes

/**
 * @brief A command line interface for communicating with a chat server.
*/
class ClientConsole: public core::CLI
{
private:
  Client _handler = Client("", 0);
  std::string _userId = "";
public:
  ClientConsole();
  void exit();
  void login(std::string userId);
  void logout();
  void list();
  void join(std::string channel);
};

#endif // CLIENTCONSOLE_HPP