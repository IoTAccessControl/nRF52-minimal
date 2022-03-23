
尝试将lib输出为section。


### 学习LD脚本  
[学习过程](../learn_ld/readme.md)

### Link设计 
模块1：shell_app，包含start_up和各种Handler。  
``` 

```

模块2：hal层  
```

```


NewLib启动：
https://github.com/bminor/newlib/blob/master/newlib/libc/sys/arm/crt0.S



### TODO 
 
1. 用gdb刷机调试 (装gef插件)  

2. 接入报错库  
输出hardware_fualt错误细节。  
https://interrupt.memfault.com/blog/cortex-m-fault-debug  
cm_backtrace

3. MPU学习
https://interrupt.memfault.com/blog/fix-bugs-and-secure-firmware-with-the-mpu  


