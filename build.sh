#!/usr/bin//sh

COMP_DIR=$(pwd)

cd $(pwd)/src
SRC_VAR=""

for f in *.cpp; do
    echo "COMPILING /src/${f}"
    SRC_VAR+="src/"
    SRC_VAR+="${f} "
done

cd $COMP_DIR

set -e

clang++ -g -DNDEBUG -DVLOG -DVLOG_VERBOSE ${SRC_VAR} `llvm-config --cxxflags --ldflags --system-libs --libs core` -o voltt

echo "RUNNING voltt"
./voltt
