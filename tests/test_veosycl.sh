#!/bin/bash

# This script should be placed in the top level of veo-sycl-bench

set -eu

for i in sycl/*; do
    name=$(basename "$i")
    # We skip the lin_reg_coeff benchmark for now
    if [ "$name" == "include" -o "$name" == "lin_reg_coeff" ]; then
        continue
    fi

    pushd "$i"

    # Patch source code to use VE
    sed -i -e 's/queue q;/queue q(ve_selector{});/g' "$name.cpp"
    sed -i -e 's/queue device_queue;/queue device_queue(ve_selector{});/g' "$name.cpp"
    sed -i -e 's/bitmap.h/..\/include\/bitmap.h/g' "$name.cpp"

    # Compile benchmark
    # If using veo-stubs, set DEVICE_COMPILER to a host compiler (e.g. g++)
    nsc++.py "$name.cpp"

    # Run benchmark
    # If using veo-stubs, SPDLOG_LEVEL=debug will enable verbose logging
    ./a.out 10

    popd
done
