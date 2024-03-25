#include "LoadMVCam.h"

LoadMVCam::LoadMVCam(int cameraIndex, int exposure_time) {
    m_cameraHandler = m_cameraManager.getCameraHandler(cameraIndex);

    if (m_cameraHandler.getCameraIsOpened()) {
        throw CameraException("相机已经被打开");
    } else {
        m_cameraHandler.openCamera();
    }

    CameraSdkStatus cameraSdkStatus = CameraSetIspOutFormat(m_cameraHandler.getCameraHandle(), CAMERA_MEDIA_TYPE_RGB16);
    cameraSdkStatus += CameraSetMonochrome(m_cameraHandler.getCameraHandle(), FALSE);
    cameraSdkStatus += CameraSetAeState(m_cameraHandler.getCameraHandle(), FALSE);
    cameraSdkStatus += CameraSetExposureTime(m_cameraHandler.getCameraHandle(), exposure_time);
    if (cameraSdkStatus != 0){
        throw CameraException("相机参数设置失败");
    }
}
