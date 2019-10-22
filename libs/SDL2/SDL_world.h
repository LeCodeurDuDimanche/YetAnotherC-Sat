#ifndef SDL_WORLD_H_INCLUDED
#define SDL_WORLD_H_INCLUDED

#include <SDL2/SDL.h>

class Entity{

private:
    Shape* hitbox;
    Animation anim;
    SDL_Rect pos;
    unsigned id;
};
class Physics{

private:
    Uint16 flags; //Gravity, chute dmg,
    double gravity;
    };
class World{
    World(SDL_Renderer* render, int wWorld, int hWorld);

    int setBackground(const std::string& fileName);
    int setPhysics(Uint16 flags, double gravity);
    int setViewFollowed(const int&x,const int&y);
    int setViewFollowed(const Entity* entity);
    int setViewFollowed(unsigned id);
    unsigned addEntity(Entity* entity, unsigned group=0);



private:
    std::vector<std::vector<Entity*> > entities;
    Scroll view;
    Backgroung background;
    Physics rules;
    SDL_Renderer* render;
};

#endif // SDL_WORLD_H_INCLUDED
