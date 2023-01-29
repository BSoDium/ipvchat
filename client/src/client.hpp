#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "request.hpp"

const int BUF_SIZE = 1024;

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

  void send(Request request);
  Request receive();
};

#endif // CLIENT_HPP