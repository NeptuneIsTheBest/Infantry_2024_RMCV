#include <iostream>
#include <thread>
#include <csignal>

#include "include/LoadMVCam.h"

// #define MT //多线程标志

void signalHandler(int signal) {
    exit(signal);
}

int main() {
    signal(SIGABRT, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
#ifdef MT
    //TODO
#else
    try {
        int totalCameraCount = CameraManager::getInstance().getTotalCameraCount();
        if (totalCameraCount == 0) {
            throw CameraException("未找到相机");
        }
        LoadMVCam loadMVCam = LoadMVCam(0);
    } catch (CameraException &e) {
        cerr << e.what() << " " << e.getCameraSdkStatus() << endl;
    }
#endif
    return 0;
}
