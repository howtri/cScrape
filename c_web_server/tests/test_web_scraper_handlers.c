#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>
#include "web_scraper_handlers.h"
#include "web_scraper_queue.h"

static void test_handle_scrape_new_request_valid_url(void **state) {
    (void)state; // Unused

    queue_t * queue = queue_create();
    char *valid_url = "http://example.com";

    int result = handle_scrape_new_request(valid_url, queue);
    assert_int_equal(result, EXIT_SUCCESS);

    queue_destroy(&queue);
}

static void test_handle_scrape_new_request_invalid_scheme(void **state) {
    (void)state; // Unused

    queue_t *queue = queue_create();
    char *invalid_url = "ftp://example.com";

    assert_int_equal(handle_scrape_new_request(invalid_url, queue), EXIT_FAILURE);

    queue_destroy(&queue);
}

static void test_handle_scrape_new_request_null_arguments(void **state) {
    (void)state; // Unused

    assert_int_equal(handle_scrape_new_request(NULL, NULL), EXIT_FAILURE);
}

static void test_handle_scrape_new_request_empty_url(void **state) {
    (void)state; // Unused

    queue_t *queue = queue_create();
    char *empty_url = "";

    assert_int_equal(handle_scrape_new_request(empty_url, queue), EXIT_FAILURE);

    queue_destroy(&queue);
}

// Mock function for socket
int __wrap_socket(int domain, int type, int protocol) {
    // Use mock_type and will_return from CMocka to control the behavior of this mock function
    return mock_type(int);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_handle_scrape_new_request_valid_url),
        cmocka_unit_test(test_handle_scrape_new_request_invalid_scheme),
        cmocka_unit_test(test_handle_scrape_new_request_null_arguments),
        cmocka_unit_test(test_handle_scrape_new_request_empty_url),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
