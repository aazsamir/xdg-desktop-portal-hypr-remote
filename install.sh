#!/bin/sh

set -e

cd "$(dirname "$0")"

echo "Building"
./build.sh
echo "Installing"
cd build
sudo make install
echo "Copying systemd service file"
sudo cp contrib/systemd/xdg-desktop-portal-hypr-remote.service /etc/systemd/user/xdg-desktop-portal-hypr-remote.service
