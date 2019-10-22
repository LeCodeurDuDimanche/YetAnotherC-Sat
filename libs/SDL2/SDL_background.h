#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_Scroll.h>
#include <SDL_utilities.h>

class Background
{
public:
    Background();
    Background(const std::string& imageFile, SDL_Renderer* render,int wScreen, int hScreen);
    Background(SDL_Surface* imageBack, SDL_Renderer* render,int wScreen, int hScreen);

    int set(const std::string& imageFile, SDL_Renderer* render,int wScreen, int hScreen);
    int set(SDL_Surface* imageBack, SDL_Renderer* render,int wScreen, int hScreen);

    int render(SDL_Renderer* render,const Scroll& view, bool clip=true) const;
    int render(SDL_Renderer* render) const;
private:
    Texture texture;
    int w,h, wT,hT;
};

#endif // BACKGROUND_H_INCLUDED
