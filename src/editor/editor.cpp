#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <hitbox.h>

#include "workspace.h"

int main()
{
    Workspace editeur;
    while(editeur.gererEvents()>0)
    {
        if (editeur.afficher()<0)
            return -1;
    }
    return 0;
}
