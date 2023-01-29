#ifndef CLIENTCONSOLE_HPP
#define CLIENTCONSOLE_HPP

#include <clifford.hpp>
#include <thread>
#include <termios.h>

#include "client.hpp"
#include "request.hpp"

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
  void join(std::string channel);
};

#endif // CLIENTCONSOLE_HPP