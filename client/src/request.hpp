#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <sstream>
#include <map>

#define REQUEST_DELIMITER ","

typedef std::map<std::string, std::string> args_t;

class Request
{
private:
  std::string _action;
  args_t _args;

public:
  Request(std::string action, std::map<std::string, std::string> args) : _action(action), _args(args) {}
  /**
   * @brief Construct a new Request object from a buffer
   * 
   * The buffer should be in the format:
   * 
   * `action{REQUEST_DELIMITER}arg1=value1{REQUEST_DELIMITER}arg2=value2{REQUEST_DELIMITER}´
   * 
   * - the argument names should contain any character except {REQUEST_DELIMITER} and =
   * - the values can contain any character except {REQUEST_DELIMITER}
  */
  Request(std::string buffer);
  
  std::string getAction() { return _action; }
  std::map<std::string, std::string> getArgs() { return _args; }
  std::string serialize();
};

#endif