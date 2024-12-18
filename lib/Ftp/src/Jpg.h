#ifndef CAM_FILE_H
#define CAM_FILE_H

#include "FtpClient.h"

class Jpg {
protected:
    String filename;
    FtpClient &client;
public:
    Jpg(String &name, FtpClient &c);

    void Upload(uint8_t *buf, size_t len);
};


#endif //CAM_FILE_H
