// Mouse Driver Task - Demonstrates STL and std::function callbacks
// Reads mouse input from /dev/input/mice and reports absolute coordinates

#include <functional>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// Mouse driver class using std::function for callback-based event handling
class MouseDriver {
public:
    using MouseCallback = std::function<void(int x, int y)>;

    void setCallback(MouseCallback cb) {
        callback = cb;
    }

    void start() {
        // Start reading mouse input in a separate thread
        std::thread([this]() {
            int fd, bytes;
            unsigned char data[3];
            const char *pDevice = "/dev/input/mice";

            int absoluteX = 0;
            int absoluteY = 0;

            // Open mouse device
            fd = open(pDevice, O_RDWR);
            if(fd == -1) {
                printf("ERROR Opening %s\n", pDevice);
                return;
            }

            signed char x, y;
            while(1) {
                // Blocking read - waits for mouse events
                bytes = read(fd, data, sizeof(data));

                if(bytes > 0) {
                    // Extract relative movement from mouse data
                    x = data[1];
                    y = data[2];

                    // Calculate absolute position
                    absoluteX += x;
                    absoluteY += y;

                    // Invoke callback with absolute coordinates
                    if(callback) {
                        callback(absoluteX, absoluteY);
                    }
                }
            }
        }).detach();
    }

private:
    MouseCallback callback;
};

// Simple printer class for displaying coordinates
class CoordinatePrinter {
public:
    void printxy(float x, float y) {
        printf("x=%f, y=%f\n", x, y);
    }
};

int main() {
    MouseDriver driver;
    CoordinatePrinter printer;

    // Connect callback using lambda to bind printer method
    driver.setCallback([&printer](int x, int y) {
        printer.printxy(static_cast<float>(x), static_cast<float>(y));
    });

    driver.start();

    printf("Mouse driver running. Press Enter to stop...\n");
    getchar();

    return 0;
}
