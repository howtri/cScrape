#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "web_scraper_utils.h"

// Test case for verifying the djb_hash function with a known input and expected output
static void test_djb_hash_known_value(void ** state) {
    (void)state; // Unused

    const char * p_input = "http://example.com";
    // WARNING: Assumed running on a 64-bit system
    size_t expected_hash = (size_t)0x8b57b087543fba60;

    size_t result = djb_hash(p_input);
    assert_int_equal(result, expected_hash);
}

// Test case for verifying the djb_hash function with an empty string
static void test_djb_hash_empty_string(void ** state) {
    (void)state; // Unused

    const char * p_input = "";
    size_t expected_hash = 5381; // Expected hash value for an empty string

    size_t result = djb_hash(p_input);
    assert_int_equal(result, expected_hash);
}

// Test case for verifying the djb_hash function with a null pointer
static void test_djb_hash_null_pointer(void ** state) {
    (void)state; // Unused

    size_t expected_hash = 0;
    size_t result = djb_hash(NULL);
    assert_int_equal(result, expected_hash);
}

// Mock function for socket
int __wrap_socket(int domain, int type, int protocol) {
    // Use mock_type and will_return from CMocka to control the behavior of this mock function
    return mock_type(int);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_djb_hash_known_value),
        cmocka_unit_test(test_djb_hash_empty_string),
        cmocka_unit_test(test_djb_hash_null_pointer),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
