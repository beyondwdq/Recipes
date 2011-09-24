#!/usr/bin/env bash
set -e

src=$1

basename=${src%.*}

set -x

g++ -c $src -Wall -o $basename.o 

g++ $basename.o keygen.o libleveldb.a -o $basename

