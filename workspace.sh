#!/bin/bash

source "bash/lib_file.sh"
source "bash/lib_git.sh"
source "bash/lib_apt.sh"
source "bash/lib_fetch.sh"
source "bash/lib_compile.sh"

if [ "$1" = "clean" ]; then
  lib_file --path build --state absent
else
  cc="g++-10"
  flags="-std=c++20 -Wall -pedantic -Werror -O3"
  libs="-Icpp"

  lib_file --path build --state directory
  lib_compile_test --out build/format.test.out \
                   --src cpp/test/format.test.cpp \
                   --flags "$flags" \
                   --libs "$libs"
  lib_file --path build/format.test.out --mode +x --state file
  # lib_exec --bin build/format.test.out
  ./build/format.test.out
fi
