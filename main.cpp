#include <iostream>
#include <thread>

#include "include/ImageLoader.h"

#define MT

int main() {
#ifdef MT
    ImageLoader imageLoader = ImageLoader();
    imageLoader.GetCameraList();
    imageLoader.OpenCamera(imageLoader.GetCameraCounts() - 1);
    imageLoader.WarmUp(CAMERA_MEDIA_TYPE_BGR8);
    Size boardSize = Size(9, 6);
    float squareSize = 0.025f;
    imageLoader.CalibrateCamera(boardSize, squareSize);
    Mat cameraMatrix, distCoeffs;
    vector<Mat> rvecs, tvecs;
    imageLoader.GetCameraParams(cameraMatrix, distCoeffs, rvecs, tvecs);
    while (true){
        cv::Mat rawFrame, frame;
        imageLoader.GetFrame(rawFrame);
        undistort(rawFrame, frame, cameraMatrix, distCoeffs);
        cv::imshow("frame", frame);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    imageLoader.CloseCamera();
#else
    //
#endif
    return 0;
}
