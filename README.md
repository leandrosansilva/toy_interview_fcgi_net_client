# IFNC - exposes network connnection information through a fastcgi interface

Build requirements:

Optional build/runtime requirements:

You can also build and run the code using Docker (and Docker Composer), 
which will spawn several instances of the server, with a Nginx working
as a load balancer to them, running on `http://localhost:9999`
```
$ ./build_docker_image.sh
$ ./run_on_docker.sh
```
