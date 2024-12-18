#include "Detector.h"

bool Detector::hasHuman(camera_fb_t *f) {
    if (!fmt2rgb888(f->buf, f->len, PIXFORMAT_JPEG, matrix->item)) {
        return false;
    }

    mtmn_config_t detectConfig = mtmn_init_config();
    box_array_t *boxes = face_detect(matrix, &detectConfig);
    bool has = false;
    if (boxes != nullptr) {
        has = true;
        dl_lib_free(boxes->score);
        dl_lib_free(boxes->box);
        dl_lib_free(boxes->landmark);
        dl_lib_free(boxes);
    }

    return has;
}

Detector::Detector(framesize_t frameSize) {
    auto w = 0;
    auto h = 0;
    switch (frameSize) {
        case FRAMESIZE_QVGA:
            w = 320;
            h = 240;
            break;
        case FRAMESIZE_VGA:
            w = 640;
            h = 480;
            break;
    }
    matrix = dl_matrix3du_alloc(1, w, h, 3);
    if (!matrix) {
        throw exception();
    }
}
