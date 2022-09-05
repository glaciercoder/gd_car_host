from email import message
from http import server
import socket
import numpy as np
serverPort = 1088
serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverSocket.bind(('', serverPort))
print("The server is ready to receive")
while True:
    message, clientAddress = serverSocket.recvfrom(6)
    if message:
        print(message)

    serverSocket.sendto(modifiedMessage.encode(), clientAddress)