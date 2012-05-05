#!/bin/sh

ZDC="../zdc"

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

  make $test_name > /dev/null

  $ZDC $test_source_file 2> "errors_log" &&
  {
    echo -n "no errors reported ";
    return 1;
  }

  reported_errors_diff=$(diff -u $expected_errors_file errors_log)

  if [ "$reported_errors_diff" != "" ]; then
     echo "expected errors missmatch";
     diff -u errors_log $expected_errors_file;
     return 1;
  fi

  return 0
}

run_test "syntx_err" && echo "          ok" || echo "FAILED"
run_test "array_slice" && echo "        ok" || echo "FAILED"
run_test "assignment" && echo "         ok" || echo "FAILED"
run_test "unknown_symb" && echo "       ok" || echo "FAILED"
run_test "unknown_array_symb" && echo " ok" || echo "FAILED"
run_test "sym_clash" && echo "          ok" || echo "FAILED"
run_test "func_call" && echo "          ok" || echo "FAILED"
run_test "types" && echo "              ok" || echo "FAILED"
run_test "array_element" && echo "      ok" || echo "FAILED"
run_test "init_exp" && echo "           ok" || echo "FAILED"
run_test "arry_literal" && echo "       ok" || echo "FAILED"
run_test "stat_array" && echo "         ok" || echo "FAILED"
run_test "entry_point" && echo "        ok" || echo "FAILED"
run_test "bin_ops" && echo "            ok" || echo "FAILED"
run_test "exprs" && echo "              ok" || echo "FAILED"
run_test "cast" && echo "               ok" || echo "FAILED"
run_test "foreach" && echo "            ok" || echo "FAILED"
run_test "foreach_range" && echo "      ok" || echo "FAILED"
run_test "foreach_range_symbs" && echo "ok" || echo "FAILED"
run_test "module_syntx_err1" && echo "  ok" || echo "FAILED"
run_test "module_syntx_err2" && echo "  ok" || echo "FAILED"
run_test "imprt_not_found" && echo "    ok" || echo "FAILED"
run_test "imprt_syntx_err" && echo "    ok" || echo "FAILED"
run_test "ambig_imprt" && echo "        ok" || echo "FAILED"
run_test "private_imprt" && echo "      ok" || echo "FAILED"
run_test "unknown_prop" && echo "       ok" || echo "FAILED"
run_test "invalid_prop" && echo "       ok" || echo "FAILED"
run_test "invalid_returns" && echo "    ok" || echo "FAILED"
run_test "immutable" && echo "          ok" || echo "FAILED"
run_test "esc_seq" && echo "            ok" || echo "FAILED"
run_test "alias" && echo "              ok" || echo "FAILED"
run_test "usr_types_conflict" && echo " ok" || echo "FAILED"
run_test "unknown_usr_type" && echo "   ok" || echo "FAILED"
run_test "enum_init_err" && echo "      ok" || echo "FAILED"
run_test "structs" && echo "            ok" || echo "FAILED"
run_test "struct_methods" && echo "     ok" || echo "FAILED"
run_test "struct_opaque" && echo "      ok" || echo "FAILED"
run_test "inc_dec_ops" && echo "        ok" || echo "FAILED"
run_test "inv_break" && echo "          ok" || echo "FAILED"
run_test "for_loop" && echo "           ok" || echo "FAILED"
run_test "invalid_func_calls" && echo " ok" || echo "FAILED"
run_test "address_of" && echo "         ok" || echo "FAILED"
run_test "attributes" && echo "         ok" || echo "FAILED"
run_test "inv-mod-name" && echo "       ok" || echo "FAILED"
run_test "null_exprs" && echo "         ok" || echo "FAILED"
run_test "new" && echo "                ok" || echo "FAILED"
