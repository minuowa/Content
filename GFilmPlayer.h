#pragma once
#include "GFilmCommon.h"
class GFilmRecord;
class GFilmFrame;
struct GFilmAddFrameEvent:public CXEventArgs
{
	uString mFrameName;
	uString mPrevName;
	uString mNextName;
};
struct GFilmDeleteFrameEvent:public CXEventArgs
{
	uString mFrameName;
};
class GFilmPlayer
{
    DeclareFilmObj ( GFilmPlayer )
public:
	CXDelegate mDelegateAddFrame;
	CXDelegate mDelegateDeleteFrame;
public:
    GFilmPlayer ( void );
    ~GFilmPlayer ( void );
    bool play ( const char* filename );
    void gotoFrame ( u32 id );
    void gotoFrame ( const char* name );

	void deleteFrame(const char* frame);
    DeclareFilmTool void addFrame ( const char* name,const char* nextName,int lifeSeconds );

    void addFrameBehind ( const char* prev );

    void save();
    void beginRecord();
    void endRecord();
    void beginRecordFrame();
    void endRecordFrame();
    void process();


	void renameFrame(const char* name,const char* newName);
	GFilmFrame* getFrame ( const char* name );
	GFilmFrame* updateHeader();
protected:
	void setAllPrev();
	void clear();
    bool start();
	void updateList();
protected:
    eFilmMode mFilmMode;
    CXMap<uString, GFilmFrame*> mNameMap;
    GFilmFrame* mCurFrame;
    bool mEnd;
};
DeclareFilmToolGlobal GFilmPlayer* getFilmPlayer();
