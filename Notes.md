
echo 0 > /proc/sys/kernel/apparmor_restrict_unprivileged_userns
is bigger hammer, but works as well

# yocto-assignments-base

////////////////////////////////////////////////


CONF_VERSION = "2"


LICENSE_FLAGS_ACCEPTED += "synaptics-killswitch"

MACHINE = "raspberrypi4-64"
IMAGE_FSTYPES = "wic.bz2 rpi-sdimg"

# Additional configurations (optional)
IMAGE_INSTALL:append = " linux-firmware-rpidistro-bcm43455 v4l-utils python3 ntp wpa-supplicant"
IMAGE_FEATURES += "ssh-server-openssh"
DISTRO_FEATURES:append = "wifi"
BOOT_SPACE = "1073741" -- bootspace increment

GPU_MEM = "128"
ENABLE_UART = "1"


//////
usb to ttl
https://www.adafruit.com/product/954

https://www.google.com/url?sa=i&url=https%3A%2F%2Ftoptechboy.com%2Funderstanding-raspberry-pi-4-gpio-pinouts%2F&psig=AOvVaw096qGO0d9iz3tLDSDSBt36&ust=1721065072977000&source=images&cd=vfe&opi=89978449&ved=0CBEQjRxqFwoTCMiA_5SJp4cDFQAAAAAdAAAAABAE


Connect the USB-to-TTL serial cable to the GPIO pins as follows:

Black wire (GND): Connect to pin 6 (GND)
White wire (RXD): Connect to pin 8 (GPIO14, TXD)
Green wire (TXD): Connect to pin 10 (GPIO15, RXD)
///////////////////////////////////////////////


sureshk@bflinuxserv:/data/kannaian/CU/systemprj/final-project-sureshkannaian/build/tmp/deploy/images/raspberrypi4-64$ sudo umount /dev/sdd1

sureshk@bflinuxserv:/data/kannaian/CU/systemprj/final-project-sureshkannaian/build/tmp/deploy/images/raspberrypi4-64$ bmaptool create -o core-image-base-raspberrypi4-64.rootfs.rpi-sdimg.bmap core-image-base-raspberrypi4-64.rootfs.rpi-sdimg

sureshk@bflinuxserv:/data/kannaian/CU/systemprj/final-project-sureshkannaian/build/tmp/deploy/images/raspberrypi4-64$ sudo bmaptool copy core-image-base-raspberrypi4-64.rootfs.rpi-sdimg /dev/sdd




tftpboot ${fdt_addr_r} bcm2711-rpi-4-b.dtb
fdt addr ${fdt_addr_r}
tftpboot 0x200000 Image
fdt get value bootargs /chosen bootargs
booti 0x200000 - ${fdt_addr_r}


sdd
// u-boot settings
ethact=bcmgenet
ipaddr=192.168.100.2
gateway=192.168.100.1
	memory@0 {
		device_type = "memory";
		reg = <0x00000000 0x00000000 0x38000000 0x00000000 0x40000000 0xbc000000 0x00000001 0x00000000 0x80000000 0x00000001 0x80000000 0x80000000>;
	};

	memreserve = <0x38000000 0x08000000>;
	chosen {
		bootargs = "coherent_pool=1M 8250.nr_uarts=1 snd_bcm2835.enable_headphones=0 snd_bcm2835.enable_hdmi=0  smsc95xx.macaddr=D8:3A:DD:61:7E:A4 vc_mem.mem_base=0x3ec00000 vc_mem.mem_size=0x40000000  dwc_otg.lpm_enable=0 console=ttyS0,115200 root=/dev/mmcblk0p2 rootfstype=ext4 rootwait  net.ifnames=0";
	};


tftpboot ${fdt_addr_r} bcm2711-rpi-4-b.dtb
fdt addr ${fdt_addr_r}
tftpboot 0x200000 Image
fdt get value bootargs /chosen bootargs
booti 0x200000 - ${fdt_addr_r}

//////

work area

bootargs = "coherent_pool=1M 8250.nr_uarts=1 snd_bcm2835.enable_headphones=0 snd_bcm2835.enable_hdmi=0  smsc95xx.macaddr=D8:3A:DD:61:7E:A4 vc_mem.mem_base=0x3ec00000 vc_mem.mem_size=0x40000000  dwc_otg.lpm_enable=0 console=ttyS0,115200 root=/dev/mmcblk0p2 rootfstype=ext4 rootwait  net.ifnames=0";

[    0.000000] Kernel command line: coherent_pool=1M 8250.nr_uarts=1 snd_bcm2835.enable_headphones=0 snd_bcm2835.enable_hdmi=0  smsc95xx.macaddr=D8:3A:DD:61:7E:A4 vc_mem.mem_base=0x3ec00000 vc_mem.mem_size=0x40000000  dwc_otg.lpm_enable=0 console=ttyS0,115200 root=/dev/mmcblk0p2 rootfstype=ext4 rootwait  net.ifnames=0

////////////////////////////////

https://docs.yoctoproject.org/2.2/kernel-dev/kernel-dev.html
poky/meta-skeleton/recipes-kernel/hello-mod/hello-mod_0.1.bb


https://www.raspberrypi.com/documentation/computers/config_txt.html#what-is-config-txt


cp -L bcm2711-rpi-4-b.dtb /var/lib/tftpboot/
cp /data/kannaian/CU/systemprj/final-project-sureshkannaian/build/tmp/work/raspberrypi4_64-poky-linux/uzrramstore-mod/0.1/sources/uzrramstore.ko /var/lib/tftpboot/


