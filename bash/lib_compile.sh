#!/bin/bash

# Module file de lib permettant l'utilisation idempotente
# de TODO

# --out path to final binary
# --src src necessary for build the out binary
# --flags list of all flags to forward to the compiler
# --libs list of all libs directory

lib_compile_test() {
  local out
  local src
  local flags
  local libs

  while [ $# -gt 0 ]; do
    case "$1" in
    "--out")
      out="$2"
      shift 2
      ;;
    "--src")
      src="$2"
      shift 2
      ;;
    "--flags")
      flags="$2"
      shift 2
      ;;
    "--libs")
      libs="$2"
      shift 2
      ;;
    *)
      echo "Unexpected option: $1 - this should not happen."
      return 2
      ;;
    esac
  done

  if [ ! -f "$src" ]; then
    echo "$src can not be compiled : $src does not exist"
    return 2
  fi

  if [ "$src" -nt "$out" ]; then
    "$cc" -c $src $libs $flags -S
    "$cc" -o "$out.o" -c $src $libs $flags
    "$cc" -o "$out" "$out.o" $flags
  fi

}
