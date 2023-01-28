#ifndef RUNNERS_HPP
#define RUNNERS_HPP

#include <string>
#include <vector>
#include <iostream>

class Server;

std::string sendMessage(Server &instance, std::vector<std::string> args);

std::string listChannels(Server &instance, std::vector<std::string> args);

std::string listMessages(Server &instance, std::vector<std::string> args);

std::string createChannel(Server &instance, std::vector<std::string> args);

std::string deleteChannel(Server &instance, std::vector<std::string> args);

#endif // RUNNERS_HPP