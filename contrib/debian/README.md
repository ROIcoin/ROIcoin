
Debian
====================
This directory contains files used to package roicoind/roicoin-qt
for Debian-based Linux systems. If you compile roicoind/roicoin-qt yourself, there are some useful files here.

## roicoin: URI support ##


roicoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install roicoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your roicoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/roicoin128.png` to `/usr/share/pixmaps`

roicoin-qt.protocol (KDE)

