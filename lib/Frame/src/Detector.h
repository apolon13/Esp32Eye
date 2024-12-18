#ifndef CAM_DETECTOR_H
#define CAM_DETECTOR_H

#include "Camera.h"
#include <fd_forward.h>
#include <esp_camera.h>

class Detector {
private:
    dl_matrix3du_t *matrix;
public:
    explicit Detector(framesize_t frameSize);

    bool hasHuman(camera_fb_t *f);
};


#endif //CAM_DETECTOR_H
