import pygame as pg

class TextRect():
    def __init__(self, x, y, text = '') -> None:
        self.font = pg.font.SysFont('Comic Sans MS', 30)
        self.text_surface = self.font.render(text, False, (0, 0, 0))
        self.x = x
        self.y = y

    def draw(self, screen):
        screen.blit(self.text_surface, (self.x, self.y)) 