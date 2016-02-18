#pragma once

#include <string>
#include <vector>

namespace ifnc { namespace dto { 

struct interface_information 
{
  std::string name;
  std::string ip_address;
  std::string netmask;
  std::string default_gateway;
};

using info = std::vector<interface_information>;

}
}
