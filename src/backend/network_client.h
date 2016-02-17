#pragma once

#include <dto/info.h>

namespace ifnc { namespace backend {
struct network_client
{
  virtual ifnc::dto::info get_network_information() = 0;
};
}
}
