#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

mkdir lib

export LD_LIBRARY_PATH=$DIR/lib:$LD_LIBRARY_PATH
