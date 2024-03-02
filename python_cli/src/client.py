"""Web Scraper Client

This file interacts with the c_web_server files. It sends tcp requests based on
CLI arguments from the user.

WARNING: Not all sites permit web scraping. Please ensure the site you pass to the
web scraper as an argument permits web scrapers to scrape its content.

Usage:

    $ python3 client.py <1/2> <http://<url>.com>
"""

import socket
import sys


def receive_full_message(sock, buffer_size=1024):
    """Receive a full message in chunks until no more data is available"""
    message = b""
    while True:
        chunk = sock.recv(buffer_size)
        message += chunk
        if len(chunk) < buffer_size:
            # There is no more data being sent based on a full buffer not being sent.
            break
    return message


def connect_to_server(server_host, server_port, message):
    """Connects to the server and sends a message.

    Args:
        server_host (str): The hostname of the server to connect to.
        server_port (int): The port number of the server to connect to.
        message (str): The message to send.
    """
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((server_host, server_port))

        # Encode the message
        encoded_message = message.encode("utf-8")

        # Send combined data to server
        s.send(encoded_message)
        print("Client: Data Sent.")

        # Receive response from the server
        data = receive_full_message(s)
        print(f"Client: Server responded: {data.decode('utf-8')}")


def main():
    """Main function."""
    if len(sys.argv) != 3:
        print("Usage: python3 cli.py <message_part1> <message_part2>")
        sys.exit(1)

    message_part1 = sys.argv[1]
    message_part2 = sys.argv[2]
    message = f"{message_part1} {message_part2}"

    server_host = socket.gethostname()
    server_port = 8082

    connect_to_server(server_host, server_port, message)


if __name__ == "__main__":
    main()
