#pragma once

#include "GComponentTrans.h"
#include "GObject.h"
#include "GFactory.h"
enum eObjState
{
	eObjState_Lighting				, //是否开灯光
	eObjState_Render				, //是否渲染
	eObjState_GetInput				, //是否可以操控
	eObjState_Update				, //是否可以更新
	eObjState_Picked				, //是否被点击了
	eObjState_ChangeWhenPicked		, //点击时是否发生改变
	eObjState_CanBeSelect			, //是否可以被选择
	eObjState_BeSelect				, //是否被选择了

	eUINodeState_AcpectEvent,	///是否可接受事件
	eUINodeState_CanHover,
	eUINodeState_CanCapture,
	eUINodeState_IsCapture,
	eUINodeState_IsHover,
	eUINodeState_CanScale,
	eUINodeState_CanDrag,
};
enum eForceType
{
	ftUpAlways,			//上方向总是（0,1,0）
	ftUpWithMap,		//上方向与地面垂直，随地图而改变
};
/** 删除场景节点 **/
struct GNodeDeleteArgs:public CXEventArgs 
{
	GString mDeleteNode;
};
/** 添加场景节点 **/
struct GNodeAddArgs:public CXEventArgs
{
	GString mAddNode;
	GString mAddNodeParent;
};
/** 场景节点组件发生了变化 **/
struct GNodeComponentChangeArgs:public CXEventArgs 
{
	GString mChangedNode;
};
//有坐标属性的物体
class GNode: public GObject
{
    DeclareEditorType ( GNode );
    DeclareFilmObjBase ( GNode, GObject )
public:

    GNode();

    virtual ~GNode();

    GNode* getNodeByName ( const char* name );
    GNode* getFirstNodeByCategoryName ( const char* category );

    void linkTo ( CXRapidxmlNode* parent );
    virtual void clear();
    DeclareFilmTool virtual bool recreate();
public:

	void setState(eObjState state,bool b);
	bool isState(eObjState state) const;

    void setCanGetInput ( bool can );
    u32 getLocalID() const;
    virtual GNode* addChild ( GNode* c );
    bool removeChild ( GNode* child );
    void deleteChild ( GNode* node );
    GNode* getParent() const;

    virtual bool draw();
    virtual void getInput ( DWORD frameTimeMs );
    virtual void update();
    virtual void updateWorld();

    virtual D3DXMATRIX GetWorldMatrixByBone ( const char *sBoneName, bool bForTrans = false );
    void setDir ( D3DXVECTOR3 vNormal );

    void ForceOnMap ( void *pMap, float fForceHeight, eForceType ft );

    IntersectInfo *GetBlockPoint();

    DeclareFilmTool GComponentTrans* getTrans();
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
    void	makeXMLNode ( CXRapidxmlNode& node );

public:
    static CXDelegate mDelegateAddObj;
    static CXDelegate mDelegateDeleteObj;
    static CXDelegate mDelegateComponentChange;

    CXDynaArray<GNode*> mChildren;

    GComponentOwner	mComponentOwner;


    //eHitType m_HitType;					//是否撞到物体

    //void *m_pOnObj;						//本对象下面的非地图对象

    bool mCulledByCamera;					//是否需要眼睛裁剪

    bool m_bForceOnMap;					//是否依附于地图


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

    bool m_bBlock;

    bool mDynamic;

    float m_fBoundRadius;
public:
	static CXIDObjectManager<GNode>& getObjIDManager();
    /** @brief 物体在所有的创建的物体中的绝对ID  **/
    u32 mLocalID;
public:
    /** @brief refer to eObjState **/
    CXBitArray32 mNodeState;

protected:
    GNode *mParent;							//依附的对象

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
inline GComponentTrans* GNode::getTrans()
{
    GComponentTrans* pTrans = ( GComponentTrans* ) mComponentOwner.getComponent ( eComponentType_Trans );
    assert ( pTrans );
    return pTrans;
}
inline u32 GNode::getLocalID() const
{
    return mLocalID;
}
inline GNode* GNode::getParent() const
{
    return mParent;
}
typedef CXDynaArray<GNode*> GNodeArr;
