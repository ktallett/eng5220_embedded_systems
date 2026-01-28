#include "mouse.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

MouseDriver::MouseDriver() : running(false) {}

MouseDriver::~MouseDriver() {
    stop();
}

void MouseDriver::setCallback(MouseCallback cb) {
    callback = cb;
}

void MouseDriver::start() {
    running = true;
    mouseThread = std::thread([this]() {
        int fd, bytes;
        unsigned char data[3];
        const char *pDevice = "/dev/input/mice";

        int absoluteX = 0;
        int absoluteY = 0;

        fd = open(pDevice, O_RDWR);
        if(fd == -1) {
            printf("ERROR Opening %s\n", pDevice);
            running = false;
            return;
        }

        signed char x, y;
        while(running) {
            bytes = read(fd, data, sizeof(data));

            if(bytes > 0) {
                x = data[1];
                y = data[2];

                absoluteX += x;
                absoluteY += y;

                if(callback) {
                    callback(absoluteX, absoluteY);
                }
            }
        }

        close(fd);
    });
}

void MouseDriver::stop() {
    if(running) {
        running = false;
        if(mouseThread.joinable()) {
            mouseThread.join();
        }
    }
}

void CoordinatePrinter::printxy(float x, float y) {
    printf("x=%f, y=%f\n", x, y);
}
