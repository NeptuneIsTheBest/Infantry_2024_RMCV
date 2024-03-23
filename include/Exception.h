#ifndef INFANTRY_2024_CV_EXCEPTION_H
#define INFANTRY_2024_CV_EXCEPTION_H

#include <sstream>
#include <stdexcept>

#include "CameraApi.h"

using namespace std;

class Exception {
public:
    static runtime_error returnRuntimeError(const string &message);

    static runtime_error returnRuntimeError(const string &message, CameraSdkStatus cameraSdkStatus);
};


#endif //INFANTRY_2024_CV_EXCEPTION_H
