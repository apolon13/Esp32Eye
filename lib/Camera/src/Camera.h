#ifndef CAM_CAMERA_H
#define CAM_CAMERA_H
#include "exception"
#include "esp_camera.h"
#include <functional>

using namespace std;

class CameraInitException: public exception {

};

class Camera {
public:
    Camera(framesize_t frameSize, int jpegQuality);
    void onFrame(const function<void(camera_fb_t *)> &n);
};

#endif //CAM_CAMERA_H
