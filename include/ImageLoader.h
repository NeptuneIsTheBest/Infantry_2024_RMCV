#ifndef INFANTRY_2024_ARMORAIM_IMAGELOADER_H
#define INFANTRY_2024_ARMORAIM_IMAGELOADER_H

#include <cstdio>
#include "opencv2/opencv.hpp"
#include "CameraApi.h"

using namespace std;
using namespace cv;

class ImageLoader {
public:
    ImageLoader() {
        CameraSdkStatus sdkInitStatus = CameraSdkInit(1);
        if (sdkInitStatus != CAMERA_STATUS_SUCCESS) {
            printf("初始化相机SDK失败 (%d)", sdkInitStatus);
        }
    }

    int GetCameraCounts() {
        return CameraCounts;
    };

    void GetCameraParams(Mat &cameraMatrix, Mat &distCoeffs, vector<Mat> &rvecs, vector<Mat> &tvecs) {
        CameraMatrix.copyTo(cameraMatrix);
        DistCoeffs.copyTo(distCoeffs);
        rvecs.assign(Rvecs.begin(), Rvecs.end());
        tvecs.assign(Tvecs.begin(), Tvecs.end());
    }

    CameraSdkStatus SetAeState(BOOL aeState){
        return CameraSetAeState(CameraHandle, aeState);
    };

    CameraSdkStatus GetAeState(BOOL* aeState){
        return CameraGetAeState(CameraHandle, aeState);
    };

    CameraSdkStatus SetExposureTime(float exposureTime){
        return CameraSetExposureTime(CameraHandle, exposureTime);
    };

    CameraSdkStatus SetOutFormat(UINT format){
        return CameraSetIspOutFormat(CameraHandle, format);
    };

    CameraSdkStatus GetCameraList();
    CameraSdkStatus OpenCamera(int cameraIndex);
    CameraSdkStatus WarmUp(UINT format);
    CameraSdkStatus CloseCamera();
    CameraSdkStatus GetFrame(Mat &frame);
    CameraSdkStatus CalibrateCamera(Size boardSize, float squareSize);

private:
    Mat CameraMatrix, DistCoeffs;
    vector<Mat> Rvecs, Tvecs;

    int CameraCounts = 2;
    tSdkCameraDevInfo CameraEnumList[2];
    CameraHandle CameraHandle;
};

#endif //INFANTRY_2024_ARMORAIM_IMAGELOADER_H
