import socket
import struct


def connect_to_server(server_host, server_port, message_id, message):
    """Connects to the server and sends a message.

    Args:
        server_host (str): The hostname of the server to connect to.
        server_port (int): The port number of the server to connect to.
        message_id (int): The message ID to send.
        message (str): The message to send.

    Returns:
        str: The response from the server.
    """
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((server_host, server_port))

        # Encode the message
        encoded_message = message.encode("utf-8")

        # Send combined data to server
        s.send(encoded_message)
        print("Client: Data Sent.")


def main():
    """Main function."""
    server_host = socket.gethostname()
    server_port = 8082
    message_id = 123
    message = input("Enter the message in format <number> <url>")

    response = connect_to_server(server_host, server_port, message_id, message)
    print(f"Client: Server responded: {response}")


if __name__ == "__main__":
    main()
