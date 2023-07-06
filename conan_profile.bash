#!/bin/bash

echo "Creating package profile"
conan profile new package --detect
conan profile update env.CXX=clang++-9 default
conan profile show package
