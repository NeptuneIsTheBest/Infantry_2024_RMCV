#include "CameraManager.h"

CameraManager::CameraManager() {
    CameraSdkStatus cameraSdkStatus;
    cameraSdkStatus = CameraSdkInit(1);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        throw Exception::returnRuntimeError("初始化SDK时失败", cameraSdkStatus);
    }

    m_sdkInitialized = true;

    cameraSdkStatus = CameraEnumerateDevice(m_cameraInfos, &m_totalCameraCount);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        throw Exception::returnRuntimeError("枚举相机时失败", cameraSdkStatus);
    }
    for (int i = 0; i < m_totalCameraCount; i++) {
        m_cameraHandlers[i] = CameraHandler(m_cameraInfos[i]);
    }
}

CameraHandler CameraManager::getCameraHandler(int cameraIndex) const {
    if (cameraIndex >= m_totalCameraCount) {
        throw Exception::returnRuntimeError("相机索引大于可用相机数量");
    }

    return m_cameraHandlers[cameraIndex];
}
