#pragma once


#define CURSORNUM 10

enum eCursor
{
	curNormal,		//����
	curAttack,		//����
	curTalk,		//�Ի�
	curTouch,		//����
	curNotCanBeSelected,	//����ѡ��
	curGrasp,		//ץס
	curNotMine,		//���Ǳ��˵�
	curRepair,		//����
	curTrade,		//����
	curSystem,		//ϵͳ
	//curNoUse,		//������
	//curDynamic,		//��̬��
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