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
    if [[ "$2" =~ "$3" ]]; then
	echo "ok $n - $1"
    else
	echo "not ok $n - $1"
	echo "$2"
	echo "$3"
	ret=1
    fi
}

# Calls the binary and compared the test output
#
# Params:
#   $1: The test description.
#   $2: The creation input string of the binary.
#   $3: The query input string of the binary.
#   $4: The expected output.
function test_task_output() {
    creation=`mktemp`
    echo -ne "$2" > "$creation"
    queries=`mktemp`
    echo -ne "$3" > "$queries"

    # Convert the bash '\n' character to an ANSI C quoted newline
    o=$(echo -ne "$4")

    out=`$BIN $creation $queries`
    assert_string "$1" "$out" "$o"

    n=$(($n+1))
    vout=$(valgrind --error-exitcode=1 --leak-check=full --track-origins=yes $BIN $creation $queries 2>&1)
    if [[ $? -eq 0 ]]; then
        echo "ok $n - [Valgrind] $1"
    else
	echo "not ok $n - [Valgrind] $1"
	echo "$vout$"
    fi

    rm "$creation"
    rm "$queries"
}

# Tests come here

function test_usage_information() {
    out=`"$BIN"`
    assert_string "Displays usage information" "$out" "Usage: ti_programm [-du] INPUT_FILE"
}


function test_trie_creation() {
    test_task_output "Trie creation - One letter words" \
		     "a\0\nb\0\nc\0\n" \
		     "a\0c\nc\0c\nd\0c\n" \
		     "true\ntrue\nfalse"

    test_task_output "Trie creation - Non-overlapping words" \
		     "apple\0\nbanana\0\ncar\0\n" \
		     "apple\0c\ncar\0c\ndelete\0c\n" \
		     "true\ntrue\nfalse"

    test_task_output "Trie creation - Overlapping words" \
		     "apple\0\napplication\0\naprentice\0\n" \
		     "apple\0c\naprentice\0c\ndelete\0c\n" \
		     "true\ntrue\nfalse"

    test_task_output "Trie creation - Suffix words" \
		     "apple\0\napplep\0\napp\0\n" \
		     "apple\0c\napplepp\0c\nap\0c\napplep\0c\n" \
		     "true\nfalse\nfalse\ntrue"
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
