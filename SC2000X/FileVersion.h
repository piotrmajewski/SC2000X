#pragma once
#include <Windows.h>
#include <cstdio>
#include <map>
#include <string>

struct FileVersion
{
  static bool PrintFileVersionInfo(LPCSTR filename)
  {
    DWORD handle = 0;
    DWORD size = GetFileVersionInfoSizeA(filename, &handle);
    if (size == 0)
    {
      printf("GetFileVersionInfoSizeA returned a size of 0\n");
      return false;
    }
    LPVOID pVersionInfo = new BYTE[size];
    if (!GetFileVersionInfoA(filename, NULL, size, pVersionInfo))
    {
      printf("GetFileVersionInfoA returned false\n");
      return false;
    }

    UINT puLen2;
    struct LANGANDCODEPAGE {
      WORD wLanguage;
      WORD wCodePage;
    } *lpTranslate;

    if (!VerQueryValueA(pVersionInfo, "\\VarFileInfo\\Translation", (LPVOID*)&lpTranslate, &puLen2))
    {
      printf("VerQueryValueA returned false\n");
      return false;
    }

    const std::string descriptions[4] = {
      "FileVersion", "CompanyName", "FileDescription", "OriginalFilename"
    };

    for (size_t i = 0; i < 4; ++i)
    {
      UINT puLen;
      char buffer[256];
      sprintf_s(buffer, sizeof(buffer), 
        "\\StringFileInfo\\%04lx%04lx\\%s",
        lpTranslate->wLanguage,
        lpTranslate->wCodePage,
        descriptions[i].c_str());
      LPCSTR lpBuffer;
      if (!VerQueryValueA(pVersionInfo, buffer, (LPVOID*)&lpBuffer, &puLen))
      {
        printf("VerQueryValueA returned false\n");
        return false;
      }
      printf("%s: %s\n", descriptions[i].c_str(), lpBuffer);
    }
    return true;
  }
};