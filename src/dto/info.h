#pragma once

#include <string>

namespace ifnc { namespace dto { 

struct info 
{
  std::string ip_address;
  std::string netmask;
  std::string default_gateway;
};

}
}
