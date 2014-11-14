#pragma once
class GFilm;
class GFilmPlayer
{
public:
    GFilmPlayer ( void );
    ~GFilmPlayer ( void );
    bool play ( const char* filename );
};

