#!/bin/bash

# Ce module aura pour but de mettre en place
# une utilisation idempotente du programme git
# pour le clonage d'un repository git

# --ref reference to checkout after clone
# --repo git, SSH, or HTTP(S) protocol address of the git repository.
# --dest The path of where the repository should be checked out.
# --accept_hostkey If yes, ensure that "-o StrictHostKeyChecking=no" is present as an ssh option.

lib_git() {
  local ref
  local repo
  local dest
  local accept_hostkey

  while [ $# -gt 0 ]; do
    case "$1" in
    "--ref")
      ref="$2"
      shift 2
      ;;
    "--repo")
      repo="$2"
      shift 2
      ;;
    "--dest")
      dest="$2"
      shift 2
      ;;
    "--accept_hostkey")
      accept_hostkey="true"
      shift 1
      ;;
    *)
      echo "Unexpected option: $1 - this should not happen."
      return 2
      ;;
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
  *) echo "Unexpected value for 'state' option : $state" ;;
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
