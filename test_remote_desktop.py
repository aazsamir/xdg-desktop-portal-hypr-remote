#!/usr/bin/env python3

import dbus
import sys

def test_remote_desktop_portal():
    """Test if the RemoteDesktop portal is working"""
    try:
        # Connect to session bus
        bus = dbus.SessionBus()
        
        # Get the main portal proxy
        portal_proxy = bus.get_object('org.freedesktop.portal.Desktop', '/org/freedesktop/portal/desktop')
        
        print("🔍 Testing RemoteDesktop portal...")
        
        # Try to get the RemoteDesktop interface
        remote_desktop = dbus.Interface(portal_proxy, 'org.freedesktop.portal.RemoteDesktop')
        
        print("✅ RemoteDesktop interface found!")
        
        # Try to get the version property
        properties = dbus.Interface(portal_proxy, 'org.freedesktop.DBus.Properties')
        version = properties.Get('org.freedesktop.portal.RemoteDesktop', 'version')
        print(f"📋 RemoteDesktop portal version: {version}")
        
        # Try to create a session (this should trigger portal loading)
        print("🚀 Attempting to create RemoteDesktop session...")
        
        # Create session options
        options = {}
        
        # This should trigger your portal!
        response = remote_desktop.CreateSession(
            dbus.ObjectPath('/org/freedesktop/portal/desktop/request/test'),
            dbus.ObjectPath('/org/freedesktop/portal/desktop/session/test'),
            'test-app',
            options
        )
        
        print(f"✅ CreateSession response: {response}")
        return True
        
    except dbus.exceptions.DBusException as e:
        print(f"❌ D-Bus error: {e}")
        return False

if __name__ == '__main__':
    print("🔥 Testing RemoteDesktop Portal")
    print("=" * 40)
    
    success = test_remote_desktop_portal()
    
    if success:
        print("🎉 Portal test completed successfully!")
        sys.exit(0)
    else:
        print("💥 Portal test failed!")
        sys.exit(1) 