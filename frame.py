from operator import mod
from traceback import FrameSummary
import numpy as np

class Frame:
    def __init__(self, ID) -> None:
        self.data_size = 8
        self.data = np.zeros(self.data_size, dtype = np.int8)
        self.ID = ID
        
    ### Generate the steering command
    def ADCmd_gen(self, speed, steer, auto_driving, stop, mode):
        speed = float(speed)
        steer = float(steer)
        mode = int(mode)
        speed = np.int16(speed * 100)
        steer = np.int16(steer * 100)
        self.data[0] = np.bitwise_and(speed, 0x00FF)
        self.data[1] = np.right_shift(np.bitwise_and(speed, 0xFF00), 8)
        self.data[2] = np.bitwise_and(steer, 0x00FF)
        # print(np.binary_repr(np.bitwise_and(steer, 0x00FF)))
        self.data[3] = np.right_shift(np.bitwise_and(steer, 0xFF00), 8)
        self.data[4] = np.bitwise_or(np.bitwise_or(np.left_shift(stop, 2), auto_driving), np.left_shift(mode, 4)) 
    
    def ADCmd_decode(self):
        speed = np.int16(0)
        steer = np.int16(0)
        # print(np.binary_repr(1012, width=16))
        speed = np.bitwise_or(np.left_shift(self.data[1], 8), np.bitwise_and(self.data[0], 0x00FF))
        steer = np.bitwise_or(np.left_shift(self.data[3], 8), np.bitwise_and(self.data[2], 0x00FF))
        # print(np.binary_repr(steer, width=16))
        auto_driving = np.bitwise_and(0x03, self.data[4]) 
        stop = np.right_shift(np.bitwise_and(0x0C, self.data[4]), 2)
        mode = np.right_shift(np.bitwise_and(0xC0, self.data[4]), 4)
        return speed, steer, auto_driving, stop, mode

    def display_message(self):
        message = ''
        for i in range(0, 8):
            message = message + ' ' + str(np.uint8(self.data[i]))
        print(message)

# frame = Frame(0x100)
# frame.ADCmd_gen('1.3', '1',  0, 0, 4)
# frame = Frame(0x100)
# frame.ADCmd_gen(-1.3, -10.12, 0, 0, 4)
# frame.display_message()
# frame.ADCmd_decode()