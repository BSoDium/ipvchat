#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <ctime>
#include <vector>
#include <iostream>

typedef std::map<std::string, std::string> users_t;

typedef long timestamp_t;
struct Message
{
  timestamp_t timestamp;
  std::string user_id;
  std::string message;
};

/**
 * Conversation channel class, containing a list of messages.
*/
class Channel
{
private:
  std::string _channel_id;
  std::map<timestamp_t, Message> _messages;
  users_t _users;

public:
  ~Channel();
  Channel(std::string channel_id, users_t users);
  
  std::string getChannelId();
  std::map<timestamp_t, Message> getMessages();
  users_t getUsers();

  void addMessage(std::string user_id, std::string message);
  void addMessage(Message message);
  void addUser(std::string user_id, std::string user_name);
};


#endif // CHANNEL_HPP