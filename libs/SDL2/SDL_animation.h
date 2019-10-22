#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED
/*INSTRUCTIONS:
-Format des Sprites:
    1 animation par ligne, il peut y avoir plusieurs lignes par animation
    Tous les sprites ont le meme nombre de colonnes (images) dans une meme spritesheet
    Toutes les frames d'une meme spriteSheet doivent etre de la meme taille.
    Chaque personnage/entite a une spritesheet
- Return:
    Les fonctions return -1 en cas d'erreur, 0 ou la valeur qu'elles doivent retourner sinon
-Tips:
    Pour les index de type d'animation et de ligne d'animation creer une enum ou des defines

*/
//Chaque spritesheet constient les animations d'un objet d'idAnimation
#include <string>
#include <map>

#include "SDL2/SDL.h"

typedef struct{
bool init;
unsigned nbFrameX,nbFrameY,IPS,spriteW,spriteH;
std::string spriteSheetName;
SDL_Renderer* renderer;
SDL_Texture* spriteSheet;
}SpriteSheetInfo;

class Animation{
    private:
        static std::map<unsigned,SpriteSheetInfo> infos;

        long unsigned int lastAnimationTime;
        unsigned int animation, idSpritesheet,index, nbLignesAnim, IPS, loopCount;
        int loops;
        SDL_Rect source;
        int w,h;

        void incrementIndex(int pas=1);
    public:
        ~Animation(){};
        //Cette fonction retourne un idenifiant libre
        static unsigned getNextFreeId();

        //Cette fonction initialise une aniamtion, chaque animation a besoin d'etre initialisee
        static int init(unsigned int idSpritesheet,SDL_Renderer* renderer,const std::string& spriteSheet, unsigned int nbFrameX, unsigned int nbFrameY, unsigned int IPS=25);

        /*Cette fonction libere une Spritesheet, cette Spritesheet ne doit plus etre utilisee apres
         avec idSpritesheet=-1 cette fonction liberer toutes les Spritesheet*/
        static void quit(int idSpritesheet=-1);

        /*Constructeur, idSpritesheet est l'id du spritesheet que devra utiliser l'Animation
        anim est la ligne d'animation de depart et nbLignesAnim le nombre de lignes qu'occupe cette animation*/
        Animation(unsigned int idSpritesheet,unsigned anim=0, unsigned nbLignesAnim=1, int IPS=-1, int loops=-1);

        //int setIPS(int IPS);
        //Change la ligne de l'animation
        void changerAnimation(unsigned int anim);

        unsigned int getAnimation() const;


        int getIPS() const;
        void setIPS(int IPS);
        //int getIndex();

        int actualiserEtAfficher(SDL_Renderer* rendu,const SDL_Rect& dest, time_t actualTime=SDL_GetTicks());

        int actualiserEtAfficherEx(SDL_Renderer* rendu,const SDL_Rect& dest,double angle, const SDL_Point* rotatePoint,SDL_RendererFlip flip, time_t actualTime=SDL_GetTicks());

        const SDL_Rect& getTailleFrame() const;
        void setTailleFrame(int w, int h);

};

#endif // ANIMATION_H_INCLUDED
