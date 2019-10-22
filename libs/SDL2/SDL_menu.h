#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>
#include <string>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define DEFAULT_FONTSIZE 12
#define DEFAULT_ALPHA 100

#define COLOR_BLACK (SDL_Color){0,0,0,255}
#define COLOR_WHITE (SDL_Color){255,255,255,255}
#define COLOR_RED (SDL_Color){255,0,0,255}
#define COLOR_GREEN (SDL_Color){0,255,0,255}
#define COLOR_BLUE (SDL_Color){0,0,255,255}
#define COLOR_TRANSPARENT (SDL_Color){0}



#define MENU_STYLE_BORDERLESS 0x0001
/* Styles du menu:
    MENU_STYLE_BORDERLESS: Menu sans bords
*/

#define MENU_ITEM_CENTRAGEAUTO 0x0001
#define MENU_ITEM_REPERESDL 0x0002
/* Styles d'item:
    MENU_ITEM_CENTRAGEAUTO: Les coordonnes x et y indique le centre de l'item
    MENU_ITEM_REPERESDL:    Les coordonnes x et y sont specifie dans le repere SDL.
                            L'origine du repere SDL est le coin superieur gauche de la fenetre, avec l'axe Y oriente vers le bas.
                            Sinon le repere est un repere dont l'origine et le coin inferieur gauche de la fenetre et dont l'axe Y est oriente vers le haut.
*/

#define MENU_TEXT_CENTRAGEAUTO 0x0001
#define MENU_TEXT_REPERESDL 0x0002
#define MENU_TEXT_ENCADRE 0x0004
#define MENU_TEXT_FOND 0x0008
#define MENU_TEXT_RAPIDE 0x0010
#define MENU_TEXT_TAILLEVARIABLE 0x0020
#define MENU_TEXT_HISLINES 0x0040

/* Styles de texte:
    MENU_TEXT_CENTRAGEAUTO:     Les coordonnes x et y indique le centre de l'item
    MENU_TEXT_REPERESDL:        Les coordonnes x et y sont specifie dans le repere SDL.
                                L'origine du repere SDL est le coin superieur gauche de la fenetre, avec l'axe Y oriente vers le bas.
                                Sinon le repere est un repere dont l'origine et le coin inferieur gauche de la fenetre et dont l'axe Y est oriente vers le haut.
    MENU_TEXT_ENCADRE:          Le texte est encadree par une bande contrastant avec le fond ndu texte
    MENU_TEXT_FOND:             Le texte possede un fond
    MENU_TEXT_RAPIDE:           Le texte est genere en qualite mediocre, ce qui permet un rendu rapide.
                                A utiliser si le texte est change tres frequemment.
    MENU_TEXT_POLICEVARIBABLE:  La taille de la police du texte est modifiee si le texte est trop grand pour les dimensions donnees
    MENU_TEXT_HISLINES:         La hauteur (h) de la position n'est pas la taille du clip mais le nombre de lignes

*/

#define MENU_EVENTTEXT_TEXT 0x0080
/*Styles d' eventTextButton
    MENU_EVENTTEXT_TEXT:    Seul le texte est affiché
*/


#define MENU_TEXTBOX_DESCRIPTION 0x0080
#define MENU_TEXTBOX_DISCRET 0x0100
/*Styles de textbox:
    MENU_TEXT_DESCRIPTION:      Le texte specifie est un texte de descirption present quand la textbox est vide
    MENU_TEXTBOX_DISCRET:       Seul le texte, sans encadrements, est affiche
*/

//Utilisee pour les items de menu (NOT IMPLEMENTED)
typedef enum{ITEM_ITEM,ITEM_CHECKBOX,ITEM_EVENTBUTTON,ITEM_TEXT,ITEM_TEXTBUTTON,ITEM_TEXTBOX,ITEM_EVENTTEXTBUTTON,ITEM_SCROLL}ITEM_TYPE;

//BASIC CLASS

class Objet{
public:
    virtual int render()=0;
    virtual bool click(bool click)=0;

    virtual bool isClicked(const SDL_Event &event)=0;
    virtual bool isInItem(int x, int y) const=0;

    virtual bool setCoords(int x, int y)=0;
    virtual void setSize(int w, int h)=0;
    virtual void setMaxSize(int w, int h)=0;
    virtual void setSizeProportional(int w, int h)=0;

    virtual void getCoords(int &x,int &y) const=0;
    virtual void getSize(int &w, int &h) const=0;

    virtual ITEM_TYPE getType() const=0;

    virtual void hide(bool state=true)=0;
    virtual bool isHidden() const=0;

    virtual int setRenderer(SDL_Renderer* render)=0;
    virtual ~Objet()=0;
};
class Item: public Objet{
protected:
    bool hidden;
    int flags;
    SDL_Rect coords;
    SDL_Texture* texture;
    SDL_Surface* surface;
    SDL_Renderer* menu;
    ITEM_TYPE type;
public:
    Item(){surface=NULL; texture=NULL;menu=NULL;};
    Item(SDL_Surface* apparence,SDL_Renderer *renderer, int x,int y, int flags=MENU_ITEM_CENTRAGEAUTO);
    int create(SDL_Surface* apparence,SDL_Renderer *renderer,int x,int y, int flags=MENU_ITEM_CENTRAGEAUTO, bool first=true);

    virtual int render();
    virtual bool click(bool click);
    virtual int setStyle(SDL_Surface* aspect);

    virtual bool isClicked(const SDL_Event &event);
    virtual bool isInItem(int x, int y) const;

    virtual bool setCoords(int x, int y);
    virtual void setSize(int w, int h);
    virtual void setMaxSize(int w, int h);
    virtual void setSizeProportional(int w, int h);

    virtual void getCoords(int &x,int &y) const;
    virtual void getSize(int &w, int &h) const;
    virtual ITEM_TYPE getType() const;

    virtual void hide(bool state=true);
    virtual bool isHidden() const;

    virtual int setRenderer(SDL_Renderer* render);
    virtual ~Item();
};

//1st GENERATION CLASS

class Text: public Item{
protected:
    void redim(const std::string&text);

    SDL_Color textColor,backgroundColor;
    static std::map<std::pair<std::string,int>,TTF_Font*> polices;
    std::string police;
    std::string texte;
    int pSize,nbLignes;
    SDL_Rect clip;
    bool hidden;

    void checkSize(int w, int h);
public:

    Text(SDL_Renderer* renderer,SDL_Rect pos,const std::string& texte,const std::string&police,int flags=0, int pixelSize=DEFAULT_FONTSIZE, SDL_Color textColor=COLOR_BLACK,  SDL_Color backgroundColor=COLOR_WHITE);

    int changerTexte(const std::string& nouveau);
    const std::string& getTexte() const{return texte;};
    virtual int render();

    virtual bool click(bool click){return false;};

    void effacer(int pos=0,int taille=-1);
    void setColor(SDL_Color textColor=COLOR_BLACK,  SDL_Color backgroundColor=COLOR_WHITE);
    virtual void setSize(int w, int h);
    virtual int setRenderer(SDL_Renderer* render);

    virtual void hide(bool state=true);
    virtual bool isHidden() const;

    int renderSurface();

    void inverseSurface();

    virtual ITEM_TYPE getType() const{return ITEM_TEXT;};

    virtual ~Text();
    static void quit();
};
class CheckBox: public Item{
public:
    CheckBox(SDL_Surface* apparenceChecked,SDL_Surface* apparenceUnChecked,SDL_Renderer* renderer,int x, int y,int flags=MENU_ITEM_CENTRAGEAUTO, bool checked=false);
    virtual int setRenderer(SDL_Renderer* render);
    virtual bool click(bool click);
    virtual int render();

    bool getChecked() const;
    void check(bool check=true);

    virtual ITEM_TYPE getType() const{return ITEM_CHECKBOX;};

    virtual ~CheckBox();
private:
    SDL_Surface* checkedS;
    SDL_Texture* checkedT;
    bool checked;
};
//EventButton me semble plus pertinent
#if 0
//typedef de callback pour actionButton du menu
typedef void* (* ACTIONCALLBACK) (void*param, int size);

//typedef de callback sans parametres pour actionButton du menu
typedef void* (* NOPARAMCALLBACK) ();

/* Pour cette classe on definit une fonction callback appellee lors du click,
cette fonction doit etre de type:
ACTIONCALLBACK: void* fonction(void* params,size_t size)
NOPARAMCALLBACK: void* fonction(void)
Les parametres sont un pointeur sur void et la taille de la memoire pointee par ce pointeur.
Cette fonction retourne un pointeur retrouvable dans la variable d'instance returnValue*/
class ActionButton: public Button{
protected:
    void* params;
    size_t size;

    NOPARAMCALLBACK noparam;
    ACTIONCALLBACK action;
    void *returnValue;
public:

    ActionButton(SDL_Renderer* renderer,int x, int y,ACTIONCALLBACK fonc, BUTTON_ASPECT defaultAspect=BUTTON_DEFAULTASPECT, SDL_Surface* aspect=NULL);
    ActionButton(SDL_Renderer* renderer,int x, int y,NOPARAMCALLBACK fonc, BUTTON_ASPECT defaultAspect=BUTTON_DEFAULTASPECT, SDL_Surface* aspect=NULL);
    virtual bool click();
    void setParams(void* params,size_t size);
};
#endif
/* Envoie un UserEvent a travers la SDL avec les informations suivantes:
code: eventCode, code d'identification de l'event
data1: vide
data2: vide
*/
//Auto Refresh text at render
class SmartText: public Text{
public:
   SmartText(SDL_Renderer* renderer,SDL_Rect pos,const std::string& text,const std::string&police,int flags=0, int pixelSize=DEFAULT_FONTSIZE, SDL_Color textColor=COLOR_BLACK,  SDL_Color backgroundColor=COLOR_WHITE);
    virtual int render();
    int changerTexte(const std::string& nouveau);

private:
    const std::string* refStr;
};

class EventButton: public Item{
public:
    EventButton(SDL_Surface* apparence,SDL_Renderer* renderer,int x, int y,int eventCode,int flags=MENU_ITEM_CENTRAGEAUTO);
    virtual bool click(bool click);

    virtual ITEM_TYPE getType() const{return ITEM_EVENTBUTTON;};

   ~EventButton(){};
private:
    int eventCode;
    bool inverse;
};

//2nd GENERATION CLASS
/*class ActionTextButton: public TextButton,public ActionButton{
};*/


class TextBox: public Text{
public:
    TextBox(SDL_Renderer* renderer,SDL_Rect pos,const std::string& text,const std::string&police,int flags=MENU_TEXT_ENCADRE, int pixelSize=DEFAULT_FONTSIZE, SDL_Color textColor=COLOR_BLACK,  SDL_Color backgroundColor=COLOR_WHITE);

    virtual int render();

    virtual bool isClicked(const SDL_Event &event);
    virtual bool isInItem(int x, int y) const;

    virtual bool setCoords(int x, int y);
    virtual void setSize(int w, int h);

    virtual ITEM_TYPE getType() const{return ITEM_TEXTBOX;};
private:
    int cursor;
    bool editing, afficherCurseur;
    Uint32 dernierTempsCurseur;
    double tailleChar;
    SDL_Rect position;
    std::string description;
};

//Combos
class EventTextButton: public Objet{
    public:

    EventTextButton(SDL_Surface* apparence,SDL_Renderer* renderer,int x, int y,int eventCode,const std::string& text,const std::string&police, SDL_Color textColor=COLOR_BLACK, int flags=MENU_ITEM_CENTRAGEAUTO);
    EventTextButton(SDL_Renderer* renderer,SDL_Rect pos,int eventCode,const std::string& text,const std::string&police,int fontSize=16, SDL_Color textColor=COLOR_BLACK, SDL_Color backgroundColor=COLOR_WHITE, int flags=MENU_ITEM_CENTRAGEAUTO);

    virtual int render();
    virtual bool click(bool click);
    virtual int setStyle(SDL_Surface* aspect);

    void setColor(SDL_Color textColor=COLOR_BLACK,  SDL_Color backgroundColor=COLOR_WHITE);
    int changerTexte(const std::string& nouveau);

    virtual bool isClicked(const SDL_Event &event);
    virtual bool isInItem(int x, int y) const;

    virtual bool setCoords(int x, int y);
    virtual void setSize(int w, int h);
    virtual void setMaxSize(int w, int h);
    virtual void setSizeProportional(int w, int h);

    virtual void getCoords(int &x,int &y) const;
    virtual void getSize(int &w, int &h) const;

    virtual int setRenderer(SDL_Renderer* render);

    virtual void hide(bool state=true);
    virtual bool isHidden() const;

    virtual ITEM_TYPE getType() const{return ITEM_EVENTTEXTBUTTON;};

    ~EventTextButton();

    protected:
        Text* text;
        EventButton* button;
        int flags;
        bool inverse, hidden;
};
class SmartEventTextButton: public EventTextButton{
    public:

    SmartEventTextButton(SDL_Surface* apparence,SDL_Renderer* renderer,int x, int y,int eventCode,const std::string& text,const std::string&police, SDL_Color textColor=COLOR_BLACK, int flags=MENU_ITEM_CENTRAGEAUTO);
    SmartEventTextButton(SDL_Renderer* renderer,SDL_Rect pos,int eventCode,const std::string& text,const std::string&police,int fontSize=16, SDL_Color textColor=COLOR_BLACK, SDL_Color backgroundColor=COLOR_WHITE, int flags=MENU_ITEM_CENTRAGEAUTO);

    virtual int render();
    private:
       const std::string *refStr;
};
class ScrollList: public Objet{
public:
    ScrollList(SDL_Rect position, int nbItems, int start=0);
    int addItem(Objet* item, int place=-1);

    virtual int render();
    virtual bool click(bool click);

    virtual bool isClicked(const SDL_Event &event);
    virtual bool isInItem(int x, int y) const;

    virtual bool setCoords(int x, int y);
    virtual void setSize(int w, int h);
    virtual void setMaxSize(int w, int h);
    virtual void setSizeProportional(int w, int h);

    virtual void getCoords(int &x,int &y) const;
    virtual void getSize(int &w, int &h) const;

   virtual ITEM_TYPE getType() const{return ITEM_SCROLL;};

    const Objet* getItem(int id) const;
    Objet* getItem(int id);

    virtual void hide(bool state=true);
    virtual bool isHidden() const;
    int scroll(bool sens);

    int getPos() const;

    virtual int setRenderer(SDL_Renderer* render);
    ~ScrollList();

private:
    bool hidden;
    unsigned taille, pos;
    SDL_Rect coords;
    std::vector<Objet*>items;

};

//MENU
class Menu{
    protected:
        SDL_BlendMode previousBlendMode;
        bool fermer;
        SDL_Color couleurMenu;
        std::vector<Objet*> items;
        int flags;

        SDL_Window* fenetre, *parent;
        Uint32 parentID;
        SDL_Rect prevParentPos;

        SDL_Renderer* renderer;
        SDL_Texture* previousBack;
        SDL_Rect prevClip;

    public:
        //NEW window
        Menu(const SDL_Rect& position,int style=0,SDL_Window* parentWindow=NULL,SDL_Color couleur=COLOR_BLACK);
        //No new window, fullscreen clipped
        Menu(SDL_Renderer* renderer,int style=0,SDL_Color couleur=COLOR_BLACK, SDL_Rect* clip=NULL);

        ~Menu();

       // int load(std::string& templateFile);


        void fermerMenu();
        int renderItems(bool clearBeforeRender=true, bool actualizeRenderer=true);
        bool processEvent(const SDL_Event &event);

        bool isClicked(int x, int y);

        //retourne l'id de l'item ajoute
        int addItem(Objet* item);

        void removeItem(int id=-1);
        void removeItems(int debut=0, int taille=-1);
        void removeItems(ITEM_TYPE type, int taille=-1);

        int getItemId(int x, int y) const;
        int getItemId(ITEM_TYPE type) const;
        const Objet* getItem(int id) const;
        Objet* getItem(int id);

        int nbItems();

    };

#endif // MENU_H_INCLUDED
