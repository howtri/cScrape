#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h> 
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <netinet/in.h>
#include "web_scraper_server.h"

// Test for initialize_server_address function
static void test_initialize_server_address(void **state) {
    (void)state; // Unused parameter

    struct sockaddr_in address = initialize_server_address();

    assert_int_equal(address.sin_family, AF_INET);
    assert_int_equal(address.sin_addr.s_addr, INADDR_ANY);
    assert_int_equal(ntohs(address.sin_port), PORT);
}

// Mock function for socket
int __wrap_socket(int domain, int type, int protocol) {
    // Use mock_type and will_return from CMocka to control the behavior of this mock function
    return mock_type(int);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_initialize_server_address),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

// end of file.
