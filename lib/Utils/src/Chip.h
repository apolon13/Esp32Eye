#ifndef CAM_UTILS_H
#define CAM_UTILS_H

#include "Arduino.h"

class Chip {
public:
    static uint32_t id() {
        uint32_t chipId = 0;
        for (int i = 0; i < 17; i = i + 8) {
            chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
        }
        return chipId;
    }
};


#endif //CAM_UTILS_H
