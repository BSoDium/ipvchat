#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>

#include "clientConsole.hpp"

int main(int argc, char* argv[]) {
    ClientConsole client;
    client.run();
    
    return EXIT_SUCCESS;
}
