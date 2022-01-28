# 一些经典图像处理算法复现

## 傅里叶变换(Fourier Transformer)

### 任务要求

- 编写程序对图像(自行转换为灰度图)展开**傅里叶变换**
- 提取傅里叶变换图像(将频率原点移至图像中心)

### 个人实现

- 也就是实现DFT2的功能, 实际代码编写时并未实现快速傅里叶变换, 使用二维傅里叶变换的可分离性以及矩阵化进行运算加速

## 直方图均衡化（img_hist）与同态滤波（homefilter）

- 对低照度图像进行灰度化，计算并显示该图像的**灰度直方图**和**离散傅里叶变换频谱幅度图**
- 再对该图像分别进行**直方图均衡化**和**同态滤波**操作

### 个人实现

- 灰度直方图就是统计下图像的各个灰度频率，然后画出来就行了
- 同态滤波的滤波器使用巴特沃斯高通滤波，理想高通可能会出现振铃效应（对原本的巴特沃斯高通滤波器做了一些改进, 原本的巴特沃斯滤波器最低值为零, 但我做了很多实验后发现不能直接取零, 不然结果图片整体就会偏的特别暗, 所以取了一个非零最小值, 然后还对最大值做了一些约束）
- 对图片先取对数，然后做傅里叶变换，使用高通滤波器进行滤波，然后反变换回空域，再去指数取回去，最后可以加个归一化，不过做实验发现不做好像也没啥问题

- 两个对比着看，同态滤波的结果比直方图均衡化的要好一些

## Mean-shift

- 用C语言编写
- 读取bmp图片
- mean-shift算法简单描述下就是以图像的RGB三个维度构成的三维空间不停的取中值点然后平移，再升级一下就是取导数方向，这个就是正常的mean-shift了
- 具体细节见报告



