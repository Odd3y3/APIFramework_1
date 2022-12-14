#pragma once

#include "../Ref.h"

class CTexture :
    public CRef
{
private:
    friend class CResourceManager;

private:
    CTexture();
    ~CTexture();

private:
    HDC     m_hMemDC;
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;
    BITMAP  m_tInfo;

public:
    bool LoadTexture(HINSTANCE hInst, HDC hDC, const string& strKey,
        const WCHAR* pFileName, const string& strPathKey = TEXTURE_PATH);
    HDC GetDC() const
    {
        return m_hMemDC;
    }
};

