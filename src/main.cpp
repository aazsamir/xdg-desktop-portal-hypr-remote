#include "portal.h"
#include "wayland_virtual_keyboard.h"
#include "wayland_virtual_pointer.h"
#include "libei_handler.h"
#include <iostream>
#include <thread>
#include <signal.h>
#include <chrono>

static bool running = true;

void signal_handler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    running = false;
}

int main(int argc, char* argv[]) {
    // Parse command line arguments
    bool verbose = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--verbose" || arg == "-v") {
            verbose = true;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --verbose, -v    Enable verbose debug output" << std::endl;
            std::cout << "  --help, -h       Show this help message" << std::endl;
            return 0;
        }
    }
    
    // Set up signal handling
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    std::cout << "Hyprland Remote Desktop Portal starting..." << std::endl;
    if (verbose) {
        std::cout << "[VERBOSE MODE ENABLED]" << std::endl;
    }
    
    // Initialize components
    WaylandVirtualKeyboard waylandVK;
    WaylandVirtualPointer waylandVP;
    LibEIHandler libeiHandler;
    Portal portal;
    
    // Initialize Wayland virtual keyboard
    if (!waylandVK.init()) {
        std::cerr << "Failed to initialize Wayland virtual keyboard" << std::endl;
        return 1;
    }
    std::cout << "✓ Virtual keyboard initialized" << std::endl;
    
    // Initialize Wayland virtual pointer
    if (!waylandVP.init()) {
        std::cerr << "Failed to initialize Wayland virtual pointer" << std::endl;
        waylandVK.cleanup();
        return 1;
    }
    std::cout << "✓ Virtual pointer initialized" << std::endl;
    
    // Initialize libei handler
    if (!libeiHandler.init(&waylandVK, &waylandVP)) {
        std::cerr << "Failed to initialize LibEI handler" << std::endl;
        waylandVP.cleanup();
        waylandVK.cleanup();
        return 1;
    }
    std::cout << "✓ LibEI handler initialized" << std::endl;
    
    // Start LibEI handler in background thread
    std::thread libei_thread([&libeiHandler]() {
        libeiHandler.run();
    });
    
    std::cout << "✓ LibEI handler started and ready for connections" << std::endl;
    
    // Set verbose mode
    portal.setVerbose(verbose);
    
    // Initialize portal
    if (!portal.init(&libeiHandler)) {
        std::cerr << "Failed to initialize D-Bus portal" << std::endl;
        libeiHandler.stop();

        if (libei_thread.joinable()) {
            libei_thread.join();
        }

        libeiHandler.cleanup();
        waylandVP.cleanup();
        waylandVK.cleanup();
        std::cerr << "Exiting..." << std::endl;

        return 1;
    }
    std::cout << "✓ D-Bus portal initialized" << std::endl;
    
    std::cout << "\n🚀 Hyprland Remote Desktop Portal is ready!" << std::endl;
    std::cout << "Portal available at: org.freedesktop.impl.portal.desktop.hypr-remote" << std::endl;
    std::cout << "Press Ctrl+C to stop." << std::endl;
    
    // Start portal in separate thread
    std::thread portal_thread([&portal]() {
        portal.run();
    });
    
    // Main loop - just wait for shutdown signal
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "\nShutting down components..." << std::endl;
    
    // Stop and cleanup
    portal.stop();
    if (portal_thread.joinable()) {
        portal_thread.join();
    }
    
    libeiHandler.stop();
    if (libei_thread.joinable()) {
        libei_thread.join();
    }
    
    // Cleanup in reverse order
    portal.cleanup();
    libeiHandler.cleanup();
    waylandVP.cleanup();
    waylandVK.cleanup();
    
    std::cout << "✓ Shutdown complete" << std::endl;
    return 0;
} 