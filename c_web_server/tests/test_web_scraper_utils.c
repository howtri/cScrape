#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include "web_scraper_utils.h"

// The linker expects the mocked syscalls to be present in every function, so
// unfortunately here they are even if not required.

ssize_t
__wrap_send (int sockfd, const void *buf, size_t len, int flags)
{
    check_expected(sockfd);
    check_expected(buf);
    check_expected(len);
    check_expected(flags);

    return mock_type(ssize_t);
}

// End of mocked syscalls.

// Helper function to compare expected filename result
static void
check_filename_format (const char *url,
                       const char *expected,
                       size_t      buffer_size)
{
    char buffer[buffer_size];
    int  result = util_create_filename(url, buffer, buffer_size);
    assert_int_equal(result, EXIT_SUCCESS);
    assert_string_equal(buffer, expected);
}

// Test with a valid URL
static void
test_create_filename_valid_url (void **state)
{
    (void)state; // Unused
    // Assuming util_djb_hash("http://example.com") returns a specific hash,
    // e.g., 123456
    check_filename_format(
        "http://example.com", "data/10040687989526346336.txt", 256);
}

// Test with NULL URL
static void
test_create_filename_null_url (void **state)
{
    (void)state; // Unused
    char buffer[256];
    int  result = util_create_filename(NULL, buffer, sizeof(buffer));
    assert_int_equal(result, EXIT_FAILURE);
}

// Test Empty String URL
static void
test_create_filename_empty_url (void **state)
{
    (void)state; // Unused
    char buffer[256];
    int  result = util_create_filename("", buffer, sizeof(buffer));
    // Expect success but with a specific hash for an empty string, if your hash
    // function handles it
    assert_int_equal(result, EXIT_SUCCESS);
    // Assuming util_djb_hash("") returns a specific hash, e.g., 5381
    assert_string_equal(buffer, "data/5381.txt");
}

int
main (void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_create_filename_valid_url),
        cmocka_unit_test(test_create_filename_null_url),
        cmocka_unit_test(test_create_filename_empty_url),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
