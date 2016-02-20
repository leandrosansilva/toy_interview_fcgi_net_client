#!/bin/bash

# NOTE: requires docker-compose

NGINX_CONF=$PWD/deploy/nginx.conf docker-compose -f deploy/docker-compose.yml up
