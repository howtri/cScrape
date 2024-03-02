#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "web_scraper_queue.h"

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
test_queue_create_destroy (void **state)
{
    (void)state; // Unused parameter

    queue_t *p_queue = queue_create();
    assert_non_null(p_queue);

    queue_destroy(&p_queue);
    assert_null(p_queue);
}

static void
test_queue_enqueue_dequeue (void **state)
{
    (void)state; // Unused parameter

    queue_t *p_queue = queue_create();
    assert_non_null(p_queue);

    // Enqueue test URLs
    char *p_url1 = "http://example.com/1";
    char *p_url2 = "http://example.com/2";
    char *p_url3 = "http://example.com/3";

    assert_int_equal(queue_enqueue(p_queue, p_url1, strlen(p_url1)),
                     EXIT_SUCCESS);
    assert_int_equal(queue_enqueue(p_queue, p_url2, strlen(p_url2)),
                     EXIT_SUCCESS);
    assert_int_equal(queue_enqueue(p_queue, p_url3, strlen(p_url3)),
                     EXIT_SUCCESS);

    // Dequeue and check the URLs in order
    char *dequeued_url_1 = queue_dequeue(p_queue);
    assert_string_equal(dequeued_url_1, p_url1);
    free(dequeued_url_1);

    char *dequeued_url_2 = queue_dequeue(p_queue);
    assert_string_equal(dequeued_url_2, p_url2);
    free(dequeued_url_2);

    char *dequeued_url_3 = queue_dequeue(p_queue);
    assert_string_equal(dequeued_url_3, p_url3);
    free(dequeued_url_3);

    // Check the queue is empty
    assert_null(queue_dequeue(p_queue));

    queue_destroy(&p_queue);
}

static void
test_queue_empty_after_last_dequeue (void **state)
{
    (void)state; // Unused parameter

    queue_t *p_queue = queue_create();
    assert_non_null(p_queue);

    char *p_url = "http://example.com";
    assert_int_equal(queue_enqueue(p_queue, p_url, strlen(p_url)),
                     EXIT_SUCCESS);

    char *p_dequeuedUrl = queue_dequeue(p_queue);
    assert_string_equal(p_dequeuedUrl, p_url);
    free(p_dequeuedUrl);

    // queue should be empty.
    assert_null(queue_dequeue(p_queue));

    queue_destroy(&p_queue);
}

int
main (void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_queue_create_destroy),
        cmocka_unit_test(test_queue_enqueue_dequeue),
        cmocka_unit_test(test_queue_empty_after_last_dequeue),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

// end of file.
