#pragma once
class GAnimEntity;
class GStillEntity;
class GFilmFrame
{
    DeclareFilmObj ( GFilmFrame );
public:
    GFilmFrame ( void );
    ~GFilmFrame ( void );
	void setInfo(CXMillSecond life,const char* name);
    void reload();
	bool isEnd();
	void start();
	bool advanceTime(u32 timeMS);
	const char* getName();
	const char* getNext();
	const char* getPrev();
	void setNext(const char* next);
	void setPrev(const char* prev);
	void setName(const char* name);
protected:
	GString mName;
	GString mNextFrame;
	GString mPrevFrame;
	CXMillSecond mLifeTime;
	u32 mElapsedTime;
	static CXIDGenerator mIDGenerator;
};
