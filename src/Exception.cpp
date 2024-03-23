#include "Exception.h"

runtime_error Exception::returnRuntimeError(const string &message) {
    ostringstream oss;
    oss << message << ".";
    return runtime_error(oss.str());
}

runtime_error Exception::returnRuntimeError(const string &message, CameraSdkStatus cameraSdkStatus) {
    ostringstream oss;
    oss << message << " - Camera SDK Status (" << cameraSdkStatus << ").";
    return runtime_error(oss.str());
}
