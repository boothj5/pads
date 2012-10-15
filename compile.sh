#!/bin/sh

rm -f pads
gcc pads.c `pkg-config --libs ncurses` -o pads
