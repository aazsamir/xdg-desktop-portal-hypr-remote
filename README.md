# Notice

This is a fork of https://github.com/gac3k/xdg-desktop-portal-hypr-remote. It has been modified to use new dbus interfaces, it is largely vibe-coded and done due to personal interest. It seems to work fine for using Hyprland with kdeconnect.

# Hyprland Remote Desktop Portal

A complete implementation of the `org.freedesktop.impl.portal.RemoteDesktop` portal for Hyprland, providing remote desktop capabilities using libei and Wayland virtual input protocols.

#### **System Installation**

```bash
# Install portal system-wide
sudo make install

# Copy portal configuration
sudo cp data/hyprland.portal /usr/share/xdg-desktop-portal/portals/

# Restart portal services
systemctl --user restart xdg-desktop-portal
```

## 🚀 Quick Start

### Manual Build

Install dependencies:
- cmake, pkg-config, gcc
- wayland-client, wayland-protocols, wayland-scanner
- libei-1.0, sdbus-c++, systemd

Then:
```bash
./build.sh
```
## 🔧 Troubleshooting

### ✅ "Permission denied" D-Bus Errors - SOLVED
- **Fixed**: Now uses session bus instead of system bus
- **Fixed**: Correct method signatures prevent registration errors

### ✅ "File exists" D-Bus Errors - EXPECTED
- **Normal**: Occurs when another portal uses the same service name
- **Solution**: Use development mode (automatic in `test_portal.sh`)

### Testing Portal Integration

Check if the portal is discoverable:

```bash
# List all portal services
busctl --user list | grep portal

# Monitor D-Bus calls to your portal
busctl --user monitor org.freedesktop.impl.portal.desktop.hyprland.dev

# Test method calls
busctl --user call org.freedesktop.impl.portal.desktop.hyprland.dev \
  /org/freedesktop/portal/desktop \
  org.freedesktop.impl.portal.RemoteDesktop \
  CreateSession 'a{sv}' 0
```
## 📁 Project Structure

```
hyprland-remote-desktop/
├── src/
│   ├── main.cpp                    # Main application entry point
│   ├── portal.cpp/.h               # D-Bus portal implementation
│   ├── wayland_virtual_keyboard.cpp/.h  # Virtual keyboard protocol
│   ├── wayland_virtual_pointer.cpp/.h   # Virtual pointer protocol
│   └── libei_handler.cpp/.h        # LibEI event processing
├── protocols/
│   ├── virtual-keyboard-unstable-v1.xml      # Wayland keyboard protocol
│   └── wlr-virtual-pointer-unstable-v1.xml   # wlroots pointer protocol
├── data/
│   ├── hyprland.portal                        # Portal configuration
│   └── org.freedesktop.impl.portal.desktop.hyprland.service.in
├── shell.nix                       # NixOS development environment
├── CMakeLists.txt                  # Build configuration
├── build.sh                        # Build script
├── test_portal.sh                  # Development testing script
└── README.md                       # This file
```

## 🧪 Testing Commands

```bash
# Build and test
./test_portal.sh

# Manual testing
nix-shell
./build.sh
./build/hyprland-remote-desktop

# D-Bus testing
busctl --user introspect org.freedesktop.impl.portal.desktop.hyprland.dev /org/freedesktop/portal/desktop
busctl --user call org.freedesktop.impl.portal.desktop.hyprland.dev /org/freedesktop/portal/desktop org.freedesktop.impl.portal.RemoteDesktop CreateSession 'a{sv}' 0
```

## 🤝 Contributing

1. Use the provided `shell.nix` for development
2. Follow the existing code structure
3. Test with `./test_portal.sh`
4. Ensure both keyboard and pointer protocols work
5. Verify D-Bus method signatures match the portal specification

## 📄 License

MIT License - see LICENSE file for details.
