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

void Client::send(Request request)
{
  // Send the message to the server
  std::string message = request.serialize();
  if (::send(_socket, message.c_str(), message.size(), ::MSG_NOSIGNAL) < 0) {
    std::cerr << "Client failed to send message" << std::endl;
  }
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

Request Client::receive()
{
  // Receive a response from the server (blocking)
  char buf[BUF_SIZE];
  int bytesReceived = ::recv(_socket, buf, BUF_SIZE, 0);
  if (bytesReceived < 0) {
    std::cerr << "Client failed to receive message" << std::endl;
    return Request("error", {
      {"origin", "Client"},
      {"message", "Client failed to receive message"}
    });
  }

  // Return the response
  return Request(std::string(buf, bytesReceived));
}