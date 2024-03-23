#include "CameraHandler.h"

CameraSdkStatus CameraHandler::openCamera() {
    if (m_cameraIsOpened) {
        return CAMERA_STATUS_DEVICE_IS_OPENED;
    }

    // 检查相机是否已经被打开
    BOOL cameraIsOpened;
    CameraSdkStatus cameraSdkStatus = CameraIsOpened(&m_cameraDevInfo, &cameraIsOpened);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        return cameraSdkStatus;
    } else if (cameraIsOpened == TRUE) {
        return CAMERA_STATUS_DEVICE_IS_OPENED;
    }

    cameraSdkStatus = CameraInit(&m_cameraDevInfo, -1, -1, &m_cameraHandle); // 进行其他操作之前需要先初始化相机
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        return cameraSdkStatus;
    }

    cameraSdkStatus = CameraPlay(m_cameraHandle); // 使相机进入工作模式
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        m_cameraIsOpened = true;
    } else {
        CameraUnInit(m_cameraHandle); // 进入工作模式失败时尝试反初始化相机
    }
    return cameraSdkStatus;
}

CameraSdkStatus CameraHandler::closeCamera() {
    if (!m_cameraIsOpened) {
        return CAMERA_STATUS_DEVICE_IS_CLOSED;
    }

    CameraSdkStatus cameraSdkStatus = CameraStop(m_cameraHandle); // 使SDK进入停止状态
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        return cameraSdkStatus;
    }
    cameraSdkStatus = CameraUnInit(m_cameraHandle); // 反初始化相机
    if (cameraSdkStatus == CAMERA_STATUS_SUCCESS) {
        m_cameraIsOpened = false;
    }
    return cameraSdkStatus;
}

bool CameraHandler::getCameraIsOpened() const {
    return m_cameraIsOpened;
}

CameraHandle CameraHandler::getCameraHandle() const {
    return m_cameraHandle;
}

CameraHandler::~CameraHandler() {
    closeCamera();
}
