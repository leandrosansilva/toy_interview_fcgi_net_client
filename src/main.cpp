#include <fcgicc.h>
#include <thread>

#include <service.h>
#include <backend/network_client.h>
#include <backend/network_manager_client.h>

DBus::BusDispatcher dispatcher;

int main(int, char **argv)
{
  std::string socket_filename = argv[1];

  try {
    DBus::_init_threading();

    DBus::default_dispatcher = &dispatcher;

    auto dbus_conn = DBus::Connection::SystemBus();   

    ifnc::backend::network_manager_client nm_cli(dbus_conn);

    ifnc::service service(nm_cli);

    FastCGIServer server;
    server.listen(socket_filename);
    service.register_to_server(server);
    server.process_forever();

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
