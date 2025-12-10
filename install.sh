#!/bin/sh

set -e

cd "$(dirname "$0")"

echo "Building"
./build.sh
echo "Installing"
cd build
sudo make install
cd ..
echo "Copying portal file"
sudo cp data/hypr-remote.portal /usr/share/xdg-desktop-portal/portals/hypr-remote.portal
echo "Copying systemd service file"
sudo cp contrib/systemd/xdg-desktop-portal-hypr-remote.service.in /etc/systemd/user/xdg-desktop-portal-hypr-remote.service
