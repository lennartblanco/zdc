#!/bin/sh

tests="empty "\
"only_comments "\
"rets "\
"comments "\
"uninit_vars "\
"extern_c "\
"eels "\
"neg "\
"if_else "\
"while_loop "\
"func_call "\
"fact "\
"implicit_cast "\
"bool_op "\
"nested_blocks "\
"module_test "\
"imports "\
"pkg_name_tst "\
"stat_array "\
"properties "
#"uint_op "\
#"dyn_array "\
#"foreach "\
#"char_literals "\
#"immutable "\
#"enums "\
#"str_literals "

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
    test_binary="check_"$test_name

    echo -n "Compiling $test_name "
    (make CFLAGS="-g -m32 -lgc" DFLAGS=--march=x86 $test_binary > /dev/null) ||
        { echo "error"; return 1; }
    echo "[ok]"
    (run_checked ./$test_binary) || { return 1; }

    return 0;
}

run_all_tests()
{
    local failed=0
    local passed=0

    for test_name in $tests
    do
        $RUN_TEST $test_name
        if [ "$?" = 0 ]; then
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

if [ "$1" = "--march=x86" ]; then
    RUN_TEST=run_test_x86
else
    echo "unsupported target architecture specified with option '$1'"
    exit 1
fi

if [ "$2" ]; then
    $RUN_TEST $2
    if [ "$?" = 0 ]; then
        echo "'$2' test passed"
        exit 0
    else
        echo "'$2' test failed"
        exit 1
    fi
fi


run_all_tests
exit $?
