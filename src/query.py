import socket

# DNS query for "example.com"
query = b"\x12\x34\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\x07example\x03com\x00\x00\x01\x00\x01"

# Send the query to the server
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(query, ("127.0.0.1", 5354))

# Receive the response
response, _ = sock.recvfrom(1024)
print("Response:", response)
