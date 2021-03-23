#!/bin/bash

source "bash/lib_file.sh"
source "bash/lib_git.sh"
source "bash/lib_apt.sh"
source "bash/lib_fetch.sh"

cc="g++-10"
flags="-std=c++20 -Wall -pedantic -Werror -O3"


$cc -o cpp/test/format.test.o -c cpp/test/format.test.cpp -Icpp $flags


