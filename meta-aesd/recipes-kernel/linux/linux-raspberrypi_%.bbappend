
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append:raspberrypi4 = " \
    file://0001-tftpboot-patch.patch \
    file://0002-Dveice-Tree-Reserve-memory-with-kernel.patch \
"

