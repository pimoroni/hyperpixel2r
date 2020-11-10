#!/bin/bash

SERVICE_NAME="hyperpixel2r-init.service"
SERVICE_PATH="/etc/systemd/system"
BINARY_NAME="hyperpixel2r-init"
ROTATE_NAME="hyperpixel2r-rotate"
BINARY_PATH="/usr/bin"
OVERLAY_PATH="/boot/overlays"
OVERLAY_NAME="hyperpixel2r.dtbo"
OVERLAY_SRC="hyperpixel2r-overlay.dts"

CONFIG="/boot/config.txt"

CONFIG_LINES=(
	"dtoverlay=hyperpixel2r"
	"enable_dpi_lcd=1"
	"dpi_group=2"
	"dpi_mode=87"
	"dpi_output_format=0x7f216"
	"dpi_timings=480 0 10 16 55 480 0 15 60 15 0 0 0 60 0 19200000 6"
)

if [ $(id -u) -ne 0 ]; then
	printf "Script must be run as root. Try 'sudo ./uninstall.sh'\n"
	exit 1
fi

if [ -f "$SERVICE_PATH/$SERVICE_NAME" ]; then
	systemctl stop $SERVICE_NAME
	systemctl disable $SERVICE_NAME
	rm -f "$SERVICE_PATH/$SERVICE_NAME"
	systemctl daemon-reload
	printf "Removed: $SERVICE_PATH/$SERVICE_NAME\n"
else
	printf "Skipped: $SERVICE_PATH/$SERVICE_NAME, not installed\n"
fi

if [ -f "$BINARY_PATH/$ROTATE_NAME" ]; then
    rm -rf "$BINARY_PATH/$ROTATE_NAME"
    printf "Removed: $BINARY_PATH/$ROTATE_NAME\n"
else
    printf "Skipped: $BINARY_PATH/$ROTATE_NAME, not installed\n"
fi

if [ -f "$BINARY_PATH/$BINARY_NAME" ]; then
	rm -f "$BINARY_PATH/$BINARY_NAME"
	printf "Removed: $BINARY_PATH/$BINARY_NAME\n"
else
	printf "Skipped $BINARY_PATH/$BINARY_NAME, not installed\n"
fi

if [ -f "$OVERLAY_PATH/$OVERLAY_NAME" ]; then
	rm -f "$OVERLAY_PATH/$OVERLAY_NAME"
	printf "Removed: $OVERLAY_PATH/$OVERLAY_NAME\n"
else
	printf "Skipped: $OVERLAY_PATH/$OVERLAY_NAME, not installed\n"
fi

if [ -f "$CONFIG" ]; then
	for ((i = 0; i < ${#CONFIG_LINES[@]}; i++)); do
		CONFIG_LINE="${CONFIG_LINES[$i]}"

		grep -e "^$CONFIG_LINE" $CONFIG > /dev/null
		STATUS=$?
		if [ $STATUS -eq 0 ]; then
			sed -i "/^$CONFIG_LINE/d" $CONFIG
			printf "Removed: line $CONFIG_LINE from $CONFIG\n"
		else
			printf "Skipped: $CONFIG, line $CONFIG_LINE not found\n"
		fi
	done
fi
