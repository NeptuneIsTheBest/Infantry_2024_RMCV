#include "ImageLoader.h"

using namespace std;
using namespace cv;

CameraSdkStatus ImageLoader::GetCameraList() {
    CameraSdkStatus cameraSdkStatus;
    cameraSdkStatus = CameraEnumerateDevice(CameraEnumList, &CameraCounts);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("枚举相机失败 (%d)\n", cameraSdkStatus);
    } else {
        printf("共找到%d个相机\n", CameraCounts);
    }
    return cameraSdkStatus;
}

CameraSdkStatus ImageLoader::OpenCamera(int cameraIndex) {
    CameraSdkStatus cameraSdkStatus;
    BOOL cameraIsOpened;
    cameraSdkStatus = CameraIsOpened(&CameraEnumList[cameraIndex], &cameraIsOpened);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("检查相机%d是否打开失败 (%d)\n", cameraIndex, cameraSdkStatus);
        return cameraSdkStatus;
    }

    if (cameraIsOpened == TRUE) {
        printf("相机%d已经被打开\n", cameraIndex);
        return CAMERA_STATUS_DEVICE_IS_OPENED;
    }

    cameraSdkStatus = CameraInit(&CameraEnumList[cameraIndex], -1, -1, &CameraHandle);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("初始化相机%d失败 (%d)\n", cameraIndex, cameraSdkStatus);
        return cameraSdkStatus;
    }
    printf("初始化相机%d成功\n", cameraIndex);

    cameraSdkStatus = CameraPlay(CameraHandle);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("相机进入工作模式失败\n");
    } else {
        printf("相机进入工作模式成功\n");
    }
    return cameraSdkStatus;
}

CameraSdkStatus ImageLoader::CloseCamera() {
    CameraSdkStatus cameraSdkStatus = CameraStop(CameraHandle);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("关闭相机失败 (%d)\n", cameraSdkStatus);
    } else {
        printf("关闭相机成功\n");
    }
    return cameraSdkStatus;
}

CameraSdkStatus ImageLoader::WarmUp(UINT format) {
    CameraSdkStatus cameraSdkStatus;
    cameraSdkStatus = CameraSetIspOutFormat(CameraHandle, format);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("设置相机输出格式失败 (%d)\n", cameraSdkStatus);
        return cameraSdkStatus;
    }

    cameraSdkStatus = CameraSetMonochrome(CameraHandle, FALSE);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("设置相机彩色模式失败 (%d)\n", cameraSdkStatus);
        return cameraSdkStatus;
    }

    cameraSdkStatus = CameraSetAeState(CameraHandle, FALSE);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("设置相机自动曝光失败 (%d)\n", cameraSdkStatus);
        return cameraSdkStatus;
    }

    cameraSdkStatus = CameraSetExposureTime(CameraHandle, 1000);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("设置相机曝光时间失败 (%d)\n", cameraSdkStatus);
        return cameraSdkStatus;
    }
    return cameraSdkStatus;
}

CameraSdkStatus ImageLoader::GetFrame(cv::Mat &frame) {
    CameraSdkStatus cameraSdkStatus;
    tSdkCameraCapbility CameraCapability;

    cameraSdkStatus = CameraGetCapability(CameraHandle, &CameraCapability);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("获取相机特性失败 (%d)\n", cameraSdkStatus);
        return cameraSdkStatus;
    }

    int frameBufferSize =
            CameraCapability.sResolutionRange.iHeightMax * CameraCapability.sResolutionRange.iWidthMax * 3;
    tSdkFrameHead frameInfo;
    BYTE *rawFrameBuffer;
    BYTE *frameBuffer = (BYTE *) CameraAlignMalloc(frameBufferSize, 16);

    cameraSdkStatus = CameraGetImageBuffer(CameraHandle, &frameInfo, &rawFrameBuffer, 1000);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("获取相机图像失败 (%d)\n", cameraSdkStatus);
        return cameraSdkStatus;
    }

    cameraSdkStatus = CameraImageProcess(CameraHandle, rawFrameBuffer, frameBuffer, &frameInfo);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("图像处理失败 (%d)\n", cameraSdkStatus);
        return cameraSdkStatus;
    }

    frame = Mat(frameInfo.iHeight, frameInfo.iWidth, CV_8UC3, frameBuffer);

    cameraSdkStatus = CameraReleaseImageBuffer(CameraHandle, frameBuffer);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("释放图像缓冲区失败 (%d)\n", cameraSdkStatus);
        return cameraSdkStatus;
    }

    CameraAlignFree(frameBuffer);
    return cameraSdkStatus;
}

CameraSdkStatus ImageLoader::CalibrateCamera(Size boardSize, float squareSize) {
    vector<vector<Point3f>> objectPoints;
    vector<vector<Point2f>> imagePoints;

    vector<Point3f> obj;
    for (int i = 0; i < boardSize.height; i++) {
        for (int j = 0; j < boardSize.width; j++) {
            obj.emplace_back((float) j * squareSize, (float) i * squareSize, 0);
        }
    }

    CameraSdkStatus cameraSdkStatus;
    BOOL aeState;
    double exposureTime;
    cameraSdkStatus = CameraGetAeState(CameraHandle, &aeState);
    if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
        printf("获取相机自动曝光失败 (%d)\n", cameraSdkStatus);
        return cameraSdkStatus;
    } else {
        if (aeState == FALSE) {
            cameraSdkStatus = CameraGetExposureTime(CameraHandle, &exposureTime);
            if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
                printf("获取相机曝光时间失败 (%d)\n", cameraSdkStatus);
                return cameraSdkStatus;
            }

            printf("开启相机自动曝光\n");
            cameraSdkStatus = CameraSetAeState(CameraHandle, TRUE);
            if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
                printf("设置相机自动曝光失败 (%d)\n", cameraSdkStatus);
                return cameraSdkStatus;
            }
        }
    }

    Mat frame, garyFrame;
    int validFrameCount = 0;
    while (validFrameCount < 20) {
        cameraSdkStatus = GetFrame(frame);
        if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
            printf("获取相机图像失败 (%d)\n", cameraSdkStatus);
            return cameraSdkStatus;
        }

        imshow("Calibrate", frame);
        if (waitKey(1) != 32) {
            continue;
        }

        cvtColor(frame, garyFrame, COLOR_BGR2GRAY);

        vector<Point2f> corners;
        bool found = findChessboardCorners(garyFrame, boardSize, corners,
                                           CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
        if (found) {
            cornerSubPix(garyFrame, corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));

            imagePoints.emplace_back(corners);
            objectPoints.emplace_back(obj);

            validFrameCount++;

            printf("已找到%d/%d张棋盘格\n", validFrameCount, 20);
        } else {
            printf("未找到棋盘格\n");
        }
    }
    destroyWindow("Calibrate");

    calibrateCamera(objectPoints, imagePoints, frame.size(), CameraMatrix, DistCoeffs, Rvecs, Tvecs);
    printf("相机标定完成\n");

    if (aeState == FALSE) {
        printf("关闭相机自动曝光\n");
        cameraSdkStatus = CameraSetAeState(CameraHandle, FALSE);
        if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
            printf("设置相机自动曝光失败 (%d)\n", cameraSdkStatus);
            return cameraSdkStatus;
        }
        cameraSdkStatus = CameraSetExposureTime(CameraHandle, exposureTime);
        if (cameraSdkStatus != CAMERA_STATUS_SUCCESS) {
            printf("设置相机曝光时间失败 (%d)\n", cameraSdkStatus);
            return cameraSdkStatus;
        }
    }

    return cameraSdkStatus;
}
