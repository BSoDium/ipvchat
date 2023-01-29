#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <string>

#include "packet.hpp"

class Client
{
private:
  std::string _ipAddress;
  int _portNumber;
  int _socket;
public:
  Client(std::string ipAddress, int portNumber);
  ~Client();
  
  void setIpAddress(std::string ipAddress);
  std::string getIpAddress();
  
  void setPortNumber(int portNumber);
  int getPortNumber();

  bool connect();
  bool disconnect();
  bool isConnected();

  void send(Packet packet);
  Packet receive();
};

#endif // CLIENT_HPP