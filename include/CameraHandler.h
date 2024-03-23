#ifndef INFANTRY_2024_CV_CAMERAHANDLER_H
#define INFANTRY_2024_CV_CAMERAHANDLER_H

#include "CameraApi.h"

class CameraHandler {
public:
    explicit CameraHandler(tSdkCameraDevInfo cameraDevInfo = {}) : m_cameraDevInfo(cameraDevInfo) {};

    CameraSdkStatus openCamera(); // 初始化相机并使相机进入工作模式
    CameraSdkStatus closeCamera(); // 反初始化并关闭相机

    bool getCameraIsOpened() const;

    CameraHandle getCameraHandle() const;

    ~CameraHandler();

private:
    tSdkCameraDevInfo m_cameraDevInfo{}; // 相机信息
    CameraHandle m_cameraHandle = {}; // 相机句柄，用于传入SDK区分多个相机
    bool m_cameraIsOpened = false; // 指示相机是否打开
};


#endif //INFANTRY_2024_CV_CAMERAHANDLER_H
