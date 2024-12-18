#include "Jpg.h"

Jpg::Jpg(String &name, FtpClient &c) : filename(name), client(c) {

}

void Jpg::Upload(uint8_t *buf, size_t len) {
    client.InitFile("Type I");
    auto nameWithExt = filename + ".jpg";
    client.NewFile(nameWithExt.c_str());
    client.WriteData(buf, len);
    client.CloseFile();
}