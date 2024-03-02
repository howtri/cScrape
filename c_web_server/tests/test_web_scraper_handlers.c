#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include "web_scraper_queue.h" // Assuming this is where queue_t and related functions are defined.
#include "web_scraper_handlers.h"

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

static void
test_handle_scrape_valid_url (void **state)
{
    (void)state; // Unused

    queue_t *p_queue = queue_create();
    assert_non_null(p_queue);

    char *valid_url       = "http://example.com";
    int   dummy_socket_fd = 42;

    // Expectations for __wrap_send
    expect_value(__wrap_send, sockfd, dummy_socket_fd);
    expect_string(__wrap_send, buf, "SUCCESS: URL queued to be scraped.\n");
    expect_value(
        __wrap_send, len, strlen("SUCCESS: URL queued to be scraped.\n"));
    expect_value(__wrap_send, flags, 0);
    will_return(
        __wrap_send,
        strlen("SUCCESS: URL queued to be scraped.\n")); // Mock successful send

    assert_int_equal(
        handle_scrape_new_request(dummy_socket_fd, valid_url, p_queue),
        EXIT_SUCCESS);

    // Clean up
    queue_destroy(&p_queue);
}

static void
test_handle_scrape_invalid_url (void **state)
{
    (void)state; // Unused

    queue_t *p_queue = queue_create();
    assert_non_null(p_queue);

    char *invalid_url     = "ftp://example.com";
    int   dummy_socket_fd = 42; // Dummy socket file descriptor for testing

    // No expectations for __wrap_send since it should not be called

    assert_int_equal(
        handle_scrape_new_request(dummy_socket_fd, invalid_url, p_queue),
        EXIT_FAILURE);

    // Clean up
    queue_destroy(&p_queue);
}

static void
test_handle_invalid_request_success (void **state)
{
    (void)state; // Unused

    // Expecting send to be called with the specific message
    expect_value(__wrap_send, sockfd, 42);
    expect_string(
        __wrap_send, buf, "SUCCESS: Invalid option or message, expected <1 or 2> <url>.\n");
    expect_value(__wrap_send,
                 len,
                 strlen("SUCCESS: Invalid option or message, expected <1 or 2> <url>.\n"));
    expect_value(__wrap_send, flags, 0);
    will_return(__wrap_send,
                strlen("SUCCESS: Invalid option or message, expected <1 or 2> <url>.\n"));

    assert_int_equal(handle_invalid_request(42), EXIT_SUCCESS);
}

int
main (void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_handle_scrape_valid_url),
        cmocka_unit_test(test_handle_scrape_invalid_url),
        cmocka_unit_test(test_handle_invalid_request_success),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
