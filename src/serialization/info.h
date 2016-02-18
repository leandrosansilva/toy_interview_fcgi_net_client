#pragma once

#include <dto/info.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

namespace cereal {
  template <class Archive>
  inline void serialize(Archive &archive, const ifnc::dto::interface_information &info)
  {
    archive(
      cereal::make_nvp("name", info.name),
      cereal::make_nvp("address", info.ip_address),
      cereal::make_nvp("netmask", info.netmask),
      cereal::make_nvp("default_gateway", info.default_gateway)
    );
  }
}
