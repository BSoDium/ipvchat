#include "packet.hpp"

Packet::Packet(std::string buffer)
{
  std::istringstream iss(buffer);
  std::string token;
  int i = 0;
  while (std::getline(iss, token, DELIMITER[0]))
  {
    if (i == 0)
    {
      _action = token;
    }
    else
    {
      std::size_t pos = token.find("=");
      if (pos != std::string::npos)
      {
        _args[token.substr(0, pos)] = token.substr(pos + 1);
      }
    }
    i++;
  }
}

Packet::Packet(int socket): Packet(receive(socket)) {}

std::string Packet::serialize()
{
  std::string buffer = _action + DELIMITER;
  for (auto const& [key, val] : _args)
  {
    buffer += key + "=" + val + DELIMITER;
  }
  return buffer;
}

std::string Packet::receive(int socket)
{
  char header[HEADER_SIZE];
  ::recv(socket, header, HEADER_SIZE, 0);

  int size = std::stoi(header);
  char buffer[size];
  ::recv(socket, buffer, size, 0);

  return std::string(buffer);
}

void Packet::send(int socket)
{
  std::string buffer = serialize();

  std::string header = std::to_string(buffer.length());
  header = std::string(HEADER_SIZE - header.length(), '0') + header;

  ::send(socket, header.c_str(), HEADER_SIZE, 0);
  ::send(socket, buffer.c_str(), buffer.length(), 0);
}
