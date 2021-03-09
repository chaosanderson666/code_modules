https://zhuanlan.zhihu.com/p/130765229
 IEEE 1588 协议简称精确时钟协议 PTP（Precision Timing Protocol），它的全称是“网络测量和控制系统的精密时钟同步协议标准”（IEEE 1588 Precision Clock Synchronization Protocol）。
 
!NOTE：如果知道传输处理时延delay的值是多少，只需传送前两帧即可。后两帧就是用于确定delay的值是多少。

Slave设备根据算出的Offset即可以进行本地时钟校准。但是1588V1协议依赖于链路的对称性，即Master到Slave与Slave到Master时延一致，这在实际网络状况下很难满足，故需要额外的不对称算法进行链路延时差计算和补偿校准。