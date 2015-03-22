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
        updateHoverNode();
    }
    else if ( d == Content::InputSystem.mDelegateMouseUp )
    {
        if ( Content::InputSystem.isButtonUp ( eButtonType_LeftButton ) )
        {
            updateClickedNode();
        }
    }
}

void GUIManager::resetNode()
{
    mRootNode->setColor ( 0 );
    mRootNode->setRect ( 0, 0,  Content::Device.GetScreenWidth(),  Content::Device.GetScreenHeight() );
}

void GUIManager::processInput()
{
    updateHoverNode();
}

bool GUIManager::init()
{
    mRootNode = new GUINode;
	mRootNode->setState ( eUINodeState_CanHover, false );
	mRootNode->setState ( eUINodeState_CanCapture, false );
    resetNode();
    Content::Device.mOnLostDevice += this;
    Content::Device.mOnResetDevice += this;
    Content::InputSystem.mDelegateMouseMove += this;
    Content::InputSystem.mDelegateMouseUp += this;
    return true;
}

void GUIManager::updateHoverNode()
{
    POINT pt =  Content::InputSystem.getMousePoint();
    GUINode* newHover = mRootNode->getNode ( pt.x, pt.y, eUINodeState_CanHover );

    if ( newHover != mHoverNode )
    {
        if ( mHoverNode )
            mHoverNode->setState ( eUINodeState_IsHover, false );
        GUIHoverNodeChangedEvent arg;
        arg.mOldNode = mHoverNode;
        arg.mNewNode = newHover;
        mDelegateHoverNodeChanged.trigger ( &arg );
    }

    if ( newHover )
        newHover->setState ( eUINodeState_IsHover, true );

    mHoverNode = newHover;
}

void GUIManager::updateClickedNode()
{
    POINT pt =  Content::InputSystem.getMousePoint();
    GUINode* node = mRootNode->getNode ( pt.x, pt.y, eUINodeState_CanCapture );
    if ( node )
    {
        if ( mCaptureNode )
            mCaptureNode->onLostCapture();
        mCaptureNode = node;
        mDelegateNodeClicked.trigger();
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

GUINode* getUIRootNode()
{
    return Content::UIMgr.getRootNode();
}
