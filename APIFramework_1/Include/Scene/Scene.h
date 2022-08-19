#pragma once

#include "../Game.h"

class CScene
{
protected:
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

protected:
	list<class CLayer*> m_LayerList;

public:
	class CLayer* CreateLayer(const string& strTag, int iZOrder = 0);
	class CLayer* FindLayer(const string& strTag);

public:
	virtual bool Init();

public:
	static bool LayerSort(class CLayer* pL1, class CLayer* pL2);
};
