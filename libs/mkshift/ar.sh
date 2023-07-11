#!/bin/bash
#
# This file is a wrapper around AR to fix .a files.

if [ $1 = "" ]; then
    echo "usage: AR ARFLAGS DESTINATION ARCHIVES..."
    exit 1
fi

# get every argument after the first one

CURDIR=$(pwd)
TMPDIR=$(mktemp -d)
AR=$1; shift
ARFLAGS=$1; shift
DEST=$1; shift

cp $@ $TMPDIR

cd $TMPDIR

for archive in $(find -type f); do
    case $archive in
        *.a)
            ar x $archive
            rm $archive
            ;;
        *.o)
            ;;
        *)
            echo "Unknown file '$archive'"
            exit
            ;;
    esac
done

$AR $ARFLAGS $DEST $(find -type f)

cd $CURDIR
cp $TMPDIR/$DEST $CURDIR/$DEST

rm -r $TMPDIR
