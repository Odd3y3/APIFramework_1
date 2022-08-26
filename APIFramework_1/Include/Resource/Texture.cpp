#include "Texture.h"
#include "../Core/PathManager.h"

CTexture::CTexture()
	:m_hMemDC(NULL)
{
}

CTexture::~CTexture()
{
	//������ �����Ǿ� �ִ� ������ �ٽ� �������ش�.
	SelectObject(m_hMemDC, m_hOldBitmap);
	
	// Bitmap�� �����ش�.
	DeleteObject(m_hBitmap);

	// DC�� �����ش�.
	DeleteDC(m_hMemDC);
}

bool CTexture::LoadTexture(HINSTANCE hInst, HDC hDC, const string& strKey, const WCHAR* pFileName, const string& strPathKey)
{
	// �޸� DC�� ������ش�.
	m_hMemDC = CreateCompatibleDC(hDC);

	// ��ü ��θ� ������ش�.PathManager
	const WCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	wstring strPath;
	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// ������ ������� ��Ʈ�� ������ DC�� �����Ѵ�.
	// �����Ҷ� ��ȯ�Ǵ� ���� DC�� �⺻���� �����Ǿ� �ִ� ������ ��ȯ�ȴ�.
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	GetObject(m_hBitmap, sizeof(m_tInfo), &m_tInfo);


	return true;
}