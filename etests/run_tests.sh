#!/bin/sh

XDC="../xdc"

run_test()
{
  local test_name
  local test_res
  local test_file
  local test_source_file
  local expected_errors_file
  local reported_errors_diff

  test_name=$1

  test_res=0
  test_file="$test_name.inv_d"
  test_source_file="$test_name.d"
  expected_errors_file="$test_name.expected_errs"

  echo -n "[$test_name] "

  cp $test_file $test_source_file || return 1;

  $XDC $test_source_file 2> "errors_log" && 
  {
    echo -n "no errors reported ";
    return 1;
  }

  reported_errors_diff=$(diff -u errors_log $expected_errors_file)

  if [ "$reported_errors_diff" != "" ]; then
     echo "expected errors missmatch";
     diff -u errors_log $expected_errors_file;
     return 1;
  fi

  return 0
}

# TODO make a loop over all *.inv_d files instead
run_test "array_slice" && echo "  ok" || echo "FAILED"
run_test "assigment" && echo "    ok" || echo "FAILED"
run_test "sym_clash" && echo "    ok" || echo "FAILED"
run_test "func_call" && echo "    ok" || echo "FAILED"
run_test "types" && echo "        ok" || echo "FAILED"
run_test "array_element" && echo "ok" || echo "FAILED"
