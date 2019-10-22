#include <SDL2/SDL_image.h>
#include "tinyfiledialogs.h"
#include "workspace.h"

void drawPoints(const std::vector<Point>& points,SDL_Renderer* render)
{
    unsigned i;
    for (i=0; i<points.size()-1;i++)
        SDL_RenderDrawLine(render,points[i].x,points[i].y,points[i+1].x,points[i+1].y);
    SDL_RenderDrawLine(render,points[i].x,points[i].y,points[0].x,points[0].y);
}

Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

Fenetre::Fenetre()
{
    SDL_CreateWindowAndRenderer(WINDOW_X, WINDOW_Y,SDL_WINDOW_SHOWN,&fenetre,&render);
    blit=(SDL_Rect){0,0,0,0};
}
int Fenetre::openImage()
{
    char* filter[] = {"*.jpg","*.bmp","*jpeg","*png","*.exif","*.gif"};
    const char* filename = tinyfd_openFileDialog("Choisir une image de fond", nullptr, 1, filter, "Images", 0);
    if (!filename)
        return -1;
    return openImage(filename);
}
int Fenetre::openImage(const std::string& filename)
{
    SDL_Surface* temp=IMG_Load(filename.c_str());
    if (!temp)
        return -1;

    blit.w=temp->w;
    blit.h=temp->h;
    blit.x=WINDOW_X/2-blit.w/2;
    blit.y=WINDOW_Y/2-blit.h/2;
    if (points)
        delete[] points;
    points=new SDL_Point[blit.w*blit.h];

    fond=SDL_CreateTextureFromSurface(render,temp);
    SDL_FreeSurface(temp);
    if (!fond)
        return -1;
    return 0;
}
int Fenetre::load()
{
    char* filter[] = {".hbx"};
    const char* filename = tinyfd_openFileDialog("Ouvrir un fichier", nullptr, 1, filter, "Hitbox Shapes", 0);
    if (!filename)
        return -1;
    return load(filename);
}
int Fenetre::load(const std::string& filename)
{
    Shape* shape=loadShape(blit.x,blit.y,filename);
    if (!shape)
        return -1;
    Polygon* poly=dynamic_cast<Polygon*>(shape);
    if (!poly)
    {
        ConvexPolygon* cPoly=dynamic_cast<ConvexPolygon*>(shape);
        if (!cPoly)
            return -1;
        sommets.assign(cPoly->getPoints().begin(),cPoly->getPoints().end());
    }
    else
        sommets.assign(poly->getPoints().begin(),poly->getPoints().end());

    index=0;
    selected=0;
    return 0;
}

int Fenetre::save() const
{
    char* filter[] = {".hbx"};
    const char* filename = tinyfd_saveFileDialog("Sauvegarder", nullptr, 1, filter, "Hitbox Shapes");
    if (!filename)
        return -1;
    return save(filename);
}

int Fenetre::save(const std::string& filename) const
{
    Shape* shape=new Polygon(blit.x,blit.y,sommets);
    return shape->save(filename,0,0);
}

void Fenetre::autodetect()
{
    SDL_Surface* surface=SDL_GetWindowSurface(fenetre);
    if (!surface)
        return;
    //Coté droit y descend x a gauche
    for (int y=blit.y;y<blit.y+blit.w;y++)
    {
        for (int x=blit.x+blit.w-1; x>=blit.x;x--)
        {
            //Si le pixel est transparent on a trouve le bout, on rajoute le point
            unsigned char a;
            SDL_GetRGBA(getPixel(surface,x,y),surface->format,nullptr,nullptr,nullptr,&a);
            if (a<10)
            {
                sommets.push_back(Point(x,y));
                break;
            }
        }
    }
    //Coté bas y monte x a gauche

    for (int x=blit.x+blit.w-1; x>=blit.x;x--)
    {
        for (int y=blit.y+blit.w-1;y>=blit.y;y--)
        {
            //Si le pixel est transparent on a trouve le bout, on rajoute le point
            unsigned char a;
            SDL_GetRGBA(getPixel(surface,x,y),surface->format,nullptr,nullptr,nullptr,&a);
            if (a<10)
            {
                sommets.push_back(Point(x,y));
                break;
            }
        }
    }
    //Coté gauche y monte x a droite
    for (int y=blit.y+blit.w-1;y>=blit.y;y--)
    {
        for (int x=blit.x; x<blit.x+blit.w;x++)
        {
            //Si le pixel est transparent on a trouve le bout, on rajoute le point
            unsigned char a;
            SDL_GetRGBA(getPixel(surface,x,y),surface->format,nullptr,nullptr,nullptr,&a);
            if (a<10)
            {
                sommets.push_back(Point(x,y));
                break;
            }
        }
    }
    //Coté haut y descend x a droite
    for (int x=blit.x; x<blit.x+blit.w;x++)
    {
        for (int y=blit.y;y<blit.y+blit.w;y++)
        {
            //Si le pixel est transparent on a trouve le bout, on rajoute le point
            unsigned char a;
            SDL_GetRGBA(getPixel(surface,x,y),surface->format,nullptr,nullptr,nullptr,&a);
            if (!a)
            {
                sommets.push_back(Point(x,y));
                break;
            }
        }
    }
}
int Fenetre::gererEvents(const SDL_Event& event)
{
    bool isSel=false;
    int dplcX=0, dplcY=0, count=0;
    switch(event.type)
    {
        case SDL_MOUSEMOTION:
            mouseCursor=Point(event.motion.x,event.motion.y);
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event==SDL_WINDOWEVENT_CLOSE)
                return -1;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_RETURN:
                case SDLK_RETURN2:
                case SDLK_INSERT:
                case SDLK_p:
                    autodetect();
                    break;
                case SDLK_SPACE:
                    onlySelected=!onlySelected;
                    break;
                case SDLK_k:
                    if (save()<0)
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Erreur à la sauvegarde du fichier","L'éditeur n'a pas pu sauvegarder le fichier correctement",fenetre);
                    break;
                case SDLK_o:
                    if (openImage()<0)
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Erreur à l'ouverture de l'image","L'éditeur n'a pas pu ouvrir l'image",fenetre);
                    break;
                case SDLK_l:
                    if (load()<0)
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Erreur à l'ouverture du fichier","L'éditeur n'a pas pu ouvrir le fichier",fenetre);
                    break;
                //Autoloop, auto 255 values <- unsigned char
                case SDLK_RIGHT:
                    bright++;
                    break;
                case SDLK_LEFT:
                    bright--;
                    break;
                case SDLK_w:
                    dplcY=-1;
                    break;
                case SDLK_s:
                    dplcY=1;
                    break;
                case SDLK_a:
                    dplcX=-1;
                    break;
                case SDLK_d:
                    dplcX=1;
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            for (unsigned i=0;i<sommets.size();i++)
            {
                int delta = 3;
                //On regarde si on a clique sur un point (a delta * 2 +1 px pres)
                if (event.button.x >= sommets[i].x - delta && event.button.x <= sommets[i].x + delta
                    && event.button.y >= sommets[i].y - delta && event.button.y <= sommets[i].y + delta)
                {
                    selected=i;
                    isSel=true;
                    break;
                }
            }
            if (isSel)
                break;
            if (event.button.button==SDL_BUTTON_RIGHT||event.button.button==SDL_BUTTON_MIDDLE)
            {
                if (sommets.size())
                    sommets.pop_back();
                if (selected>=sommets.size()-1)
                    selected=0;
            }
            if(event.button.button!=SDL_BUTTON_RIGHT)
            {
                if (!selected)
                    sommets.push_back(Point(event.button.x,event.button.y));
            }
            break;
        case SDL_MOUSEBUTTONUP:
             if (selected&&event.button.button==SDL_BUTTON_LEFT)
            {
                sommets[selected].x=event.button.x;
                sommets[selected].y=event.button.y;
                selected=0;
            }
            break;
        case SDL_MOUSEWHEEL:
            for (std::vector<Point>::iterator i=sommets.begin();i!=sommets.end();++i)
            {
                if (++count>4)
                {
                    count=0;
                    i=sommets.erase(i);
                    if (i==sommets.end())
                        break;
                }

            }
           /* testW=blit.w*(1+double(event.wheel.y)/10);
            testH=blit.h*(1+double(event.wheel.y)/10);
            if (testW<=WINDOW_X&&testH<=WINDOW_Y&&testH>10 && testH>10)
            {
                blit.x+=(blit.w-testW)/2;
                blit.y+=(blit.h-testH)/2;
                blit.w=testW;
                blit.h=testH;
            }*/
            break;
    }
    for (auto &p: sommets)
    {
        p.x+=dplcX;
        p.y+=dplcY;
    }
    return 0;
}
int Fenetre::afficher()
{
    SDL_SetRenderDrawColor(render,0,0,0,0);
    SDL_RenderClear(render);

    if (fond)
    {
        if (SDL_RenderCopy(render,fond,nullptr,&blit)<0)
            return -1;
    }

    SDL_SetRenderDrawColor(render,bright,bright,bright,255);

    if (sommets.size())
    {
        Point temp;
        if (selected)
        {
            temp=sommets[selected];
            sommets[selected].x=mouseCursor.x;
            sommets[selected].y=mouseCursor.y;
        }
        drawPoints(sommets,render);
        if (selected)
            sommets[selected]=temp;
    }
    if (onlySelected)
    {
        SDL_SetRenderDrawColor(render,0,0,0,0);
        size_t size=0;
        Polygon poly(sommets);
        for (int x=blit.x;x<blit.x+blit.w;x++)
        {
            for (int y=blit.y;y<blit.y+blit.h;y++)
            {
                if (!poly.isColliding(Point(x,y)))
                {
                    points[size]=(SDL_Point){x,y};
                    size++;
                }
            }
        }
        SDL_RenderDrawPoints(render,points,size);
    }
    SDL_RenderPresent(render);
    return 0;
}
unsigned Fenetre::getID() const
{
    return SDL_GetWindowID(fenetre);
}

Fenetre::~Fenetre()
{
    if (points)
        delete[] points;
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(fenetre);
}

Workspace::Workspace()
{
    nouvelleFenetre();
}

int Workspace::nouvelleFenetre()
{
    fenetres.push_back(new Fenetre());
    return fenetres.size();
}
int Workspace::gererEvents()
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    for (std::vector<Fenetre*>::iterator i=fenetres.begin();i!=fenetres.end();++i)
    {
        if ((*i)->getID()==event.window.windowID)
        {
             if ((*i)->gererEvents(event)<0)
                i=fenetres.erase(i);
            if (i==fenetres.end())
                break;
        }
    }
    return fenetres.size();
}
int Workspace::afficher()
{
    for (auto f: fenetres)
    {
        if (f->afficher()<0)
            return -1;
    }
    return 0;
}

Workspace::~Workspace()
{
    while(fenetres.size())
    {
        delete fenetres.back();
        fenetres.pop_back();
    }
}
