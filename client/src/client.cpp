#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

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

void Client::setPortNumber(int portNumber)
{
  _portNumber = portNumber;
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

std::string Client::send(std::string message)
{
  // Send the message to the server
  if (::send(_socket, message.c_str(), message.size(), ::MSG_NOSIGNAL) < 0) {
    std::cerr << "Client failed to send message" << std::endl;
    return "";
  }

  // Receive a response from the server (blocking)
  char buffer[BUF_SIZE];
  int bytes_received = recv(_socket, buffer, BUF_SIZE, 0);
  if (bytes_received < 0) {
    std::cerr << "Client failed to receive response" << std::endl;
    return "";
  }

  return std::string(buffer, bytes_received);
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