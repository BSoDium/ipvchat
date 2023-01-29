#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <ctime>
#include <map>

#include "channel.hpp"

/**
 * Map associating channel IDs to channels.
*/
typedef std::map<std::string, std::shared_ptr<Channel>> channels_t;

/**
 * Storage class, stores channels and users.
*/
class Storage
{
private:
  users_t _users = {
    {"J0hn", "John"},
    {"Jn", "Jane"},
    {"j4ckieChan", "Jack"}
  };
  channels_t _channels = {
    {"general", std::make_shared<Channel>("general", _users)},
    {"J0hn-Jn", std::make_shared<Channel>("J0hn-Jn", _users)},
  };

public:
  Storage() = default;
  ~Storage() = default;

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