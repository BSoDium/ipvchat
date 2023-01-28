#include "runners.hpp"

std::string sendMessage(Server &instance, std::vector<std::string> args)
{
  std::string user_id = args[0];
  std::string channel_id = args[1];
  std::string message = args[2];

  return "Message sent";
}

std::string listChannels(Server &instance, std::vector<std::string> args)
{
  std::string user_id = args[0];

  std::cout << "User " << user_id << " requested list of channels" << std::endl;
  return "Channel list";
}

std::string listMessages(Server &instance, std::vector<std::string> args)
{
  std::string user_id = args[0];
  std::string channel_id = args[1];

  std::cout << "User " << user_id << " requested list of messages in channel " << channel_id << std::endl;
  return "Message list";
}

std::string createChannel(Server &instance, std::vector<std::string> args)
{
  std::string user_id = args[0];
  std::string channel_id = args[1];

  std::cout << "User " << user_id << " created channel " << channel_id << std::endl;
  return "Channel created";
}

std::string deleteChannel(Server &instance, std::vector<std::string> args)
{
  std::string user_id = args[0];
  std::string channel_id = args[1];

  std::cout << "User " << user_id << " deleted channel " << channel_id << std::endl;
  return "Channel deleted";
}