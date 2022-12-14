#include "Texture.h"
#include "../Core/PathManager.h"

CTexture::CTexture()
	:m_hMemDC(NULL)
{
}

CTexture::~CTexture()
{
	//기존에 지정되어 있던 도구를 다시 지정해준다.
	SelectObject(m_hMemDC, m_hOldBitmap);
	
	// Bitmap을 지워준다.
	DeleteObject(m_hBitmap);

	// DC를 지워준다.
	DeleteDC(m_hMemDC);
}

bool CTexture::LoadTexture(HINSTANCE hInst, HDC hDC, const string& strKey, const WCHAR* pFileName, const string& strPathKey)
{
	// 메모리 DC를 만들어준다.
	m_hMemDC = CreateCompatibleDC(hDC);

	// 전체 경로를 만들어준다.PathManager
	const WCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	wstring strPath;
	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 위에서 만들어준 비트맵 도구를 DC에 지정한다.
	// 지정할때 반환되는 값은 DC에 기본으로 지정되어 있던 도구가 반환된다.
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	GetObject(m_hBitmap, sizeof(m_tInfo), &m_tInfo);


	return true;
}
