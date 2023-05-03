#!/bin/sh

g++ AsusUtils.cpp -g -o AsusUtils
sudo chown root:root AsusUtils
sudo chmod 4755 AsusUtils
