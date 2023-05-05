#!/bin/sh

g++ AsusUtils.cpp -g -o AsusUtils

sudo cp 45-asusutils-backlight.rules /etc/udev/rules.d/45-asusutils-backlight.rules
sudo usermod -aG video $USER
