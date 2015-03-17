#include "GGameDemoHeader.h"
#include "GUIManager.h"
#include "GUINode.h"
#include "GD9Device.h"
#include "GD8Input.h"
#include "Content.h"

GUIManager::GUIManager ( void )
    : mHoverNode ( nullptr )
{

}


GUIManager::~GUIManager ( void )
{
    Content::Device.mOnLostDevice -= this;
    Content::Device.mOnResetDevice -= this;
    dSafeDelete ( mRootNode );
}

void GUIManager::beginRender()
{
}

void GUIManager::draw()
{
    Content::Device.beginRenderUI();
    mRootNode->draw();
}

void GUIManager::onCallBack ( const CXDelegate& d, CXEventArgs* e )
{
    if ( d ==  Content::Device.mOnLostDevice )
    {
    }
    else if ( d ==  Content::Device.mOnResetDevice )
    {
        resetNode();
    }
}

void GUIManager::resetNode()
{
    mRootNode->setColor ( 0 );
    mRootNode->setRect ( 0, 0,  Content::Device.GetScreenWidth(),  Content::Device.GetScreenHeight() );
}

void GUIManager::processInput()
{
    POINT pt =  Content::InputSystem.getMousePoint();
    GUINode* newHover = mRootNode->getHoverNode ( pt.x, pt.y );

    if ( mHoverNode )
        mHoverNode->setState ( eUINodeState_IsHover, false );

    if ( newHover != mHoverNode )
    {
        GUIHoverNodeChangedEvent arg;
        arg.mOldNode = mHoverNode;
        arg.mNewNode = newHover;
        mDelegateHoverNodeChanged.trigger ( &arg );
    }

    if ( newHover )
        newHover->setState ( eUINodeState_IsHover, true );

    mHoverNode = newHover;
}

bool GUIManager::init()
{
	mRootNode = new GUINode;
	mRootNode->setState ( eUINodeState_CanHover, false );
	resetNode();
	Content::Device.mOnLostDevice += this;
	Content::Device.mOnResetDevice += this;
	return true;
}

GUINode* getUIRootNode()
{
	return Content::UIMgr.getRootNode();
}
