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
  users_t& _users;

public:
  ~Channel();
  Channel(std::string channel_id, users_t& users);
  Channel& operator=(const Channel& other);
  
  std::string getChannelId();
  std::map<timestamp_t, Message> getMessages();

  void addMessage(std::string user_id, std::string message);
  void addMessage(Message message);
};


#endif // CHANNEL_HPP