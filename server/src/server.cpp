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
  _clients.push_back(Client{std::to_string(_clients.size()), client_ip, client_port, sock});
  
  keepAlive(sock);

  // Remove the client from the list
  _clients.erase(std::remove_if(_clients.begin(), _clients.end(), [sock](Client client) {
    return client.socket == sock;
  }), _clients.end());

  std::cout << "Client " << client_ip << ":" << client_port << " disconnected" << std::endl;
  close(sock);
}

void Server::keepAlive(int sock)
{
  char buf[BUF_SIZE];
  while (true)
  {
    int len = ::recv(sock, buf, BUF_SIZE, 0);
    if (len < 0)
    {
      std::cerr << "Failed to receive data" << std::endl;
      break;
    }
    if (len == 0)
    {
      break;
    }
    std::string rawRequest(buf, len);

    // Parse the command and arguments
    Request request(rawRequest);
    
    std::cout << "Received request: " << request.serialize() << std::endl;

    for (auto commandOption: _commands) {
      if (commandOption.name == request.getAction()) {
        commandOption.command(request.getArgs());
        break;
      }
    }

    // Clear the buffer
    std::memset(buf, 0, BUF_SIZE);
  }
}

void Server::notifyClients(Request request)
{
    for (auto client: _clients)
  {
    std::string rawRequest = request.serialize();
    if (::send(client.socket, rawRequest.c_str(), rawRequest.size(), ::MSG_NOSIGNAL) < 0)
    {
      std::cerr << "Failed to send data" << std::endl;
      continue;
    }
  }
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