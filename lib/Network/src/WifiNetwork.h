
#ifndef CAM_WIFINETWORK_H
#define CAM_WIFINETWORK_H

#include "Arduino.h"

struct Credentials {
    String stationPass;
    String stationSsid;
    String appPass;
    String appSsid;
};

class WifiNetwork {
public:
    explicit WifiNetwork() = default;

    void connect(const Credentials&);

    bool connected();
};


#endif
