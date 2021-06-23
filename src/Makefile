build:
	dtc -I dts -O dtb -Wno-unit_address_vs_reg -o hyperpixel4.dtbo hyperpixel4.dts

install:
	cp hyperpixel4.dtbo /boot/overlays/

test:
	dtoverlay hyperpixel4.dtbo

init:
	gcc -o ../dist/hyperpixel2r-init -static hyperpixel2r-init.c -lgpiod
