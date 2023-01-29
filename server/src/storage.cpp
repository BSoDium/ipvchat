#include "storage.hpp"

std::map<std::string, std::shared_ptr<Channel>> Storage::getChannels()
{
  return _channels;
}

std::map<std::string, std::string> Storage::getUsers()
{
  return _users;
}

std::shared_ptr<Channel> Storage::getChannel(std::string channel_id)
{
  return _channels[channel_id];
}

std::string Storage::getUserName(std::string user_id)
{
  return _users[user_id];
}

void Storage::addChannel(std::string channel_id)
{
  auto new_channel = std::make_shared<Channel>(channel_id, _users);
  _channels[channel_id] = new_channel;
}

std::string Storage::addChannel()
{
  std::string channel_id = std::to_string(std::time(0));
  auto new_channel = std::make_shared<Channel>(channel_id, _users);
  _channels[channel_id] = new_channel;
  return channel_id;
}

void Storage::removeChannel(std::string channel_id)
{
  _channels.erase(channel_id);
}

void Storage::addUser(std::string user_id, std::string user_name)
{
  _users[user_id] = user_name;
}

void Storage::addUser(std::string user_id)
{
  _users[user_id] = user_id;
}

std::string Storage::addUser()
{
  std::string user_id = std::to_string(std::time(0));
  _users[user_id] = user_id;
  return user_id;
}

void Storage::removeUser(std::string user_id)
{
  _users.erase(user_id);
}