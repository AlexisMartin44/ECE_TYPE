#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED

/***********************************************************/
//                          TIRS
/***********************************************************/
typedef struct acteur
{

    float x,y;    //position
    int precx, precy;
    float dx,dy;  //deplacement
    int tx,ty;  // largeur hauteur
    int type;   //type d'acteur
    int couleur;
    int comportement; //explosion ou deplacement
    int cptexplo; // temps depuis l'explosion
    int vivant; //oui ou non

    int imgcourante;
    BITMAP *img[2];

} t_acteur;


typedef struct listeActeurs
{
    int max;    //taille tableau de pointeurs, max d'acteurs
    int n;      //nbr de pointeurs utilises
    t_acteur **tab; //le tableau des acteurs

} t_listeActeurs;

/***********************************************************/
//                          JOUEUR
/***********************************************************/
typedef struct joueur
{
    int x,y;     // position
    int tx,ty;   // taille
    int vit;     // vitesse des déplacements (nombre de pixels)
    int cpttir0; // tempo armement 0
    int cpttir1; // tempo armement 1
    int cpt;
    int precx, precy;
    int chargeTir;
    int maintientEspace;
    int couleurChargeTir1, couleurChargeTir2;
    int vieActuelle, degatsSubis;
    int invincibilite;
    int clignote1, clignote2, tempsInvinc;
    int imgcourante;
    int score, scoreTemporaire;
    int dmgUp, tempsDmgUp;
    int bouclier, imgBouclier, cptBouclier;

    SAMPLE* vieSon;
    BITMAP* vie;
    BITMAP *img[4];
    BITMAP* bouclierVisuel[6];
    BITMAP *btmBouc;
} t_joueur;

/***********************************************************/
//                          ENNEMIS
/***********************************************************/
typedef struct ennemi
{
    int x,y;     // position
    int dx,dy;   // vecteur déplacement
    int tx,ty;   // taille
    int type;
    int vivant;
    int vie;
    int cpttir;
    int cpt;
    BITMAP *img[6]; // sprite (image chargée)
    BITMAP *oeil[8];
    int imgoeil;
    int alea;
    int imgcourante;
    int precx, precy;
} t_ennemi;

typedef struct listeEnnemis
{
    int maximum;
    int n;
    t_ennemi **tab;

}t_listeEnnemis;

/***********************************************************/
//                          EXPLOSIONS
/***********************************************************/
typedef struct explosion
{
    int cptexplo;
    int x, y;
    int dx;
    int imgcourante;
}t_explosion;

typedef struct listeExplosions
{
    int n;
    int maximum;
    t_explosion **tab;
}t_listeExplosions;

/***********************************************************/
//                          OBJETS
/***********************************************************/

typedef struct objet
{
    int x, y;
    int tx, ty;
    int dx, dy;
    int type;
    int cpt;
    int imgcourante;
    int vivant;
    BITMAP *img[12];
}t_objet;

typedef struct listeObjets
{
    int n;
    int maximum;
    t_objet **tab;
}t_listeObjets;


/***********************************************************/
//                          TUILES
/***********************************************************/
typedef struct tuiles
{
    BITMAP* Horizontale0;
    BITMAP* Horizontale1;
    BITMAP* Horizontale2;
    BITMAP* Sol0;
    BITMAP* Sol1;
    BITMAP* Horizontale0Col;
    BITMAP* Horizontale1Col;
    BITMAP* Horizontale2Col;
    BITMAP* Sol0Col;
    BITMAP* Sol1Col;
    BITMAP* MurCassable;
    BITMAP* Boss1;
    BITMAP* Boss2;

    int alea1;
    int xBoss1, xBoss2;

}t_tuiles;


/***********************************************************/
//                          PAGE
/***********************************************************/
typedef struct bitmaps
{
    int niveau;
    int screenx1;
    int screenx2;
    int first1;
    int first2;
    int vitesseScreen;
    int dxTuiles;
    int couleurTableauBord;
    int retourMenu;
    int screenActuel;
    int cpt;
    int vieMurCassable;
    int meilleursScores[10];
    int mute;
    int cptEnnemi;
    unsigned int temps;
    unsigned int minutes;
    FONT *writing;
    SAMPLE *tir;
    SAMPLE *finNiveau;
    SAMPLE *mort;

    BITMAP *regles;
    BITMAP* buffer;
    BITMAP* collision;
    BITMAP* plateau1;
    BITMAP* plateau2;
    BITMAP* plateau3;
    BITMAP* plateau4;
    BITMAP* decorMenu;
    BITMAP* fondBoss;
    BITMAP *ecriture;

    BITMAP* missile;
    BITMAP* tirCharge;
    BITMAP* jaugeDeTir;
    BITMAP* explosion;
    BITMAP* explo[6];

    BITMAP *game[3];
    BITMAP *over[3];
    BITMAP *crane;
    BITMAP *titre;
    BITMAP *fleche;
    BITMAP *bossApproche;
    BITMAP *stageClear;
    BITMAP *note;

}t_bitmaps;


//MENU PRINCIPAL
void menuPrincipal();

//MEILLEURS SCORES
int highScores();

//PAUSE
void incrementa_timer();
void pause(t_bitmaps *page, BITMAP *img[3]);
void attributionImagesPause(BITMAP *img2[3]);

//TIR CHARGER
void tirCharger(t_joueur *joueur, t_listeActeurs *la, t_bitmaps *page);

//VAISSEAU
t_joueur * creerJoueur(t_bitmaps *page);
int actualiserJoueur(t_joueur *joueur,t_listeActeurs *la, t_bitmaps *page);
void dessinerJoueur(t_joueur *joueur, t_bitmaps *page);
void reinitialisationVaisseau(t_joueur* joueur);

//TIRS
t_acteur * creerActeur(int x,int y,int type, t_joueur* joueur, t_bitmaps *page);
void ajouterActeur(t_listeActeurs *la,int x,int y,int type, t_joueur *joueur, t_bitmaps *page);
t_listeActeurs * creerListeActeurs(int maxacteurs);
void actualiserActeur(t_acteur *acteur, t_bitmaps *page);
void enleverActeur(t_listeActeurs *la,int i);
void actualiserListeActeurs(t_listeActeurs *la, t_bitmaps *page);
void dessinerListeActeurs(t_bitmaps *bmp,t_listeActeurs *la);
void dessinerActeur(t_bitmaps *bmp,t_acteur *acteur);

//ECRAN
void initialisationBitmaps(t_tuiles *tuile, t_bitmaps *page);
void tuilesNiveau3(t_bitmaps* page, t_tuiles* tuile);
void tuilesNiveau2(t_bitmaps* page, t_tuiles* tuile);
void ajoutTuilesAlea(t_bitmaps* page, t_tuiles* tuile);
void screenNiveau3(t_bitmaps *page, t_tuiles *tuile);
void screenFinNiveau3(t_bitmaps *page, t_tuiles *tuile, t_joueur *joueur, t_listeEnnemis *ennemis, t_listeActeurs *acteurs);
void screenNiveau2(t_bitmaps *page, t_tuiles *tuile, t_joueur *joueur, t_listeEnnemis *ennemis);
void screenFinNiveau3(t_bitmaps *page, t_tuiles *tuile, t_joueur *joueur, t_listeEnnemis *ennemis, t_listeActeurs *acteurs);
void bossNiveau3(t_bitmaps *page, t_tuiles *tuile, t_listeEnnemis *ennemis);
void affichageEtTestcollision(t_bitmaps *page, int *test, t_listeEnnemis *ennemis);
void gererCollisions(t_bitmaps *page, t_joueur *joueur);
void dessinerTableauBord(t_bitmaps *page, t_joueur *joueur);

//ENNEMIS
t_listeEnnemis *creerListeEnnemis(int maxacteurs);
void actualiserListeEnnemis(t_listeEnnemis *ennemis, t_bitmaps* page);
void actualiserEnnemi(t_ennemi *ennemi, t_bitmaps* page);
void enleverEnnemis(t_listeEnnemis *ennemis,int i);
t_ennemi *ajouterEnnemis(t_listeEnnemis *ennemis, t_bitmaps *page);
t_ennemi *creerEnnemi(int type, t_bitmaps *page);
void dessinerListeEnnemis(t_bitmaps *page,t_listeEnnemis *ennemis, t_joueur *joueur);

//TOUCHER UN ENNEMI
void destinActeur(t_acteur *acteur, t_ennemi *ennemi, t_joueur *joueur, int action, t_bitmaps *page, t_tuiles *tuile, t_listeExplosions *explosions, t_listeObjets *objets);
void collisionActeur(t_ennemi *ennemi ,t_acteur *acteur, t_joueur *joueur, t_bitmaps *page, t_tuiles *tuile, t_listeExplosions *explosions, t_listeObjets *objets);
void collisionListeActeurs(t_listeEnnemis *ennemis,t_listeActeurs *acteurs, t_joueur *joueur, t_bitmaps *page, t_tuiles *tuile, t_listeExplosions *explosions, t_listeObjets *objets);

//TIRS ENNEMIS
void tirEnnemis(t_listeEnnemis *ennemis, t_bitmaps *page, t_listeActeurs *acteurs, t_joueur *joueur);
void tirFixes(t_listeEnnemis*ennemis, t_bitmaps *page, t_listeActeurs *acteurs, t_joueur *joueur);///*******************MODIF LAURE******************///

//GAME OVER
void gameover(t_joueur *joueur, t_bitmaps *page);

//BOSS
void ajouterBoss(t_listeEnnemis *ennemis, t_bitmaps *page);
void mortBoss(t_bitmaps *page, t_joueur *joueur, t_tuiles *tuile);

//EXPLOSIONS
t_listeExplosions *creerListeExplosions(int maxEnnemis);
t_explosion *creerExplosion(int x, int y, t_bitmaps *page);
t_explosion *ajouterExplosion(t_listeExplosions *explosions, int x, int y, t_bitmaps *page);
void enleverExplosion(t_listeExplosions *explosions, int i);

//OBJETS
t_listeObjets *creerListeObjets(int maxObjets);
t_objet *creerObjet(int x, int y, t_bitmaps *page, int type);
t_objet *ajouterObjet(t_listeObjets *objets, int x, int y, t_bitmaps *page);
void animationObjets(t_listeObjets *objets, t_bitmaps *page);
void enleverObjet(t_listeObjets *objets, int i);

//CHOIX NIVEAU
void choixNiveaux(t_bitmaps *page, t_tuiles *tuile, t_joueur *joueur, t_listeEnnemis *ennemis, t_listeActeurs *acteurs);

//BOUCLE DE JEU
void boucleJeu(t_bitmaps *page,t_tuiles *tuile, t_joueur *vaisseau);





#endif // PROTO_H_INCLUDED
