#!/bin/sh

g++ AsusUtils.cpp -g -o AsusUtils

sudo cp asusutils-backlight.rules /etc/udev/rules.d/asusutils-backlight.rules
sudo usermod -aG video $USER
