#pragma once
class GFilm;
class GFilmFrame;
class GFilmPlayer
{
    DeclareFilmObj ( GFilmPlayer )
public:
    GFilmPlayer ( void );
    ~GFilmPlayer ( void );
    bool play ( const char* filename );
    void gotoFrame ( u32 id );
    void gotoFrame ( const char* name );
	DeclareFilmTool void addFrame ( u32 id, int lifeSeconds );
    void process();
protected:
    bool start();
protected:
    CXDynaArray<GFilmFrame*> mFilmSequence;
    GFilmFrame* mLastFrame;
	GFilmFrame* mCurFrame;
	bool mEnd;
};
#define FilmPlayer CXSingleton<GFilmPlayer>::getInstance()

DeclareFilmToolGlobal inline GFilmPlayer* getFilmPlayer()
{
    return FilmPlayer;
}
