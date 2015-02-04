#include "GGameDemoHeader.h"
#include "GUIManager.h"
#include "GUINode.h"
#include "GD9Device.h"
#include "GD8Input.h"

GUIManager::GUIManager ( void )
    : mHoverNode ( nullptr )
{
    mRootNode = new GUINode;
    mRootNode->setState ( eUINodeState_CanHover, false );
    resetNode();
    D9DEVICE->mOnLostDevice += this;
    D9DEVICE->mOnResetDevice += this;
}


GUIManager::~GUIManager ( void )
{
    D9DEVICE->mOnLostDevice -= this;
    D9DEVICE->mOnResetDevice -= this;
    dSafeDelete ( mRootNode );
}

void GUIManager::beginRender()
{
}

void GUIManager::draw()
{
    D9DEVICE->beginRenderUI();
    mRootNode->draw();
}

void GUIManager::onCallBack ( const CXDelegate& d, CXEventArgs* e )
{
    if ( d == D9DEVICE->mOnLostDevice )
    {
    }
    else if ( d == D9DEVICE->mOnResetDevice )
    {
        resetNode();
    }
}

void GUIManager::resetNode()
{
    mRootNode->setColor ( 0 );
    mRootNode->setRect ( 0, 0, D9DEVICE->GetScreenWidth(), D9DEVICE->GetScreenHeight() );
}

void GUIManager::processInput()
{
    POINT pt = INPUTSYSTEM.getMousePoint();
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
