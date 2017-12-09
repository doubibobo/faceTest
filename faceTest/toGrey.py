import os
import cv2
from PIL import Image

def read_path(path_name):
    for dir_item in os.listdir(path_name):
        full_path = os.path.abspath(os.path.join(path_name, dir_item))
        print(full_path)
        if os.path.isdir(full_path):
            read_path(full_path)
        else:
            I = Image.open(path_name)
            L = I.convert('L')
            L.save(path_name)