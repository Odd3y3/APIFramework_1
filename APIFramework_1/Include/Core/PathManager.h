#pragma once

#include "../Game.h"

class CPathManager
{
	DECLARE_SINGLE(CPathManager)

private:
	unordered_map<string, wstring> m_mapPath;

public:
	bool Init();
	bool CreatePath(const string& strKey, const WCHAR* pPath, const string& strBaseKey = ROOT_PATH);
	const WCHAR* FindPath(const string& strKey);
};

