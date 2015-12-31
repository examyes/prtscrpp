#pragma once
class Uploader {
    public:
        Uploader();
        ~Uploader();
        std::string imgur(CString str);
        CStringA ToBase64(const void* bytes, int byteLength);
};