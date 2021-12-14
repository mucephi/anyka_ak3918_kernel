# Anyka ak3918 Linux kernel

This repository its part of a research for customising Firmware IP cameras based on V380 Anyka SoC.

Firmware source code for cheap Chinese IP cameras based on Anyka SoC aren't available for public domain.
Anyways an earlier project based on this SoC has all its needed to produce campatible kernel modules besides all video source code drivers.

Just copy some default kernel configuration from arch/arm/configs/aimer39_ak3918_XXX to your kernel .config and the kernel configuration will be readdy to add new features by kernel modules. 

```
arch/arm/configs:
aimer39_ak3916_defconfig
aimer39_ak3916_dlna_defconfig
aimer39_ak3916_ram_defconfig
aimer39_ak3918_D1_defconfig
aimer39_ak3918_defconfig
aimer39_ak3918_dlna_defconfig
ak39_micro_defconfig
ak39_producer_defconfig
ak39_sdk3910_defconfig
```
## Achievements:

tun.ko => Universal TUN/TAP device driver.
TUN/TAP driver is responsible for create virtual network devices; most common VPNs connections goes through this device.

edge => P2P VPN client.
edge its a virtual private network client, it gives the camera the capability to being visible outside the local network. For more information about this VPN client see: https://github.com/ntop/n2n  


![This is an image](./board-config.jpg)
