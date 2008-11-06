#!/bin/sh

XDC=../xdc

run_checked()
{
    ($1 $2) || 
    {
        echo "ERROR: '$1 $2' failed";
        exit 1;
    }
}

run_test()
{
    local test_name

    test_name=$1

    echo -n "Compiling $test_name.d "
    run_checked $XDC $test_name.d
    echo "[ok]"
    run_checked jasmin $test_name.j
    run_checked javac "$test_name"Check.java
    run_checked java "$test_name"Check
}

run_test neg
run_test func_call
run_test bool_op
echo "_all_tests_passed_"
