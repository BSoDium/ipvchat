#include "channel.hpp"

Channel::Channel(std::string channel_id, users_t users):
  _channel_id(channel_id),
  _messages({}),
  _users(users)
{}


Channel::~Channel()
{
}

std::string Channel::getChannelId()
{
  return _channel_id;
}

std::map<timestamp_t, Message> Channel::getMessages()
{
  return _messages;
}

users_t Channel::getUsers()
{
  return _users;
}

void Channel::addMessage(std::string user_id, std::string message)
{
  timestamp_t timestamp = std::time(0);
  Message new_message = {timestamp, user_id, message};
  _messages[timestamp] = new_message;
}

void Channel::addMessage(Message message)
{
  _messages[message.timestamp] = message;
}