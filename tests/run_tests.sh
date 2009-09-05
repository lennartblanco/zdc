#!/bin/sh

XDC="../xdc"


run_checked()
{
    ($@) || 
    {
        echo "ERROR: '$@' failed"
        exit 1
    }
}

run_test_x86()
{
    local test_name

    test_name=$1
    test_binary="x86_check_"$test_name

    echo -n "Compiling $test_name.d "
    run_checked $XDC -march=x86 $test_name.d
    echo "[ok]"
    run_checked gcc -g -m32 -o $test_binary $test_name.s check_"$test_name".c \
                check_utils.c
    run_checked ./$test_binary
}

run_test_java()
{
    local test_name

    test_name=$1

    rm -f $test_name.j
    rm -f $test_name.class
    echo -n "Compiling $test_name.d "
    run_checked $XDC -march=java $test_name.d
    echo "[ok]"
    run_checked jasmin $test_name.j
    run_checked javac "$test_name"Check.java
    run_checked java "$test_name"Check
}

run_all_tests()
{
    $RUN_TEST empty
    $RUN_TEST rets
    $RUN_TEST only_comments
    $RUN_TEST comments
    $RUN_TEST neg
    $RUN_TEST func_call
    $RUN_TEST implicit_cast
    $RUN_TEST bool_op
    $RUN_TEST nested_blocks
    $RUN_TEST if_else
    $RUN_TEST fact
    $RUN_TEST while_loop
    $RUN_TEST func
    $RUN_TEST stat_array
    $RUN_TEST foreach
    echo "_all_tests_passed_"
}

if [ "$1" = "--march=java" ]; then
    RUN_TEST=run_test_java
elif [ "$1" = "--march=x86" ]; then
    RUN_TEST=run_test_x86
else
    echo "unsupported target architecture specified with option '$1'"
    exit 1
fi

$HEJ $ARCH

if [ "$2" ]; then
    $RUN_TEST $2
    echo "'$2' test passed"
    exit 0
fi

run_all_tests
