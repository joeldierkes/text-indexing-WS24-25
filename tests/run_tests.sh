#!/usr/bin/env bash

BASEDIR=$(dirname "$(realpath $0)")
WORK_DIR=$(mktemp -d)
BIN_FOLDER=$(dirname $BASEDIR)
BIN="$BIN_FOLDER/ti_programm"

# check if tmp dir was created
if [[ ! "$WORK_DIR" || ! -d "$WORK_DIR" ]]; then
  echo "Could not create temp dir"
  exit 1
fi

# deletes the temp directory
CURR_DIR=`pwd`
function cleanup {
  cd "$CURR_DIR"
  rm -rf "$WORK_DIR"
}

# register the cleanup function to be called on the EXIT signal
trap cleanup EXIT

cd "$WORK_DIR"
n=0
ret=0
echo "TAP version 14"

function assert_string() {
    n=$(($n+1))
    if [[ $2 =~ $3 ]]; then
	echo "ok $n - $1"
    else
	echo "not ok $n - $1"
	echo "$2"
	ret=1
    fi
}

# Tests come here

function test_usage_information() {
    out=`"$BIN"`
    assert_string "Displays usage information" "$out" "Usage: ti_programm \[-du\] INPUT_FILE"
}


function test_trie_creation() {
    creation=`mktemp`
    echo "apple\0\napplep\0\napp\0\n" > $creation
    queries=`mktemp`
    echo "apple\0c\napplepp\0c\nap\0c\n" > $queries

    out=`"$BIN" "$creation $queries"`
    assert_string "Trie creation" "$out" ""

    rm "$creation"
    rm "$queries"
}

function test_trie_insertion() {
    file=`mktemp`
    echo "apple\0\n" > $file

    out=`"$BIN" "$file"`
    assert_string "Trie insertion" "$out" ""

    rm "$file"
}

function test_trie_contains() {
    file=`mktemp`
    echo "apple\0\n" > $file

    out=`"$BIN" "$file"`
    assert_string "Trie contains" "$out" ""

    rm "$file"
}

test_usage_information
test_trie_creation
test_trie_insertion
test_trie_contains

echo "1..$n"
exit $ret
