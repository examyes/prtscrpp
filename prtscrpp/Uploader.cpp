#include "stdafx.h"
#include "atlenc.h"
#include <afxinet.h>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <string>
#include "Uploader.h"

#define _EMPTY ""
#define new DEBUG_NEW

Uploader::Uploader() {
    // Grab settings here in a variable.
}

Uploader::~Uploader() {
}

CString Uploader::getTempFile() {
    TCHAR szPath[MAX_PATH];
    GetTempPath(MAX_PATH, szPath);

    // TODO: Add a custom format here someday.
    return CString(szPath + CString("prtscrpp_temp.png"));
}

// xLink's amazing inBetween https://github.com/xLink/CybershadeCMS/blob/c9602626163c753d1a13eafa0a6a00a13822a11c/core/baseFunctions.php
std::string InBetween(std::string content, std::string begin, std::string end) {
    if(content.empty() || begin.empty() || end.empty()) return _EMPTY;

    unsigned int pos1 = content.find(begin);
    unsigned int pos2 = 0;

    if(pos1 != std::string::npos) {
        pos1 += begin.length();
        pos2 = content.find(end, pos1);

        if(pos2 != std::string::npos) {
            return content.substr(pos1, pos2 - pos1);
        }
    }

    return _EMPTY;
}

// http://weblogs.asp.net/kennykerr/visual-c-in-short-encoding-and-decoding-with-base64
CStringA Uploader::ToBase64(const void* bytes, int byteLength) {
    ASSERT(0 != bytes);

    CStringA base64;
    int base64Length = Base64EncodeGetRequiredLength(byteLength);

    VERIFY(Base64Encode(static_cast<const BYTE*>(bytes),
        byteLength,
        base64.GetBufferSetLength(base64Length),
        &base64Length));

    base64.ReleaseBufferSetLength(base64Length);

    // URLEncode escape sequences, we'll need this.
    base64.Replace("\r\n", ""); // Also remove carriage returns, idk why this is added.
    base64.Replace("+", "%2B");
    base64.Replace("/", "%2F");
    base64.Replace("=", "%3D");

    return base64;
}

std::string Uploader::imgur() {
    FILE *fp; // File pointer
    unsigned char *buffer; // our buffer, a buffer of BYTE[]
    long fileLen; // Just the file length in bytes.

    // Convert the CStringW to a char* for fopen
    CString absPath = this->getTempFile();
    const size_t __path = (absPath.GetLength() + 1) * 2;
    char *absPathChar = new char[__path];
    size_t convertedCharsw = 0;
    wcstombs_s(&convertedCharsw, absPathChar, __path, absPath, _TRUNCATE);

    // Open the file in binary mode
    fp = fopen(absPathChar, "rb");
    if(!fp) {
        AfxMessageBox(_T("Failed to open the screenshot file."));
        return _EMPTY;
    }

    fseek(fp, 0, SEEK_END); // Jump to the end of the file
    fileLen = ftell(fp); // Get the size
    rewind(fp); // Jump back to the beginning of the file

    // Allocate memory
    buffer = (unsigned char *)malloc((fileLen)*sizeof(unsigned char));
    if(!buffer) {
        AfxMessageBox(_T("Failed to allocate memory."));
        return _EMPTY;
    }

    fread(buffer, fileLen, 1, fp); // Read in the entire file
    fclose(fp); // Close

    CStringA Dest = this->ToBase64(buffer, fileLen); // Base64 encode

    // Now construct an HTTP request.
    CString requestHeaders = _T("Content-Type: application/x-www-form-urlencoded\r\nAuthorization: Client-ID 3c1a6553ba0bbe4");

    // Cast it to a CStringA which can then be used to convert to a char *
    CStringA postData("image=" + Dest);

    // Convert to char* otherwise the server drops requests, time_wasted = ~10 hours.
    const size_t newsizea = (postData.GetLength() + 1);
    char *postDataChar    = new char[newsizea];
    strcpy_s(postDataChar, newsizea, postData);

    // Spawn a session here
    CInternetSession session(NULL, 1, PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, 0);
    if(!session) return _EMPTY;

    // Look up for 10 seconds
    session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 10000);
    session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 3);
    INTERNET_PORT port = 443; // We are demanded to use HTTPS so

    // Open up the connection
    CHttpConnection *pConnection = session.GetHttpConnection(_T("api.imgur.com"), port);

    // Some self-explanatory flags.
    DWORD dwRequestFlags = INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_SECURE | INTERNET_DEFAULT_HTTPS_PORT;

    // Open up a connection
    CHttpFile *pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, _T("/3/image.xml"), NULL, 1, NULL, NULL, dwRequestFlags);
    pFile->AddRequestHeaders(requestHeaders);

    // Send the request
    BOOL result = pFile->SendRequest(requestHeaders, (LPVOID)postDataChar, strlen(postDataChar));
    DWORD retCode;
    pFile->QueryInfoStatusCode(retCode);
    if(retCode != HTTP_STATUS_OK) { // API Failed
        AfxMessageBox(_T("API did not return 200 OK. Request failed."));

        delete[] postDataChar;free(buffer);
        return _EMPTY;
    }

    // Grab the request now
    char *szBuff = new char[1024];
    pFile->Read(szBuff, 1024);

    // Check out if it's empty or not
    if(!szBuff) {
        AfxMessageBox(_T("Failed to retrieve a response."));

        delete []postDataChar;delete []szBuff;free(buffer);
        return _EMPTY;
    }

    // Construct this into a std string
    std::string response(szBuff);
    response = InBetween(response, "<link>", "</link>"); // Grab the direct link.

    // Cleanup
    delete[] szBuff;
    delete[] postDataChar;

    pFile->Close();
    delete pFile;
    delete pConnection;

    free(buffer);
    session.Close();

    return response;
}