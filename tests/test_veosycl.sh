#!/bin/bash

set -eu

for i in sycl/*; do
    name=$(basename "$i")
    if [ "$name" == "include" -o "$name" == "lin_reg_coeff" ]; then
        continue
    fi

    pushd "$i"

    sed -i -e 's/queue q;/queue q(ve_selector{});/g' "$name.cpp"
    sed -i -e 's/queue device_queue;/queue device_queue(ve_selector{});/g' "$name.cpp"
    sed -i -e 's/bitmap.h/..\/include\/bitmap.h/g' "$name.cpp"
    nsc++.py "$name.cpp"
    ./a.out 10

    popd
done
