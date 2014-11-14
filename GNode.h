#pragma once

#include "GComponentTrans.h"
#include "GObject.h"
#include "GFactory.h"

//���������Ե�����
class GNode: public GObject
{
	DeclareEditorType ( GNode );
public:

	GNode();

	virtual ~GNode();

	void setNodeName ( CChar* name );
	GNode* getNodeByName ( const char* name );
	GNode* getFirstNodeByCategoryName ( const char* category );

	void linkTo ( CXRapidxmlNode* parent );
	virtual void clear();
	virtual bool recreate();
public:
	void setParentBone ( GNode *Parent, const char *sName );
	void setCanGetInput(bool can);
	int getObjID() const;
	GNode* addChild ( GNode* c );
	bool removeChild ( GNode* child );
	void deleteChild ( GNode* node );
	GNode* getParent() const;

	virtual bool draw();
	virtual void getInput ( DWORD frameTimeMs );
	virtual void update();
	virtual void updateWorld();


	//������������һ�£��õ������ڵ�ͼ�ϵ�λ��
	IntersectInfo *UpdateForForceOnMap();

	IntersectInfo *UpdateForForceOnObj ( void *pObj );
	virtual eObjAnimState SetState ( eObjAnimState oas, bool bBack ) ;
	virtual D3DXMATRIX GetWorldMatrixByBone ( const char *sBoneName, bool bForTrans = false );
	//void setWorldTranslate ( D3DXVECTOR3& v );
	void setDir ( D3DXVECTOR3 vNormal );

	void ForceOnMap ( void *pMap, float fForceHeight, eForceType ft );

	IntersectInfo *GetBlockPoint();


	GComponentTrans& getTrans() const;
	void updateTrans();
	void onComponentChange ( GComponentInterface* component, bool canDetach, bool notifyEditor );
	virtual void onPropertyChange ( void* pre, void* changed );
	virtual void onPropertyChangeEnd ( void* cur );
	CXDynaArray<GNode*>& getChildren();
protected:
	virtual void registerAllProperty();
protected:
	virtual void beginRender();
	virtual void endRender();
	virtual bool render();
	void	MakeXMLNode ( CXRapidxmlNode& node );

public:
	static CXDelegate mDelegateCreateObj;

	static GNode* mOperatorParentObj;
	static GNode* mOperatorObj;
	static CXDelegate mDelegateAddObj;
	static CXDelegate mDelegateDeleteObj;

	static CXDelegate mDelegateComponentChange;

	CXDynaArray<GNode*> mChildren;

	GComponentOwner	mComponentOwner;
	GMatrix mWorldMat;
	//eHitType m_HitType;					//�Ƿ�ײ������

	void *m_pOnObj;						//����������ķǵ�ͼ����

	bool m_bEyeCliper;					//�Ƿ���Ҫ�۾��ü�

	bool m_bForceOnMap;					//�Ƿ������ڵ�ͼ

	float m_fForceHeight;				//�����ڵ�ͼ�ϵĸ߶�

	void *mForceMap;					//�����ĵ�ͼ��ָ��

	eForceType m_ForceType;				//�����ڵ�ͼ�ϵ�����

	IntersectInfo m_InsectInfo;			//��ײ��Ϣ��ע�⣺�ڳ����е���һһ���ط���������ײ�����ܸı���ֵ



	float m_fBlockArea;					//���������������뾶��С

	float m_fBlockHeight;				//������������߶�


	DWORD mObjState;					//������Ⱦ״̬

	bool m_bBehaviour;					//�Ƿ������������û�в���������LastPos��Pos�������ͼ����ײ������׼ȷ

	bool mCanSelect;					//�Ƿ��ѡ�񲢿���

	bool m_bBeSelected;					//��ѡ����

	bool m_bUseMatrialColor;

	D3DXVECTOR3 m_vBlockPoint;

	bool m_bHit;

	bool m_bBlock;

	bool mCanGetInput;
	bool mDynamic;

	float m_fBoundRadius;

protected:

	eObjAnimState m_ObjAnimState;		//������������ǰ״̬

	//D3DXMATRIX _matWorld;				//������������

public:

	eObjParentType mOpt;

	GString mParentName;

	GNode *mParent;							//�����Ķ���

protected:

	DWORD _nID;								//���������еĴ����������еľ���ID


private:

	static DWORD __OBJCOUNTER;				//������Ŀ����

public:

	inline GComponentInterface* attachComponent ( eComponentType type, bool canDetach = true, bool notifyEditor = true )
	{
		GComponentInterface* component = mComponentOwner.attachComponent ( type );
		onComponentChange ( component, canDetach , notifyEditor );
		return component;
	}
	inline GComponentInterface* attachComponent ( const char* name, bool canDetach = true, bool notifyEditor = true )
	{
		GComponentInterface* component = mComponentOwner.attachComponent ( name );
		onComponentChange ( component, canDetach, notifyEditor );
		return component;
	}
	inline void detachComponent ( const char* name )
	{
		mComponentOwner.detachComponent ( name );
		onComponentChange ( 0, false, true );
	}
	inline GComponentOwner& getComponentOwner()
	{
		return mComponentOwner;
	}
};
inline GComponentTrans& GNode::getTrans() const
{
	GComponentTrans* pTrans = ( GComponentTrans* ) mComponentOwner.getComponent ( eComponentType_Trans );
	assert ( pTrans );
	return *pTrans;
}
typedef CXDynaArray<GNode*> GNodeArr;
