import random
import numpy as np

# from sklearn.cross_validation import train_test_split
from keras.preprocessing.image import ImageDataGenerator
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from keras.optimizers import SGD
from keras.utils import np_utils
from keras.models import load_model
from keras import backend as K

from sklearn import cross_validation


from load_face_datatest import load_datatest, resetSizeoftheImage, IMAGE_SIZE

# 进行训练的训练集合
class Datatest:
    def __init__(self, path_name):
        # 训练集
        self.train_images = None
        self.train_labels = None
        # 验证集
        self.valid_images = None
        self.valid_labels = None
        # 测试集
        self.test_images = None
        self.test_labels = None
        # 数据加载路径
        self.path_name = path_name
        # 当前库采用的维度顺序，不同的训练模型采用的维度顺序可能不一致，如height，width，RGB or RGB，height，width
        self.input_shape = None

    # 按照交叉验证的原则划分数据集，进行预处理操作
    def load(self, img_rows = IMAGE_SIZE, img_cols = IMAGE_SIZE, img_channels = 3, nb_classes = 2):
        # 加载数据集到内存
        images, labels = load_datatest(self.path_name)
        # 训练集和验证集
        # images 表示要划分的样本特征集，labels 表示要划分的样本结果， test_size 是样本在整个样本集中所占的比例
        self.train_images, self.valid_images, self.train_labels, self.valid_labels = cross_validation.train_test_split(images, labels, test_size=0.3, random_state=random.randint(0, 100))
        # 测试集
        _, self.test_images, _,self.test_labels = cross_validation.train_test_split(images, labels, test_size=0.5, random_state=random.randint(0, 100))
        # 如果当前的维度顺序为th, 则输入的图片顺序应该为：channels, rows, cols, 否则为rows, cols, channels
        # 根据keras库中要求的维度顺序集重组训练的数据集合
        if K.image_dim_ordering() == 'th':
            self.train_images = self.train_images.reshape(self.train_images.shape[0], img_channels, img_rows, img_cols)
            self.valid_images = self.valid_images.reshape(self.valid_images.shape[0], img_channels, img_rows, img_cols)
            self.test_images = self.test_images.reshape(self.test_images.shape[0], img_channels, img_rows, img_cols)
            self.input_shape = (img_channels, img_rows, img_cols)
        else:
            self.train_images = self.train_images.reshape(self.train_images.shape[0], img_rows, img_cols, img_channels)
            self.valid_images = self.valid_images.reshape(self.valid_images.shape[0], img_rows, img_cols, img_channels)
            self.test_images = self.test_images.reshape(self.test_images.shape[0], img_rows, img_cols, img_channels)
            self.input_shape = (img_rows, img_cols, img_channels)

        # 输出训练集、验证集、测试集的数量
        print(self.test_images.shape[0], 'train examples')
        print(self.valid_images.shape[0], 'valid examples')
        print(self.test_images.shape[0], 'test examples')
        # 采用categorical_crossentropy作为损失函数
        # 需要根据类别数量nb_classes将类别标签进行one-hot编码使其向量化
        # 经过转化后，标签数量转化成二维
        self.train_labels = np_utils.to_categorical(self.train_labels, nb_classes)
        self.valid_labels = np_utils.to_categorical(self.valid_labels, nb_classes)
        self.test_labels = np_utils.to_categorical(self.test_labels, nb_classes)

        # 像素数据浮点化以便归一化
        self.train_images = self.train_images.astype('float32')
        self.valid_images = self.valid_images.astype('float32')
        self.test_images = self.test_images.astype('float32')

        # 将像素数据进行归一化
        self.train_images /= 255
        self.valid_images /= 255
        self.test_images /= 255

class Model:
    def __init__(self):
        self.model = None

    # 建立自己的训练模型
    def build_model(self, dataset, nb_classes = 2):
        # 创立一个空的神经网络模型，线性堆叠模型，各神经网络层会执行顺序添加
        self.model = Sequential()
        # 以下每一个add都意味着顺序添加一个网络层

        # 二维卷积层
        self.model.add(Convolution2D(32, (3, 3), padding='same', input_shape=dataset.input_shape))
        # 激活函数层
        self.model.add(Activation('relu'))
        # 二维卷积层
        self.model.add(Convolution2D(32, (2, 2)))
        # 激活函数层
        self.model.add(Activation('relu'))
        # 池化层
        self.model.add(MaxPooling2D(pool_size=(2, 2)))
        # Dropout层
        self.model.add(Dropout(0.25))
        # 二维卷积层
        self.model.add(Convolution2D(64, (3, 3), padding='same'))
        # 激活函数层
        self.model.add(Activation('relu'))
        # 二维卷积层
        self.model.add(Convolution2D(64, (3, 3)))
        # 激活函数层
        self.model.add(Activation('relu'))
        # 池化层
        self.model.add(MaxPooling2D(pool_size=(2, 2)))
        # Dropout层
        self.model.add(Dropout(0.25))
        # Flatten层
        self.model.add(Flatten())
        # Dense层（全连接层）
        self.model.add(Dense(512))
        # 激活函数层
        self.model.add(Activation('relu'))
        # Dropout层
        self.model.add(Dropout(0.5))
        # Dense层
        self.model.add(Dense(nb_classes))
        # 分类层，并且输出最终的结果
        self.model.add(Activation('softmax'))
        # 输出模型概况
        self.model.summary()

    # 训练自己的模型
    def train(self, dataset, batch_size=20, nb_epoch=10, data_augmentation = True):
        # 采用sgd优化器进行训练，首先生成一个优化器对象
        sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
        # 完成实际模型的配置工作
        self.model.compile(loss='categorical_crossentropy', optimizer=sgd, metrics=['accuracy'])
        # 不使用提升数据集方法
        # 提升是指从提供的训练数据中利用旋转、翻转、加噪声等方法创造新的训练数据、
        # 有意识的提升训练数据的规模，增加模型训练量
        if not data_augmentation:
            self.model.fit(dataset.train_imagics, dataset.train_labels, batch_size=batch_size, nb_epoch=nb_epoch, validation_data=(dataset.valid_imagics, dataset.valid_labels), shuffle = True)
        # 使用数据提升
        else:
            # 使用数据生成器用于数据提升，返回生成器对象
            # 生成器对象每被调用一次生成一组数据
            datagen = ImageDataGenerator(
                featurewise_center=False,   # 是否使输入数据去中心化
                samplewise_center=False,    # 是否使输入数据的每个样本均值为0
                featurewise_std_normalization=False,     # 是否数据标准化（输入数据除以数据集的标准差）
                samplewise_std_normalization=False,      # 是否将每个样本数据除以自身的标准差
                zca_whitening=False,                     # 是否对输入数据施以ZCA以白化
                rotation_range=20,                       # 数据提升时图片随机转动的角度
                width_shift_range=0.2,                   # 数据提升时图片水平偏移的角度
                height_shift_range=0.2,                  # 数据提升时图片垂直偏移的角度
                horizontal_flip=True,                    # 是否进行随机水平翻转
                vertical_flip=False                      # 是否进行随机垂直翻转
            )

            # 计算整个训练样本集的数量以用于特征值的归一化，ZCA白化处理
            datagen.fit(dataset.train_images)

            # 使用生成器开始训练模型
            self.model.fit_generator(
                datagen.flow(dataset.train_images, dataset.train_labels, batch_size=batch_size),
                samples_per_epoch = dataset.train_images.shape[0],
                nb_epoch=nb_epoch,
                validation_data=(dataset.valid_images, dataset.valid_labels)
            )

    # 将模型进行保存
    MODEL_PATH = 'E:\pythonProject\doubibobo.face.model.h5'
    def save_model(self, file_path = MODEL_PATH):
        self.model.save(file_path)

    def load_model(self, file_path = MODEL_PATH):
        self.model = load_model(file_path)

    # 识别人脸的预测函数
    def face_predict(self, image):
        # 根据后端系统确定维度顺序
        if K.image_dim_ordering() == 'th' and image.shape != (1, 3, IMAGE_SIZE, IMAGE_SIZE):
            # 调整其尺寸与数据集保持一致
            image = resetSizeoftheImage(image)
            image = image.reshape((1, 3, IMAGE_SIZE, IMAGE_SIZE))
        elif K.image_dim_ordering() == 'tf' and image.shape != (1, IMAGE_SIZE, IMAGE_SIZE, 3):
            image = resetSizeoftheImage(image)
            image = image.reshape((1, IMAGE_SIZE, IMAGE_SIZE, 3))

        # 浮点归一化
        image = image.astype('float32')
        image /= 255
        reslut = self.model.predict_proba(image)
        print('result:', reslut)
        # 返回结果集
        reslut = self.model.predict_classes(image)
        return reslut[0]

if __name__ == '__main__':
    dataset = Datatest("E:\pythonProject\Practice")
    dataset.load()

    model = Model()
    model.build_model(dataset)
    model.train(dataset)
    model.save_model(file_path='E:\pythonProject\model\doubibobo.face.model.h5')