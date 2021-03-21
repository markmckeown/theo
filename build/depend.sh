#!/bin/sh

# This comes from the "Recursive Make Considered Harmful" paper.
OS=$(uname)

DIR="$1"
shift 1

if [ "${OS}" = "Darwin" ]; then
        CC=/usr/local/bin/gcc-5
else
        CC=gcc
fi

case "$DIR" in
        "" | ".")
                ${CC} -MM -MG "$@" | sed -e 's@^\(.*\)\.o:@\1.dep \1.o:@'
                ;;
        *)
                ${CC} -MM -MG "$@" | \
                        sed -e "s@^\(.*\)\.o:@$DIR/\1.dep $DIR/\1.o:@"
                ;;
esac

