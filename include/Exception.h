#ifndef INFANTRY_2024_CV_EXCEPTION_H
#define INFANTRY_2024_CV_EXCEPTION_H

#include <string>
#include <exception>
#include <utility>

#include "CameraStatus.h"

using namespace std;

class CameraException : public exception {
private:
    string m_message;
    CameraSdkStatus m_cameraSdkStatus;
    bool m_hasSdkStatus;
public:
    explicit CameraException(string msg) : m_message(std::move(msg)), m_cameraSdkStatus(), m_hasSdkStatus(false) {}

    CameraException(string msg, CameraSdkStatus status)
            : m_message(std::move(msg)), m_cameraSdkStatus(status), m_hasSdkStatus(true) {}

    const char * what() const noexcept override;

    bool hasCameraSdkStatus() const;

    CameraSdkStatus getCameraSdkStatus() const;
};

#endif //INFANTRY_2024_CV_EXCEPTION_H
