import cv2
import sys
import tensorflow
from PIL import Image

def CatchUsbVideo(window_name, camera_index, catchNumber, imagePath):
    # 为窗口设置名称
    cv2.namedWindow(window_name)

    # 打开USB摄像头
    cap = cv2.VideoCapture(camera_index)

    # 使用人脸识别的分类器，可以按照自己的需求进行训练
    somethingClassfier = cv2.CascadeClassifier("E:\OpenCV3.3\opencv\sources\data\haarcascades\haarcascade_frontalface_alt2.xml")

    # 识别出人脸之后截取人脸边框所需要的颜色，采用RGB格式
    color = (0, 255, 0)

    num = 0

    # 如果摄像头打开，读取摄像头当中的数据
    while cap.isOpened():
        ok, frame = cap.read()
        if not ok:
            break

        # 将当前帧转化成灰度图像
        grey = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # 人脸检测，1.2为图像缩放比例，2表示要检测的有效点数
        # grey表示要识别的图像数据，转化成为灰度图之后，检测哈尔特征，此时三维RGB转化成一维的灰度，降低了计算的强度
        # scaleFactor表示图像的缩放比例，距离相机的距离不同
        # minNeighbors对特征点周边有所少有效点进行检测，避免因选取的特征点太小而遗漏检测
        # minSize特征监测点的最小值
        faceRects = somethingClassfier.detectMultiScale(grey, scaleFactor = 1.2, minNeighbors = 3, minSize = (32, 32))
        # 大于0表示可以检测到人脸，faceRects表示人脸数目
        print(len(faceRects))
        if len(faceRects) > 0:
            for faceRect in faceRects:
                # x, y表示人脸起始点的坐标，w，h表示长度和宽度，最后一个2表示的线框的粗细
                x, y, w, h = faceRect

                # 将当前帧保存为图片
                imgName = '%s/%d.jpg'%(imagePath, num)
                image = grey[y-10:y+h+10, x-10: x+w+10]
                cv2.imwrite(imgName, image)

                num = num + 1
                # 如果超出给定的最大值，则退出循环
                if num > catchNumber:
                    break

                # 画出矩形框
                cv2.rectangle(frame, (x-10,y-10),(x+w+10, y+h+10), color, 2)
                # 显示当前捕捉的人脸图片数目
                font = cv2.FONT_HERSHEY_SIMPLEX
                cv2.putText(frame, 'num:%d' % (num), (x+30, y+30), font, 1, (255, 0, 255), 4)

        # 超过指定的最大保存数量结束程序
        if num > (catchNumber):
            break

        # 将截取到的图片进行展示，每隔10个单位的时间点
        cv2.imshow(window_name, frame)
        c = cv2.waitKey(10)
        if c & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    CatchUsbVideo("catch the picture from the video", 0, 1000, "E:\pythonProject\\faceTest\Practice\me")

# print("hello, world!")