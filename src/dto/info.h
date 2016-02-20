#pragma once

#include <string>
#include <vector>

namespace ifnc { namespace dto { 

struct interface_information 
{
  struct ip_info
  {
    std::vector<std::string> addresses;
    std::string gateway;
  };

  std::string name;
  ip_info ipv4;
  ip_info ipv6;
};

using info = std::vector<interface_information>;

}
}
