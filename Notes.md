
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
umount: /dev/sdd1: not mounted.
sureshk@bflinuxserv:/data/kannaian/CU/systemprj/final-project-sureshkannaian/build/tmp/deploy/images/raspberrypi4-64$ sudo bmaptool copy core-image-base-raspberrypi4-64.rootfs.rpi-sdimg /dev/sdd
