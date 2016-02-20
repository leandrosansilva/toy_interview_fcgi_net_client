#include <iostream>
#include <algorithm>

#include <backend/network_manager_client.h>

using namespace ifnc::backend::details;

DBus::BusDispatcher dispatcher;

int main(int, char **)
{
  DBus::_init_threading();

  DBus::default_dispatcher = &dispatcher;

	auto dbus_conn = DBus::Connection::SystemBus();   
	
  NetworkManager nm(dbus_conn);

  for (auto conn_path: nm.ActiveConnections()) {
    ActiveConnection conn(dbus_conn, conn_path);

    IP4Config ip4_config(dbus_conn, conn.Ip4Config());
    IP6Config ip6_config(dbus_conn, conn.Ip6Config());

    for (auto dev_path: conn.Devices()) {
      Device device(dbus_conn, dev_path);
      std::cout << device.InterfaceName() << std::endl;
    }

    std::cout << "ip4 gateway: " << ip4_config.Gateway() << std::endl;
    std::cout << "ip6 gateway: " << ip4_config.Gateway() << std::endl;

    for (auto data: ip4_config.AddressData())
    {
      std::cout << "ip4: " << data["address"].operator std::string() << "/" << uint32_t(data["prefix"]) << std::endl;
    }

    for (auto data: ip6_config.AddressData())
    {
      std::cout << "ip6: " << data["address"].operator std::string() << "/" << uint32_t(data["prefix"]) << std::endl;
    }

    std::cout << std::endl;
  }

	//dispatcher.enter();
}
