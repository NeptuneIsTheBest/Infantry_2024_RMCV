#ifndef INFANTRY_2024_CV_LOADMVCAM_H
#define INFANTRY_2024_CV_LOADMVCAM_H

#include "CameraManager.h"

#include "opencv2/opencv.hpp"

using namespace cv;

class LoadMVCam {
public:
    explicit LoadMVCam(int cameraIndex, int exposure_time = 1000);
    CameraSdkStatus getFrame(Mat& frame);
private:
    CameraManager& m_cameraManager = CameraManager::getInstance();
    CameraHandler m_cameraHandler;
};


#endif //INFANTRY_2024_CV_LOADMVCAM_H
