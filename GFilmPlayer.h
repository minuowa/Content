#pragma once
class GFilm;
class GFilmPlayer
{
public:
    GFilmPlayer ( void );
    ~GFilmPlayer ( void );
    bool play ( const char* filename );
    void gotoFrame ( u32 id );
    void gotoFrame ( const char* name );
};
#define FilmPlayer CXSingleton<GFilmPlayer>::getInstance()
