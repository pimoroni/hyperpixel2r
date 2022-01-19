build:
	dtc -I dts -O dtb -Wno-unit_address_vs_reg -o hyperpixel2r.dtbo hyperpixel2r-overlay.dts

install:
	cp hyperpixel2r.dtbo /boot/overlays/

test:
	dtoverlay hyperpixel2r.dtbo

remove:
	dtoverlay -r hyperpixel2r

init:
	gcc -o ../dist/hyperpixel2r-init -static hyperpixel2r-init.c -lgpiod
