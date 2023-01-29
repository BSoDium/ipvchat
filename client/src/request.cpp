#include "request.hpp"

Request::Request(std::string buffer)
{
  std::istringstream iss(buffer);
  std::string token;
  int i = 0;
  while (std::getline(iss, token, REQUEST_DELIMITER[0]))
  {
    if (i == 0)
    {
      _action = token;
    }
    else
    {
      std::size_t pos = token.find("=");
      if (pos != std::string::npos)
      {
        _args[token.substr(0, pos)] = token.substr(pos + 1);
      }
    }
    i++;
  }
}

std::string Request::serialize()
{
  std::string buffer = _action + REQUEST_DELIMITER;
  for (auto const& [key, val] : _args)
  {
    buffer += key + "=" + val + REQUEST_DELIMITER;
  }
  return buffer;
}
