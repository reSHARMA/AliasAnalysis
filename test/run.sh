#!/bin/bash

CC="$LLVM_HOME/bin/clang"

for x in src/*; do
    $CC -S -emit-llvm -o /tmp/test.ll $x
    AADriver /tmp/test.ll
done
