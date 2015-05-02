#include "GGameDemoHeader.h"
#include "GUIManager.h"
#include "GUINode.h"
#include "GD9Device.h"
#include "GD8Input.h"
#include "Content.h"

GUIManager::GUIManager ( void )
    : mHoverNode ( nullptr )
    , mCaptureNode ( nullptr )
{

}


GUIManager::~GUIManager ( void )
{
    Content::Device.mOnLostDevice -= this;
    Content::Device.mOnResetDevice -= this;
    Content::InputSystem.mDelegateMouseMove -= this;
    Content::InputSystem.mDelegateMouseUp -= this;
    Content::InputSystem.mDelegateMouseDown -= this;

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
    else if ( d == Content::InputSystem.mDelegateMouseMove )
    {
        if ( !Content::InputSystem.isButtonPressing ( eButtonType_LeftButton ) )
            updateHoverNode();
    }
    else if ( d == Content::InputSystem.mDelegateMouseUp )
    {
        if ( mCaptureNode  )
        {
            mCaptureNode->setState ( eUINodeState_IsCapture, false );
            mCaptureNode->onLostCapture();
            mCaptureNode = nullptr;
        }
    }
    else if ( d == Content::InputSystem.mDelegateMouseDown )
    {
        if ( Content::InputSystem.isButtonDown ( eButtonType_LeftButton ) )
        {
            updateCaptureNode();
        }
    }
}

void GUIManager::resetNode()
{
    mRootNode->setColor ( 0 );
    mRootNode->setRect ( mRootNode->getRect().mX, mRootNode->getRect().mY,  Content::Device.GetScreenWidth(),  Content::Device.GetScreenHeight() );
}

void GUIManager::processInput()
{
    updateHoverNode();
}

bool GUIManager::init()
{
    recreateRootNode();
    Content::Device.mOnLostDevice += this;
    Content::Device.mOnResetDevice += this;
    Content::InputSystem.mDelegateMouseMove += this;
    Content::InputSystem.mDelegateMouseUp += this;
    Content::InputSystem.mDelegateMouseDown += this;
    return true;
}

void GUIManager::updateHoverNode()
{
    POINT pt =  Content::InputSystem.getMousePoint();
    GUINode* newHover = mRootNode->getNode ( pt.x, pt.y, eUINodeState_CanHover );

    if ( mHoverNode )
        mHoverNode->setState ( eUINodeState_IsHover, false );

    if ( newHover )
        newHover->setState ( eUINodeState_IsHover, true );

    if ( newHover != mHoverNode )
    {
        if ( mHoverNode )
            mHoverNode->onLostHover();
        if ( newHover )
            newHover->onHover();

        GUIHoverNodeChangedEvent arg;
        arg.mOldNode = mHoverNode;
        arg.mNewNode = newHover;
        mHoverNode = newHover;
        mDelegateHoverNodeChanged.trigger ( &arg );
    }
    mHoverNode = newHover;
}

void GUIManager::updateCaptureNode()
{
    POINT pt =  Content::InputSystem.getMousePoint();
    GUINode* node = mRootNode->getNode ( pt.x, pt.y, eUINodeState_CanCapture );
    if ( node )
    {
        if ( mCaptureNode != node && mCaptureNode )
        {
            mCaptureNode->setState ( eUINodeState_IsCapture, false );
            mCaptureNode->onLostCapture();
        }
        mCaptureNode = node;
        mCaptureNode->setState ( eUINodeState_IsCapture, true );
        mCaptureNode->onCapture();
        mDelegateNodeButtonDown.trigger();
    }
}

GUINode* GUIManager::getCapture() const
{
    return mCaptureNode;
}

GUINode* GUIManager::getNode ( const char* name ) const
{
    return ( GUINode* ) mRootNode->getNodeByName ( name );
}

void GUIManager::recreateRootNode()
{
    dSafeDelete ( mRootNode );
    mRootNode = new GUINode;
    mRootNode->setState ( eUINodeState_CanHover, false );
    mRootNode->setState ( eUINodeState_CanCapture, false );
    mRootNode->setState ( eUINodeState_CanScale, true );
    resetNode();
}

GUINode* GUIManager::getHover() const
{
    return mHoverNode;
}

GUINode* getUIRootNode()
{
    return Content::UIMgr.getRootNode();
}
