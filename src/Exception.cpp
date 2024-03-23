#include "Exception.h"

const char *CameraException::what() const noexcept {
    return message.c_str();
}

bool CameraException::hasCameraSdkStatus() const {
    return hasSdkStatus;
}

CameraSdkStatus CameraException::getCameraSdkStatus() const {
    if (!hasSdkStatus){
        throw logic_error("相机SDK状态码不适用于这个错误");
    }
    return cameraSdkStatus;
}
