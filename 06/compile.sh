#! /usr/bin/env bash
set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

function displaytime {
    local T=$1
    local D=$((T/60/60/24))
    local H=$((T/60/60%24))
    local M=$((T/60%60))
    local S=$((T%60))
    (( $D > 0 )) && printf '%d days ' $D
    (( $H > 0 )) && printf '%d hours ' $H
    (( $M > 0 )) && printf '%d minutes ' $M
    (( $D > 0 || $H > 0 || $M > 0 )) && printf 'and '
    printf '%d seconds\n' $S
}


DIR=/sources/linux-next
if [ -e $DIR ]
then

    cd $DIR
    VERSION=$(git describe --tags)
    NAME=vmlinuz-$VERSION-next

    start=`date +%s`
    printf "${GREEN}[START] ${DIR} compilation${time}${NC}\n"

    make mrproper || true
    make defconfig
    echo 'CONFIG_LOCALVERSION_AUTO=y' >> .config
    make
    make modules_install

    if [ -e /boot/$NAME ]
    then
        rm -rf /boot/$NAME
    fi
    cp -iv arch/x86/boot/bzImage /boot/$NAME

    if [ -e /boot/System.map-$VERSION ]
    then
        rm -rf /boot/System.map-$VERSION
    fi
    cp -iv System.map /boot/System.map-$VERSION

    if [ -e /boot/config-$VERSION ]
    then
        rm -rf /boot/config-$VERSION
    fi
    cp -iv .config /boot/config-$VERSION

    printf "${GREEN}Copy kernel documentation in /usr/share/doc/linux-${VERSION}.${NC}\n"
    if [ -e /usr/share/doc/linux-$VERSION ]
    then
        rm -rf /usr/share/doc/linux-$VERSION
    fi
    install -d /usr/share/doc/linux-$VERSION
    cp -r Documentation/* /usr/share/doc/linux-$VERSION

    chown -R 0:0 $DIR

    printf "${GREEN}Copy kernel sources in /usr/src/kernel-${VERSION}${NC}\n"
    if [ -e /usr/src/kernel-$VERSION ]
    then
        rm -rf /usr/src/kernel-$VERSION
    fi
    cp -r $DIR /usr/src/kernel-$VERSION

    end=`date +%s`
    time=$(displaytime `expr $end - $start`)
    printf "${GREEN}[DONE] $DIR in ${time}${NC}\n"

else
    printf "[ALREADY] $DIR\n"
fi

cat >> /boot/grub/grub.cfg << "EOF"

menuentry "GNU/Linux, Linux $VERSION" {
        linux   /vmlinuz-$VERSION-next root=/dev/sda3 ro
}
EOF
