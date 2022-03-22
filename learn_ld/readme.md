
### LD教程

教程1：  
https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware  
教程2：  
https://interrupt.memfault.com/blog/how-to-write-a-bootloader-from-scratch  
教程3：  
https://medium.com/@xsumirx/gnu-linker-script-and-memory-relocation-for-embedded-devices-5d3d16d1f0
教程4：  
https://microcontrollerslab.com/bare-metal-embedded-systems-linker-script-file/


查看elf header：  
```
D:\tools\IoT\eclipse\GNU-MCU-Eclipse\arm-none-eabi-gcc-9.2.1-1.1\bin\arm-none-eabi-objdump.exe -t .\build\cross\cortex-m4\release\nrf-app

```

查看变量地址：  
```
D:\tools\IoT\eclipse\GNU-MCU-Eclipse\arm-none-eabi-gcc-9.2.1-1.1\bin\arm-none-eabi-nm.exe .\build\cross\cortex-m4\release\nrf-app | sort
```

