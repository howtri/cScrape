#!/bin/bash

# Start client
./client.sh &

# Switch to directory the web server expects to be run from.
cd ../c_web_server/
make
bin/web_scraper &
SERVER_PID=$!

sleep 20

# Send SIGINT
kill -2 $SERVER_PID

wait $SERVER_PID

echo "Server Manual Test Complete. Please verify the output."

# Expected:
# See some sample output in server_out.txt
# Basically just not a bunch of ConnectionRefusedError: [Errno 111] Connection refused
# Large HTML outputs
# URL queued successes
# File already exists:
# etc
# ends with
# INFO: Starting Server Shutdown.