#ifndef WORKSPACE_H_INCLUDED
#define WORKSPACE_H_INCLUDED

#include <vector>
#include <string>

#include "SDL2/SDL.h"
#include "hitbox.h"

#define WINDOW_X 640
#define WINDOW_Y 480

class Fenetre{
public:
    Fenetre();

    int openImage();
    int openImage(const std::string& filename);
    int load(const std::string& filename);
    int load();

    int save(const std::string& filename) const;
    int save() const;

    int afficher();

    void autodetect();

    int gererEvents(const SDL_Event& event);
    unsigned getID() const;

    ~Fenetre();
private:
    SDL_Renderer* render=nullptr;
    SDL_Texture* fond=nullptr;
    SDL_Window* fenetre=nullptr;
    SDL_Point *points=nullptr;

    SDL_Rect blit;
    std::vector<Point> sommets;
    bool onlySelected=false;
    unsigned index=0, selected=0;
    unsigned char bright=255;
    Point mouseCursor=Point(0,0);

    };

class Workspace{
public:
    Workspace();

    int nouvelleFenetre();
    int gererEvents();
    int afficher();
    ~Workspace();
private:
    std::vector<Fenetre*> fenetres;
    };

#endif // WORKSPACE_H_INCLUDED
