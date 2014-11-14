#include "GGameDemoHeader.h"
#include "GFilmPlayer.h"


GFilmPlayer::GFilmPlayer ( void )
{
}


GFilmPlayer::~GFilmPlayer ( void )
{
}

bool GFilmPlayer::play ( const char* filename )
{
    gLuaScript.doFile ( filename );
}
