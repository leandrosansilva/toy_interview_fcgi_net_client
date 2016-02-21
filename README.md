# IFNC - exposes network connnection information through a fastcgi interface

Build requirements:
On Debian 8.3, the following command installs everything needed to compile this project:
```
# apt-get install libdbus-1-3 cmake g++ autoconf \
  libtool-bin libdbus-1-dev libexpat-dev \
  libctemplate-dev -y --force-yes
```

Then compile the code with the usual:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make test # to run the unit tests
```

And execute the server with:
```
$ ./src/main /tmp/ifnc.socket
```

This will create a socket file /tmp/ifnc.socket, used to communicate with the HTTP server.

## Automated build and deploy using Docker

It's also possible to build and run this software using <a href="http://docker.com">Docker</a> 
and <a href="https://docs.docker.com/compose/">Docker Compose</a>. First of all, you must have 
them installed on your computer, and then, execute, from this project root directory:

First, build the image:
```
$ ./build_docker_image.sh
```

And then execute it with:

```
$ ./run_on_docker.sh
```

This command will instanciate 8 instances of the fcgi server, as well as a Nginx instance, which
will work as a load balancer for all requests, running on http://localhost:9999/

## BUGS and Cheats

Well, I could not manage to handle multiple connections concurrently with the current implementation,
so I "cheated" by using multiple processes and a load balancer in order to mimic the same behaviour,
although probably with much inferior performance. Probably I have used the wrong fcgi library, or should
I have used something based on asio... :-(

There is also another bug where some network interfaces cause the communication to NetworkManager gets 
a bit... weird, due the creation of several virtual interfaces `veth` by docker.

The workaround I found to it is to restart NetworkManager with:

```
# systemctl restart network-manager
```

And wait a few seconds, that everything will work fine :-)

I probably can solve this with a better handling of the communication with NetworkManager, but it will
consume too much time, which I don't have right now :-(
