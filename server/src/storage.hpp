#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <string>
#include <map>
#include <ctime>
#include <vector>
#include <iostream>
#include <memory>

#include "channel.hpp"

/**
 * Map associating channel IDs to channels.
*/
typedef std::map<std::string, std::shared_ptr<Channel>> channels_t;

/**
 * Storage class
*/
class Storage
{
private:
  channels_t _channels;
  users_t _users;

public:
  Storage();
  ~Storage();

  channels_t getChannels();
  users_t getUsers();

  std::shared_ptr<Channel> getChannel(std::string channel_id);
  std::string getUserName(std::string user_id);
  
  void addChannel(std::string channel_id);
  std::string addChannel();
  
  void removeChannel(std::string channel_id);

  void addUser(std::string user_id, std::string user_name);
  void addUser(std::string user_id);
  std::string addUser();
  
  void removeUser(std::string user_id);
};

#endif // STORAGE_HPP