#!/bin/bash

ROOT=$(git rev-parse --show-toplevel)
EXTERNAL=${ROOT}/external
GTEST=${EXTERNAL}/googletest

pushd ${GTEST}

if [[ ! -d "build" ]]; then
    mkdir build
    cd build
    cmake ../
    make -j8
    cd ../
fi

popd
