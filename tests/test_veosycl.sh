#!/bin/bash

# This script should be placed in the top level of veo-sycl-bench

set -eu

DEVICE="$1"

for i in sycl/*; do
    name=$(basename "$i")
    # We skip the lin_reg_coeff benchmark for now
    if [ "$name" == "include" -o "$name" == "lin_reg_coeff" ]; then
        continue
    fi

    pushd "$i"

    # Compile benchmark

    if [ "$DEVICE" == "host" ]; then
        g++ -std=c++17 -I../include -o a.out "$name.cpp" -lpthread
    else
        # If using veo-stubs, set DEVICE_COMPILER to a host compiler (e.g. g++)
        nsc++.py -I../include --device "$DEVICE" --debug "$name.cpp"
    fi

    # Run benchmark
    ./a.out 10

    popd
done
