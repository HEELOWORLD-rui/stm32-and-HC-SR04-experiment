# stm32-and-HC-SR04-experiment
基于正点原子的stm32f103战舰开发板V4开发的超声测距实验，泥南的孩纸有福啦！

有两个branch

Plus是包含提高题的部分，提高题的部分有：LCD显示、LCD中文显示、报警蜂鸣器、精度提高（灵敏度提高）、四个按键中断

Basic是基础题部分，主要区别就是采用串口进行界面展示

全部下载下来后打开project里面的keil文件编译+下载（配置好STlink），如果使用串口需要在魔术棒--create hex里面生成hex文件后再串口下载（flyMCU）下载
