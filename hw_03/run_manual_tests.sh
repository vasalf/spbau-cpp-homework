#!/bin/bash

for test in $(ls manual_tests/*.in); do
    out=$(echo $test | sed 's/.in/.huf/')
    res=$(echo $test | sed 's/.in/.res/')
    echo "Running test $test"
    ./huffman -c -f $test -o $out
    ./huffman -u -f $out  -o $res
    diff $test $res || exit -1
done

echo "OK"
