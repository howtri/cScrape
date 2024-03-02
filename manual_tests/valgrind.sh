#!/bin/bash

# Start client
./client.sh &

# Switch to directory the web server expects to be run from.
cd ../c_web_server/
make
# Run valgrind
valgrind --leak-check=full --show-leak-kinds=all bin/web_scraper &
SERVER_PID=$!

sleep 20

# Send SIGINT
kill -2 $SERVER_PID

wait $SERVER_PID

echo "Valgrind Manual Test Complete. Please verify their are no possible memory leaks."

# Expect:
# ==5671== HEAP SUMMARY:
# ==5671==     in use at exit: 0 bytes in 0 blocks
# ==5671==   total heap usage: 54 allocs, 54 frees, 17,630 bytes allocated
# ==5671==
# ==5671== All heap blocks were freed -- no leaks are possible
# ==5671==
# ==5671== For lists of detected and suppressed errors, rerun with: -s
# ==5671== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
# Valgrind Complete. Please verify their are no possible memory leaks.