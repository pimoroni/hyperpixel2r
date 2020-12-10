# HyperPixel 2.0" Round Drivers for Raspberry Pi 4

HyperPixel 2.0r is an 480x480 pixel display for the Raspberry Pi, with optional capacitive touchscreen.

These drivers are for the Raspberry Pi 4 specifically, and include new tools to take advantage of the "Screen Configuration" (xrandr) support that Pi 4's 3D accelerated desktop enables.

## Installing / Uninstalling

First, clone this GitHub repository branch to your Pi:

```
git clone https://github.com/pimoroni/hyperpixel2r
```

Then run the installer to install:

```
cd hyperpixel2r
sudo ./install.sh
```

## Rotation

To keep your touchscreen rotated with the display, you should rotate HyperPixel2r using the `hyperpixel2r-rotate` command rather than "Screen Configuration."

This command will update your touch settings and screen configuration settings to match, and you can rotate between four modes: left, right, normal, inverted.

* left - landscape, power/HDMI on bottom
* right - landscape, power/HDMI on top
* normal - portrait, USB ports on top
* inverted - portrait, USB ports on bottom

If you want to change the position of your HyperPixel2r in a multi-display setup, you can use "Screen Configuration" as normal.

## Touch rotation

Touch is only supported via the Python library - https://github.com/pimoroni/hyperpixel2r-python
