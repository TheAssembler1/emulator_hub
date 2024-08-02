#!/bin/bash

cd build
cmake ..
make
cp compile_commands.json ../
