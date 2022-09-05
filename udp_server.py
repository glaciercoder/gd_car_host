from email import message
from http import server
import socket
import numpy as np
from frame import Frame
serverPort = 1088
serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverSocket.bind(('', serverPort))
print("The server is ready to receive")
while True:
    message, clientAddress = serverSocket.recvfrom(2048)
    if message:
        frame = Frame(0x100)
        for i in range(0, 5):
            frame.data[i] = message[i]
        frame.display_message()