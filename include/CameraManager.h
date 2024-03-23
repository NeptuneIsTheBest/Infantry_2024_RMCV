#ifndef INFANTRY_2024_CV_CAMERAMANAGER_H
#define INFANTRY_2024_CV_CAMERAMANAGER_H

#include "CameraApi.h"
#include "CameraHandler.h"
#include "Exception.h"

#define MAX_CAMERA_NUM 2

class CameraManager {
private:
    CameraManager();

    bool m_sdkInitialized = false;

    tSdkCameraDevInfo m_cameraInfos[MAX_CAMERA_NUM] = {};
    int m_totalCameraCount = 0;

    CameraHandler m_cameraHandlers[MAX_CAMERA_NUM];

public:
    CameraManager(const CameraManager &) = delete;

    CameraManager &operator=(const CameraManager &) = delete;

    static CameraManager &getInstance() {
        static CameraManager instance;
        return instance;
    }

    int getTotalCameraCount() const;
    CameraHandler getCameraHandler(int cameraIndex) const;
};


#endif //INFANTRY_2024_CV_CAMERAMANAGER_H
