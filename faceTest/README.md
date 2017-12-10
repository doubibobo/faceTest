最开始采用Ubutun16.04（虚拟机环境）以及opencv来做

在最开始的时候是正常的，但是在虚拟机下发生了摄像头无法打开的问题

所以把它迁移到Windows系统，pycharm环境下，结果失败

环境是python3.6.2， OpenCV应该装3.3.0的版本，使用命令：

pip install opencv-python即可

pip install opencv-contrib-python

这里真的有好多坑……

参考博客如下：膜拜大神！

http://www.cnblogs.com/neo-T/p/6426029.html

http://www.cnblogs.com/neo-T/p/6430583.html

http://www.cnblogs.com/neo-T/p/6432596.html

http://www.cnblogs.com/neo-T/p/6445227.html

http://www.cnblogs.com/neo-T/p/6477378.html

http://www.cnblogs.com/neo-T/p/6511273.html

更：关于各种包的版本问题

构建人工神经网络CNN时，遇到了sklearn这个包有关模型弃用的问题
好在pycharm有强大的包可视化管理工具，在File->setting->Project->Project Interpreter中
将sklearn还原到上一个版本即可
在参考大神博客的时候
注意到大神用的构建二维卷积层的Keras接口API发生了改变，于是进行了调整，这里划重点！！！

附Keras中文文档链接

http://keras-cn.readthedocs.io/en/latest/layers/convolutional_layer/


耶耶耶