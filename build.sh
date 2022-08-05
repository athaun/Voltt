#!/usr/bin//sh

CPP_FILES=$(find src/ -type f -name '*cpp')
echo "$CPP_FILES"

set -e

clang++ -g -DNDEBUG -DVLOG -DVLOG_VERBOSE ${CPP_FILES} `llvm-config --cxxflags --ldflags --system-libs --libs core` -o voltt

echo "RUNNING voltt"
./voltt
