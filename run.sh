#!/bin/sh
PATH_FILE=.config

while getopts ":g" opt; do
  case $opt in
    g)
      GDB=gdb
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      ;;
  esac
done

if [ ! -f $PATH_FILE ] ; then
  echo $PATH_FILE not found!
  exit 1
fi

# strip out the parenthesis from the Makefile config so it can be used by sh
eval `tr -d '()' < $PATH_FILE`

export NSPR_LOG_MODULES=all:5
export DYLD_LIBRARY_PATH=$GECKO_OBJ/dist/lib/
export LD_LIBRARY_PATH=$GECKO_OBJ/dist/lib/
$GDB ./test
