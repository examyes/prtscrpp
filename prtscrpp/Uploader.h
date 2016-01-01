#pragma once
class Uploader {
    public:
        Uploader();
        ~Uploader();
        CString getTempFile();
        std::string imgur();
        CStringA ToBase64(const void* bytes, int byteLength);
};