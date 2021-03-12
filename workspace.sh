#!/bin/bash

source "lib/bash/lib_file.sh"
source "lib/bash/lib_git.sh"
source "lib/bash/lib_apt.sh"
source "lib/bash/lib_fetch.sh"



cc="g++-10"
flags="-std=c++20 -Wall -pedantic -Werror -O3"



$cc -o a.out main.cpp $flags




