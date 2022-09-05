class Settings():
    def __init__(self) -> None:
        self.screen_width = 1200
        self.screen_height = 800
        self.bg_color = (230, 230, 230)
        # Input box size
        self.boxwidth = 140
        self.boxheight = 32
        # Offset from input tips to box
        self.box_offset_x = 200
        self.box_offset_y = -5
        # Input origin
        self.input_orgin_x = 100
        self.input_orgin_y = 200
        # Offset from input tips to input tips
        self.tips_offset_y = 80
        # Button position
        self.button_origin_x = 500
        self.button_origin_y = 500
        # Button size
        self.button_width  = 100
        self.button_height = 30