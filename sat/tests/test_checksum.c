#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <check.h>
#include <static/checksum.h>

#define TST_CHKSUM_BUFFSIZE (64)

void setup(void) {
  /* NULL */
}

void teardown(void) {
  /* NULL */
}

START_TEST(test_xor_simple_case_1) {
  char test_buff[4] = {0x00, 0x00, 0x00, 0x00};

  ck_assert_int_eq(compute_XOR_checksum(test_buff, 4), 0x00);
}
END_TEST

START_TEST(test_xor_simple_case_2) {
  char test_buff[2] = {0x0F, 0xF0};

  ck_assert_int_eq(compute_XOR_checksum(test_buff, 2), 0xFF);
}
END_TEST

START_TEST(test_xor_simple_null_case) {
  char test_buff[16] = {0x01, 0x10, 0x02, 0x20, 
                        0x04, 0x40, 0x08, 0x80, 
                        0x0A, 0xA0, 0x0B, 0xB0, 
                        0x0C, 0xC0, 0x0D, 0xD0};

  ck_assert_int_eq(compute_XOR_checksum(NULL, 16), 0x00);
}
END_TEST

Suite * make_sample_creation_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Sample Creation Test Suite");

  /* Creation test case */
  tc_core = tcase_create("Test Cases with Setup and Teardown");

  tcase_add_checked_fixture(tc_core, setup, teardown);
  tcase_add_test(tc_core, test_xor_simple_case_1);
  tcase_add_test(tc_core, test_xor_simple_case_2);
  tcase_add_test(tc_core, test_xor_simple_null_case);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  SRunner *sr;

  sr = srunner_create(make_sample_creation_suite());
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_set_log (sr, "test.log");
  srunner_set_xml (sr, "test.xml");
  srunner_run_all(sr, CK_VERBOSE);

  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
