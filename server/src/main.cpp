#include <iostream>
#include <string>

#include "server.hpp"

/**
 * @brief Main function
*/
int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    return 1;
  }

  std::string ipAddress = argv[1];
  int portNumber = std::stoi(argv[2]);

  Server server(ipAddress, portNumber);
  server.run();
  
  return EXIT_SUCCESS;
}
