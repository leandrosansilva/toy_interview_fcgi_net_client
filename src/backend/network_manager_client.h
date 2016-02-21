#pragma once

#include <algorithm>
#include <sstream>

#include "network_client.h"

#include <dbus-c++/dbus.h>
#include <dbus_stubs/nm/nm-manager.h>
#include <dbus_stubs/nm/nm-active-connection.h>
#include <dbus_stubs/nm/nm-ip4-config.h>
#include <dbus_stubs/nm/nm-ip6-config.h>
#include <dbus_stubs/nm/nm-device.h>

namespace ifnc { namespace backend {

namespace details {

static const auto NM_SERVICE = "org.freedesktop.NetworkManager";

class NetworkManager: 
  public org::freedesktop::NetworkManager_proxy,
  public DBus::IntrospectableProxy,
  public DBus::PropertiesProxy,
  public DBus::ObjectProxy
{
public:
  NetworkManager(DBus::Connection &conn):
	DBus::ObjectProxy(conn, "/org/freedesktop/NetworkManager", NM_SERVICE)
  {
  }

	void CheckPermissions() override
  {
  }

	void StateChanged(const uint32_t &) override
  {
  }

	void PropertiesChanged(const std::map<std::string, ::DBus::Variant> &) override
  {
  }

	void DeviceAdded(const ::DBus::Path &) override
  {
  }

	void DeviceRemoved(const ::DBus::Path &) override
  {
  }
};

class ActiveConnection:
  public org::freedesktop::NetworkManager::Connection::Active_proxy,
  public DBus::IntrospectableProxy,
  public DBus::PropertiesProxy,
  public DBus::ObjectProxy
{
public:
  ActiveConnection(DBus::Connection &conn, const DBus::Path &path):
	DBus::ObjectProxy(conn, path, NM_SERVICE)
  {
  }

  void PropertiesChanged(const std::map<std::string, ::DBus::Variant> &) override
  {
  }
}; 

class IP4Config:
  public org::freedesktop::NetworkManager::IP4Config_proxy,
  public DBus::IntrospectableProxy,
  public DBus::PropertiesProxy,
  public DBus::ObjectProxy
{
public:
  IP4Config(DBus::Connection &conn, const DBus::Path &path):
	DBus::ObjectProxy(conn, path, NM_SERVICE)
  {
  }

  void PropertiesChanged(const std::map<std::string, ::DBus::Variant> &) override
  {
  }
}; 

class IP6Config:
  public org::freedesktop::NetworkManager::IP6Config_proxy,
  public DBus::IntrospectableProxy,
  public DBus::PropertiesProxy,
  public DBus::ObjectProxy
{
public:
  IP6Config(DBus::Connection &conn, const DBus::Path &path):
	DBus::ObjectProxy(conn, path, NM_SERVICE)
  {
  }

  void PropertiesChanged(const std::map<std::string, ::DBus::Variant> &) override
  {
  }
}; 

class Device:
  public org::freedesktop::NetworkManager::Device_proxy,
  public DBus::IntrospectableProxy,
  public DBus::PropertiesProxy,
  public DBus::ObjectProxy
{
public:
  Device(DBus::Connection &conn, const DBus::Path &path):
	DBus::ObjectProxy(conn, path, NM_SERVICE)
  {
  }

  // NOTE: implemented to solve ambiguity of the method Interface()
  std::string InterfaceName()
  {
    return org::freedesktop::NetworkManager::Device_proxy::Interface();
  }

  void StateChanged(const uint32_t &, const uint32_t &, const uint32_t &) override
  {
  }
}; 

}

struct network_manager_client: network_client
{
  details::NetworkManager _nm;
  DBus::Connection &_dbus;
public:
  network_manager_client(DBus::Connection &dbus):
  _nm(dbus),
  _dbus(dbus)
  {
  }

  static std::string transform_address(std::map<std::string, DBus::Variant> d)
  {
    std::stringstream ss;
    ss << (d["address"].operator std::string()) 
       << "/" << uint32_t(d["prefix"]);
    return ss.str();
  };

  ifnc::dto::info get_network_information() override
  {
    ifnc::dto::info info;

    auto conns = _nm.ActiveConnections();

    std::transform(std::begin(conns), std::end(conns), std::back_inserter(info), 
      [this](const std::string &path) {
        ifnc::dto::interface_information i_info;

        details::ActiveConnection conn(_dbus, path);
        details::IP4Config ip4_config(_dbus, conn.Ip4Config());
        details::IP6Config ip6_config(_dbus, conn.Ip6Config());

        auto devices = conn.Devices();

        // FIXME: is it possible to return zero or more than one interface?
        if (!devices.size()) {
          throw std::runtime_error("Could not find device. This is a BUG");
        }

        i_info.name = details::Device(_dbus, devices[0]).InterfaceName();

        i_info.ipv4.gateway = ip4_config.Gateway();
        i_info.ipv6.gateway = ip6_config.Gateway();

        auto ipv4_addresses = ip4_config.AddressData();
        auto ipv6_addresses = ip6_config.AddressData();

        std::transform(std::begin(ipv4_addresses), std::end(ipv4_addresses),
          std::back_inserter(i_info.ipv4.addresses), transform_address);

        std::transform(std::begin(ipv6_addresses), std::end(ipv6_addresses),
          std::back_inserter(i_info.ipv6.addresses), transform_address);

        return i_info;
      }
    );

    return info;
  }
};

}
}
