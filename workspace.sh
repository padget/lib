#!/bin/bash

source "bash/lib_file.sh"
source "bash/lib_git.sh"
source "bash/lib_apt.sh"
source "bash/lib_fetch.sh"
source "bash/lib_compile.sh"

if [ "$1" = "clean" ]; then
  lib_file --path build --state absent
  rm -f *.bc *.ii *.s
else
  cc="g++-10"
  flags="-std=c++20 -Wall -pedantic -Werror -O3 -save-temps"
  libs="-Icpp"

  lib_file \
    --path build \
    --state directory

  lib_compile_test \
    --out build/format.test.out \
    --src cpp/test/format.test.cpp \
    --flags "$flags" \
    --libs "$libs"

  lib_file \
    --path build/format.test.out \
    --mode +x \
    --state file

  ./build/format.test.out

  # lib_compile_test \
  #   --out build/array.test.out \
  #   --src cpp/test/array.test.cpp \
  #   --flags "$flags" \
  #   --libs "$libs"

  # lib_file \
  #   --path build/array.test.out \
  #   --mode +x \
  #   --state file

  # ./build/array.test.out

  # lib_compile_test \
  #   --out build/tree.test.out \
  #   --src cpp/test/tree.test.cpp \
  #   --flags "$flags" \
  #   --libs "$libs"

  # lib_file \
  #   --path build/tree.test.out \
  #   --mode +x \
  #   --state file

  # ./build/tree.test.out


  time lib_compile_test \
    --out build/clon.test.out \
    --src cpp/test/clon.test.cpp \
    --flags "$flags" \
    --libs "$libs"

  time lib_file \
    --path build/clon.test.out \
    --mode +x \
    --state file

  time ./build/clon.test.out


fi
