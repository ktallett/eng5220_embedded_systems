// Reads mouse input from /dev/input/mice and reports absolute coordinates

#include "mouse.h"
#include <csignal>
#include <cstdlib>
#include <stdio.h>

// Global pointer for signal handler
MouseDriver* g_driver = nullptr;

void signalHandler(int signum) {
    printf("\nInterrupt signal received. Stopping...\n");
    if(g_driver) {
        g_driver->stop();
    }
    exit(signum);
}

int main() {
    MouseDriver driver;
    CoordinatePrinter printer;

    g_driver = &driver;
    signal(SIGINT, signalHandler);

    driver.setCallback([&printer](int x, int y) {
        printer.printxy(static_cast<float>(x), static_cast<float>(y));
    });

    driver.start();

    printf("Mouse driver running. Press Enter to stop (or Ctrl-C)...\n");
    getchar();

    driver.stop();

    return 0;
}
