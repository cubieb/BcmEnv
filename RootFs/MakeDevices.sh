 #!/bin/bash

FsRoot=$PWD/FsRoot

# Create "Character device"->"Enhanced Real Time Clock Support"
mknod $FsRoot/dev/rtc0 c 254 0

# Create FIFO devices
mknod $FsRoot/dev/initctl p

#Create character devices
mknod $FsRoot/dev/mem c 1 1
mknod $FsRoot/dev/kmem c 1 2
mknod $FsRoot/dev/null c 1 3
mknod $FsRoot/dev/port c 1 4
mknod $FsRoot/dev/zero c 1 5
mknod $FsRoot/dev/ptyp0 c 2 0
mknod $FsRoot/dev/ptyp1 c 2 1
mknod $FsRoot/dev/ptyp2 c 2 2
mknod $FsRoot/dev/ptyp3 c 2 3
mknod $FsRoot/dev/ptyp4 c 2 4
mknod $FsRoot/dev/ptyp5 c 2 5
mknod $FsRoot/dev/ptyp6 c 2 6
mknod $FsRoot/dev/ptyp7 c 2 7
mknod $FsRoot/dev/ptyp8 c 2 8
mknod $FsRoot/dev/ptyp9 c 2 9
mknod $FsRoot/dev/ptypa c 2 10
mknod $FsRoot/dev/ptypb c 2 11
mknod $FsRoot/dev/ptypc c 2 12
mknod $FsRoot/dev/ptypd c 2 13
mknod $FsRoot/dev/ptype c 2 14
mknod $FsRoot/dev/ptypf c 2 15
mknod $FsRoot/dev/ttyp0 c 3 0
mknod $FsRoot/dev/ttyp1 c 3 1
mknod $FsRoot/dev/ttyp2 c 3 2
mknod $FsRoot/dev/ttyp3 c 3 3
mknod $FsRoot/dev/ttyp4 c 3 4
mknod $FsRoot/dev/ttyp5 c 3 5
mknod $FsRoot/dev/ttyp6 c 3 6
mknod $FsRoot/dev/ttyp7 c 3 7
mknod $FsRoot/dev/ttyp8 c 3 8
mknod $FsRoot/dev/ttyp9 c 3 9
mknod $FsRoot/dev/ttypa c 3 10
mknod $FsRoot/dev/ttypb c 3 11
mknod $FsRoot/dev/ttypc c 3 12
mknod $FsRoot/dev/ttypd c 3 13
mknod $FsRoot/dev/ttype c 3 14
mknod $FsRoot/dev/ttypf c 3 15
mknod $FsRoot/dev/tty0 c 4 0
mknod $FsRoot/dev/tty1 c 4 1
mknod $FsRoot/dev/ttyS0 c 4 64
mknod $FsRoot/dev/ttyS1 c 4 65
mknod $FsRoot/dev/tty c 5 0
mknod $FsRoot/dev/console c 5 1
mknod $FsRoot/dev/ptmx c 5 2
mknod $FsRoot/dev/ppp c 108 0
mknod $FsRoot/dev/printer0 c 180 0
mknod $FsRoot/dev/urandom c 1 9
mknod $FsRoot/dev/i2c-0 c 89 0
mknod $FsRoot/dev/mtd0 c 90 0
mknod $FsRoot/dev/mtd1 c 90 2
mknod $FsRoot/dev/mtd2 c 90 4
mknod $FsRoot/dev/mtd3 c 90 6
mknod $FsRoot/dev/ubi0 c 254 0
mknod $FsRoot/dev/ubi_ctrl c 10 63 
mknod $FsRoot/dev/random c 1 8
mknod $FsRoot/dev/hwrandom c 10 183

# Create Broadcom specific devices
mknod $FsRoot/dev/dect c 197 0
mknod $FsRoot/dev/dectdbg c 197 1
mknod $FsRoot/dev/bcmatm0 c 205 0
mknod $FsRoot/dev/brcmboard c 206 0
mknod $FsRoot/dev/bcmendpoint0 c 209 0
mknod $FsRoot/dev/bcmaal20 c 210 0
mknod $FsRoot/dev/bcmles0 c 211 0
mknod $FsRoot/dev/bcm c 212 0
mknod $FsRoot/dev/gpio_driver c 212 8
mknod $FsRoot/dev/bounce c 213 0
mknod $FsRoot/dev/pmon c 214 0
mknod $FsRoot/dev/ac97 c 222 0
mknod $FsRoot/dev/slac c 223 0
mknod $FsRoot/dev/bcmprof c 224 0
mknod $FsRoot/dev/bcmatmb0 c 226 0
mknod $FsRoot/dev/p8021ag0 c 227 0
mknod $FsRoot/dev/bcmxtmcfg0 c 228 0
mknod $FsRoot/dev/pktcmf c 232 0
mknod $FsRoot/dev/spu c 233 0
mknod $FsRoot/dev/bcmmoca0 c 234 0
mknod $FsRoot/dev/bcmmoca10 c 234 1
mknod $FsRoot/dev/bcm_user_ploam c 235 0
mknod $FsRoot/dev/bcm_omci c 236 0
mknod $FsRoot/dev/bcm_ploam c 237 0
mknod $FsRoot/dev/bcmvlan c 238 0
mknod $FsRoot/dev/pwrmngt c 240 0
mknod $FsRoot/dev/bcmfap c 241 0
mknod $FsRoot/dev/fcache c 242 0
mknod $FsRoot/dev/capi20 c 68 0
mknod $FsRoot/dev/ippp0 c 45 128
mknod $FsRoot/dev/ippp1 c 45 129
mknod $FsRoot/dev/linux-bcm-knet c 122 0
mknod $FsRoot/dev/linux-uk-proxy c 125 0
mknod $FsRoot/dev/linux-user-bde c 126 0
mknod $FsRoot/dev/linux-kernel-bde c 127 0
mknod $FsRoot/dev/bcmmdd c 32 0 

# Create block devices
mknod $FsRoot/dev/ram0 b 1 0
mknod $FsRoot/dev/ram1 b 1 1
mknod $FsRoot/dev/ram2 b 1 2
mknod $FsRoot/dev/ram3 b 1 3
ln -s ram1 $FsRoot/dev/ram

mknod $FsRoot/dev/mtdblock0 b 31 0
mknod $FsRoot/dev/mtdblock1 b 31 1
mknod $FsRoot/dev/mtdblock2 b 31 2
mknod $FsRoot/dev/mtdblock3 b 31 3
mknod $FsRoot/dev/mtdblock4 b 31 4
mknod $FsRoot/dev/mtdblock5 b 31 5
mknod $FsRoot/dev/mtdblock6 b 31 6
mknod $FsRoot/dev/mtdblock7 b 31 7

