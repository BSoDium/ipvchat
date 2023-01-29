#include "client.hpp"

Client::Client(std::string ipAddress, int portNumber)
{
  _ipAddress = ipAddress;
  _portNumber = portNumber;
}

Client::~Client()
{
  disconnect();
}

void Client::setIpAddress(std::string ipAddress)
{
  _ipAddress = ipAddress;
}

std::string Client::getIpAddress()
{
  return _ipAddress;
}

void Client::setPortNumber(int portNumber)
{
  _portNumber = portNumber;
}

int Client::getPortNumber()
{
  return _portNumber;
}

bool Client::connect()
{
  // Create a socket to connect to the server
  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket < 0) {
    return false;
  }

  // Connect to the server on the specified port
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(_portNumber); // Port number
  inet_pton(AF_INET, _ipAddress.c_str(), &addr.sin_addr); // IP address
  return ::connect(_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0;
}

bool Client::disconnect()
{
  // Close the socket
  return ::close(_socket) == 0;
}

bool Client::isConnected()
{
  return _socket > 0;
}

void Client::send(Packet packet)
{
  packet.send(_socket);
}

Packet Client::receive()
{
  return Packet(_socket);
}