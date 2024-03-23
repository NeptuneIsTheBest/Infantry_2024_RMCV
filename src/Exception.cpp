#include "Exception.h"

const char *CameraException::what() const noexcept {
    return m_message.c_str();
}

bool CameraException::hasCameraSdkStatus() const {
    return m_hasSdkStatus;
}

CameraSdkStatus CameraException::getCameraSdkStatus() const {
    if (!m_hasSdkStatus){
        throw logic_error("相机SDK状态码不适用于这个错误");
    }
    return m_cameraSdkStatus;
}
