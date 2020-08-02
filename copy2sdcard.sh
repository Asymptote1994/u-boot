sleep 2
ls /dev/sd*
sudo dd if=/home/zhangxu/study/s3c2440/u-boot/u-boot.bin of=/dev/sdb bs=512 seek=4
sync
