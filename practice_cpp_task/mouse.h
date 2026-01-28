#ifndef MOUSE_H
#define MOUSE_H

#include <functional>
#include <thread>

class MouseDriver {
public:
    using MouseCallback = std::function<void(int x, int y)>;

    MouseDriver();
    ~MouseDriver();

    void setCallback(MouseCallback cb);
    void start();
    void stop();

private:
    MouseCallback callback;
    std::thread mouseThread;
    bool running;
};

class CoordinatePrinter {
public:
    void printxy(float x, float y);
};

#endif
