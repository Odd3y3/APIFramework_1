#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
}

bool CPathManager::Init()
{
	WCHAR strPath[MAX_PATH] = {};

	GetModuleFileName(NULL, strPath, MAX_PATH);

	for (int i = lstrlen(strPath) - 1; i >= 0; i--)
	{
		if (strPath[i] == '/' || strPath[i] == '\\')
		{
			memset(strPath + (i + 1), 0, sizeof(WCHAR) * (MAX_PATH - (i+1)));
			break;
		}
	}

	m_mapPath.insert(make_pair(ROOT_PATH, strPath));

	//Texture ��� ����
	if (!CreatePath(TEXTURE_PATH, TEXT("Texture\\")))
		return false;

	return true;
}

bool CPathManager::CreatePath(const string& strKey, const WCHAR* pPath, const string& strBaseKey)
{
	const WCHAR* pBasePath = FindPath(strBaseKey);

	wstring strPath;

	if (pBasePath)
		strPath = pBasePath;

	strPath += pPath;

	m_mapPath.insert(make_pair(strKey, strPath));

	return true;
}

const WCHAR* CPathManager::FindPath(const string& strKey)
{
	unordered_map<string, wstring>::iterator iter = m_mapPath.find(strKey);
	if (iter == m_mapPath.end())
		return NULL;

	return iter->second.c_str();
}
