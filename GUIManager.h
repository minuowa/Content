#pragma once
class GUINode;
struct GUIHoverNodeChangedEvent: public CXEventArgs
{
    GUINode* mOldNode;
    GUINode* mNewNode;
    GUIHoverNodeChangedEvent()
    {
        mOldNode = nullptr;
        mNewNode = nullptr;
    }
};

class GUIManager: public CXCallBack
{
public:
    GUIManager ( void );
    ~GUIManager ( void );

    CXDelegate mDelegateHoverNodeChanged;

	bool init();

    void beginRender();
    void draw();
    void endRender();

    virtual void onCallBack ( const CXDelegate& d, CXEventArgs* e );

    void processInput();

    inline GUINode* getRootNode();
protected:
    void resetNode();

    GUINode* mRootNode;
    GUINode* mHoverNode;
};

inline GUINode* GUIManager::getRootNode()
{
    return mRootNode;
}

GUINode* getUIRootNode();