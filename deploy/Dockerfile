FROM debian:8.3

MAINTAINER Leandro Santiago <leandrosansilva@gmail.com>

ENV DEBIAN_FRONTEND=noninteractive

ADD . /source/

ADD deploy/build.sh /build.sh

RUN apt-get update && apt-get dist-upgrade -y --force-yes \ 
  && apt-get install libdbus-1-3 cmake g++ autoconf \
  libtool-bin libdbus-1-dev libexpat-dev \
  libctemplate-dev -y --force-yes && \
  /build.sh && \
  apt-get purge -y --force-yes cmake g++ \
  autoconf libtool-bin libdbus-1-dev libexpat-dev \
  libctemplate-dev && apt-get autoremove -y --force-yes && \
  apt-get clean -y --force-yes && rm -rf /source

ENTRYPOINT ["/main"]
