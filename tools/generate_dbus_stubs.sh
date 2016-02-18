#!/bin/bash

set -e

SOURCE_DIR=$1
BINARY_DIR=$2
XML_DIR=$3
PREFIX=$4

STUBS_DIR=$BINARY_DIR/dbus_stubs/

mkdir -p $STUBS_DIR/$PREFIX

XML2CPP=$BINARY_DIR/3rd/bin/dbusxx-xml2cpp
TPL_FILE=$SOURCE_DIR/3rd/dbus-cplusplus/tools/proxy-stubs.tpl

for XML_FILE in $XML_DIR/*.xml; do
  BASENAME=$(basename $XML_FILE)
  H_FILE=$STUBS_DIR/$PREFIX/${BASENAME%.*}.h
  $XML2CPP $XML_FILE --proxy=$H_FILE --proxy-template=$SOURCE_DIR/3rd/dbus-cplusplus/tools/proxy-stubs.tpl
  echo Generated D-bus stub file $PREFIX/${BASENAME%.*}.h
done

echo Success
