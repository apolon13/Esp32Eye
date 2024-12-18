#include "WiFi.h"

class FtpClient {
private:
    void WriteClientBuffered(WiFiClient *cli, unsigned char *data, int dataLength);

    char outBuf[128];
    unsigned char outCount;
    WiFiClient client;
    WiFiClient dclient;
    uint8_t verbose;

    template<typename T>
    void FTPdbg(T msg) {
        if (verbose == 2) Serial.print(msg);
    }

    template<typename T>
    void FTPdbgn(T msg) {
        if (verbose == 2) Serial.println(msg);
    }

    template<typename T>
    void FTPerr(T msg) {
        if (verbose == 1 || verbose == 2) Serial.print(msg);
    }

    char *userName;
    char *passWord;
    char *serverAdress;
    uint16_t port;
    bool _isConnected = false;
    uint16_t timeout = 10000;

public:
    FtpClient(char *_serverAdress, uint16_t _port, char *_userName, char *_passWord, uint16_t _timeout = 10000,
              uint8_t _verbose = 1);

    void OpenConnection();

    void CloseConnection();

    bool isConnected();

    void NewFile(const char *fileName);

    void WriteData(unsigned char *data, int dataLength);

    void CloseFile();

    void GetFTPAnswer();

    void InitFile(const char *type);

    void ChangeWorkDir(const char *dir);
};
