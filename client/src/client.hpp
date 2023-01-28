#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

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
  void setPortNumber(int portNumber);
  bool connect();
  std::string send(std::string message);
  bool disconnect();
  bool isConnected();
};

#endif // CLIENT_HPP