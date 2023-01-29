#include "server.hpp"

Server::Server(std::string ipAddress, int portNumber)
{
  _storage = Storage();
  _ipAddress = ipAddress;
  _portNumber = portNumber;
}

Server::~Server()
{
  close(_socket);
}

void Server::connectionHandler(int sock)
{
  sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  getpeername(sock, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_size);
  std::string client_ip = inet_ntoa(client_addr.sin_addr);
  int client_port = ntohs(client_addr.sin_port);
  std::cout << "Client connected from " << client_ip << ":" << client_port << std::endl;

  // Add the client to the list
  Client client{client_ip, client_port, sock, ""};
  _clients.push_back(client);
  
  keepAlive(_clients.back());

  // Remove the client from the list
  _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());

  std::cout << "Client " << client_ip << ":" << client_port << " disconnected" << std::endl;
  close(sock);
}

void Server::keepAlive(Client &client)
{
  int sock = client.socket;
  while (true)
  {
    Packet packet(sock);
    
    std::cout << "Received packet: " << packet.serialize() << std::endl;

    Packet response = Packet("response", {
      {"status", "error"},
      {"message", "Command not found"}
    });
    for (auto commandOption: _commands) {
      if (commandOption.name == packet.getAction()) {
        response = commandOption.command(client, packet.getArgs());
        break;
      }
    }

    // Send the response
    send(client, response);
  }
}

void Server::broadcast(Packet packet)
{
  broadcast(packet, *_storage.getChannel("general"));
}

void Server::broadcast(Packet packet, Channel channel)
{
  users_t users = channel.getUsers();
  for (auto client: _clients)
  {
    if (users.find(client.user_id) != users.end())
    {
      send(client, packet);
    }
  }
}

void Server::send(Client client, Packet packet)
{
  packet.send(client.socket);
}

void Server::run()
{
  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket < 0)
  {
    std::cerr << "Failed to create socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(_portNumber);
  addr.sin_addr.s_addr = inet_addr(_ipAddress.c_str());

  if (bind(_socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
  {
    std::cerr << "Failed to bind socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (listen(_socket, 10) < 0)
  {
    std::cerr << "Failed to listen on socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Server started on " << _ipAddress << ":" << _portNumber << std::endl;

  while (true)
  {
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock = accept(_socket, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_size);
    if (client_sock < 0)
    {
      std::cerr << "Failed to accept client" << std::endl;
      continue;
    }

    std::thread client_thread(&Server::connectionHandler, this, client_sock);
    client_thread.detach();
  }
}

Storage &Server::storage()
{
  return _storage;
}