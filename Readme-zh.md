## NRF52840开发环境
由于偶尔需要使用Keli等IDE，所以在Windows下进行开发。为了加深对IoT OS的理解，并降低开发调试复杂度，从裸设备开始开发。

#### 环境部署
依赖安装： 
- windows下的GNU Make。
- windows下的arm-none-eabi-gcc。
- windows下的nrfjprog命令行工具（提供flash命令）。
- windows下的SEGGER JLink（gdb调试）。
- NRF SDK, 当前用的版本是nRF5_SDK_17.0.0_9d13099。

#### 编译
basic项目依赖NRF SDK，需要去Makefile中设置路径。
```
# 编译
make

# 刷固件
make flash

# 调试，先打开SEGGER JLink下的JLinkGDBServer.exe
make debug
```

#### 串口通信
利用python自带的miniterm.py命令行：
``` bash
λ python D:\dev\Python\Python3.8.5\Scripts\miniterm.py "COM6" 115200 --eol LF
--- Miniterm on COM6  115200,8,N,1 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
```

## 项目结构
基于这个博客熟悉NRF52840的开发：
https://interrupt.memfault.com/blog/cortex-m-debug-monitor