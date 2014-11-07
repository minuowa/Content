#pragma once


#define CURSORNUM 10

enum eCursor
{
	curNormal,		//正常
	curAttack,		//攻击
	curTalk,		//对话
	curTouch,		//抚摸
	curNotCanBeSelected,	//不可选择
	curGrasp,		//抓住
	curNotMine,		//不是本人的
	curRepair,		//修理
	curTrade,		//交易
	curSystem,		//系统
	//curNoUse,		//不可用
	//curDynamic,		//动态的
};

class CXCursor
{
public:

	CXCursor(void);

	~CXCursor(void);

	bool Init();

	void SetNowCursor(eCursor cur);

	HCURSOR GetNowCursor();

private:

	HCURSOR mhCursor[CURSORNUM];

	HCURSOR mhNowCursor;

};
extern CXCursor gCursor;