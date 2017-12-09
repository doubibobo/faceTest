import os
import sys
import numpy as np
import cv2

# 设置照片的大小
IMAGE_SIZE = 64

# 按照指定图像大小调整尺寸
def resetSizeoftheImage(image, height = IMAGE_SIZE, width = IMAGE_SIZE):
    top, bottom, left, right = (0, 0, 0, 0)
    # 截取图像的尺寸
    h, w, _ = image.shape
    # 如果图像的长宽不相同，则补充图像，是的图像的长和宽相同
    longest_edgt = max(h, w)
    # 计算较短的边所需要补充的像素大小范围
    if h < longest_edgt:
        dh = longest_edgt - h
        top = dh // 2
        bottom = dh - top
    elif w < longest_edgt:
        dw = longest_edgt - w
        left = dw // 2
        right = dw - left
    else:
        pass

    # 设置调整大小所补充的像素范围的颜色RGB
    BLACK = [0, 0, 0]

    # 调整图像的边界并返回，top, bottom, left, right 分别是四个方向上需要扩充的像素点数目
    constant = cv2.copyMakeBorder(image, top, bottom, left, right, cv2.BORDER_CONSTANT, value=BLACK)

    # 返回新的图像
    return cv2.resize(constant, (height, width))

# 读取训练数据
images = []
labels = []
def read_path(path_name):
    for dir_item in os.listdir(path_name):
        full_path = os.path.abspath(os.path.join(path_name, dir_item))
        print(full_path)
        if os.path.isdir(full_path):
            read_path(full_path)
        else:
            if dir_item.endswith('.jpg'):
                image = cv2.imread(full_path)
                image = resetSizeoftheImage(image, IMAGE_SIZE, IMAGE_SIZE)
                images.append(image)
                labels.append(path_name)
    return images, labels

# 从指定位置读取训练数据
def load_datatest(path_name):
    images, labels = read_path(path_name)
    images = np.array(images)
    print(images.shape)

    # labels = np.array(labels)

    # 设置数组的大小，将标签转化为分类器中的类别种类，数据等
    # np.arange(len(labels))
    theLabel = np.arange(len(labels))

    # 为训练数据设置类别编号
    for index in range(len(labels)):
        if labels[index].endswith("me"):
            theLabel[index] = np.array([0])
        elif labels[index].endswith("guo"):
            theLabel[index] = np.array([1])
        elif labels[index].endswith("li"):
            theLabel[index] = np.array([2])
        elif labels[index].endswith("ge"):
            theLabel[index] = np.array([3])
        elif labels[index].endswith("yao"):
            theLabel[index] = np.array([4])
        else:
            theLabel[index] = np.array([5])

    # print(theLabel)
    labels = theLabel
    # labels = np.array([0 if label.endswith('me') else 1 for label in labels])
    print(labels.shape)
    print(labels)
    return images, labels

if __name__ == '__main__':
    images, labels = load_datatest("E:\pythonProject\\faceTest\Practice")
