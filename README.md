# yocto-assignments-base
Base repository for AESD Yocto assignments
echo 0 > /proc/sys/kernel/apparmor_restrict_unprivileged_userns
LICENSE_FLAGS_ACCEPTED += "synaptics-killswitch"

MACHINE = "raspberrypi4-64"


# Additional configurations (optional)
IMAGE_INSTALL:append = " linux-firmware-rpidistro-bcm43455"
GPU_MEM = "128"
ENABLE_UART = "1"