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
	CXDelegate mDelegateNodeClicked;
	CXDelegate mDelegateNodeButtonDown;
	CXDelegate mDelegateNodeButtonUp;

	bool init();

    void beginRender();
    void draw();
    void endRender();

    virtual void onCallBack ( const CXDelegate& d, CXEventArgs* e );

    void processInput();
	GUINode* getNode(const char* name) const;
	GUINode* getCapture() const;
	GUINode* getHover() const;
    inline GUINode* getRootNode();
	void recreateRootNode();
protected:
    void resetNode();
	void updateHoverNode();
	void updateCaptureNode();

    GUINode* mRootNode;
    GUINode* mHoverNode;
	//获得点击的node
	GUINode* mCaptureNode;
};

inline GUINode* GUIManager::getRootNode()
{
    return mRootNode;
}

GUINode* getUIRootNode();