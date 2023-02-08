#ifndef PACKET_HPP
#define PACKET_HPP

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sstream>
#include <map>

#define DELIMITER "\30" // record separator
#define HEADER_SIZE 4

#define RESPONSE(status, message) Packet("response", {{"status", status}, {"message", message}})
#define ERROR(message) RESPONSE("error", message)
#define SUCCESS(message) RESPONSE("success", message)
#define INFO(message) RESPONSE("info", message)

typedef std::map<std::string, std::string> args_t;

class Packet
{
private:
  std::string _action;
  args_t _args;

public:
  Packet(std::string action, std::map<std::string, std::string> args) : _action(action), _args(args) {}
  /**
   * @brief Construct a new Packet object from a buffer
   * 
   * The buffer should be in the format:
   * 
   * `action{DELIMITER}arg1=value1{DELIMITER}arg2=value2{END}´
   * 
   * - the argument names should contain any character except {DELIMITER} and =
   * - the values can contain any character except {DELIMITER}
  */
  Packet(std::string buffer);
  /**
   * @brief Construct a new Packet object from a socket by
   * reading the header and then the body.
   * 
   * The header should be in the format:
   * 
   * `0000{HEADER_SIZE}´
   * 
   * - the header should contain the size of the body
   * - the header should be padded with 0s to the left
  */
  Packet(int socket);
  
  std::string getAction() { return _action; }
  std::map<std::string, std::string> getArgs() { return _args; }
  std::string serialize();
  std::string receive(int socket);
  void send(int socket);

  std::string toString();
};

#endif // PACKET_HPP