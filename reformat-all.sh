#!/bin/bash

find . -name '*.h' -o -name '*.c' | xargs clang-format -i 

