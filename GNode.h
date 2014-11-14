#pragma once

#include "GComponentTrans.h"
#include "GObject.h"
#include "GFactory.h"

//有坐标属性的物体
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


	//创建物体后更新一下，得到物体在地图上的位置
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
	//eHitType m_HitType;					//是否撞到物体

	void *m_pOnObj;						//本对象下面的非地图对象

	bool m_bEyeCliper;					//是否需要眼睛裁剪

	bool m_bForceOnMap;					//是否依附于地图

	float m_fForceHeight;				//依附于地图上的高度

	void *mForceMap;					//依附的地图的指针

	eForceType m_ForceType;				//依附于地图上的类型

	IntersectInfo m_InsectInfo;			//碰撞信息，注意：在程序中的任一一个地方做射线碰撞都可能改变其值



	float m_fBlockArea;					//该物体的阻塞区域半径大小

	float m_fBlockHeight;				//该物体的阻塞高度


	DWORD mObjState;					//物体渲染状态

	bool m_bBehaviour;					//是否产生过动作，没有产生过动作LastPos和Pos不能与地图做碰撞，否则不准确

	bool mCanSelect;					//是否可选择并控制

	bool m_bBeSelected;					//被选中了

	bool m_bUseMatrialColor;

	D3DXVECTOR3 m_vBlockPoint;

	bool m_bHit;

	bool m_bBlock;

	bool mCanGetInput;
	bool mDynamic;

	float m_fBoundRadius;

protected:

	eObjAnimState m_ObjAnimState;		//骨骼动画对象当前状态

	//D3DXMATRIX _matWorld;				//物体的世界矩阵

public:

	eObjParentType mOpt;

	GString mParentName;

	GNode *mParent;							//依附的对象

protected:

	DWORD _nID;								//物体在所有的创建的物体中的绝对ID


private:

	static DWORD __OBJCOUNTER;				//物体数目计数

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
