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
    if [[ "$3" =~ "$4" ]]; then
    	echo "ok $n - Variante $2 $1"
    else
	    echo "not ok $n - Variante $2 $1"
	    echo "Expected: $4"
	    echo "Got: $3"
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
  test_task_output_with_variant "$1" "1" "$2" "$3" "$4"
  test_task_output_with_variant "$1" "2" "$2" "$3" "$4"
  test_task_output_with_variant "$1" "3" "$2" "$3" "$4"
}

# Calls the binary and compared the test output
#
# Params:
#   $1: The test description.
#   $2: The variant to run.
#   $3: The creation input string of the binary.
#   $4: The query input string of the binary.
#   $5: The expected output.
function test_task_output_with_variant() {
    creation=`mktemp`
    echo -ne "$3" > "$creation"
    queries=`mktemp`
    echo -ne "$4" > "$queries"

    # Convert the bash '\n' character to an ANSI C quoted newline
    o=$(echo -ne "$5")

    out=`$BIN "-variante=$2" -p $creation $queries`
    assert_string "$1" "$2" "$out" "$o"

    n=$(($n+1))
    vout=$(valgrind --error-exitcode=1 --leak-check=full --track-origins=yes $BIN "-variante=$2" -p $creation $queries 2>&1)
    if [[ $? -eq 0 ]]; then
        echo "ok $n - Variante $2 [Valgrind] $1"
    else
  	  echo "not ok $n - Variante $2 [Valgrind] $1"
	    echo "$vout$"
    fi

    rm "$creation"
    rm "$queries"
}

# Tests come here

function test_usage_information() {
    out=`"$BIN"`
    assert_string "Displays usage information" "1" "$out" "Usage: ti_programm [-tpdu] -variante=n INPUT_FILE"

    out=$(valgrind --error-exitcode=1 --leak-check=full --track-origins=yes "$BIN" 2>&1)
    assert_string "[Valgrind] Displays usage information" "1" "$out" "Usage: ti_programm [-tpdu] -variante=n INPUT_FILE"

    out=`"$BIN"`
    assert_string "Displays usage information" "2" "$out" "Usage: ti_programm [-tpdu] -variante=n INPUT_FILE"

    out=$(valgrind --error-exitcode=1 --leak-check=full --track-origins=yes "$BIN" 2>&1)
    assert_string "[Valgrind] Displays usage information" "2" "$out" "Usage: ti_programm [-tpdu] -variante=n INPUT_FILE"


    out=`"$BIN"`
    assert_string "Displays usage information" "3" "$out" "Usage: ti_programm [-tpdu] -variante=n INPUT_FILE"

    out=$(valgrind --error-exitcode=1 --leak-check=full --track-origins=yes "$BIN" 2>&1)
    assert_string "[Valgrind] Displays usage information" "3" "$out" "Usage: ti_programm [-tpdu] -variante=n INPUT_FILE"
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
    test_task_output "Trie insertion - One letter words" \
		     "a\0\nb\0\nc\0\n" \
		     "a\0i\nd\0i\ne\0i\nb\0i\n" \
		     "false\ntrue\ntrue\nfalse"

    test_task_output "Trie insertion - Non-overlapping words" \
		     "apple\0\nbaby\0\nclass\0\n" \
		     "apple\0i\ndab\0i\nextacy\0i\nbaby\0i\n" \
		     "false\ntrue\ntrue\nfalse"

    test_task_output "Trie insertion - Overlapping words" \
		     "apple\0\nbaby\0\nclass\0\n" \
		     "apple\0i\nappletree\0i\nclassic\0i\nbaby\0i\n" \
		     "false\ntrue\ntrue\nfalse"

    test_task_output "Trie insertion - Overlapping words" \
		     "apple\0\nbaby\0\nclass\0\n" \
		     "apple\0i\napp\0i\ncla\0i\nbaby\0i\n" \
		     "false\ntrue\ntrue\nfalse"
}

function test_trie_contains() {
    test_task_output "Trie contains - One letter words" \
		     "a\0\nb\0\nc\0\n" \
		     "a\0c\nd\0c\nb\0c\n" \
	             "true\nfalse\ntrue\n"

    test_task_output "Trie contains - Non-overlapping words" \
		     "apple\0\nbaby\0\nclass\0\n" \
                     "apple\0c\ndelete\0c\nbaby\0c\n" \
                     "true\nfalse\ntrue\n"
}

function test_trie_delete() {
    test_task_output "Trie delete - One letter words" \
		     "a\0\nb\0\nc\0\n" \
		     "a\0d\nd\0d\na\0d\na\0c\n" \
                     "true\nfalse\nfalse\nfalse\n"

    test_task_output "Trie delete - Non-overlapping words" \
		     "apple\0\nbaby\0\nclass\0\n" \
		     "apple\0d\napple\0d\ndelete\0d\nbaby\0d\n" \
                     "true\nfalse\nfalse\ntrue\n"

    test_task_output "Trie delete - Overlapping words" \
		     "apple\0\nappletree\0\nbaby\0\nclass\0\n" \
		     "apple\0d\napple\0d\nappletree\0d\ndelete\0d\nbaby\0d\n" \
                     "true\nfalse\ntrue\nfalse\ntrue\n"
}

function test_trie_dollar() {
    test_task_output "Trie dollar creation - One letter words" \
		     "a\$\nb\$\nc\$\n" \
		     "a\$c\nc\$c\nd\$c\n" \
		     "true\ntrue\nfalse"

    test_task_output "Trie dollar big test" \
		     "apple\$\nbook\$\ncfddf\$\napp\$\nseven\$\nap\$\nboo\$\nbooklets\$\n" \
		     "apple\$c\nap\$c\napp\$c\napple\$d\napple\$c\n" \
		     "true\ntrue\ntrue\ntrue\nfalse"

    test_task_output "Trie dollar big big test" \
		     "apple\$\nbook\$\ncfddf\$\napp\$\nseven\$\nap\$\nboo\$\nbooklets\$\napple\$\n" \
		     "apple\$c\nap\$c\napp\$c\napple\$d\napple\$c\napple\$i\napple\$i\n" \
		     "true\ntrue\ntrue\ntrue\nfalse\ntrue\nfalse"
}

function test_trie_commands_with_space() {
    test_task_output "Trie command with space creation - One letter words" \
		     "a\$\nb\$\nc\$\n" \
		     "a\$ c\nc\$ c\nd\$ c\n" \
		     "true\ntrue\nfalse"
}

test_usage_information
test_trie_creation
test_trie_insertion
test_trie_contains
test_trie_delete
test_trie_dollar
test_trie_commands_with_space

echo "1..$n"
exit $ret

}
