#pragma once
class GAnimEntity;
class GStillEntity;
class GFilmFrame
{
    DeclareFilmObj ( GFilmFrame );
public:
    GFilmFrame ( void );
    ~GFilmFrame ( void );
	void setInfo(CXMillSecond life,u32 id);
    void reload();
	bool isEnd();
	void start();
	void tail(GFilmFrame* next);
	bool advanceTime(u32 timeMS);
	GFilmFrame* next() const;
protected:
	CXMillSecond mLifeTime;
	u32 mElapsedTime;
	u32 mID;
	GFilmFrame* mNext;
};
inline GFilmFrame* GFilmFrame::next() const
{
	return mNext;
}
