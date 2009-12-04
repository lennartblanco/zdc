#!/bin/sh

XDC="../xdc"


run_checked()
{
    ($@) || 
    {
        echo "ERROR: '$@' failed"
        return 1
    }

    return 0;
}

run_test_x86()
{
    local test_name

    test_name=$1
    test_binary="x86_check_"$test_name

    echo -n "Compiling $test_name.d "
    (run_checked $XDC -march=x86 -S $test_name.d) || { return 1; }
    echo "[ok]"
    (run_checked gcc -g -m32 -o $test_binary $test_name.s check_"$test_name".c \
                check_utils.c) || { return 1; }
    (run_checked ./$test_binary) || { return 1; }

    return 0;
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
    local failed=0
    local passed=0

    local tests="empty rets only_comments comments neg func_call "\
"implicit_cast bool_op uint_op nested_blocks if_else fact extern_c "\
"while_loop stat_array foreach"

    for test_name in $tests
    do
        $RUN_TEST $test_name
        if [ "$?" == 0 ]; then
            passed=$(($passed + 1));
        else
            failed=$(($failed + 1));
        fi
    done

    echo "$(($passed + $failed)) TEST SUITES RUN, $passed PASSED $failed FAILED"

    if [ $failed != 0 ]; then
       return 1
    fi

    return 0
}

if [ "$1" = "--march=java" ]; then
    RUN_TEST=run_test_java
elif [ "$1" = "--march=x86" ]; then
    RUN_TEST=run_test_x86
else
    echo "unsupported target architecture specified with option '$1'"
    exit 1
fi

if [ "$2" ]; then
    $RUN_TEST $2
    echo "'$2' test passed"
    exit 0
fi


run_all_tests
exit $?
