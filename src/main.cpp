#include <fcgicc.h>

#include <service.h>
#include <backend/network_client.h>
#include <backend/network_manager_client.h>

DBus::BusDispatcher dispatcher;

int main(int, char **)
{
  DBus::_init_threading();

  DBus::default_dispatcher = &dispatcher;

	auto dbus_conn = DBus::Connection::SystemBus();   

  ifnc::backend::network_manager_client nm_cli(dbus_conn);

  ifnc::service service(nm_cli);

  FastCGIServer server;

  service.register_to_server(server);
  server.listen("./socket");
  server.listen(8089);

  server.process_forever();
}
