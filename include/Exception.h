#ifndef INFANTRY_2024_CV_EXCEPTION_H
#define INFANTRY_2024_CV_EXCEPTION_H

#include <string>
#include <exception>
#include <utility>

#include "CameraStatus.h"

using namespace std;

class CameraException : public exception {
private:
    string message;
    CameraSdkStatus cameraSdkStatus;
    bool hasSdkStatus;
public:
    explicit CameraException(string msg) : message(std::move(msg)), cameraSdkStatus(), hasSdkStatus(false) {}

    CameraException(string msg, CameraSdkStatus status)
            : message(std::move(msg)), cameraSdkStatus(status), hasSdkStatus(true) {}

    const char * what() const noexcept override;

    bool hasCameraSdkStatus() const;

    CameraSdkStatus getCameraSdkStatus() const;
};

#endif //INFANTRY_2024_CV_EXCEPTION_H
