#ifndef INTERACTIVECLIENT_HPP
#define INTERACTIVECLIENT_HPP

#include <clifford.hpp>

#include "client.hpp"

/**
 * @brief A command line interface for communicating with a server.
*/
class ClientConsole: public core::CLI
{
private:
  Client _handler = Client("", 0);
public:
  ClientConsole();
  void exit();
};

#endif // INTERACTIVECLIENT_HPP