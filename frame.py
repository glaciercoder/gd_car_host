from operator import mod
from traceback import FrameSummary
import numpy as np
import socket 

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
        mode = np.right_shift(np.bitwise_and(0x30, self.data[4]), 4)
        return speed, steer, auto_driving, stop, mode
    ###Generate the end command
    def End_gen(self, speed, position, displacement, enable):
        if self.ID == 0x107:
            speed = np.int16(speed * 1000)  #for precision is 0.0003,there is a problem that int16 is not enough for a num between -10 and 10 under the precision of 0.0003
            position = np.int16(position * 100) #for precision is 0.0064 
            displacement = np.int8(displacement)
            enable = np.int8(enable)
            self.data[0] = np.bitwise_and(speed, 0x00FF)
            self.data[1] = np.right_shift(np.bitwise_and(speed, 0xFF00), 8)
            self.data[2] = np.bitwise_and(position,0x00FF)
            self.data[3] = np.right_shift(np.bitwise_and(position, 0xFF00), 8)
            self.data[4] = displacement + 2 * enable
        if self.ID == 0x108:
            speed = np.int16(speed * 1000)  
            position = np.int16(position * 100) 
            displacement = np.int8(displacement)
            enable = np.int8(enable)
            self.data[0] = np.bitwise_and(speed, 0x00FF)
            self.data[1] = np.right_shift(np.bitwise_and(speed, 0xFF00), 8)
            self.data[2] = np.bitwise_and(position,0x00FF)
            self.data[3] = np.right_shift(np.bitwise_and(position, 0xFF00), 8)
            self.data[4] = displacement + 2 * enable
        if self.ID == 0x109:
            speed = np.int16(speed * 1000)  
            position = np.int16(position * 100) 
            displacement = np.int8(displacement)
            enable = np.int8(enable)
            self.data[0] = np.bitwise_and(speed, 0x00FF)
            self.data[1] = np.right_shift(np.bitwise_and(speed, 0xFF00), 8)
            self.data[2] = np.bitwise_and(position,0x00FF)
            self.data[3] = np.right_shift(np.bitwise_and(position, 0xFF00), 8)
            self.data[4] = displacement + 2 * enable
        if self.ID == 0x10A:
            speed = np.int16(speed * 1000)  
            position = np.int16(position * 100) 
            displacement = np.int8(displacement)
            enable = np.int8(enable)
            self.data[0] = np.bitwise_and(speed, 0x00FF)
            self.data[1] = np.right_shift(np.bitwise_and(speed, 0xFF00), 8)
            self.data[2] = np.bitwise_and(position,0x00FF)
            self.data[3] = np.right_shift(np.bitwise_and(position, 0xFF00), 8)
            self.data[4] = displacement + 2 * enable
        if self.ID == 0x10B:
            speed = np.int16(speed * 1000)  
            position = np.int16(position * 100) 
            displacement = np.int8(displacement)
            enable = np.int8(enable)
            self.data[0] = np.bitwise_and(speed, 0x00FF)
            self.data[1] = np.right_shift(np.bitwise_and(speed, 0xFF00), 8)
            self.data[2] = np.bitwise_and(position,0x00FF)
            self.data[3] = np.right_shift(np.bitwise_and(position, 0xFF00), 8)
            self.data[4] = displacement + 2 * enable
        if self.ID == 0x10C:
            speed = np.int16(speed * 1000)  
            position = np.int16(position * 100) 
            displacement = np.int8(displacement)
            enable = np.int8(enable)
            self.data[0] = np.bitwise_and(speed, 0x00FF)
            self.data[1] = np.right_shift(np.bitwise_and(speed, 0xFF00), 8)
            self.data[2] = np.bitwise_and(position,0x00FF)
            self.data[3] = np.right_shift(np.bitwise_and(position, 0xFF00), 8)
            self.data[4] = displacement + 2 * enable
        
        


    def End_decode(self):
        if self.ID == 0x107:
            speed = np.int16(0)
            position = np.int16(0)
            displacement = np.int8(0)
            enable = np.int8(0)
            speed = np.bitwise_or(np.left_shift(self.data[1], 8), np.bitwise_and(self.data[0], 0x00FF))
            position = np.bitwise_or(np.left_shift(self.data[3],8),np.bitwise_and(self.data[2],0x00FF))
            displacement = np.bitwise_and(self.data[4],0x0001)
            enable = self.data[4] - displacement
        if self.ID == 0x108:
            speed = np.int16(0)
            position = np.int16(0)
            displacement = np.int8(0)
            enable = np.int8(0)
            speed = np.bitwise_or(np.left_shift(self.data[1], 8), np.bitwise_and(self.data[0], 0x00FF))
            position = np.bitwise_or(np.left_shift(self.data[3],8),np.bitwise_and(self.data[2],0x00FF))
            displacement = np.bitwise_and(self.data[4],0x0001)
            enable = self.data[4] - displacement
        if self.ID == 0x109:
            speed = np.int16(0)
            position = np.int16(0)
            displacement = np.int8(0)
            enable = np.int8(0)
            speed = np.bitwise_or(np.left_shift(self.data[1], 8), np.bitwise_and(self.data[0], 0x00FF))
            position = np.bitwise_or(np.left_shift(self.data[3],8),np.bitwise_and(self.data[2],0x00FF))
            displacement = np.bitwise_and(self.data[4],0x0001)
            enable = self.data[4] - displacement
        if self.ID == 0x10A:
            speed = np.int16(0)
            position = np.int16(0)
            displacement = np.int8(0)
            enable = np.int8(0)
            speed = np.bitwise_or(np.left_shift(self.data[1], 8), np.bitwise_and(self.data[0], 0x00FF))
            position = np.bitwise_or(np.left_shift(self.data[3],8),np.bitwise_and(self.data[2],0x00FF))
            displacement = np.bitwise_and(self.data[4],0x0001)
            enable = self.data[4] - displacement
        if self.ID == 0x10B:
            speed = np.int16(0)
            position = np.int16(0)
            displacement = np.int8(0)
            enable = np.int8(0)
            speed = np.bitwise_or(np.left_shift(self.data[1], 8), np.bitwise_and(self.data[0], 0x00FF))
            position = np.bitwise_or(np.left_shift(self.data[3],8),np.bitwise_and(self.data[2],0x00FF))
            displacement = np.bitwise_and(self.data[4],0x0001)
            enable = self.data[4] - displacement
        if self.ID == 0x10C:
            speed = np.int16(0)
            position = np.int16(0)
            displacement = np.int8(0)
            enable = np.int8(0)
            speed = np.bitwise_or(np.left_shift(self.data[1], 8), np.bitwise_and(self.data[0], 0x00FF))
            position = np.bitwise_or(np.left_shift(self.data[3],8),np.bitwise_and(self.data[2],0x00FF))
            displacement = np.bitwise_and(self.data[4],0x0001)
            enable = self.data[4] - displacement
        
        return speed / 1000, position / 100, displacement, enable

    def Lift_gen(self, LiftSpeed, LiftPosition, DispMode, CtrlMode):
        if self.ID == 0x105:
            LiftSpeed = np.int16(LiftSpeed * 1000)  
            LiftPosition = np.int16(LiftPosition * 100) 
            DispMode = np.int8(DispMode)
            CtrlMode = np.int8(CtrlMode)
            self.data[0] = np.bitwise_and(LiftSpeed, 0x00FF)
            self.data[1] = np.right_shift(np.bitwise_and(LiftSpeed, 0xFF00), 8)
            self.data[2] = np.bitwise_and(LiftPosition,0x00FF)
            self.data[3] = np.right_shift(np.bitwise_and(LiftPosition, 0xFF00), 8)
            self.data[4] = DispMode + 2 * CtrlMode

    def Lift_decode(self):
        if self.ID == 0x105:
            LiftSpeed = np.int16(0)
            LiftPosition = np.int16(0)
            DispMode = np.int8(0)
            CtrlMode = np.int8(0)
            LiftSpeed = np.bitwise_or(np.left_shift(self.data[1], 8), np.bitwise_and(self.data[0], 0x00FF))
            LiftPosition = np.bitwise_or(np.left_shift(self.data[3],8),np.bitwise_and(self.data[2],0x00FF))
            DispMode = np.bitwise_and(self.data[4],0x0001)
            CtrlMode = self.data[4] - DispMode

            return LiftSpeed / 1000, LiftPosition / 100, DispMode, CtrlMode 

    def sendto(self, ip_address, port_number):
        self.display_message()
        serverName = ip_address
        serverPort = port_number
        clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        clientSocket.sendto(self.data, (serverName, serverPort))
        clientSocket.close()

    def display_message(self):
        message = ''
        for i in range(0, 8):
            message = message + ' ' + str(np.uint8(self.data[i]))
        print(message)

# frame = Frame(0x100)
# frame.ADCmd_gen('-1.3', '1',  0, 0, 3)
# frame.sendto('192.168.1.200', 1088)
# frame.ADCmd_decode()
frame = Frame(0x105)
frame.Lift_gen(12, 130, 1, 0)
print(frame.Lift_decode())   