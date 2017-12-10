import cv2
import sys
import gc
from face_train_use_keras import Model

import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

if __name__ == '__main__':
    # 加载模型
    model = Model()
    model.load_model(file_path='E:\\pythonProject\\faceTest\\model\\allPeople.face.model.h5')

    # 人脸矩形的边框颜色
    color = (0, 255, 255)

    # 捕获实时的视频流
    capture = cv2.VideoCapture(0)

    # 使用人脸识别的分类器，可以按照自己的需求进行训练
    cascade_path = "E:\OpenCV3.3\opencv\sources\data\haarcascades\haarcascade_frontalface_alt2.xml"
    somethingClassfier = cv2.CascadeClassifier(cascade_path)

    # 循环识别出人脸
    while True:
        ok, frame = capture.read()
        if not ok:
            break

        # 将当前帧转化成灰度图像
        grey = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # 利用分类器识别人脸
        faceRects = somethingClassfier.detectMultiScale(grey, scaleFactor = 1.2, minNeighbors = 3, minSize = (32, 32))
        # 大于0表示可以检测到人脸，faceRects表示人脸数目

        theCount = len(faceRects)

        print(len(faceRects))
        count = "current number:" + str(theCount)
        print(count)

        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(frame, count, (30, 30), font, 1, (255, 0, 255), 4)

        if len(faceRects) > 0:
            for faceRect in faceRects:
                x, y, w, h = faceRect
                # 截取图像进行识别
                image = frame[y-10: y+h+10,x-10:x+w+10]
                imageGrey = grey[y-10: y+h+10, x-10:x+w+10]
                # 画出矩形框
                cv2.rectangle(frame, (x-10,y-10),(x+w+10, y+h+10), color, 2)
                print(image)
                # print(imageGrey)
                thePredict = model.face_predict(image)
                print(thePredict)
                who = thePredict[0]
                theName = "unknown"
                if who == 0:
                    theName = "doubibobo"
                elif who == 1:
                    theName = "guojuncheng"
                elif who == 2:
                    theName = "lipeng"
                elif who == 3:
                    theName = "gelingqiao"
                elif who == 4:
                    theName = "yaoxinzhi"
                else:
                    pass
                cv2.putText(frame, theName, (x + 30, y + 30), font, 1, (255, 0, 255), 4)

        # 将截取到的图片进行展示，每隔10个单位的时间点
        cv2.imshow("reply", frame)
        c = cv2.waitKey(10)
        if c & 0xFF == ord('q'):
            break

    capture.release()
    cv2.destroyAllWindows()