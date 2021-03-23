#!/bin/bash

# Module file de lib permettant l'utilisation idempotente 
# de  pour l'installation de package et la mise à jour 
# de ceux-ci ainsi que de la base des packages

# --path Path to the file being managed.
# --group Name of the group that should own the file/directory, as would be fed to chown.
# --mode 777 | 664 ... 
# --owner Name of the user that should own the file/directory, as would be fed to chown.
# --state absent | directory | file

lib_file() {
  local path
  local group 
  local mode
  local owner
  local state
  
  while [ $# -gt 0  ]
  do 
    case "$1" in
    "--path") path="$2"; shift 2;;
    "--group") group="$2"; shift 2;;
    "--mode") mode="$2"; shift 2;;
    "--owner") owner="$2"; shift 2;;
    "--state") state="$2"; shift 2;;
    *) echo "Unexpected option: $1 - this should not happen."; return 2;;
    esac
  done
  
  case "$state" in
  "absent") 
    rm -rf "$path"
    ;;
  "directory") 
    mkdir -p "$path"
    ;;
  "file") 
    touch "$path"
    ;;
  *) echo "Unexpected value for 'state' option : $state";;
  esac

  if [ "$state" = "file" ] || [ "$state" = "directory" ]; then
    if [ ! -z "$group" ]; then
      chgrp --silent "$group" "$path"
    fi
    if [ ! -z "$owner" ]; then
      chown --silent "$owner" "$path"
    fi
    if [ ! -z "$mode" ]; then
      chmod --silent "$mode" "$path"
    fi
  fi
}