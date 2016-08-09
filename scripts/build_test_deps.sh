#!/bin/bash

ROOT=$(git rev-parse --show-toplevel)
EXTERNAL=${ROOT}/external
GTEST=${EXTERNAL}/googletest


# test for submodule existence
if [[ ! -d ${EXTERNAL}/googletest/googletest ]]; then
    pushd ${ROOT}
    git submodule init
    git submodule update
    popd
fi

pushd ${GTEST}

# build the library if it doesn't exist
if [[ ! -d "build" ]]; then
    mkdir build
    cd build
    cmake ../
    make -j8
    cd ../
fi

popd
