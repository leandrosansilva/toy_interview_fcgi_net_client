#pragma once

#include <dto/info.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

namespace cereal {
  template <class Archive>
  inline void serialize(Archive &archive, const ifnc::dto::interface_information::ip_info &info)
  {
    archive(
      cereal::make_nvp("addresses", info.addresses),
      cereal::make_nvp("default_gateway", info.gateway)
    );
  }

  template <class Archive>
  inline void serialize(Archive &archive, const ifnc::dto::interface_information &info)
  {
    archive(
      cereal::make_nvp("name", info.name),
      cereal::make_nvp("ipv4", info.ipv4),
      cereal::make_nvp("ipv6", info.ipv6)
    );
  }
}
