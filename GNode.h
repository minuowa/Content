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

    void linkTo ( CXRapidxmlNode* parent );
    GNode* getNodeByName ( const char* name );
    virtual void clear();
    virtual bool reCreate();
public:
    DWORD getObjCount();					//��ȡ���д��������������������Ѿ����ٵĶ���
	void setParentBone ( GNode *Parent, const char *sName );
    int getObjID() const;
	GNode* addChild ( GNode* c);
    bool	removeChild ( GNode* child );

    virtual bool draw();
    virtual void GetInput ( DWORD frameTimeMs );
    virtual void update();


    //������������һ�£��õ������ڵ�ͼ�ϵ�λ��
    IntersectInfo *UpdateForForceOnMap();

    IntersectInfo *UpdateForForceOnObj ( void *pObj );
    D3DXMATRIX GetWorldMatrix ( bool bForTrans );
    virtual eObjAnimState SetState ( eObjAnimState oas, bool bBack ) ;
	virtual D3DXMATRIX GetWorldMatrixByBone (const char *sBoneName, bool bForTrans = false );

    void ForceOnMap ( void *pMap, float fForceHeight, eForceType ft );

    IntersectInfo *GetBlockPoint();

    void SetDir ( D3DXVECTOR3 vNormal );

    GComponentTrans& getTrans() const;
    void updateTrans();
    void onComponentChange ( GComponentInterface* component, bool canDetach, bool notifyEditor );
    virtual void onPropertyChange ( void* pre, void* changed );
protected:
	virtual void registerAllProperty();
protected:
    virtual void beginRender();
    virtual void endRender();
    virtual bool render();
    void	MakeXMLNode ( CXRapidxmlNode& node );

public:
	static CXDelegate mDelegateCreateObj;
	static CXDelegate mDelegateDesotoryObj;

	static GNode* mOperatorParentObj;
	static GNode* mOperatorObj;
	static CXDelegate mDelegateAddObj;

	static CXDelegate mDelegateComponentChange;

    CXDynaArray<GNode*> mChildren;

    GComponentOwner	mComponentOwner;

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

    bool m_bCanSelect;					//�Ƿ��ѡ�񲢿���

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

    D3DXMATRIX _matWorld;				//������������

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

typedef CXDynaArray<GNode*> GNodeArr;
