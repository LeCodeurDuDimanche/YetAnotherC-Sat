#ifndef SDL_SCROLL_H_INCLUDED
#define SDL_SCROLL_H_INCLUDED

#include "SDL2/SDL.h"

#define SCROLL_NO_CHANGE -1

/// Option performant un flip horiwontal aux coordonées, notamment utile pour passer d'un repère conventionnel au repère SDL.
#define SCROLL_Y_INVERSED 0x0001
/**
    @brief La classe Scroll permet de gérer le scrolling 2D
*/
class Scroll
{
    public:
    /// @brief Constructeur par défaut
    Scroll();
    /** @brief Constructeur complet

    @param[in] xFollowing Définit la coordonnée sur l'axe X à suivre lors du scroll
    @param[in] yFollowing Définit la coordonnée sur l'axe Y à suivre lors du scroll
    @param[in] wMap Largeur totale de la carte utilisée pour le scroll
    @param[in] hMap Hauteur totale de la carte utilisée pour le scroll
    @param[in] outputView Définit la vue de sortie (cf setOutputView() )
    @param[in] options @a Optionnel Définit les options

    */
    Scroll(const int& xFollowing, const int &yFollowing, int wMap, int hMap, const SDL_Rect& outputView,Uint16 options=0);
    /** @brief Initialise l'objet de scroll, si non initialisée à la construction

    @param[in] xFollowing Définit la coordonnée sur l'axe X à suivre lors du scroll
    @param[in] yFollowing Définit la coordonnée sur l'axe Y à suivre lors du scroll
    @param[in] wMap Largeur totale de la carte utilisée pour le scroll
    @param[in] hMap Hauteur totale de la carte utilisée pour le scroll
    @param[in] outputView Définit la vue de sortie (cf setOutputView() )
    @param[in] options @a Optionnel Définit les options

    */
    void init(const int& xFollowing, const int &yFollowing, int wMap, int hMap, const SDL_Rect& outputView,Uint16 options=0);
    /** @brief Initialise l'objet de scroll, si non initialisée à la construction

    @param[in] wMap Largeur totale de la carte utilisée pour le scroll
    @param[in] hMap Hauteur totale de la carte utilisée pour le scroll
    @param[in] outputView Définit la vue de sortie (cf setOutputView() )
    @param[in] options @a Optionnel Définit les options

    */
    void init(int wMap, int hMap, const SDL_Rect& outputView,Uint16 options=0);

    /** @brief Définit les options

    @param[in] options Options @sa #SCROLL_Y_INVERSED

    */
    void setOptions(Uint16 options);
    /** @brief Retourne les options

    @return Options @sa #SCROLL_Y_INVERSED

    */
    Uint16 getOptions() const;

    /** @brief Définit la vue de sortie

    @param[in] outputView Zone d'affichage (voir #remarks )

    @remark Le SDL_Rect outputView sert à définir la zone d'affichage, dans laquelle seront comprises les coordonnées relatives.
    x: Début de la zone d'affichage sur l'axe horizontal (0 pour l'écran complet)
    y: Début de la zone d'affichage sur l'axe vertical (0 pour l'écran complet)
    w: Largeur de la zone d'affichage (Largeur de la fenêtre pour l'écran complet)
    h: Hateur de la zone d'affichage (Hauteur de la fenêtre pour l'écran complet)

    */
    void setOutputView(const SDL_Rect& outputView);
    /** @brief Définit la vue

     @param[in] x Abscisse du début de la vue (zone de scrolling)
     @param[in] y Ordonnée du début de la vue (zone de scrolling)

     @sa incrementView()
    */
    void setView(int x, int y);
    /** @brief Incrémente la vue

     @param x Abscisse à ajouter au début de la vue (zone de scrolling)
     @param y Ordonnée à ajouter au début de la vue (zone de scrolling)

     @sa setView()

    */
    void incrementView(int x, int y=SCROLL_NO_CHANGE);

    /** @brief Effectue le scroll

     Recentre la vue sur les coordonnées suivies si le scrolling est activé
     @sa setView(), setObjectFollowed()

    */
    void scroll();
    /** @brief Définit les coordonnées suivies par le scroll

     @param[in] x Référence sur l'abscisse suivie par le scrolling
     @param[in] y Référence sur l'ordonnée suivie par le scrolling

     @sa scroll()

    */
    void setObjectFollowed(const int &x, const int&y);

     /** @brief Passe des coordonnées absolues à des coordonnées relatives

     Cette fonction retourne les coordonnées de position tels qu'elles doivent être affichées, à partir des coordonnées
     absolues sur la carte totale.

     @param[in,out] position Position à actualiser

     @return Vrai si le rectangle est visible, faux s'il ne l'est pas

     @sa relativeToAbsolute()

    */
    bool absoluteToRelative(SDL_Rect& position) const;
     /** @brief Passe des coordonnées relatives à des coordonnées absolues

     @param[in,out] position Position à actualiser

     @return Retourne vrai si le rectangle est dans la carte, faux s'il ne l'est pas

     @sa relativeToAbsolute()

    */
    bool relativeToAbsolute(SDL_Rect& position) const;

    /** @brief Retourne la vue

     @return Retourne la vue (zone de scrolling)

     @sa setView()

    */
    const SDL_Rect& getView() const;
    /** @brief Retourne la vue de sortie

    @return Retourne la zone d'affichage (voir #remarks )

    @remark Le SDL_Rect outputView sert à définir la zone d'affichage, dans laquelle seront comprises les coordonnées relatives.
    x: Début de la zone d'affichage sur l'axe horizontal (0 pour l'écran complet)
    y: Début de la zone d'affichage sur l'axe vertical (0 pour l'écran complet)
    w: Largeur de la zone d'affichage (Largeur de la fenêtre pour l'écran complet)
    h: Hateur de la zone d'affichage (Hauteur de la fenêtre pour l'écran complet)

    */
    SDL_Rect getOutputView() const;

    /** @brief Stop le scrolling
     La vue (zone de scrolling) est immobilisée et les appels à #scroll() seront ineffectifs.
     Appeler #resume() pour relancer le scrolling.

     @sa scroll(),getState()

    */
    void stop();
    /** @brief Lance le scrolling
     Le scrolling est activé (il est activé par défaut, à moins d'un appel à #stop() ).

     @sa scroll(),getState()

    */
    void resume();
    /** @brief Retourne l'état du scrolling

     @return Retourne vrai si le scrolling est activé, faux sinon.

     @sa resume(),stop()

    */
    bool getState() const;

    private:
    SDL_Rect view;
    int outputX,outputY;
    Uint16 options;
    const int *x, *y;
    bool scrolling; //scrolling active
    int lastX,lastY; //dernieres coordonnees
    int wMap, hMap;
};

#endif // SDL_SCROLL_H_INCLUDED
