#include <WiFiClient.h>
#include "FtpClient.h"

FtpClient::FtpClient(char *_serverAdress, uint16_t _port, char *_userName, char *_passWord, uint16_t _timeout, uint8_t _verbose) {
    userName = _userName;
    passWord = _passWord;
    serverAdress = _serverAdress;
    port = _port;
    timeout = _timeout;
    verbose = _verbose;
}

bool FtpClient::isConnected() {
    if (!_isConnected) {
        FTPerr("FTP error: ");
        FTPerr(outBuf);
        FTPerr("\n");
    }

    return _isConnected;
}

void FtpClient::WriteClientBuffered(WiFiClient *cli, unsigned char *data, int dataLength) {
    if (!isConnected()) {
        return;
    }
    cli->write(data, dataLength);
}

void FtpClient::GetFTPAnswer() {
    char thisByte;
    outCount = 0;

    unsigned long _m = millis();
    while (!client.available() && millis() < _m + timeout) {
        delay(1);
    }

    if (!client.available()) {
        memset(outBuf, 0, sizeof(outBuf));
        strcpy(outBuf, "Offline");

        _isConnected = false;
        isConnected();
        return;
    }

    while (client.available()) {
        thisByte = client.read();
        if (outCount < sizeof(outBuf)) {
            outBuf[outCount] = thisByte;
            outCount++;
            outBuf[outCount] = 0;
        }
    }

    if (outBuf[0] == '4' || outBuf[0] == '5') {
        _isConnected = false;
        isConnected();
        return;
    } else {
        _isConnected = true;
    }
}

void FtpClient::WriteData(unsigned char *data, int dataLength) {
    FTPdbgn(F("Writing"));
    if (!isConnected()) {
        return;
    }
    WriteClientBuffered(&dclient, data, dataLength);
}

void FtpClient::CloseFile() {
    FTPdbgn(F("Close File"));
    dclient.stop();

    if (!_isConnected) {
        return;
    }

    GetFTPAnswer();
}

void FtpClient::CloseConnection() {
    client.println(F("QUIT"));
    client.stop();
    FTPdbgn(F("Connection closed"));
}

void FtpClient::OpenConnection() {
    FTPdbg(F("Connecting to: "));
    FTPdbgn(serverAdress);
    if (client.connect(serverAdress, port, timeout)) {
        FTPdbgn(F("Command connected"));
    }

    GetFTPAnswer();

    FTPdbgn("Send USER");
    client.print(F("USER "));
    client.println(F(userName));
    GetFTPAnswer();

    FTPdbgn("Send PASSWORD");
    client.print(F("PASS "));
    client.println(F(passWord));
    GetFTPAnswer();

    FTPdbgn("Send SYST");
    client.println(F("SYST"));
    GetFTPAnswer();
}

void FtpClient::NewFile(const char *fileName) {
    FTPdbgn("Send STOR");
    if (!isConnected()) {
        return;
    }
    client.print(F("STOR "));
    client.println(F(fileName));
    GetFTPAnswer();
}

void FtpClient::InitFile(const char *type) {
    FTPdbgn("Send TYPE");
    if (!isConnected()) {
        return;
    }
    FTPdbgn(type);
    client.println(F(type));
    GetFTPAnswer();

    FTPdbgn("Send PASV");
    client.println(F("PASV"));
    GetFTPAnswer();

    char *tStr = strtok(outBuf, "(,");
    int array_pasv[6];
    for (int i = 0; i < 6; i++) {
        tStr = strtok(NULL, "(,");
        if (tStr == NULL) {
            FTPdbgn(F("Bad PASV Answer"));
            CloseConnection();
            return;
        }
        array_pasv[i] = atoi(tStr);
    }
    unsigned int hiPort, loPort;
    hiPort = array_pasv[4] << 8;
    loPort = array_pasv[5] & 255;

    IPAddress pasvServer(array_pasv[0], array_pasv[1], array_pasv[2], array_pasv[3]);

    FTPdbg(F("Data port: "));
    hiPort = hiPort | loPort;
    FTPdbgn(hiPort);
    if (dclient.connect(pasvServer, hiPort, timeout)) {
        FTPdbgn(F("Data connection established"));
    }
}

void FtpClient::ChangeWorkDir(const char *dir) {
    FTPdbgn("Send CWD");
    if (!isConnected()) {
        return;
    }
    client.print(F("CWD "));
    client.println(F(dir));
    GetFTPAnswer();
}
