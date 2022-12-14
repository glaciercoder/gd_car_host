import pygame
import sys

from setting import Settings
from inputbox import InputBox
from textrect import TextRect
from frame import Frame
from button import Button

help_string1 = "The control pane for the GD car. \n "
help_string2 =  "Mode: 2 for turning without moving; 3 for Crab-Walking. \n" 
help_string3 = "Speed: for total speed of center of gravity, useless in Mode2 \n" 
help_string4 = "Omega: deg when in Mode3, deg/s in Mode2"
# Control send
if_send = False
# IP address
Host_IP = '192.168.1.200'
PORT = 1088

def frame_gen():
    global if_send
    if_send = True
    print("Send:")

def create_box():
    settings = Settings()
    orign_x = settings.input_orgin_x
    orign_y = settings.input_orgin_y

    help_bar = TextRect(0, 0, help_string1)
    help_bar2 = TextRect(0, 20, help_string2)
    help_bar3 = TextRect(0, 40, help_string3)
    help_bar4 = TextRect(0, 60, help_string4)
    
    speed_tip = TextRect(orign_x, orign_y, "Speed(m/s):")
    speed_box = InputBox(orign_x+settings.box_offset_x, orign_y+settings.box_offset_y, settings.boxwidth, settings.boxheight)
    orign_y = orign_y + settings.tips_offset_y
    omega_tip = TextRect(orign_x, orign_y, "Omega(deg/s):")
    omega_box = InputBox(orign_x+settings.box_offset_x, orign_y+settings.box_offset_y, settings.boxwidth, settings.boxheight)
    orign_y = orign_y + settings.tips_offset_y
    mode_tip = TextRect(orign_x, orign_y, "Mode:")
    mode_box = InputBox(orign_x+settings.box_offset_x, orign_y+settings.box_offset_y, settings.boxwidth, settings.boxheight)
 
    input_tips = [help_bar, help_bar2, help_bar3, help_bar4,  speed_tip, omega_tip, mode_tip]
    input_boxes = [speed_box, omega_box, mode_box]
    return [input_tips, input_boxes]

def create_button():
    settings = Settings()
    send_button = Button(settings.button_origin_x, settings.button_origin_y, settings.button_width, settings.button_height, "Send", frame_gen)
    return send_button

def run_menu():
    pygame.init()
    ui_settings = Settings()
    screen = pygame.display.set_mode((ui_settings.screen_width, ui_settings.screen_height))
    pygame.display.set_caption("GD_car control pane")

    # create elements
    [input_tips, input_boxes] = create_box()
    send_button = create_button()

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
               sys.exit()
            for input_box in input_boxes:
                input_box.handle_event(event)
        for input_box in input_boxes:
            input_box.update()

        screen.fill(ui_settings.bg_color)

        # Blit the elements
        for input_box in input_boxes:
            input_box.draw(screen)
        for input_tip in input_tips:
            input_tip.draw(screen)
        send_button.process(screen)
        # Control messange send
        global if_send
        if if_send:
            if input_boxes[0].text and input_boxes[1].text and input_boxes[2]:
                speed = float(input_boxes[0].text)
                omega = float(input_boxes[1].text)
                mode = float(input_boxes[2].text)
                frame = Frame(0x100)
                frame.ADCmd_gen(speed, omega, 0, 0, mode)
                frame.sendto(Host_IP, PORT)
            if_send = False

        pygame.display.flip()
        



run_menu()

