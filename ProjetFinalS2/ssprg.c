#include <allegro.h>
#include <stdio.h>
#include <time.h>
#include "proto.h"


/***********************************************************************************************************************************************************************************************************/

/*                                                                                       MENU PRINCIPAL                                                                                                    */

/***********************************************************************************************************************************************************************************************************/

//A MODIFIER, menu de début de jeu, permet la sélection JOUER ou QUITTER
void menuPrincipal()
{
    srand(time(NULL));
    int first=0, screenx=0, moveAlea, xFleche = 215, yFleche = 289, i, cptFleche=0;
    char nomfichier[256];


    t_bitmaps page;
    t_tuiles tuile;
    t_joueur *vaisseau = NULL;
    FILE* fichier = NULL;
    fichier = fopen("meilleursScore.txt", "r");     //sauvegarde
    if(fichier==NULL)
    {
        allegro_message("Problème de fichier");
        exit(EXIT_FAILURE);
    }


    for(i=0;i<10; i++)
    {
        fscanf(fichier, "%d\n", &page.meilleursScores[i]);
    }

    fclose(fichier);

    page.niveau = 1;
    initialisationBitmaps(&tuile, &page);   //initialise toutes les bitmaps

    vaisseau = creerJoueur(&page);               //crée les données propres au joueur

    vaisseau->x = 100;
    vaisseau->y = 250;
    vaisseau->imgcourante = 0;




    // Boucle d'animation (pas d'interaction)
    while (!key[KEY_ESC])
    {


        clear_bitmap(page.buffer);
        if(cptFleche>0)
            cptFleche++;
        if(cptFleche>15)
            cptFleche = 0;
        vaisseau->invincibilite=0;
        vaisseau->imgcourante = 0;
        screenx+=10;                        //haute vitesse pour le menu
        if(screenx>page.decorMenu->w - SCREEN_W)    //scroll infini
            screenx=0;

        moveAlea = rand()%100;              //mouvement aleatoire du vaisseau
        if(moveAlea>98)
            first = 1;

        if(first == 1)
        {
            vaisseau->imgcourante = 1;
            vaisseau->y--;
            if(vaisseau->y<220)
            {
                first = 2;
            }
        }
        else if(first ==2)
        {
            vaisseau->imgcourante = 2;
            vaisseau->y++;
            if(vaisseau->y>275)
            {
                first = 3;
            }
        }
        else if(first==3)
        {
            vaisseau->imgcourante = 1;
            vaisseau->y--;
            if(vaisseau->y==250)
            {
                first=0;
            }
        }

        if((key[KEY_UP]||key[KEY_W])&&cptFleche==0)     //bouge la fleche du menu
        {
            if(yFleche!=289)
            {
                yFleche -= 58;
                cptFleche++;
            }
            if(yFleche==347)
                xFleche = 150;
            else
                xFleche = 215;


        }

        if((key[KEY_DOWN]||key[KEY_S])&&cptFleche==0)
        {
            if(yFleche!=463)
            {
                yFleche += 58;
                cptFleche++;
            }
            if(yFleche==347)
                xFleche = 150;
            else
                xFleche = 215;
        }

        if(key[KEY_ENTER])
        {
            if(yFleche==463)
                key[KEY_ESC] = 1;
            if(yFleche==347)
                screenx = highScores(&page, vaisseau, screenx);     //meilleurs scores
            if(yFleche==405)
            {
                 while(!key[KEY_ESC])
                 {
                     blit(page.regles, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);         //regles
                 }
                 key[KEY_ESC] = 0;
            }
            if(yFleche==289)
            {
                while(vaisseau->x<SCREEN_W) //sortie du vaisseau avant de lancer le jeu
                {
                    vaisseau->x+=15;
                    blit(page.decorMenu,page.buffer,screenx,0,0,0,SCREEN_W,SCREEN_H); // effacer buffer en appliquant décor  (pas de clear_bitmap)
                    dessinerJoueur(vaisseau, &page);        //on dessine sur la page le sprite du vaisseau
                    blit(page.buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                    rest(10);
                }
                if(page.niveau==2)
                {
                    for (i=0;i<3;i++)
                    {
                        sprintf(nomfichier,"Images/Vaisseau/Avion%d.bmp",i);
                        vaisseau->img[i] = load_bitmap(nomfichier,NULL);
                        if (!vaisseau->img[i])
                        {
                            allegro_message("pas pu trouver %s",nomfichier);
                               exit(EXIT_FAILURE);
                        }
                    }
                    vaisseau->tx = vaisseau->img[0]->w;
                    vaisseau->ty = vaisseau->img[0]->h;
                }
                if(page.niveau==1)
                {
                    for (i=0;i<4;i++)
                    {
                        sprintf(nomfichier,"Images/Vaisseau/helico%d.bmp",i);

                        vaisseau->img[i] = load_bitmap(nomfichier,NULL);
                        if (!vaisseau->img[i])
                        {
                            allegro_message("pas pu trouver %s",nomfichier);
                            exit(EXIT_FAILURE);
                        }
                    }
                    vaisseau->tx = vaisseau->img[0]->w;
                    vaisseau->ty = vaisseau->img[0]->h;
                }
                boucleJeu(&page, &tuile, vaisseau); //lance le jeu
                if(page.retourMenu==0)              //quitte le jeu suit echap pendant la partie
                    key[KEY_ESC]=1;
                else                                //retour au menu si echap pendant le game over
                {
                    key[KEY_ESC]=0;
                    for (i=0;i<3;i++)
                    {
                        sprintf(nomfichier,"Images/Vaisseau/Vaisseau%d.bmp",i);
                        vaisseau->img[i] = load_bitmap(nomfichier,NULL);
                        if (!vaisseau->img[i])
                        {
                            allegro_message("pas pu trouver %s",nomfichier);
                            exit(EXIT_FAILURE);
                        }
                    }
                    reinitialisationVaisseau(vaisseau);     //on reinitialise les variables
                    page.niveau=1;
                    vaisseau->score = 0;
                    vaisseau->scoreTemporaire = 0;
                    initialisationBitmaps(&tuile, &page);
                    page.retourMenu = 0;
                }

            }
        }

        //affichage du vaisseau normal
        blit(page.decorMenu,page.buffer,screenx,0,0,0,SCREEN_W,SCREEN_H); // effacer buffer en appliquant décor  (pas de clear_bitmap)
        dessinerJoueur(vaisseau, &page);        //on dessine sur la page le sprite du vaisseau
        draw_sprite(page.buffer, page.titre, 100, 50);
        draw_sprite(page.buffer, page.fleche, xFleche, yFleche);
        blit(page.buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);    //affichage du buffer a l'ecran
        rest(10);
    }
    destroy_sample(page.tir);
    destroy_sample(vaisseau->vieSon);
}

/***********************************************************************************************************************************************************************************************************/

/*                                                                                       MEILLEURS SCORES                                                                                                             */

/***********************************************************************************************************************************************************************************************************/
//meilleurs scores
int highScores(t_bitmaps *page, t_joueur *joueur, int screenx)
{
    int i, cpt, imgcourante= 0, y = 275;
    char nomfichier[256];
    BITMAP *Score[3];
    for(i=0; i<3; i++)
    {
        sprintf(nomfichier,"Images/HighScores/%d.bmp",i);
        Score[i] = load_bitmap(nomfichier,NULL);
        if (!Score[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
    key[KEY_ENTER] = 0;
    joueur->imgcourante = 0;
    while(!key[KEY_ESC]&&!key[KEY_SPACE]&&!key[KEY_ENTER])
    {
        blit(page->decorMenu,page->buffer,screenx,0,0,0,SCREEN_W,SCREEN_H); // effacer buffer en appliquant décor  (pas de clear_bitmap)
        dessinerJoueur(joueur, page);        //on dessine sur la page le sprite du vaisseau
        screenx+=10;                        //haute vitesse pour le menu
        if(screenx>page->decorMenu->w - SCREEN_W)    //scroll infini
            screenx=0;
        cpt++;
        if(cpt>200)
        {
            imgcourante++;
            cpt = 0;
        }

        if(imgcourante>2)
            imgcourante = 0;
        draw_sprite(page->buffer, Score[imgcourante], 75, 50);

        for(i=0; i<10; i++)
        {
            textprintf_ex(page->buffer, font, 300, y, makecol(255,255,255), -1, "%d  -                    %d", i, page->meilleursScores[i]);
            y+=20;
        }
        y = 275;




        blit(page->buffer, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
        rest(10);
    }
    key[KEY_ESC] = 0;
    key[KEY_SPACE] = 0;
    key[KEY_ENTER] = 0;


    return screenx;
}



/***********************************************************************************************************************************************************************************************************/

/*                                                                                       PAUSE                                                                                                             */

/***********************************************************************************************************************************************************************************************************/

//menu pause
void pause(t_bitmaps *page, BITMAP *img[3])
{
    int imgcourante=0, posxPause = SCREEN_W/4 -50, posyPause = SCREEN_H/6, cpt = 0;

    srand(time(NULL));

    if(key[KEY_P])              //Lors de l'appui sur la touche P, stop l'écran de fond et affiche le menu pause
    {
        key[KEY_P] = 0;         //on réinitialise la touche, sinon quitte directement la boucle
        while(!key[KEY_P])
        {
            cpt++;
            if(cpt>1000)        //toutes les 3 secondes (environ)
            {
                imgcourante++;              //change l'image (couleur de PAUSE)
                if(imgcourante==3)
                {
                    imgcourante=0;
                }
                cpt=0;                    //on reinitialise le timer
            }
            draw_sprite(page->buffer, img[imgcourante], posxPause, posyPause);
            blit(page->buffer, screen, 0,0,0,0,SCREEN_W,SCREEN_H);
        }
        key[KEY_P] = 0;     //on réinitialise la touche
    }
}

//met dans un tableau bitmap les trois images qui tournent au menu pause
void attributionImagesPause(BITMAP *img2[3])
{
    int i;
    char nomfichier[256];

    for (i=0;i<3;i++)
    {
        sprintf(nomfichier,"Images/Pause/Pause%d.bmp",i);

        img2[i] = load_bitmap(nomfichier,NULL);
        if (!img2[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
}

/***********************************************************************************************************************************************************************************************************/

/*                                                                                       TIRS CHARGER                                                                                                              */

/***********************************************************************************************************************************************************************************************************/

//jauge de tir chargé, actif quand on maintient espace
void tirCharger(t_joueur *joueur, t_listeActeurs *la, t_bitmaps* page)
{
    int type = 0;

    joueur->chargeTir++;            //incremente la variable qui sert a la fois a l'effet visuel du rectangle qui augmente dans la jauge, et le moment a partir duquel le tir est considéré chargé
    joueur->maintientEspace = 0;    //ici on a besoin de cette variable, car on execute une action quand le bouton est relache, or si le bouton n'est pas enfonce la boucle ne se lance pas du tout
    rectfill(page->buffer, SCREEN_W/3+45, 575, SCREEN_W/3+45+joueur->chargeTir, 584, joueur->couleurChargeTir1);            //effet visuel de la jauge qui monte
    rectfill(page->buffer, SCREEN_W/3+45, 578, SCREEN_W/3+45+joueur->chargeTir, 580, joueur->couleurChargeTir2);
    if(joueur->chargeTir>165)
    {
        joueur->chargeTir = 165;
        joueur->couleurChargeTir1 = makecol(255,0,0);
        joueur->couleurChargeTir2 = makecol(255,119,119);
        type = 2;       //change le type de tir si on a maintenu assez longtemps
    }
    else
    {
        joueur->couleurChargeTir1 = makecol(0,0,255);
        joueur->couleurChargeTir2 = makecol(99,106,248);

    }

    if(!key[KEY_SPACE])     //quand on relache le bouton
    {
        if(page->mute==0)
            play_sample(page->tir, 20, 128, 1000, 0);
        ajouterActeur(la,joueur->x+joueur->tx,joueur->y+joueur->ty/2,type, joueur, page);     //on cree un acteur, tir charge si type=2 car chargeTir etait = à 165
        joueur->cpttir0 = 0;
        joueur->chargeTir = 0;
        joueur->maintientEspace=0;
    }
    else
        joueur->maintientEspace= 1;

}



/***********************************************************************************************************************************************************************************************************/

/*                                                                                       Vaisseau                                                                                                          */

/***********************************************************************************************************************************************************************************************************/
//initialise le joueur
t_joueur * creerJoueur(t_bitmaps *page)
{
    int i;
    char nomfichier[256];
    t_joueur *nouv;



    // Allouer
    nouv = (t_joueur *)malloc(1*sizeof(t_joueur));

    // Initialiser
    for (i=0;i<3;i++)
    {
        sprintf(nomfichier,"Images/Vaisseau/Vaisseau%d.bmp",i);
        nouv->img[i] = load_bitmap(nomfichier,NULL);
        if (!nouv->img[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }



    nouv->vie = load_bitmap("Images/Vaisseau/Vie.bmp", NULL);
    nouv->btmBouc = load_bitmap("Images/Objets/Boucliers/7.bmp", NULL);
    for (i=0;i<6;i++)
    {
        sprintf(nomfichier,"Images/Objets/Boucliers/Bouclier%d.bmp",i);
        nouv->bouclierVisuel[i] = load_bitmap(nomfichier,NULL);
        if (!nouv->bouclierVisuel[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
    nouv->vieActuelle = 3;
    nouv->degatsSubis = 0;
    nouv->invincibilite = 0;
    nouv->score = 0;
    nouv->scoreTemporaire = 0;
    nouv->dmgUp = 0;
    nouv->bouclier = 0;
    nouv->cpt = 0;
    nouv->imgBouclier =0;
    nouv->vieSon = load_sample("Sons/VieSon.wav");

    nouv->tx = nouv->img[0]->w;
    nouv->ty = nouv->img[0]->h;

    nouv->x = 200;
    nouv->y = 250;
    nouv->vit = 3;

    nouv->cpttir0 = 0;
    nouv->cpttir1 = 0;
    nouv->chargeTir = 0;
    nouv->couleurChargeTir1 = makecol(0,0,255);
    nouv->couleurChargeTir2 = makecol(99, 106, 248);

    nouv->clignote1 = 0;        //variables pour l'effet visuel d'invincibilité
    nouv->clignote2 = 0;
    nouv->tempsInvinc = 0;
    nouv->tempsDmgUp = 0;

    nouv->maintientEspace = 0;

    return nouv;
}

//gere les coordonnées du joueur et la creation de ses tirs
int actualiserJoueur(t_joueur *joueur,t_listeActeurs *la, t_bitmaps *page)
{
    int imgcourante = 0, i, ok = 0;

    joueur->precx = joueur->x;      //besoin de ces anciennes coordonées pour test les collisions
    joueur->precy = joueur->y;

    if(key[KEY_W])
    {

        for(i=joueur->x; i<joueur->x+joueur->tx; i++)
        {
            if(getpixel(page->collision, i, joueur->y-joueur->vit)==makecol(255,255,255)&&joueur->invincibilite!=1&&page->niveau!=1) //test collision
            {
                joueur->y = joueur->precy;
                ok = 1;
            }

        }
        if(ok==0)
        {

            joueur->y -= joueur->vit;
            if (joueur->y<0)
            {
                joueur->y=0;
            }
            if(page->niveau!=1)
                imgcourante = 1;
        }
        ok = 0;
    }
    if(key[KEY_S])
    {
        for(i=joueur->x; i<joueur->x+joueur->tx; i++)
        {
            if(getpixel(page->collision, i, joueur->y+joueur->vit+joueur->ty)==makecol(255,255,255)&&joueur->invincibilite!=1&&page->niveau!=1) //test collision
            {
                joueur->y = joueur->precy;
                ok = 1;
            }

        }
        if(ok==0)
        {

           joueur->y += joueur->vit;
            if (joueur->y+joueur->ty > 550)
            {
                joueur->y=550-joueur->ty;
            }
            if(page->niveau!=1)
                imgcourante = 2;
        }
        ok = 0;
    }
    if(key[KEY_D])
    {
        for(i=joueur->y; i<joueur->y+joueur->ty; i++)
        {
            if(getpixel(page->collision, joueur->x+joueur->tx+joueur->vit, i)==makecol(255,255,255)&&joueur->invincibilite!=1&&page->niveau!=1) //test collision
            {
                joueur->x = joueur->precx-3;
                ok = 1;
            }
        }
        if(ok==0)
        {

           joueur->x += joueur->vit;
            if (joueur->x+joueur->tx > SCREEN_W)
            {
                joueur->x=SCREEN_W-joueur->tx;
            }
        }
        ok = 0;
    }
    if(key[KEY_A])
    {
        for(i=joueur->y; i<joueur->y+joueur->ty; i++)
        {
            if(getpixel(page->collision, joueur->x-joueur->vit, i)==makecol(255,255,255)&&joueur->invincibilite!=1&&page->niveau!=1) //test collision
            {
                joueur->x = joueur->precx;
                ok = 1;
            }
        }
        if(ok==0)
        {
            joueur->x -= joueur->vit;
            if (joueur->x<0)
            {
                joueur->x=0;
            }
        }
        ok = 0;
    }
    if(key[KEY_SEMICOLON])
    {
        if(page->mute==0)
            page->mute = 1;
        else
            page->mute=0;
        key[KEY_SEMICOLON] = 0;
    }


    // incrémenter la tempo des tirs
    joueur->cpttir0++;
    joueur->cpttir1++;

    if(page->niveau==1)
    {
        joueur->cpt++;
        if(joueur->cpt>5)
        {
           joueur->imgcourante++;
           if(joueur->imgcourante>3)
                joueur->imgcourante = 0;
           joueur->cpt = 0;
        }
        imgcourante = joueur->imgcourante;
    }



    // si le joueur appui sur la gachette et arme ok...
    // espace = laser
    if ((key[KEY_SPACE]||joueur->maintientEspace==1)&&joueur->cpttir0>=5)
    {


        joueur->maintientEspace = 1;
        tirCharger(joueur, la, page);
    }
    // entrée = missile
    if (key[KEY_ENTER] && joueur->cpttir1>=10)
    {
        ajouterActeur(la,joueur->x+joueur->tx,joueur->y+joueur->ty/2,1, joueur, page);
        joueur->cpttir1 = 0;
    }



    return imgcourante;
}

//permet d'afficher le vaisseau a l'ecran
void dessinerJoueur(t_joueur *joueur, t_bitmaps *page)
{
    if(joueur->invincibilite==1)        //si le joueur a pris un coup
    {
        joueur->clignote1++;
        joueur->tempsInvinc++;
        if(joueur->tempsInvinc>350)     //invincible un certains temps
        {
            if(getpixel(page->collision, joueur->x, joueur->y)!=makecol(255,255,255))       //si le joueur n'est pas dans un mur, on ne veut pas qu'il reste coincé
            {
                joueur->invincibilite = 0;
                joueur->tempsInvinc = 0;
            }
        }

    }
    if(joueur->dmgUp==1)
    {
        joueur->tempsDmgUp++;
        if(joueur->tempsDmgUp>500)
        {
            joueur->dmgUp = 0;
            joueur->tempsDmgUp = 0;
        }
    }
    if(joueur->clignote1<=10)
    {
        draw_sprite(page->buffer,joueur->img[joueur->imgcourante],joueur->x,joueur->y);
        joueur->clignote2 = 0;
    }
    else
    {
        joueur->clignote2++;
        if(joueur->clignote2>=10)
            joueur->clignote1=0;
    }

    if(joueur->bouclier==1)
    {
        joueur->cptBouclier++;
        if(joueur->cptBouclier>5)
        {
            joueur->cptBouclier = 0;
            joueur->imgBouclier++;
            if(joueur->imgBouclier>5)
                joueur->imgBouclier = 0;
        }
        if(page->niveau==1)
            draw_sprite(page->buffer, joueur->bouclierVisuel[joueur->imgBouclier], joueur->x+35, joueur->y-20);
        if(page->niveau==2)
            draw_sprite(page->buffer, joueur->bouclierVisuel[joueur->imgBouclier], joueur->x+25, joueur->y-35);
        if(page->niveau!=1&&page->niveau!=2)
            draw_sprite(page->buffer, joueur->bouclierVisuel[joueur->imgBouclier], joueur->x+7, joueur->y-20);
    }

}

void reinitialisationVaisseau(t_joueur* joueur)
{
    joueur->x = 100;
    joueur->y = 250;
    joueur->bouclier = 0;
    joueur->cptBouclier = 0;
    joueur->invincibilite = 0;
    joueur->tempsDmgUp = 0;
    joueur->tempsInvinc = 0;
    joueur->dmgUp = 0;
    joueur->vieActuelle = 3;
}
/***********************************************************************************************************************************************************************************************************/

/*                                                                                       TIRS                                                                                                              */

/***********************************************************************************************************************************************************************************************************/

//cree un tir
t_acteur * creerActeur(int x,int y,int type, t_joueur* joueur, t_bitmaps *page)
{
    int i, dyTirMissile;
    char nomfichier[256];
    t_acteur *nouv;

    // Allouer
    nouv=(t_acteur *)malloc(1*sizeof(t_acteur));
    dyTirMissile = rand()%2;

    // Initialiser ...

    // ici ce qui est commun aux acteurs
    nouv->x=x;
    nouv->y=y;
    nouv->type=type;
    nouv->comportement=0;
    nouv->cptexplo=0; // pas encore explosé mais on initialise par sécurité
    nouv->vivant=1;

    // ici ce qui est spécifique aux types
    switch (type)
    {
        // laser
        case 0:
            nouv->tx=30;
            nouv->ty=5;
            nouv->dx=10;
            nouv->dy=0;
            if(page->niveau==2)
                nouv->couleur=makecol(255,99,99);
            else
                nouv->couleur=makecol(99,99,255);
            break;

        // missile
        case 1:
            nouv->tx=40;
            nouv->ty=20;
            nouv->dx=2;
            // petite dispersion dans la trajectoire des missiles :
            nouv->dy=rand()%5-2;
            nouv->couleur=makecol(240,220,220);
            break;
        //tir charge du joueur
        case 2:
            nouv->tx = 127;
            nouv->ty = 35;
            nouv->dx = 10;
            nouv->dy = 0;
            break;
        //tir ennemi
        case 3:
            nouv->couleur = makecol(255,0,0);
            if(joueur->x<x-200)
            {
                nouv->dx = -5;
                nouv->dy = (-5*(joueur->y-y))/(joueur->x-x);
            }
            else if(joueur->x>x-100&&joueur->x<x+20)
            {
                nouv->dx = -page->vitesseScreen;
                if(joueur->y>y)
                    nouv->dy =3;
                if(joueur->y<y)
                    nouv->dy = -3;
            }
            else if(nouv->dx>-page->vitesseScreen)
                return NULL;
            nouv->tx = 5;
            nouv->ty = 5;
            break;
        case 4:
            nouv->x-=30;
            for (i=0;i<2;i++)
            {
                sprintf(nomfichier,"Images/Ennemi/MissileBoss%d.bmp",i);
                nouv->img[i] = load_bitmap(nomfichier,NULL);
                if (!nouv->img[i])
                {
                    allegro_message("pas pu trouver %s",nomfichier);
                    exit(EXIT_FAILURE);
                }
            }
            nouv->dx = -2;
            if(dyTirMissile==1)
                nouv->dy = -4;
            else
                nouv->dy = 4;
            nouv->tx = 30;
            nouv->ty = 38;
            nouv->imgcourante = 0;
            break;
        //tirs ennemis fixes: 3 cases pour trois trajectoires suivant l'image de l'ennemi affichée
        case 5:                                  ///***********************MODIFS LAURE TIRS ENNEMIS FIXES***********************///
            nouv->couleur = makecol(255,0,0);
            nouv->dx = -4;
            nouv->dy = -1;
            nouv->tx = 5;
            nouv->ty = 5;
            break;

        case 6:
            nouv->couleur = makecol(255,0,0);
            nouv->dx = 0;
            nouv->dy = -1;
            nouv->tx = 5;
            nouv->ty = 5;
            break;

        case 7:
            nouv->couleur = makecol(255,0,0);
            nouv->dx = 4;
            nouv->dy = -1;
            nouv->tx = 5;
            nouv->ty = 5;
            break;

    }

    return nouv;
}

//ajoute un tir
void ajouterActeur(t_listeActeurs *la,int x,int y,int type, t_joueur *joueur, t_bitmaps *page)
{
    int i;
    t_acteur *acteur;

    // Liste pleine, on alloue rien et on retourne NULL...
    if (la->n >= la->max)
        return NULL;

    // Allouer un acteur initialisé
    acteur=creerActeur(x,y,type, joueur, page);

    // Chercher un emplacement libre
    i=0;
    while (la->tab[i]!=NULL && i<la->max)
        i++;

    // Si on a trouvé ...
    // (normalement oui car on a vérifié n<max)
    if (i<la->max){
        // Accrocher le acteur à l'emplacement trouvé
        la->tab[i]=acteur;
        la->n++;
    }
    // Sinon c'est qu'il y a un problème de cohérence
    else
        allegro_message("Anomalie gestion ajouterActeur : liste corrompue");

}


t_listeActeurs *creerListeActeurs(int maxacteurs)
{
    int i;
    t_listeActeurs *nouv;

    nouv=(t_listeActeurs *)malloc(1*sizeof(t_listeActeurs));

    nouv->max=maxacteurs;
    nouv->n=0;
    nouv->tab=(t_acteur **)malloc(maxacteurs*sizeof(t_acteur*));

    for (i=0;i<maxacteurs;i++)
        nouv->tab[i]=NULL;

    return nouv;
}

//gere les coordonnées des tirs
void actualiserActeur(t_acteur *acteur, t_bitmaps *page)
{

    // deplacement
    acteur->x+=acteur->dx;
    acteur->y+=acteur->dy;

    // type missile : accélère en dx mais pas au dela d'une limite
    if (acteur->type==1 && acteur->dx<14)
        acteur->dx++;

    if(acteur->type==4)
    {
        acteur->precx = acteur->x;
        acteur->precy = acteur->y;
        if(acteur->dy>0)
            acteur->imgcourante= 1;
        else
            acteur->imgcourante = 0;

        if(getpixel(page->collision, acteur->x, acteur->y+acteur->dy+acteur->ty)==makecol(255,255,255)||acteur->y+acteur->ty>560||acteur->y<0||getpixel(page->collision, acteur->x, acteur->y+acteur->dy)==makecol(255,255,255)||getpixel(page->collision, acteur->x+acteur->tx, acteur->y+acteur->dy)==makecol(255,255,255)||getpixel(page->collision, acteur->x+acteur->tx, acteur->y+acteur->ty+acteur->dy)==makecol(255,255,255))
        {
            acteur->dy = -acteur->dy;
            acteur->y = acteur->precy + acteur->dy;
        }
    }

    // si dépasse le bord alors disparait
    if (acteur->x+acteur->tx<0 || acteur->x>SCREEN_W || acteur->y+acteur->ty<0 || acteur->y>SCREEN_H||acteur->x<0)
        acteur->vivant=0;

    // si en cours d'explosion incrémenter cptexplo
    // et si explose depuis trop longtemps alors disparait
    if (acteur->comportement==1)
    {
        acteur->cptexplo++;
        if (acteur->cptexplo > 7)
            acteur->vivant=0;
    }
}

void enleverActeur(t_listeActeurs *la,int i)
{

    // Vérifier qu'il y a bien un acteur accroché en indice i
    if (la->tab[i]!=NULL)
    {
        // libérer la mémoire du acteur
        free(la->tab[i]);

        // marquer l'emplacement comme libre
        la->tab[i]=NULL;
        la->n--;

    }
}

void actualiserListeActeurs(t_listeActeurs *la, t_bitmaps *page)
{
    int i;

    // actualiser chaque acteur
    // si un acteur n'est plus vivant, l'enlever de la liste
    for (i=0;i<la->max;i++)
        if (la->tab[i]!=NULL)
        {
            actualiserActeur(la->tab[i], page);
            if (!la->tab[i]->vivant)
            {
                enleverActeur(la,i);
            }
        }
}

//affiche les acteurs a l'ecran
void dessinerListeActeurs(t_bitmaps *bmp,t_listeActeurs *la)
{
    int i;

    for (i=0;i<la->max;i++)
        if (la->tab[i]!=NULL)
            dessinerActeur(bmp,la->tab[i]);
}

void dessinerActeur(t_bitmaps *bmp,t_acteur *acteur)
{
    // Si pas d'explosion
    if (acteur->comportement==0)
    {
        switch(acteur->type)
        {
            case 0:
                rectfill(bmp->buffer,acteur->x,acteur->y-1,acteur->x+acteur->tx,acteur->y+1,acteur->couleur);
                rectfill(bmp->buffer,acteur->x,acteur->y+1,acteur->x+acteur->tx,acteur->y+acteur->ty-1,makecol(255,255,255));
                rectfill(bmp->buffer,acteur->x,acteur->y+acteur->ty-1,acteur->x+acteur->tx,acteur->y+acteur->ty,acteur->couleur);
                break;
            case 1:
                draw_sprite(bmp->buffer, bmp->missile, acteur->x, acteur->y);
                break;
            case 2:
                draw_sprite(bmp->buffer, bmp->tirCharge, acteur->x, acteur->y);
                break;
            case 3:
                circlefill(bmp->buffer, acteur->x, acteur->y, 10, acteur->couleur);
                circlefill(bmp->buffer, acteur->x, acteur->y, 7, makecol(255,123,36));
                break;
            case 4:
                draw_sprite(bmp->buffer, acteur->img[acteur->imgcourante], acteur->x, acteur->y);
                break;
            case 5:                                                                 ///****************MODIFS LAURE TIRS ENNEMIS FIXES*************///
                circlefill(bmp->buffer, acteur->x, acteur->y, 10, acteur->couleur);
                circlefill(bmp->buffer, acteur->x, acteur->y, 7, makecol(255,123,36));
                break;
            case 6:
                circlefill(bmp->buffer, acteur->x, acteur->y, 10, acteur->couleur);
                circlefill(bmp->buffer, acteur->x, acteur->y, 7, makecol(255,123,36));
                break;
            case 7:
                circlefill(bmp->buffer, acteur->x, acteur->y, 10, acteur->couleur);
                circlefill(bmp->buffer, acteur->x, acteur->y, 7, makecol(255,123,36));
                break;
        }
    }
    // sinon on dessine l'explosion
    // d'un diamètre décroissant et d'une couleur rougissante
    else
    {
        switch(acteur->type)
        {
            case 0:
                circlefill(bmp->buffer,acteur->x+acteur->tx/2,acteur->y+acteur->ty/2,30-4*acteur->cptexplo,makecol(255,255-15*acteur->cptexplo,255-30*acteur->cptexplo));
                break;
            case 1:
                circlefill(bmp->buffer,acteur->x+acteur->tx/2,acteur->y+acteur->ty/2,60-6*acteur->cptexplo,makecol(255-15*acteur->cptexplo,128-15*acteur->cptexplo,0));
                break;
        }
    }
}





/***********************************************************************************************************************************************************************************************************/

/*                                                                                       ECRAN                                                                                                             */

/***********************************************************************************************************************************************************************************************************/
//initialise les bitmaps du niveau 3
void iniBitmapsNiveau3(t_bitmaps *page, t_tuiles *tuile)
{
    tuile->Horizontale0 = load_bitmap("Images/Tuiles/TuileHorizontale0.bmp", NULL);
    tuile->Horizontale1 = load_bitmap("Images/Tuiles/TuileHorizontale1.bmp", NULL);
    tuile->Horizontale2 = load_bitmap("Images/Tuiles/TuileHorizontale2.bmp", NULL);
    tuile->Sol0 = load_bitmap("Images/Tuiles/TuileSol0.bmp", NULL);
    tuile->Sol1 = load_bitmap("Images/Tuiles/TuileSol1.bmp", NULL);

    tuile->Horizontale0Col = load_bitmap("Images/Tuiles/TuileHorizontale0Col.bmp", NULL);
    tuile->Horizontale1Col = load_bitmap("Images/Tuiles/TuileHorizontale1Col.bmp", NULL);
    tuile->Horizontale2Col = load_bitmap("Images/Tuiles/TuileHorizontale2Col.bmp", NULL);
    tuile->Sol0Col = load_bitmap("Images/Tuiles/TuileSol0Col.bmp", NULL);
    tuile->Sol1Col = load_bitmap("Images/Tuiles/TuileSol1Col.bmp", NULL);
    tuile->Boss1 = load_bitmap("Images/Tuiles/TuileBoss1.bmp", NULL);
    tuile->Boss2 = load_bitmap("Images/Tuiles/TuileBoss2.bmp", NULL);
    tuile->MurCassable = load_bitmap("Images/Tuiles/MurCassable.bmp", NULL);

    tuile->xBoss1 = 801;
    tuile->xBoss2 = 912;
    page->vieMurCassable = 5;


    page->plateau1=load_bitmap("Images/Niveau/DECOR2.bmp",NULL);
    page->plateau2 = load_bitmap("Images/Niveau/Decor3 VIDE.bmp", NULL);
    page->plateau3 = load_bitmap("Images/Niveau/DECOR2Collision.bmp",NULL);
    page->plateau4 = load_bitmap("Images/Niveau/Decor3 VIDECollision.bmp",NULL);
    page->fondBoss = load_bitmap("Images/Niveau/FOND.bmp", NULL);

    page->bossApproche = load_bitmap("Images/Niveau/LastStageAnnonce.bmp", NULL);
    page->explosion = load_bitmap("Images/Vaisseau/explosion.bmp", NULL);
}


//initialise toutes les bitmaps
void initialisationBitmaps(t_tuiles *tuile, t_bitmaps *page)
{
    char nomFichier1[256], nomFichier2[256];
    int i;


    page->regles = load_bitmap("Images/regles.bmp", NULL);
    page->stageClear = load_bitmap("Images/Niveau/StageClear.bmp", NULL);
    page->ecriture=load_pcx("4.pcx",NULL);
    page->writing=grab_font_from_bitmap(page->ecriture);
    if(!page->ecriture)
    {
        allegro_message("probleme police");
    }
    if(page->niveau==1)
    {
        page->plateau1 = load_bitmap("Images/Niveau/arriere_ville1.bmp", NULL);
        page->plateau2 = load_bitmap("Images/Niveau/villeav.bmp", NULL);
        page->plateau3 = load_bitmap("Images/Niveau/villeColli.bmp", NULL);
        page->bossApproche = load_bitmap("Images/Niveau/Stage1Annonce.bmp", NULL);
    }
    if(page->niveau==2)
    {
        tuile->Horizontale0 = load_bitmap("Images/Tuiles/Nuage0.bmp", NULL);
        tuile->Horizontale1 = load_bitmap("Images/Tuiles/Nuage1.bmp",NULL);
        tuile->Horizontale0Col = load_bitmap("Images/Tuiles/Nuage0Col.bmp", NULL);
        page->bossApproche = load_bitmap("Images/Niveau/Stage2Annonce.bmp", NULL);
        page->plateau1=load_bitmap("Images/Niveau/DecorNiveau2.bmp",NULL);
        page->tirCharge = load_bitmap("Images/Vaisseau/tirCharge2.bmp", NULL);
    }

    if(page->niveau == 3||page->niveau==4||page->niveau==6)
    {
        iniBitmapsNiveau3(page, tuile);
    }
    if(page->niveau==4)
        page->bossApproche = load_bitmap("Images/Niveau/BossApproche.bmp", NULL);


    page->crane = load_bitmap("Images/Game Over/Crane.bmp", NULL);
    page->tir = load_sample("Sons/Laser_Shoot2.wav");
    page->finNiveau = load_sample("Sons/FinNiveau.wav");
    page->mort = load_sample("Sons/Mort.wav");
    page->mute = 0;
    page->cpt = 0;
    page->decorMenu=load_bitmap("Images/Niveau/DecorMenu.bmp",NULL);
    page->buffer=create_bitmap(SCREEN_W,SCREEN_H);
    page->collision = create_bitmap(1800, SCREEN_H);    //peut etre a modifier, test pour apparition des ennemis
    clear_bitmap(page->collision);
    clear_bitmap(page->buffer);
    page->missile = load_bitmap("Images/Vaisseau/missile.bmp", NULL);
    if(page->niveau!=2)
        page->tirCharge = load_bitmap("Images/Vaisseau/tirCharge.bmp", NULL);
    page->jaugeDeTir = load_bitmap("Images/Vaisseau/Jaugedetir.bmp", NULL);

    for (i=0;i<3;i++)
    {
        sprintf(nomFichier1,"Images/Game Over/Game%d.bmp",i);
        sprintf(nomFichier2,"Images/Game Over/Over%d.bmp",i);

        page->game[i] = load_bitmap(nomFichier1,NULL);
        page->over[i] = load_bitmap(nomFichier2, NULL);
    }

    for(i=0; i<6; i++)
    {
        sprintf(nomFichier1, "Images/Explosions/%d.bmp", i);
        page->explo[i] = load_bitmap(nomFichier1, NULL);
    }

    page->titre = load_bitmap("Images/Menu/Titre0.bmp", NULL);
    page->fleche = load_bitmap("Images/Menu/Fleche0.bmp", NULL);
    page->screenActuel = 0;

    page->couleurTableauBord = makecol(0,0,255);
    page->retourMenu = 0;
}

//gère les tuiles du niveau 3
void tuilesNiveau3(t_bitmaps* page, t_tuiles* tuile)
{
    if(tuile->alea1>10)
    {
        draw_sprite(page->buffer,tuile->Horizontale0,1768-page->dxTuiles,282);
        draw_sprite(page->collision,tuile->Horizontale0Col,1768-page->dxTuiles,282);
    }
    if(tuile->alea1<59)
    {
        draw_sprite(page->buffer,tuile->Horizontale1,2162-page->dxTuiles,238);
        draw_sprite(page->collision,tuile->Horizontale1Col,2162-page->dxTuiles,238);
    }
    if(tuile->alea1-50>2)
    {
        draw_sprite(page->buffer,tuile->Horizontale0,3040-page->dxTuiles,326);
        draw_sprite(page->collision,tuile->Horizontale0Col,3040-page->dxTuiles,326);
    }
    if(tuile->alea1+13<78)
    {
        draw_sprite(page->buffer,tuile->Horizontale0,3358-page->dxTuiles,237);
        draw_sprite(page->collision,tuile->Horizontale0Col,3358-page->dxTuiles,237);
    }
    if(tuile->alea1+10>90)
    {
        draw_sprite(page->buffer,tuile->Horizontale2,3742-page->dxTuiles,149);
        draw_sprite(page->collision,tuile->Horizontale2Col,3742-page->dxTuiles,149);
    }
    if(tuile->alea1-5<50)
    {
        draw_sprite(page->buffer,tuile->Horizontale0,4577-page->dxTuiles,283);
        draw_sprite(page->collision,tuile->Horizontale0Col,4577-page->dxTuiles,283);
    }
    if(tuile->alea1+5>47)
    {
        draw_sprite(page->buffer,tuile->Sol1,4886-page->dxTuiles,327);
        draw_sprite(page->collision,tuile->Sol1Col,4886-page->dxTuiles,327);
    }
    if(tuile->alea1-20<6)
    {
        draw_sprite(page->buffer,tuile->Horizontale0,5389-page->dxTuiles,17);
        draw_sprite(page->collision,tuile->Horizontale0Col,5389-page->dxTuiles,17);
    }
    if(tuile->alea1+5>48)
    {
        draw_sprite(page->buffer,tuile->Horizontale0,5984-page->dxTuiles,193);
        draw_sprite(page->collision,tuile->Horizontale0Col,5984-page->dxTuiles,193);
    }
    if(tuile->alea1-5<68)
    {
        draw_sprite(page->buffer,tuile->Horizontale2,6378-page->dxTuiles,68);
        draw_sprite(page->collision,tuile->Horizontale2Col,6378-page->dxTuiles,68);
    }
    if(tuile->alea1+20>62)
    {
        draw_sprite(page->buffer,tuile->Sol0,6576-page->dxTuiles,370);
        draw_sprite(page->collision,tuile->Sol0Col,6576-page->dxTuiles,370);
    }
    if(tuile->alea1-10<15)
    {
        draw_sprite(page->buffer,tuile->Horizontale0,7257-page->dxTuiles,151);
        draw_sprite(page->collision,tuile->Horizontale0Col,7257-page->dxTuiles,151);
    }
    if(tuile->alea1>18)
    {
        draw_sprite(page->buffer,tuile->Horizontale0,7520-page->dxTuiles,239);
        draw_sprite(page->collision,tuile->Horizontale0Col,7520-page->dxTuiles,239);
    }

    if(page->vieMurCassable>0)
    {
        draw_sprite(page->buffer, tuile->MurCassable, 9179-page->dxTuiles, 63);
        rectfill(page->collision, 9179-page->dxTuiles, 63, 9179+80-page->dxTuiles,63+484, makecol(0,0,255));
    }
}

void tuilesNiveau2(t_bitmaps* page, t_tuiles* tuile)
{
    draw_sprite(page->buffer,tuile->Horizontale0,1000-page->dxTuiles,345);
    draw_sprite(page->collision,tuile->Horizontale0Col,1000-page->dxTuiles,345);

    draw_sprite(page->buffer,tuile->Horizontale0,2000-page->dxTuiles,230);
    draw_sprite(page->collision,tuile->Horizontale0Col,2000-page->dxTuiles,230);

    draw_sprite(page->buffer,tuile->Horizontale0,3000-page->dxTuiles,264);
    draw_sprite(page->collision,tuile->Horizontale0Col,3000-page->dxTuiles,264);

    draw_sprite(page->buffer,tuile->Horizontale0,4000-page->dxTuiles,300);
    draw_sprite(page->collision,tuile->Horizontale0Col,4000-page->dxTuiles,300);

    draw_sprite(page->buffer,tuile->Horizontale0,5000-page->dxTuiles,100);
    draw_sprite(page->collision,tuile->Horizontale0Col,5000-page->dxTuiles,100);

    draw_sprite(page->buffer,tuile->Horizontale0,6000-page->dxTuiles,345);
    draw_sprite(page->collision,tuile->Horizontale0Col,6000-page->dxTuiles,345);

    draw_sprite(page->buffer,tuile->Horizontale0,7000-page->dxTuiles,264);
    draw_sprite(page->collision,tuile->Horizontale0Col,7000-page->dxTuiles,264);

    draw_sprite(page->buffer,tuile->Horizontale0,8000-page->dxTuiles,312);
    draw_sprite(page->collision,tuile->Horizontale0Col,8000-page->dxTuiles,312);

    draw_sprite(page->buffer,tuile->Horizontale0,9000-page->dxTuiles,200);
    draw_sprite(page->collision,tuile->Horizontale0Col,9000-page->dxTuiles,200);

    draw_sprite(page->buffer,tuile->Horizontale0,10000-page->dxTuiles,230);
    draw_sprite(page->collision,tuile->Horizontale0Col,10000-page->dxTuiles,230);

    draw_sprite(page->buffer,tuile->Horizontale0,11000-page->dxTuiles,190);
    draw_sprite(page->collision,tuile->Horizontale0Col,11000-page->dxTuiles,190);

    draw_sprite(page->buffer,tuile->Horizontale0,11500-page->dxTuiles,140);
    draw_sprite(page->collision,tuile->Horizontale0Col,11500-page->dxTuiles,140);

    draw_sprite(page->buffer,tuile->Horizontale1,1500-page->dxTuiles,170);
    draw_sprite(page->collision,tuile->Horizontale0Col,1500-page->dxTuiles,170);

    draw_sprite(page->buffer,tuile->Horizontale1,2500-page->dxTuiles,312);
    draw_sprite(page->collision,tuile->Horizontale0Col,2500-page->dxTuiles,312);

    draw_sprite(page->buffer,tuile->Horizontale1,3500-page->dxTuiles,164);
    draw_sprite(page->collision,tuile->Horizontale0Col,3500-page->dxTuiles,164);

    draw_sprite(page->buffer,tuile->Horizontale1,4500-page->dxTuiles,190);
    draw_sprite(page->collision,tuile->Horizontale0Col,4500-page->dxTuiles,190);

    draw_sprite(page->buffer,tuile->Horizontale1,5500-page->dxTuiles,300);
    draw_sprite(page->collision,tuile->Horizontale0Col,5500-page->dxTuiles,300);

    draw_sprite(page->buffer,tuile->Horizontale1,6500-page->dxTuiles,150);
    draw_sprite(page->collision,tuile->Horizontale0Col,6500-page->dxTuiles,150);

    draw_sprite(page->buffer,tuile->Horizontale1,7500-page->dxTuiles,200);
    draw_sprite(page->collision,tuile->Horizontale0Col,7500-page->dxTuiles,200);

    draw_sprite(page->buffer,tuile->Horizontale1,8500-page->dxTuiles,190);
    draw_sprite(page->collision,tuile->Horizontale0Col,8500-page->dxTuiles,190);

    draw_sprite(page->buffer,tuile->Horizontale1,9500-page->dxTuiles,345);
    draw_sprite(page->collision,tuile->Horizontale0Col,9500-page->dxTuiles,345);

    draw_sprite(page->buffer,tuile->Horizontale1,10500-page->dxTuiles,300);
    draw_sprite(page->collision,tuile->Horizontale0Col,10500-page->dxTuiles,300);

    draw_sprite(page->buffer,tuile->Horizontale1,11500-page->dxTuiles,345);
    draw_sprite(page->collision,tuile->Horizontale0Col,11500-page->dxTuiles,345);
}

//systeme aléatoire d'appartion des tuiles, les valeurs choisie sont purement aléatoires et n'ont pas de sens particulier
void ajoutTuilesAlea(t_bitmaps* page, t_tuiles* tuile)
{
    page->dxTuiles+=page->vitesseScreen;

    if(page->niveau==3||page->niveau==6)
    {
        tuilesNiveau3(page, tuile);
    }
    if(page->niveau==2)
    {
        tuilesNiveau2(page, tuile);
    }

}

void screenNiveau3(t_bitmaps *page, t_tuiles *tuile)
{
    if(page->screenx1<page->plateau1->w&&page->screenx1!=-1)        //premier fond pas fini
    {
        page->screenx1+=page->vitesseScreen;        //on augmente le x du screen
        blit(page->plateau1,page->buffer,page->screenx1,0,0,0,SCREEN_W,SCREEN_H);
        if(SCREEN_W+310+page->bossApproche->w-page->screenx1>0)
            draw_sprite(page->buffer, page->bossApproche, SCREEN_W+310-page->screenx1, 250);
        blit(page->plateau3,page->collision,page->screenx1,0,0,0,SCREEN_W+1000,SCREEN_H);
    }

    if(page->screenx1>page->plateau1->w-SCREEN_W)   //premier fond fini
    {
        page->screenActuel=2;
        if(page->first1==0)                         //premiere fois que l'on entre dans cette boucle
        {
            page->dxTuiles = 0;
            tuile->alea1 = rand()%100;
            page->screenx2-=800;
            page->first1 = 1;              //permet de ne pas se teleporter au debut du deuxieme fond mais d'apparaitre doucement
        }
        if(page->first2==2)
            page->first1=1;

        blit(page->plateau2,page->buffer,page->screenx2,0,0,0,SCREEN_W,SCREEN_H);
        blit(page->plateau4,page->collision,page->screenx2,0,0,0,SCREEN_W,SCREEN_H);    //plateau des collisions
        page->screenx2+=page->vitesseScreen;

        if(page->screenx2>page->plateau2->w-SCREEN_W&&page->niveau==3)      //fin du premier ecran du dernier niveau
        {
            if(page->niveau==3)
            {
                page->screenx1 = -1;
                page->bossApproche = load_bitmap("Images/Niveau/BossApproche.bmp", NULL);
                page->niveau = 4;                                           //ici niveau4 représente le boss final
            }

            page->first2 = 0;
            page->first1 = 0;
        }

        if(page->screenx2>0&&page->screenActuel==2)
            ajoutTuilesAlea(page, tuile);                                   //tuiles aleatoires
    }
}

void screenFinNiveau3(t_bitmaps *page, t_tuiles *tuile, t_joueur *joueur, t_listeEnnemis *ennemis, t_listeActeurs *acteurs)
{
    int degatsSubisTemp = 0, scoreTemp = 0, cptDegats, cptVaisseau = 0, screenX = 0, scoreDegatsSubis = 0, scoreDegatsSubisTemp = 0, cptClignoteScore = 11, i;

    joueur->x = -joueur->tx;
    joueur->y = 200;
    joueur->imgcourante = 0;


    if(joueur->degatsSubis == 0)                //points supplémentaires en fonction des degats subis
        scoreDegatsSubis = 2000;
    else if(joueur->degatsSubis==1)
        scoreDegatsSubis = 1000;
    else if(joueur->degatsSubis==2)
        scoreDegatsSubis = 500;
    else
        scoreDegatsSubis = 0;

    if(joueur->score+scoreDegatsSubis>13100)
        page->note = load_bitmap("Images/Note/0.bmp", NULL);
    else if(joueur->score+scoreDegatsSubis>9000&&joueur->score+scoreDegatsSubis<13100)
        page->note = load_bitmap("Images/Note/1.bmp", NULL);
    else if(joueur->score+scoreDegatsSubis>6000&&joueur->score+scoreDegatsSubis<9000)
        page->note = load_bitmap("Images/Note/2.bmp", NULL);
    else
        page->note = load_bitmap("Images/Note/3.bmp", NULL);

    play_sample(page->finNiveau, 255, 128, 1000, 0);

    while(!key[KEY_SPACE]&&!key[KEY_ESC])
    {
        clear_bitmap(page->buffer);
        if(joueur->x+joueur->tx<SCREEN_W/2+50)  //le vaisseau se positionne au milieu de l'écran
            cptVaisseau++;
        if(cptVaisseau>10)
        {
            joueur->x+=5;
            cptVaisseau=0;
        }

        screenX+=1;
        cptClignoteScore++;                     //fait clignoter le score final

        if(screenX>page->plateau2->w-SCREEN_W)
            screenX = 0;

        blit(page->plateau2, page->buffer, screenX, 0,0,0,SCREEN_W, SCREEN_H);
        dessinerJoueur(joueur, page);
        if(scoreTemp>=joueur->score)
        {
            draw_sprite(page->buffer, page->note, 650, 185);
            if(cptClignoteScore>250)
            {
                textprintf_ex(page->buffer, font, 390, 475, makecol(255,255,255), -1, "%d",joueur->score+scoreDegatsSubis);
                if(cptClignoteScore>500)
                    cptClignoteScore=0;
            }

        }
        draw_sprite(page->buffer,page->stageClear,60,100);
        textprintf_ex(page->buffer, font, 420, 270, makecol(255,255,255), -1, "%d", scoreTemp);
        textprintf_ex(page->buffer, font, 420, 320, makecol(255,255,255), -1, "%d              +%d", degatsSubisTemp, scoreDegatsSubisTemp);


        blit(page->buffer, screen, 0,0,0,0,SCREEN_W, SCREEN_H);
        if(scoreDegatsSubisTemp<scoreDegatsSubis)               //effet visuel du score qui monte
            scoreDegatsSubisTemp+=10;
        if(scoreTemp<joueur->score)
            scoreTemp++;
        if(degatsSubisTemp<joueur->degatsSubis)
            cptDegats++;
        if(cptDegats>500)
        {
            cptDegats = 0;
            degatsSubisTemp++;
        }
    }
    if(key[KEY_SPACE])
    {
        page->niveau = 6;
        joueur->bouclier = 0;
        joueur->cptBouclier = 0;
        joueur->invincibilite = 0;
        joueur->tempsInvinc = 0;
        page->screenx1 = 300;
        page->screenx2 = 0;
        page->screenActuel = 0;
        joueur->x = 15;
        joueur->y = SCREEN_H/2;
        joueur->imgcourante = 0;
        for(i=0; i<ennemis->maximum; i++)
        {
            enleverEnnemis(ennemis, i);
        }
        for(i=0; i<acteurs->max; i++)
        {
            enleverActeur(acteurs, i);
        }
        joueur->score += scoreDegatsSubis;
    }

}

void screenFinNiveau1(t_bitmaps *page, t_tuiles *tuile, t_joueur *joueur)
{
    int degatsSubisTemp = 0, scoreTemp = 0, cptDegats, cptVaisseau = 0, screenX = 0, scoreDegatsSubis = 0, scoreDegatsSubisTemp = 0, cptClignoteScore = 11, attente =0;

    joueur->x = -joueur->tx;
    joueur->y = 200;
    joueur->imgcourante = 0;


    if(joueur->degatsSubis == 0)                //points supplémentaires en fonction des degats subis
        scoreDegatsSubis = 2000;
    else if(joueur->degatsSubis==1)
        scoreDegatsSubis = 1000;
    else if(joueur->degatsSubis==2)
        scoreDegatsSubis = 500;
    else
        scoreDegatsSubis = 0;

    if(joueur->score+scoreDegatsSubis>4000)
        page->note = load_bitmap("Images/Note/0.bmp", NULL);
    else if(joueur->score+scoreDegatsSubis>3000&&joueur->score+scoreDegatsSubis<4000)
        page->note = load_bitmap("Images/Note/1.bmp", NULL);
    else if(joueur->score+scoreDegatsSubis>1500&&joueur->score+scoreDegatsSubis<3000)
        page->note = load_bitmap("Images/Note/2.bmp", NULL);
    else
        page->note = load_bitmap("Images/Note/3.bmp", NULL);

    play_sample(page->finNiveau, 255, 128, 1000, 0);
    while(joueur->x<SCREEN_W&&!key[KEY_ESC])
    {
        clear_bitmap(page->buffer);
        if(key[KEY_SPACE])
            attente=1;
        if(joueur->x+joueur->tx<SCREEN_W/2+50&&attente==0)  //le vaisseau se positionne au milieu de l'écran
            cptVaisseau++;
        if(attente==1)
            cptVaisseau+=5;
        if(cptVaisseau>2)
        {
            joueur->x+=5;
            cptVaisseau=0;
        }

        screenX+=1;
        cptClignoteScore++;                     //fait clignoter le score final

        if(screenX>page->plateau1->w-SCREEN_W)
            screenX = 0;

        blit(page->plateau1, page->buffer, screenX, 0,0,0,SCREEN_W, SCREEN_H);

        if(scoreTemp>=joueur->score)
        {
            draw_sprite(page->buffer, page->note, 650, 185);
            if(cptClignoteScore>250)
            {
                textprintf_ex(page->buffer, font, 390, 475, makecol(255,255,255), -1, "%d",joueur->score+scoreDegatsSubis);
                if(cptClignoteScore>500)
                    cptClignoteScore=0;
            }

        }
        dessinerJoueur(joueur, page);
        draw_sprite(page->buffer,page->stageClear,60,100);
        textprintf_ex(page->buffer, font, 420, 270, makecol(255,255,255), -1, "%d", scoreTemp);
        textprintf_ex(page->buffer, font, 420, 320, makecol(255,255,255), -1, "%d              +%d", degatsSubisTemp, scoreDegatsSubisTemp);


        blit(page->buffer, screen, 0,0,0,0,SCREEN_W, SCREEN_H);
        if(scoreDegatsSubisTemp<scoreDegatsSubis)               //effet visuel du score qui monte
            scoreDegatsSubisTemp++;
        if(scoreTemp<joueur->score)
            scoreTemp+=10;
        if(degatsSubisTemp<joueur->degatsSubis)
            cptDegats++;
        if(cptDegats>500)
        {
            cptDegats = 0;
            degatsSubisTemp++;
        }
    }
    joueur->score += scoreDegatsSubis;

}

void screenFinNiveau2(t_bitmaps *page, t_tuiles *tuile, t_joueur *joueur)
{
    int degatsSubisTemp = 0, scoreTemp = 0, cptDegats, cptVaisseau = 0, screenX = 0, scoreDegatsSubis = 0, scoreDegatsSubisTemp = 0, cptClignoteScore = 11, attente =0;

    joueur->x = -joueur->tx;
    joueur->y = 200;
    joueur->imgcourante = 0;


    if(joueur->degatsSubis == 0)                //points supplémentaires en fonction des degats subis
        scoreDegatsSubis = 2000;
    else if(joueur->degatsSubis==1)
        scoreDegatsSubis = 1000;
    else if(joueur->degatsSubis==2)
        scoreDegatsSubis = 500;
    else
        scoreDegatsSubis = 0;

    if(joueur->score+scoreDegatsSubis>8400)
        page->note = load_bitmap("Images/Note/0.bmp", NULL);
    else if(joueur->score+scoreDegatsSubis>6500&&joueur->score+scoreDegatsSubis<8400)
        page->note = load_bitmap("Images/Note/1.bmp", NULL);
    else if(joueur->score+scoreDegatsSubis>3000&&joueur->score+scoreDegatsSubis<5000)
        page->note = load_bitmap("Images/Note/2.bmp", NULL);
    else
        page->note = load_bitmap("Images/Note/3.bmp", NULL);

    play_sample(page->finNiveau, 255, 128, 1000, 0);
    while(joueur->x<SCREEN_W&&!key[KEY_ESC])
    {
        clear_bitmap(page->buffer);
        if(key[KEY_SPACE])
            attente=1;
        if(joueur->x+joueur->tx<SCREEN_W/2+50&&attente==0)  //le vaisseau se positionne au milieu de l'écran
            cptVaisseau++;
        if(attente==1)
            cptVaisseau+=5;
        if(cptVaisseau>10)
        {
            joueur->x+=5;
            cptVaisseau=0;
        }

        screenX+=1;
        cptClignoteScore++;                     //fait clignoter le score final

        if(screenX>page->plateau1->w-SCREEN_W)
            screenX = 0;

        blit(page->plateau1, page->buffer, screenX, 0,0,0,SCREEN_W, SCREEN_H);

        if(scoreTemp>=joueur->score)
        {
            draw_sprite(page->buffer, page->note, 650, 185);
            if(cptClignoteScore>250)
            {
                textprintf_ex(page->buffer, font, 390, 475, makecol(255,255,255), -1, "%d",joueur->score+scoreDegatsSubis);
                if(cptClignoteScore>500)
                    cptClignoteScore=0;
            }

        }
        dessinerJoueur(joueur, page);
        draw_sprite(page->buffer,page->stageClear,60,100);
        textprintf_ex(page->buffer, font, 420, 270, makecol(255,255,255), -1, "%d", scoreTemp);
        textprintf_ex(page->buffer, font, 420, 320, makecol(255,255,255), -1, "%d              +%d", degatsSubisTemp, scoreDegatsSubisTemp);


        blit(page->buffer, screen, 0,0,0,0,SCREEN_W, SCREEN_H);
        if(scoreDegatsSubisTemp<scoreDegatsSubis)               //effet visuel du score qui monte
            scoreDegatsSubisTemp++;
        if(scoreTemp<joueur->score)
            scoreTemp+=10;
        if(degatsSubisTemp<joueur->degatsSubis)
            cptDegats++;
        if(cptDegats>500)
        {
            cptDegats = 0;
            degatsSubisTemp++;
        }
    }
    joueur->score += scoreDegatsSubis;

}

void screenNiveau2(t_bitmaps *page, t_tuiles *tuile, t_joueur *joueur, t_listeEnnemis *ennemis)
{
    int i;
    char nomfichier[256];
    if(page->screenx1==300)
    {
        page->dxTuiles = 0;
    }
    if(page->screenx1<page->plateau1->w)
    {
        page->screenx1+=page->vitesseScreen;

        blit(page->plateau1,page->buffer,page->screenx1,0,0,0,SCREEN_W,SCREEN_H);
        if(SCREEN_W+310+page->bossApproche->w-page->screenx1>0)
            draw_sprite(page->buffer, page->bossApproche, SCREEN_W+310-page->screenx1, 250);
        ajoutTuilesAlea(page, tuile);

    }

    if(page->screenx1>page->plateau1->w-SCREEN_W)
    {

        while(joueur->x<SCREEN_W)
        {
            joueur->x+=5;
            blit(page->plateau1,page->buffer,page->screenx1,0,0,0,SCREEN_W,SCREEN_H);
            draw_sprite(page->buffer,joueur->img[joueur->imgcourante],joueur->x,joueur->y);
            blit(page->buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            rest(10);
        }
        screenFinNiveau2(page, tuile, joueur);
        page->niveau=3;
        joueur->x = 15;
        joueur->y = SCREEN_H/2;

        for (i=0;i<3;i++)
        {
            sprintf(nomfichier,"Images/Vaisseau/Vaisseau%d.bmp",i);
            joueur->img[i] = load_bitmap(nomfichier,NULL);
            if (!joueur->img[i])
            {
                allegro_message("pas pu trouver %s",nomfichier);
                exit(EXIT_FAILURE);
            }
        }
        joueur->tx = joueur->img[0]->w;
        joueur->ty = joueur->img[0]->h;
        for(i=0; i<ennemis->maximum; i++)
        {
            enleverEnnemis(ennemis, i);
        }


        page->screenx1 = 300;
        initialisationBitmaps(tuile, page);

    }
}

void screenNiveau6(t_bitmaps *page, t_tuiles *tuile)
{
    if(page->screenx1<page->plateau1->w)        //premier fond pas fini
    {
        page->screenx1+=page->vitesseScreen;        //on augmente le x du screen
        blit(page->plateau1,page->buffer,page->screenx1,0,0,0,SCREEN_W,SCREEN_H);
        blit(page->plateau3,page->collision,page->screenx1,0,0,0,SCREEN_W+1000,SCREEN_H);
    }

    if(page->screenx1>page->plateau1->w-SCREEN_W)   //premier fond fini
    {
        if(page->first1==0)                         //premiere fois que l'on entre dans cette boucle
        {
            page->screenx2-=800;                //permet de ne pas se teleporter au debut du deuxieme fond mais d'apparaitre doucement
        }
        if(page->first1==0)
            page->first1 = 1;                   //on ne veut plus de cette apparition douce car le premier fond ne revient pas
        else if(page->first2==2)                //fin du deuxieme fond
            page->first1 = 2;

        blit(page->plateau2,page->buffer,page->screenx2,0,0,0,SCREEN_W,SCREEN_H);
        blit(page->plateau4,page->collision,page->screenx2,0,0,0,SCREEN_W,SCREEN_H);    //plateau des collisions
        page->screenx2+=page->vitesseScreen;
        if(page->screenx2>page->plateau2->w-SCREEN_W)
        {
            page->first2 = 1;
            page->first1 = 2;
        }
        if(page->first1==2)
        {
            if(page->first2==1)
            {
                page->screenx2=0;
                tuile->alea1 = rand()%100;
                page->first2 = 2;
                page->dxTuiles = 0;
            }

            ajoutTuilesAlea(page, tuile);       //tuiles aleatoires
        }

    }
}

void screenNiveau1(t_bitmaps *page, t_joueur *joueur, t_listeEnnemis *ennemis, t_tuiles *tuile)
{
    int i;
    char nomfichier[256];

    page->screenx2+=page->vitesseScreen;
    page->screenx1+=page->vitesseScreen/2;

    if(page->screenx1+SCREEN_W<page->plateau1->w)
        blit(page->plateau1,page->buffer,page->screenx1,0,0,0,SCREEN_W,SCREEN_H);
    else
    {
        blit(page->plateau1, page->buffer, page->screenx1, 0, 0, 0, page->plateau1->w-page->screenx1, SCREEN_H);
        blit(page->plateau1,page->buffer,0,0,page->plateau1->w-page->screenx1,0,SCREEN_W-(page->plateau1->w-page->screenx1),SCREEN_H);
    }
    if(SCREEN_W+page->bossApproche->w-page->screenx1>0&&page->screenActuel==0)
        draw_sprite(page->buffer, page->bossApproche, SCREEN_W-page->screenx1, 250);

    if(page->screenx2+SCREEN_W<page->plateau2->w)
    {
        masked_blit(page->plateau2,page->buffer,page->screenx2,0,0,SCREEN_H-(page->plateau2->h)-40,SCREEN_W,SCREEN_H);
        masked_blit(page->plateau3,page->collision,page->screenx2,0,0,SCREEN_H-(page->plateau3->h)-40,SCREEN_W,SCREEN_H);
    }
    else
    {
        masked_blit(page->plateau2,page->buffer,page->screenx2,0,0,SCREEN_H-(page->plateau2->h)-40,page->plateau2->w-page->screenx2,SCREEN_H);
        masked_blit(page->plateau3,page->collision,page->screenx2,0,0,SCREEN_H-(page->plateau3->h)-40,page->plateau3->w-page->screenx2,SCREEN_H);
        masked_blit(page->plateau2,page->buffer,0,0,page->plateau2->w-page->screenx2,SCREEN_H-(page->plateau2->h)-40,SCREEN_W-(page->plateau2->w-page->screenx2),SCREEN_H);
        masked_blit(page->plateau3,page->collision,0,0,page->plateau3->w-page->screenx2,SCREEN_H-(page->plateau3->h)-40,SCREEN_W-(page->plateau3->w-page->screenx2),SCREEN_H);
    }




    if(page->screenx1>page->plateau1->w-1)
    {
        page->screenx1 = 0;
        page->screenActuel++;
    }
    if(page->screenx2>page->plateau2->w-1)
    {
        page->screenx2 = 0;
    }
    if(page->screenActuel>1)
    {
        while(joueur->x<SCREEN_W)
        {
            joueur->x+=5;
            blit(page->plateau1,page->buffer,page->screenx1,0,0,0,SCREEN_W,SCREEN_H);
            masked_blit(page->plateau2, page->buffer, page->screenx2, 0, 0, SCREEN_H-(page->plateau2->h)-40, SCREEN_W, SCREEN_H);
            draw_sprite(page->buffer,joueur->img[joueur->imgcourante],joueur->x,joueur->y);
            blit(page->buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            rest(10);
        }
        screenFinNiveau1(page, tuile, joueur);
        page->niveau=2;
        joueur->x = 15;
        joueur->y = SCREEN_H/2;

        for (i=0;i<3;i++)
        {
            sprintf(nomfichier,"Images/Vaisseau/Avion%d.bmp",i);
            joueur->img[i] = load_bitmap(nomfichier,NULL);
            if (!joueur->img[i])
            {
                allegro_message("pas pu trouver %s",nomfichier);
                exit(EXIT_FAILURE);
            }
        }
        joueur->tx = joueur->img[0]->w;
        joueur->ty = joueur->img[0]->h;

        for(i=0; i<ennemis->maximum; i++)
        {
            enleverEnnemis(ennemis, i);
        }


        page->screenx1 = 300;
        page->screenx2 = 0;
        page->screenActuel = 0;
        initialisationBitmaps(tuile, page);
    }
}

//affiche le screen a chaque boucle
void screenActualisation(t_bitmaps* page, t_tuiles* tuile, t_listeEnnemis *ennemis, t_joueur *joueur, t_listeActeurs *acteurs)
{
    if(page->niveau==6)
        screenNiveau6(page, tuile);
    if(page->niveau==5)                             //apres le boss, ecran de fin de stage
        screenFinNiveau3(page, tuile, joueur, ennemis, acteurs);
    if(page->niveau==4)
    {
        page->screenActuel = 3;
        bossNiveau3(page, tuile, ennemis);          //boss final
    }
    if(page->niveau==3)
        screenNiveau3(page, tuile);
    if(page->niveau==2)
        screenNiveau2(page, tuile, joueur, ennemis);
    if(page->niveau==1)
        screenNiveau1(page, joueur, ennemis, tuile);
}

//boss final
void bossNiveau3(t_bitmaps *page, t_tuiles *tuile, t_listeEnnemis *ennemis)
{
    blit(page->fondBoss,page->buffer,page->screenx2,0,0,0,SCREEN_W,SCREEN_H);
    blit(page->fondBoss, page->collision, 0,0,0,0,SCREEN_H, SCREEN_W);
    if(page->first1==0)
    {
        tuile->xBoss1-=page->vitesseScreen+5;
        draw_sprite(page->buffer, page->bossApproche, tuile->xBoss1, 200);      //annonce du boss qui traverse l'ecran
    }

    page->cpt++;

    if(page->cpt>275&&page->first1==0)
    {
        tuile->xBoss1 = 801;
        page->first1=1;
    }

    if(page->first1==1&&page->cpt>300)
    {
        ajouterBoss(ennemis, page);
        page->first1 = 2;
    }
    if(page->cpt>275)                                                       //tuiles arène du boss qui s'arrete avec le fond qui bouge
    {
        tuile->xBoss1-=page->vitesseScreen;
        tuile->xBoss2-=page->vitesseScreen;
        if(tuile->xBoss1<-124)
            tuile->xBoss1=-124;
        if(tuile->xBoss2<-10)
            tuile->xBoss2 = -10;
        draw_sprite(page->buffer, tuile->Boss1, tuile->xBoss1, 0);
        rectfill(page->collision, tuile->xBoss1, 0, tuile->Boss1->w, tuile->Boss1->h, makecol(255,255,255));
        draw_sprite(page->buffer, tuile->Boss2, tuile->xBoss2, 545);
        rectfill(page->collision, tuile->xBoss2, 545, tuile->Boss2->w, 545+tuile->Boss2->h, makecol(255,255,255));

    }

    page->screenx2+=page->vitesseScreen;
    if(page->screenx2>page->fondBoss->w-SCREEN_W)
        page->screenx2 = 0;
}


//ssprg a enlever a la fin, permet d'afficher le buffer de collision, mais aussi de gerer l'apparition des ennemis et d'éviter qu'ils apparaissent dans le decor
void affichageEtTestcollision(t_bitmaps *page, int *test, t_listeEnnemis *ennemis)
{
    int i;

    if(key[KEY_C])
        *test=1;
    if(key[KEY_V])
        *test=2;
    if(key[KEY_6_PAD])                          //fais monter ou descendre la vitesse du screen
        page->vitesseScreen++;
    if(key[KEY_4_PAD])
    {
        if(page->vitesseScreen>1)
            page->vitesseScreen--;
    }


    if(*test==1)                                        //voir les collisions
    {
           blit(page->buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }
    if(*test==2)
    {
        blit(page->collision,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }

    for(i=0; i<ennemis->maximum; i++)       //blindage apparition ennemis
    {
        if(ennemis->tab[i]!=NULL&&ennemis->tab[i]->type!=5&&ennemis->tab[i]->type!=2&&page->niveau!=1)
        {
            if(getpixel(page->collision, ennemis->tab[i]->x, ennemis->tab[i]->y)==makecol(255,255,255))
                ennemis->tab[i]->vivant = 0;
            if(getpixel(page->collision, ennemis->tab[i]->x+ennemis->tab[i]->tx,ennemis->tab[i]->y)==makecol(255,255,255))
                ennemis->tab[i]->vivant = 0;
            if(getpixel(page->collision, ennemis->tab[i]->x+ennemis->tab[i]->tx, ennemis->tab[i]->y+ennemis->tab[i]->ty)==makecol(255,255,255))
                ennemis->tab[i]->vivant = 0;
            if(getpixel(page->collision, ennemis->tab[i]->x, ennemis->tab[i]->y+ennemis->tab[i]->ty)==makecol(255,255,255))
                ennemis->tab[i]->vivant = 0;
        }
    }
}


//gere les collisions du joueur quand on appuie pas sur des touches
void gererCollisions(t_bitmaps *page, t_joueur *joueur)
{
    int i;

    if(joueur->invincibilite!=1&&page->niveau!=1)
    {
        for(i=joueur->y; i<joueur->y+joueur->ty; i++)
        {
            if(getpixel(page->collision, joueur->x+joueur->tx, i)==makecol(255,255,255))    //collisions passives du joueur
                joueur->x-=page->vitesseScreen;
        }
    }


    if(joueur->x+joueur->tx<0)      //si un obstacle pousse le joueur en dehors de l'ecran, perd une vie
    {
        if(joueur->bouclier==1)
            joueur->bouclier = 0;
        else
        {
            joueur->degatsSubis++;
            joueur->vieActuelle--;
        }
        joueur->invincibilite = 1;
        for(i=0; i<800; i++)
        {
            if(getpixel(page->collision, i, joueur->y)!=makecol(255,255,255)&&getpixel(page->collision, i, joueur->y)!=makecol(0,0,255))
            {
                joueur->x = i;
                i=800;
            }
        }
    }
}

//affiche la partie basse de l'écran avec les vies etc a chaque boucle
void dessinerTableauBord(t_bitmaps *page, t_joueur *joueur)
{




    rectfill(page->buffer, 0, 560, SCREEN_W-1, SCREEN_H-1, makecol(0,0,0));
    rect(page->buffer, 0, 560, SCREEN_W-1, SCREEN_H-1, makecol(255,0,0));

    if(joueur->vieActuelle>0)       //affiche les coeurs en bas en fonction de la vie du joueur
    {
        draw_sprite(page->buffer, joueur->vie, 15, 560);
        if(joueur->vieActuelle>1)
        {
            draw_sprite(page->buffer, joueur->vie, 30, 560);
            if(joueur->vieActuelle>2)
            {
                draw_sprite(page->buffer, joueur->vie, 45, 560);
            }
        }
    }
    if(joueur->bouclier==1)
        draw_sprite(page->buffer, joueur->btmBouc, 80, 562);
    if(joueur->scoreTemporaire<joueur->score)
        joueur->scoreTemporaire++;
    textprintf_ex(page->buffer, page->writing, 510, 570, makecol(255,255,255), -1, "SCORE : %d", joueur->scoreTemporaire);
    textprintf_ex(page->buffer, page->writing, 625,570,makecol(255,255,255), -1, "TEMPS: %dmin%ds", page->minutes,page->temps);
    draw_sprite(page->buffer, page->jaugeDeTir, SCREEN_W/3, 557); //jauge de tir statique


}


/***********************************************************************************************************************************************************************************************************/

/*                                                                                       ENNEMIS                                                                                                     */

/***********************************************************************************************************************************************************************************************************/





t_listeEnnemis *creerListeEnnemis(int maxacteurs)
{
    t_listeEnnemis *nouv;
    int i;

    nouv=(t_listeEnnemis*)malloc(1*sizeof(t_listeEnnemis));

    nouv->maximum=maxacteurs;
    nouv->n=0;
    nouv->tab=(t_ennemi**)malloc(maxacteurs*sizeof(t_ennemi*));

    for (i=0;i<maxacteurs;i++)
        nouv->tab[i]=NULL;

    return nouv;
}

void actualiserListeEnnemis(t_listeEnnemis *ennemis, t_bitmaps* page)
{
    int i;

    // actualiser chaque acteur
    // si un acteur n'est plus vivant, l'enlever de la liste
    for (i=0;i<ennemis->maximum;i++)
        if (ennemis->tab[i]!=NULL)
        {
            actualiserEnnemi(ennemis->tab[i], page);
            if (!ennemis->tab[i]->vivant)
            {
                enleverEnnemis(ennemis,i);
            }
        }
}


void actualiserEnnemi(t_ennemi *ennemi, t_bitmaps* page)
{
    ennemi->precx = ennemi->x;
    ennemi->precy = ennemi->y;
    // deplacement
    ennemi->x=ennemi->x-ennemi->dx;
    ennemi->y=ennemi->y+ennemi->dy;

    if(ennemi->type==2&&page->niveau!=2)
    {
        ennemi->cpt++;
        if(ennemi->cpt>5)
        {
            ennemi->cpt = 0;
            ennemi->imgcourante++;
            if(ennemi->imgcourante>5&&(page->niveau==1||page->niveau==3||page->niveau==6))
                ennemi->imgcourante =0;
        }
    }
    if(ennemi->type==1)
    {
        if(getpixel(page->collision, ennemi->x, ennemi->y+ennemi->dy+ennemi->ty)==makecol(255,255,255)||ennemi->y+ennemi->ty>560||ennemi->y<0||getpixel(page->collision, ennemi->x, ennemi->y+ennemi->dy)==makecol(255,255,255)||getpixel(page->collision, ennemi->x+ennemi->tx, ennemi->y+ennemi->dy)==makecol(255,255,255)||getpixel(page->collision, ennemi->x+ennemi->tx, ennemi->y+ennemi->ty+ennemi->dy)==makecol(255,255,255))
        {
            if(page->niveau!=2)
            {
                if(ennemi->dy>0)
                    ennemi->imgcourante= 0;
                else
                    ennemi->imgcourante = 1;
            }
            ennemi->dy = -ennemi->dy;
            ennemi->y = ennemi->precy + ennemi->dy;
        }
        if(page->niveau==2)
        {
            ennemi->cpt++;
            if(ennemi->cpt>5)
            {
                ennemi->cpt = 0;
                ennemi->imgcourante++;
                if(ennemi->imgcourante>2)
                    ennemi->imgcourante = 0;
            }
        }
    }

    if(ennemi->type==5)
    {
        if(ennemi->x<600)
            ennemi->dx = 0;
    }



    // type missile : accélère en dx mais pas au dela d'une limite


    // si dépasse le bord alors disparait
    if (ennemi->x+ennemi->tx<-10 || ennemi->y+ennemi->ty<0 || ennemi->y>560)
        ennemi->vivant=0;
    if(ennemi->x>800)
        ennemi->vivant=1;
}

void enleverEnnemis(t_listeEnnemis *ennemis,int i)
{

    // Vérifier qu'il y a bien un acteur accroché en indice i
    if (ennemis->tab[i]!=NULL)
    {
        // libérer la mémoire du acteur
        free(ennemis->tab[i]);

        // marquer l'emplacement comme libre
        ennemis->tab[i]=NULL;
        ennemis->n--;

    }
}


t_ennemi *ajouterEnnemis(t_listeEnnemis *ennemis, t_bitmaps *page)
{
    int i,type;
    t_ennemi *ennemi;

    type = rand()%3;
    if((type==1||type==0)&&page->niveau==1)
        type = 2;

    page->cptEnnemi++;
    if(page->cptEnnemi>=100)
    {
        page->cptEnnemi = 0;

                    // Liste pleine, on alloue rien et on retourne NULL...
            if (ennemis->n >= ennemis->maximum)
                return NULL;

                // Allouer un acteur initialisé
            ennemi=creerEnnemi(type, page);




                // Chercher un emplacement libre
            i=0;
            while (ennemis->tab[i]!=NULL && i<ennemis->maximum)
                i++;
                // Si on a trouvé ...
                // (normalement oui car on a vérifié n<max)
            if (i<ennemis->maximum){
                    // Accrocher le acteur à l'emplacement trouvé
                ennemis->tab[i]=ennemi;
                ennemis->n++;
            }
                // Sinon c'est qu'il y a un problème de cohérence
            else
                allegro_message("Anomalie gestion ajouterActeur : liste corrompue");


    }



    return ennemi;
}

t_ennemi *creerEnnemi(int type, t_bitmaps *page)
{
    int i, alea;
    char nomfichier[256];
    t_ennemi *nouv;

    // Allouer
    nouv=(t_ennemi*)malloc(1*sizeof(t_ennemi));
    alea=rand()%2;





    // ici ce qui est spécifique aux types
    switch (type)
    {
        //ennemi fixe
        case 0:
            nouv->x=799;
            if(page->niveau==1)
            {
                for (i=0; i<3; i++)
                {
                    sprintf(nomfichier,"Images/Ennemi/Fixe/Tir%d.bmp",i);

                    nouv->img[i]= load_bitmap(nomfichier,NULL);
                    if (!nouv->img[i])
                    {
                        allegro_message("pas pu trouver %s",nomfichier);
                        exit(EXIT_FAILURE);
                    }
                }
                nouv->tx = 64;
                nouv->ty = 57;
                nouv->y = 400;
            }
            if(page->niveau!=2&&page->niveau!=1)
            {
                for(i=0; i<3; i++)
                {
                    sprintf(nomfichier,"Images/Ennemi/Fixe/Fixe%d.bmp",i);
                    nouv->img[i] = load_bitmap(nomfichier,NULL);
                    if (!nouv->img[i])
                    {
                        allegro_message("pas pu trouver %s",nomfichier);
                        exit(EXIT_FAILURE);
                    }
                }
                nouv->tx = nouv->img[0]->w;
                nouv->ty = nouv->img[0]->h;
                for(i=SCREEN_H/2; i<560; i++)
                {
                    if(getpixel(page->collision, nouv->x, i)==makecol(255,255,255))
                    {
                        nouv->y = i-1-nouv->ty;
                        i=560;
                    }
                    if(i==559)
                        nouv->y = 559-nouv->ty;
                }
            }
            if(page->niveau==2)
            {
                nouv->img[0] = load_bitmap("Images/Ennemi/Fixe/FixeLvl2.bmp", NULL);
                nouv->tx = nouv->img[0]->w;
                nouv->ty = nouv->img[0]->h;
                nouv->y = 560-nouv->ty;
            }

            nouv->dx=page->vitesseScreen;
            nouv->dy=0;


            nouv->imgcourante = 0;
            nouv->type=type;
            nouv->vivant=1;
            nouv->vie = 5;
            nouv->cpttir = 0;
            break;

        // ennemi mobile
        case 1:
            if(page->niveau!=2)
            {
                if(alea==0)
                {
                    for(i=0; i<2; i++)
                    {
                        sprintf(nomfichier,"Images/Ennemi/Mobile/Mobile%d.bmp",i);
                        nouv->img[i] = load_bitmap(nomfichier,NULL);
                        if (!nouv->img[i])
                        {
                            allegro_message("pas pu trouver %s",nomfichier);
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                if(alea==1)
                {
                    for(i=0; i<2; i++)
                    {
                        sprintf(nomfichier,"Images/Ennemi/Mobile/Mobile%d.bmp",i+2);
                        nouv->img[i] = load_bitmap(nomfichier,NULL);
                        if (!nouv->img[i])
                        {
                            allegro_message("pas pu trouver %s",nomfichier);
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            }
            if(page->niveau==2)
            {
                for(i=0; i<3; i++)
                {
                    sprintf(nomfichier,"Images/Ennemi/Mobile/Helicoptere%d.bmp",i);
                    nouv->img[i] = load_bitmap(nomfichier,NULL);
                    if (!nouv->img[i])
                    {
                        allegro_message("pas pu trouver %s",nomfichier);
                        exit(EXIT_FAILURE);
                    }
                }
            }

            nouv->tx = nouv->img[0]->w;
            nouv->ty = nouv->img[0]->h;
            nouv->dx=page->vitesseScreen;
            nouv->dy=1;
            nouv->x=799;
            nouv->y=400;
            nouv->type=type;
            nouv->vivant=1;
            nouv->vie = 5;
            nouv->cpttir = 0;
            nouv->imgcourante = 0;
            nouv->cpt = 0;
            break;
        //petit ennemi volant
        case 2:
            if(page->niveau==1)
            {
                for(i=0; i<6; i++)
                {
                    sprintf(nomfichier,"Images/Ennemi/Volant/truc%d.bmp",i);
                    nouv->img[i] = load_bitmap(nomfichier,NULL);
                    if (!nouv->img[i])
                    {
                        allegro_message("pas pu trouver %s",nomfichier);
                        exit(EXIT_FAILURE);
                    }
                }
            }
            if(page->niveau!=2&&page->niveau!=1)
            {
                for(i=0; i<6; i++)
                {
                    sprintf(nomfichier,"Images/Ennemi/Volant/Volant%d.bmp",i);
                    nouv->img[i] = load_bitmap(nomfichier,NULL);
                    if (!nouv->img[i])
                    {
                        allegro_message("pas pu trouver %s",nomfichier);
                        exit(EXIT_FAILURE);
                    }
                }
            }
            if(page->niveau==2)
                nouv->img[0] = load_bitmap("Images/Ennemi/Volant/VolantLvl2.bmp", NULL);

            nouv->tx = 38;
            nouv->ty = 40;
            nouv->dx=page->vitesseScreen+3;
            nouv->dy=0;
            nouv->x=799;
            nouv->y = rand()%491+50;
            nouv->imgcourante = 0;
            nouv->type=type;
            nouv->vivant=1;
            nouv->vie = 2;
            nouv->cpttir = 0;
            nouv->cpt = 0;
            break;
        case 5:
            nouv->img[0] = load_bitmap("Images/Ennemi/Boss/Boss.bmp", NULL);
            for(i=0; i<8; i++)
            {
                sprintf(nomfichier,"Images/Ennemi/Boss/%d.bmp",i);
                nouv->oeil[i] = load_bitmap(nomfichier,NULL);
                if (!nouv->oeil[i])
                {
                    allegro_message("pas pu trouver %s",nomfichier);
                    exit(EXIT_FAILURE);
                }
            }
            nouv->imgoeil = 0;
            nouv->cpt = 0;
            nouv->tx = nouv->img[0]->w;
            nouv->ty = nouv->img[0]->h;
            nouv->dx=page->vitesseScreen;
            nouv->dy=0;
            nouv->x=1450;
            nouv->y=57;
            nouv->type=type;
            nouv->vivant=1;
            nouv->vie = 500;
            nouv->cpttir = 0;
            nouv->precx =0;
            nouv->imgcourante = 0;
            break;
    }

    return nouv;
}


void dessinerListeEnnemis(t_bitmaps *page,t_listeEnnemis *ennemis, t_joueur *joueur)
{
    int i;

    for (i=0;i<ennemis->maximum;i++)
    {
        if (ennemis->tab[i]!=NULL)
        {
            if(ennemis->tab[i]->type==0&&page->niveau!=2&&page->niveau!=6)
            {
                if(ennemis->tab[i]->x>joueur->x+joueur->tx)
                    ennemis->tab[i]->imgcourante = 0;
                else if(ennemis->tab[i]->x>joueur->x&&ennemis->tab[i]->x<joueur->x+joueur->tx)
                    ennemis->tab[i]->imgcourante = 1;
                else
                    ennemis->tab[i]->imgcourante = 2;
            }


            if(ennemis->tab[i]->type==5)
            {
                ennemis->tab[i]->alea = rand()%5;
                ennemis->tab[i]->cpt++;
                if(ennemis->tab[i]->vie>250)
                {
                    if(ennemis->tab[i]->cpt>100&&ennemis->tab[i]->alea==0)
                    {
                        ennemis->tab[i]->imgoeil++;
                        if(ennemis->tab[i]->imgoeil>3)
                            ennemis->tab[i]->imgoeil = 2;

                        ennemis->tab[i]->cpt = 0;
                    }
                }
                else if(ennemis->tab[i]->vie>0&&ennemis->tab[i]->vie<=250)
                {
                    if(ennemis->tab[i]->precx==0)
                    {
                        ennemis->tab[i]->imgoeil = 4;
                        ennemis->tab[i]->precx = -1;
                    }
                    if(ennemis->tab[i]->cpt>100&&ennemis->tab[i]->alea==0)
                    {
                        ennemis->tab[i]->imgoeil++;
                        if(ennemis->tab[i]->imgoeil>7)
                            ennemis->tab[i]->imgoeil = 6;
                        ennemis->tab[i]->cpt = 0;
                    }
                }
                else
                    ennemis->tab[i]->imgoeil = 0;


            }

            draw_sprite(page->buffer, ennemis->tab[i]->img[ennemis->tab[i]->imgcourante], ennemis->tab[i]->x, ennemis->tab[i]->y);
            if(ennemis->tab[i]->type==5)
                draw_sprite(page->buffer, ennemis->tab[i]->oeil[ennemis->tab[i]->imgoeil], ennemis->tab[i]->x+65, ennemis->tab[i]->y+227);
        }

    }
}


/***********************************************************************************************************************************************************************************************************/

/*                                                                                       TOUCHER UN ENNEMI                                                                                                     */

/***********************************************************************************************************************************************************************************************************/


void destinActeur(t_acteur *acteur, t_ennemi *ennemi, t_joueur *joueur, int action, t_bitmaps *page, t_tuiles *tuile, t_listeExplosions *explosions, t_listeObjets *objets)
{
    if(acteur->type!=3)
    {
        acteur->dx/=2;
        acteur->dy/=2;
        acteur->comportement=1;
        acteur->cptexplo=0;
    }


    if(action == 1)
    {
        if(acteur->type==0)                         //degats en fonction du tir donc missile tir charger etc
        {
            if(joueur->dmgUp==0)
                ennemi->vie-=1;
            else
                ennemi->vie-=5;
        }
        else if(acteur->type==1)
        {
            if(joueur->dmgUp==0)
                ennemi->vie-=4;
            else
                ennemi->vie-=5;
        }
        else if(acteur->type==2)
        {
            if(joueur->dmgUp==0)
                ennemi->vie-=20;
            else
                ennemi->vie-=100;
        }

        if(ennemi->vie<=0)
        {
            if(ennemi->type==5)                             //si il s'agit du boss, il se passe des choses differentes d'un ennemi normal
                mortBoss(page, joueur, tuile);
            if(ennemi->type==0)
                joueur->score+=200;
            if(ennemi->type==1)
                joueur->score+=100;
            if(ennemi->type==2)
                joueur->score+=50;

            ennemi->vivant = 0;
            if(ennemi->type!=5)
            {
                if(page->mute==0)
                    play_sample(page->mort, 50, 128, 1000, 0);
                ajouterExplosion(explosions, ennemi->x, ennemi->y, page);
                ajouterObjet(objets, ennemi->x, ennemi->y, page);
            }

        }
    }
    if(action==2)                                       //si il s'agit d'un tir ennemi sur le joueur perd une viene
    {
        if(joueur->bouclier==1)
            joueur->bouclier=0;
        else
        {
            joueur->vieActuelle--;
            joueur->degatsSubis++;
        }
        joueur->invincibilite = 1;
    }

}

void collisionActeur(t_ennemi *ennemi ,t_acteur *acteur, t_joueur *joueur, t_bitmaps *page, t_tuiles *tuile, t_listeExplosions *explosions, t_listeObjets *objets)
{
    int vx,vy,d2, vx2, vy2, d3;

    // si il n'explose déjà pas !
    if ( acteur->comportement==0 )
    {
        // si il est dans la cible alors appeler destinActeur (explosion)

        // calcul du vecteur entre acteur et centre cible
        vx = acteur->x+acteur->tx/2 - (ennemi->x+ennemi->tx/2);
        vy = acteur->y+acteur->ty/2 - (ennemi->y+ennemi->ty/2);

        vx2 = acteur->x+acteur->tx/2 - (joueur->x+joueur->tx/2);
        vy2 = acteur->y+acteur->ty/2 - (joueur->y+joueur->ty/2);

        // calcul distance au carré au centre de la cible (Pythagore)
        // (on reste sur le carré pour éviter de calculer la racine)
        d2 = vx*vx + vy*vy;
        d3 = vx2*vx2 + vy2*vy2;

        // si dans le disque alors destin...
        if ( d2 < ennemi->tx * ennemi->tx / 4 )
        {
            destinActeur(acteur, ennemi, joueur, 1, page, tuile, explosions, objets);
        }

        if(d3<joueur->tx*joueur->tx/4&&joueur->invincibilite!=1)
        {
            destinActeur(acteur, ennemi, joueur, 2, page, tuile, explosions, objets);
        }

    }

}


void collisionListeActeurs(t_listeEnnemis *ennemis,t_listeActeurs *acteurs, t_joueur *joueur, t_bitmaps *page, t_tuiles *tuile, t_listeExplosions *explosions, t_listeObjets *objets)
{

    int i, j;

    // regarder pour chaque acteur...
    for (i=0;i<acteurs->max;i++)
    {
        for(j=0; j<ennemis->maximum; j++)
        {
            if (acteurs->tab[i]!=NULL&&ennemis->tab[j]!=NULL)
            {
                collisionActeur(ennemis->tab[j],acteurs->tab[i], joueur, page, tuile, explosions, objets);
            }

        }
    }

    for(i=0; i<ennemis->maximum; i++)
    {
        if(ennemis->tab[i]!=NULL)
        {
            if((ennemis->tab[i]->x<joueur->tx+joueur->x&&ennemis->tab[i]->x>joueur->x)&&joueur->invincibilite!=1)
            {
                for(j=ennemis->tab[i]->y; j<ennemis->tab[i]->y+ennemis->tab[i]->ty; j++)
                {
                    if(j>joueur->y&&j<joueur->y+joueur->ty)
                    {
                        joueur->invincibilite = 1;
                        j = ennemis->tab[i]->y+ennemis->tab[i]->ty;
                        if(joueur->bouclier==1)
                            joueur->bouclier = 0;
                        else
                            joueur->vieActuelle--;
                    }
                }

            }
        }
    }

    for(i=0; i<objets->maximum; i++)
    {
        if(objets->tab[i]!=NULL)
        {
            if((objets->tab[i]->x<joueur->tx+joueur->x&&objets->tab[i]->x>joueur->x))//&&objets->tab[i]->y<joueur->y+joueur->ty&&objets->tab[i]->y>joueur->y)||(objets->tab[i]->x<joueur->tx+joueur->x&&objets->tab[i]->x>joueur->x&&objets->tab[i]->y+objets->tab[i]->ty<joueur->y+joueur->ty&&objets->tab[i]->y+objets->tab[i]->ty>joueur->y))
            {
                for(j=objets->tab[i]->y; j<objets->tab[i]->y+objets->tab[i]->ty; j++)
                {
                    if(j>joueur->y&&j<joueur->y+joueur->ty)
                    {
                        objets->tab[i]->vivant = 0;
                        j = objets->tab[i]->y+objets->tab[i]->ty;
                        if(objets->tab[i]->type==0)
                        {
                            if(page->mute==0)
                                play_sample(joueur->vieSon, 255, 128, 1000, 0);
                            if(joueur->vieActuelle<3)
                                joueur->vieActuelle++;
                        }
                        if(objets->tab[i]->type == 1)
                        {
                            if(joueur->dmgUp==1)
                                joueur->tempsDmgUp = 0;
                            else
                                joueur->dmgUp = 1;
                        }
                        if(objets->tab[i]->type==2)
                        {
                                joueur->bouclier = 1;
                        }
                    }
                }
            }
        }
    }


    for(i=0; i<acteurs->max; i++)
    {
        if (acteurs->tab[i]!=NULL)
        {
            if((getpixel(page->collision, acteurs->tab[i]->x, acteurs->tab[i]->y)==makecol(255,255,255)||getpixel(page->collision, acteurs->tab[i]->x+acteurs->tab[i]->tx, acteurs->tab[i]->y)==makecol(255,255,255))&&page->niveau!=1)
                acteurs->tab[i]->vivant = 0;
            if(getpixel(page->collision, acteurs->tab[i]->x+acteurs->tab[i]->tx, acteurs->tab[i]->y)==makecol(0,0,255))
            {
                acteurs->tab[i]->vivant = 0;
                page->vieMurCassable--;
            }

        }
    }


}



/***********************************************************************************************************************************************************************************************************/

/*                                                                                       TIRS ENNEMIS                                                                                                     */

/***********************************************************************************************************************************************************************************************************/

//permet le tir aleatoire des ennemis de temps en temps
void tirEnnemis(t_listeEnnemis *ennemis, t_bitmaps *page, t_listeActeurs *acteurs, t_joueur *joueur)
{
    int i, alea1, alea2, tirBoss1;

    alea1 = rand()%100;
    alea2 = rand()%100;
    tirBoss1 = rand()%10;


    // regarder pour chaque acteur...
    for (i=0;i<ennemis->maximum;i++)
    {
        if (ennemis->tab[i]!=NULL&&ennemis->tab[i]->type!=2)
        {
            ennemis->tab[i]->cpttir++;
            if(ennemis->tab[i]->cpttir>=50)
            {
                if(alea1>alea2)
                {
                    if(ennemis->tab[i]->type==5&&tirBoss1==0)
                        ajouterActeur(acteurs,ennemis->tab[i]->x,ennemis->tab[i]->y+ennemis->tab[i]->ty/2,4, joueur, page);
                    else
                        ajouterActeur(acteurs,ennemis->tab[i]->x,ennemis->tab[i]->y+ennemis->tab[i]->ty/2,3, joueur, page);
                    ennemis->tab[i]->cpttir=0;
                }

            }
        }
    }
}

//Tirs avec direction correcte pour ennemis fixes niveau 1
void tirFixes(t_listeEnnemis*ennemis, t_bitmaps *page, t_listeActeurs *acteurs, t_joueur *joueur) ///*****************MODIF LAURE***********************///
{
    int i, alea1, alea2;

    alea1 = rand()%10;
    alea2 = rand()%10;

    // regarder pour chaque acteur...
    for (i=0;i<3;i++)
    {
        if ((ennemis->tab[i]!=NULL)&&(ennemis->tab[i]->type==0))
        {
            ennemis->tab[i]->cpttir++;

            if(ennemis->tab[i]->cpttir>=50)
            {
                if(alea1>alea2)
                {
                    if(ennemis->tab[i]->imgcourante==0)
                        ajouterActeur(acteurs,ennemis->tab[i]->x,ennemis->tab[i]->y,5, joueur,page);
                    if(ennemis->tab[i]->imgcourante==1)
                        ajouterActeur(acteurs,ennemis->tab[i]->x-ennemis->tab[i]->tx/2,ennemis->tab[i]->y,6, joueur,page);
                    if(ennemis->tab[i]->imgcourante==2)
                        ajouterActeur(acteurs,ennemis->tab[i]->x+ennemis->tab[i]->tx/2,ennemis->tab[i]->y,7, joueur,page);

                    ennemis->tab[i]->cpttir=0;
                }

            }
       }

    }
}

/***********************************************************************************************************************************************************************************************************/

/*                                                                                       GAME OVER                                                                                                     */

/***********************************************************************************************************************************************************************************************************/

//pas encore realise, permettra d'afficher un ecran de fin de jeu lors de la mort du joueur
void gameover(t_joueur *joueur, t_bitmaps *page)
{
    int x, yGame, yOver, imgcourante = 0, cpt=0, temp = 0, i;
    FILE* fichier = NULL;
    fichier = fopen("meilleursScore.txt", "w");
    if(fichier==NULL)
    {
        allegro_message("Problème de fichier");
        exit(EXIT_FAILURE);
    }



    x = SCREEN_W/2-250;
    yGame = -1;
    yOver = SCREEN_H+1;
    page->retourMenu = 1;

    joueur->y = SCREEN_H/2-28;
    joueur->x = -1-joueur->tx;

    while(!key[KEY_ESC])
    {

        cpt++;
        if(cpt>300)
        {
            imgcourante++;
            cpt=0;
        }

        if(imgcourante>2)
            imgcourante=0;
        clear_bitmap(page->buffer);
        if(yGame<SCREEN_H/2-200)
            yGame++;
        if(yOver>SCREEN_H/2)
            yOver--;

        if(yOver==SCREEN_H/2)
            joueur->x+=7;

        draw_sprite(page->buffer, page->crane, 100, 10);
        draw_sprite(page->buffer, page->game[imgcourante], x, yGame);
        draw_sprite(page->buffer, page->over[imgcourante], x, yOver);
        draw_sprite(page->buffer, joueur->img[0], joueur->x, joueur->y);
        blit(page->buffer, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
        rest(10);
    }

    for(i=0;i<10; i++)
    {
        if(joueur->score>page->meilleursScores[i])
        {
            temp = page->meilleursScores[i];
            page->meilleursScores[i] = joueur->score;
            joueur->score = temp;
        }

        fprintf(fichier, "%d\n", page->meilleursScores[i]);
    }
    fclose(fichier);

}

/***********************************************************************************************************************************************************************************************************/

/*                                                                                       BOSS                                                                                                              */

/***********************************************************************************************************************************************************************************************************/

void ajouterBoss(t_listeEnnemis *ennemis, t_bitmaps *page)
{
    int i,type;
    t_ennemi *ennemi;

    type = 5;

    // Liste pleine, on alloue rien et on retourne NULL...
    if (ennemis->n >= ennemis->maximum)
        return NULL;

    // Allouer un acteur initialisé
    ennemi=creerEnnemi(type, page);

            // Chercher un emplacement libre
    i=0;
    while (ennemis->tab[i]!=NULL && i<ennemis->maximum)
        i++;

    // Si on a trouvé ...
    // (normalement oui car on a vérifié n<max)
    if (i<ennemis->maximum)
    {
        // Accrocher le acteur à l'emplacement trouvé
        ennemis->tab[i]=ennemi;
        ennemis->n++;
    }
            // Sinon c'est qu'il y a un problème de cohérence
    else
        allegro_message("Anomalie gestion ajouterActeur : liste corrompue");
}

void mortBoss(t_bitmaps *page, t_joueur *joueur, t_tuiles *tuile)
{
    int x[10], y[10], i, cptScreen = 0;
    BITMAP *boss;
    boss = load_bitmap("Images/Ennemi/Boss/Boss.bmp", NULL);

    joueur->imgcourante = 0;
    page->cpt = 0;
    page->first1 = 0;
    while(joueur->x<SCREEN_W)
    {
        page->cpt++;
        cptScreen++;
        blit(page->fondBoss,page->buffer,page->screenx2,0,0,0,SCREEN_W,SCREEN_H);

        page->screenx2+=page->vitesseScreen;
        if(page->screenx2>page->fondBoss->w-SCREEN_W)
            page->screenx2 = 0;

        draw_sprite(page->buffer, tuile->Boss1, -124, 0);
        draw_sprite(page->buffer, tuile->Boss2, -10, 545);
        if(page->cpt<475)
            draw_sprite(page->buffer, boss, 599, 57);
        for(i=0; i<10; i++)
        {
            if(page->first1 == 0)
            {
                x[i] = rand()%101+600;
                y[i] = rand()%451+50;
            }
            if(page->cpt>0&&page->cpt<75&&i>=0&&i<2)
                draw_sprite(page->buffer, page->explosion, x[i], y[i]);
            if(page->cpt>100&&page->cpt<175&&i>=2&&i<4)
                draw_sprite(page->buffer, page->explosion, x[i], y[i]);
            if(page->cpt>200&&page->cpt<275&&i>=4&&i<6)
                draw_sprite(page->buffer, page->explosion, x[i], y[i]);
            if(page->cpt>300&&page->cpt<375&&i>=6&&i<8)
                draw_sprite(page->buffer, page->explosion, x[i], y[i]);
            if(page->cpt>400&&page->cpt<475&&i>=8&&i<10)
                draw_sprite(page->buffer, page->explosion, x[i], y[i]);
        }
        if(page->cpt>500)
        {
            if(joueur->x<SCREEN_W)
                joueur->x+=5;
        }

        dessinerJoueur(joueur, page);
        page->first1 = 1;
        blit(page->buffer, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
        rest(10);
    }
    page->niveau=5;
    page->first1 = 0;
    page->first2 = 0;
    page->cpt = 0;
    joueur->score+=10000;
}

/***********************************************************************************************************************************************************************************************************/

/*                                                                                       EXPLOSIONS                                                                                                     */

/***********************************************************************************************************************************************************************************************************/

t_listeExplosions *creerListeExplosions(int maxEnnemis)
{
    t_listeExplosions *nouv;
    int i;

    nouv=(t_listeExplosions*)malloc(1*sizeof(t_listeExplosions));

    nouv->maximum=maxEnnemis;
    nouv->n=0;
    nouv->tab=(t_explosion**)malloc(maxEnnemis*sizeof(t_explosion*));

    for (i=0;i<maxEnnemis;i++)
        nouv->tab[i]=NULL;

    return nouv;
}

t_explosion *creerExplosion(int x, int y, t_bitmaps *page)
{
    t_explosion *nouv;
    nouv=(t_explosion*)malloc(1*sizeof(t_explosion));

    nouv->cptexplo = 0;
    nouv->x = x-30;
    nouv->y = y-20;
    nouv->dx = -page->vitesseScreen;
    nouv->imgcourante = 0;
    return nouv;
}

t_explosion *ajouterExplosion(t_listeExplosions *explosions, int x, int y, t_bitmaps *page)
{
    int i;
    t_explosion *explosion;


                // Liste pleine, on alloue rien et on retourne NULL...
    if (explosions->n >= explosions->maximum)
        return NULL;

            // Allouer un acteur initialisé
    explosion=creerExplosion(x, y, page);

    i=0;
    while (explosions->tab[i]!=NULL && i<explosions->maximum)
        i++;

            // Si on a trouvé ...
            // (normalement oui car on a vérifié n<max)
    if (i<explosions->maximum)
    {
                // Accrocher le acteur à l'emplacement trouvé
        explosions->tab[i]=explosion;
        explosions->n++;
    }
            // Sinon c'est qu'il y a un problème de cohérence
    else
        allegro_message("Anomalie gestion ajouterActeur : liste corrompue");

    return explosion;
}

void animationExplosion(t_listeExplosions *explosions, t_bitmaps *page)
{
    int i;
    for(i=0; i<explosions->maximum; i++)
    {
        if(explosions->tab[i]!=NULL)
        {
            explosions->tab[i]->x+=explosions->tab[i]->dx;
            explosions->tab[i]->cptexplo++;

            if(explosions->tab[i]->cptexplo>5)
            {
                explosions->tab[i]->imgcourante++;
                if(explosions->tab[i]->imgcourante>1&&explosions->tab[i]->imgcourante<3)
                    explosions->tab[i]->x-=5;
                if(explosions->tab[i]->imgcourante>5)
                    explosions->tab[i]->cptexplo = -1;
                else
                    explosions->tab[i]->cptexplo = 0;
            }


            if(explosions->tab[i]->cptexplo!=-1)
            {
                draw_sprite(page->buffer, page->explo[explosions->tab[i]->imgcourante], explosions->tab[i]->x, explosions->tab[i]->y);
            }
            else
                enleverExplosion(explosions, i);
        }
    }
}

void enleverExplosion(t_listeExplosions *explosions, int i)
{
    if (explosions->tab[i]!=NULL)
    {
        // libérer la mémoire du acteur
        free(explosions->tab[i]);

        // marquer l'emplacement comme libre
        explosions->tab[i]=NULL;
        explosions->n--;
    }
}

/***********************************************************************************************************************************************************************************************************/

/*                                                                                       OBJETS                                                                                                     */

/***********************************************************************************************************************************************************************************************************/

t_listeObjets *creerListeObjets(int maxObjets)
{
    t_listeObjets *nouv;
    int i;

    nouv=(t_listeObjets*)malloc(1*sizeof(t_listeObjets));

    nouv->maximum=maxObjets;
    nouv->n=0;
    nouv->tab=(t_objet**)malloc(maxObjets*sizeof(t_objet*));

    for (i=0;i<maxObjets;i++)
        nouv->tab[i]=NULL;

    return nouv;
}

t_objet *creerObjet(int x, int y, t_bitmaps *page, int type)
{
    int i;
    char nomfichier[256];

    t_objet *nouv;
    nouv=(t_objet*)malloc(1*sizeof(t_objet));

    switch(type)
    {
        case 0:     //Vie
            for (i=0;i<6;i++)
            {
                sprintf(nomfichier,"Images/Vaisseau/Coeur%d.bmp",i);

                nouv->img[i] = load_bitmap(nomfichier,NULL);
                if (!nouv->img[i])
                {
                    allegro_message("pas pu trouver %s",nomfichier);
                    exit(EXIT_FAILURE);
                }
            }
            nouv->type = 0;
            nouv->x = x;
            nouv->y = y;
            nouv->tx = 34;
            nouv->ty = 32;
            nouv->dx = -page->vitesseScreen;
            nouv->imgcourante = 0;
            nouv->vivant = 1;
            nouv->cpt = 0;
            break;
        case 1:     //dmgUp
            for (i=0;i<12;i++)
            {
                sprintf(nomfichier,"Images/Objets/DmgUp/%d.bmp",i);

                nouv->img[i] = load_bitmap(nomfichier,NULL);
                if (!nouv->img[i])
                {
                    allegro_message("pas pu trouver %s",nomfichier);
                    exit(EXIT_FAILURE);
                }
            }
            nouv->type = 1;
            nouv->x = x;
            nouv->y = y;
            nouv->tx = 92;
            nouv->ty = 32;
            nouv->dx = -page->vitesseScreen;
            nouv->imgcourante = 0;
            nouv->vivant = 1;
            nouv->cpt =0;
            break;
        case 2:     //boucliers
            for (i=0;i<6;i++)
            {
                sprintf(nomfichier,"Images/Objets/Boucliers/%d.bmp",i);

                nouv->img[i] = load_bitmap(nomfichier,NULL);
                if (!nouv->img[i])
                {
                    allegro_message("pas pu trouver %s",nomfichier);
                    exit(EXIT_FAILURE);
                }
            }
            nouv->type = 2;
            nouv->x = x;
            nouv->y = y;
            nouv->tx = 30;
            nouv->ty = 32;
            nouv->dx = -page->vitesseScreen;
            nouv->imgcourante = 0;
            nouv->vivant = 1;
            nouv->cpt =0;
            break;
    }

    return nouv;
}

t_objet *ajouterObjet(t_listeObjets *objets, int x, int y, t_bitmaps *page)
{
    int i, alea, type;
    t_objet *objet;
    alea = rand()%10;
    type = rand()%3;


                // Liste pleine, on alloue rien et on retourne NULL...
    if (objets->n >= objets->maximum)
        return NULL;

    if(alea==0)
    {
        objet=creerObjet(x, y, page, type);// Allouer un acteur initialisé

        i=0;
        while (objets->tab[i]!=NULL && i<objets->maximum)
            i++;

                // Si on a trouvé ...
                // (normalement oui car on a vérifié n<max)
        if (i<objets->maximum)
        {
                    // Accrocher le acteur à l'emplacement trouvé
            objets->tab[i]=objet;
            objets->n++;
        }
                // Sinon c'est qu'il y a un problème de cohérence
        else
            allegro_message("Anomalie gestion ajouterActeur : liste corrompue");
    }
    return objet;
}

void animationObjets(t_listeObjets *objets, t_bitmaps *page)
{
    int i;
    for(i=0; i<objets->maximum; i++)
    {
        if(objets->tab[i]!=NULL)
        {
            objets->tab[i]->x+=objets->tab[i]->dx;
            objets->tab[i]->cpt++;

            if(objets->tab[i]->cpt>7)
            {
                objets->tab[i]->cpt=0;
                objets->tab[i]->imgcourante++;
                if(objets->tab[i]->type==0||objets->tab[i]->type==2)
                {
                    if(objets->tab[i]->imgcourante>5)
                        objets->tab[i]->imgcourante = 0;
                }
                if(objets->tab[i]->type==1)
                {
                    if(objets->tab[i]->imgcourante>11)
                        objets->tab[i]->imgcourante = 0;
                }

            }
            draw_sprite(page->buffer, objets->tab[i]->img[objets->tab[i]->imgcourante], objets->tab[i]->x, objets->tab[i]->y);
            if(objets->tab[i]->x+objets->tab[i]->tx<0||objets->tab[i]->vivant == 0)
                enleverObjet(objets, i);
        }
    }
}


void enleverObjet(t_listeObjets *objets, int i)
{
    if (objets->tab[i]!=NULL)
    {
        // libérer la mémoire du acteur
        free(objets->tab[i]);

        // marquer l'emplacement comme libre
        objets->tab[i]=NULL;
        objets->n--;
    }
}

/***********************************************************************************************************************************************************************************************************/

/*                                                                                       CHOIX NIVEAUX                                                                                                     */

/***********************************************************************************************************************************************************************************************************/



void choixNiveaux(t_bitmaps *page, t_tuiles *tuile, t_joueur *joueur, t_listeEnnemis *ennemis, t_listeActeurs *acteurs)
{
    int xFleche = 200, yFleche = 161, cptFleche = 0, i, y = 150, screenX = 0, ecran = 1;
    char nomfichier[256];
    BITMAP* Lvl[3];
    BITMAP* fond1;
    BITMAP* fond2;
    BITMAP* fond3;

    if(key[KEY_L])              //Lors de l'appui sur la touche L, stop l'écran de fond et affiche le menu pause
    {
        for (i=0;i<3;i++)
        {
            sprintf(nomfichier,"Images/ChoixNiveau/Lvl%d.bmp",i);
            Lvl[i] = load_bitmap(nomfichier,NULL);
            if (!Lvl[i])
            {
                allegro_message("pas pu trouver %s",nomfichier);
                exit(EXIT_FAILURE);
            }
        }
        fond3 = load_bitmap("Images/Niveau/DECOR2.bmp", NULL);
        fond2 = load_bitmap("Images/Niveau/DecorNiveau2.bmp", NULL);
        fond1 = load_bitmap("Images/Niveau/arriere_ville1.bmp", NULL);


        key[KEY_L] = 0;
        while(!key[KEY_L]&&!key[KEY_ESC])
        {
            if(ecran==1)
                blit(fond1, page->buffer, screenX, 0, 0, 0, SCREEN_W, SCREEN_H);
            else if(ecran==2)
                blit(fond2, page->buffer, screenX, 0, 0, 0, SCREEN_W, SCREEN_H);
            else
                blit(fond3, page->buffer, screenX, 0, 0, 0, SCREEN_W, SCREEN_H);
            screenX+=10;
            if(ecran==1&&screenX>fond1->w-SCREEN_W)
            {
                ecran=2;
                screenX = 0;
            }
            if(ecran==2&&screenX>3700-SCREEN_W)
            {
                ecran=3;
                screenX = 0;
            }
            if(ecran==3&&screenX>3700-SCREEN_W)
            {
                ecran=1;
                screenX = 0;
            }


            if(cptFleche>0)
                cptFleche++;
            if(cptFleche>15)
                cptFleche = 0;

            if((key[KEY_UP]||key[KEY_W])&&cptFleche==0)     //bouge la fleche du menu
            {
                xFleche = 200;
                if(yFleche!=161)
                {
                    yFleche -= 100;
                    cptFleche++;
                }
            }
            if((key[KEY_DOWN]||key[KEY_S])&&cptFleche==0)
            {
                if(yFleche==361)
                    xFleche = 450;

                if(yFleche!=461)
                {
                    yFleche += 100;
                    cptFleche++;
                }
            }

            if(key[KEY_ENTER])
            {
                joueur->bouclier = 0;
                joueur->cptBouclier = 0;
                joueur->invincibilite = 0;
                joueur->tempsInvinc = 0;
                page->screenx1 = 300;
                page->screenx2 = 0;
                page->screenActuel = 0;
                joueur->x = 15;
                joueur->y = SCREEN_H/2;
                joueur->imgcourante = 0;

                if(yFleche==161)
                {
                    page->niveau=1;
                    for (i=0;i<4;i++)
                    {
                        sprintf(nomfichier,"Images/Vaisseau/helico%d.bmp",i);

                        joueur->img[i] = load_bitmap(nomfichier,NULL);
                        if (!joueur->img[i])
                        {
                            allegro_message("pas pu trouver %s",nomfichier);
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                if(yFleche==261)
                {
                    page->niveau=2;
                    for (i=0;i<3;i++)
                    {
                        sprintf(nomfichier,"Images/Vaisseau/Avion%d.bmp",i);
                        joueur->img[i] = load_bitmap(nomfichier,NULL);
                        if (!joueur->img[i])
                        {
                            allegro_message("pas pu trouver %s",nomfichier);
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                if(yFleche==361)
                {
                    page->niveau=3;
                    for (i=0;i<3;i++)
                    {
                        sprintf(nomfichier,"Images/Vaisseau/Vaisseau%d.bmp",i);
                        joueur->img[i] = load_bitmap(nomfichier,NULL);
                        if (!joueur->img[i])
                        {
                            allegro_message("pas pu trouver %s",nomfichier);
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                if(yFleche==461)
                {
                    page->niveau=4;

                    page->first2 = 0;
                    page->first1 = 0;
                    for (i=0;i<3;i++)
                    {
                        sprintf(nomfichier,"Images/Vaisseau/Vaisseau%d.bmp",i);
                        joueur->img[i] = load_bitmap(nomfichier,NULL);
                        if (!joueur->img[i])
                        {
                            allegro_message("pas pu trouver %s",nomfichier);
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                joueur->tx = joueur->img[0]->w;
                joueur->ty = joueur->img[0]->h;
                for(i=0; i<ennemis->maximum; i++)
                {
                    enleverEnnemis(ennemis, i);
                }
                for(i=0; i<acteurs->max; i++)
                {
                    enleverActeur(acteurs, i);
                }
                initialisationBitmaps(tuile, page);
                key[KEY_ESC] = 1;

            }

            draw_sprite(page->buffer, page->fleche, xFleche, yFleche);
            for(i=0; i<3; i++)
            {
                draw_sprite(page->buffer, Lvl[i], 500, y);
                rect(page->buffer, 499, y-1, 500+Lvl[i]->w, Lvl[i]->h+y, makecol(0,0,255));
                y+=100;
            }
            y = 150;
            rectfill(page->buffer, 1, 560, SCREEN_W-1, SCREEN_H-1, makecol(0, 0, 0));
            rect(page->buffer, 1, 560, SCREEN_W-1, SCREEN_H-1, makecol(0, 0, 255));

            textprintf_ex(page->buffer, font, 350, 183, makecol(255,255,255), -1, "- NIVEAU 1 -");
            textprintf_ex(page->buffer, font, 350, 283, makecol(255,255,255), -1, "- NIVEAU 2 -");
            textprintf_ex(page->buffer, font, 350, 383, makecol(255,255,255), -1, "- NIVEAU 3 -");
            textprintf_ex(page->buffer, font, 315, 575, makecol(255,255,255), -1, "- Selectionnez un niveau -");
            textprintf_ex(page->buffer, font, 550, 483, makecol(255,255,255), -1, "- BOSS -");
            blit(page->buffer, screen, 0,0,0,0,SCREEN_W,SCREEN_H);
            rest(10);
        }
        key[KEY_ESC] = 0;
        key[KEY_L] = 0;     //on réinitialise la touche
    }
}




/***********************************************************************************************************************************************************************************************************/

/*                                                                                       BOUCLE DE JEU                                                                                                     */

/***********************************************************************************************************************************************************************************************************/

t_ennemi *creerEnnemiNiveau1(t_bitmaps *page)
{
    int i;
    char nomfichier[256];
    t_ennemi *nouv;
    nouv=(t_ennemi*)malloc(1*sizeof(t_ennemi));
    if(page->niveau==1)
    {
        for (i=0; i<3; i++)
        {
            sprintf(nomfichier,"Images/Ennemi/Fixe/Tir%d.bmp",i);

            nouv->img[i]= load_bitmap(nomfichier,NULL);
            if (!nouv->img[i])
            {
                allegro_message("pas pu trouver %s",nomfichier);
                exit(EXIT_FAILURE);
            }
        }
        nouv->imgcourante=0;
        nouv->tx = 64;
        nouv->ty = 57;
        nouv->dx=page->vitesseScreen;
        nouv->dy=0;
        nouv->vivant=1;
        nouv->vie = 5;
        nouv->cpttir = 0;
        nouv->x = 800;
        nouv->type = 0;
    }
    return nouv;
}

void ajouterEnnemisNiveau1(t_listeEnnemis *ennemis, t_bitmaps *page)
{
    int i;
    t_ennemi *ennemi;
    if((page->screenx2>=86&&page->screenx2<89)||(page->screenx2>=408&&page->screenx2<411)||(page->screenx2>=887&&page->screenx2<890))
    {
        if (ennemis->n >= ennemis->maximum)
                return NULL;

                // Allouer un acteur initialisé
            ennemi=creerEnnemiNiveau1(page);
            if(page->screenx2>=86&&page->screenx2<89)
                ennemi->y = 360;
            if(page->screenx2>=408&&page->screenx2<411)
                ennemi->y = 276;
            if(page->screenx2>=887&&page->screenx2<890)
                ennemi->y = 392;

                // Chercher un emplacement libre
            i=0;
            while (ennemis->tab[i]!=NULL && i<ennemis->maximum)
                i++;
                // Si on a trouvé ...
                // (normalement oui car on a vérifié n<max)
            if (i<ennemis->maximum){
                    // Accrocher le acteur à l'emplacement trouvé
                ennemis->tab[i]=ennemi;
                ennemis->n++;
            }
                // Sinon c'est qu'il y a un problème de cohérence
            else
                allegro_message("Anomalie gestion ajouterActeur : liste corrompue");
    }
}


void boucleJeu(t_bitmaps *page,t_tuiles *tuile, t_joueur *vaisseau)
{
    int test = 1;

    t_listeActeurs *acteurs;
    t_listeEnnemis *ennemis;
    t_listeExplosions *explosions;
    t_listeObjets *objets;
    BITMAP *img2[3];

    acteurs=creerListeActeurs(100);
    ennemis = creerListeEnnemis(100);
    explosions = creerListeExplosions(50);
    objets = creerListeObjets(5);
    reinitialisationVaisseau(vaisseau);
    attributionImagesPause(img2);

    page->screenx1 = 300;
    page->screenx2 = 0;
    page->first1 = 0;
    page->first2 = 1;
    page->vitesseScreen = 3;

    //chrono
    unsigned int horlogeHeures=0;
	unsigned int horlogeMinutes=0;
	unsigned int horlogeSecondes=0;
	unsigned int horlogeMillisecondes=0;
	unsigned int tempsEcoule=0;
	clock_t debut,moment;
	tempsEcoule=0;
    debut=clock();  //heure horloge au début execution
    tempsEcoule=horlogeSecondes;   //actualise temps ecoule


    while (!key[KEY_ESC])
    {
        //chrono
        moment=clock(); // temps de l'horloge à ce moment
		horlogeMillisecondes=moment-debut;
		horlogeSecondes=(horlogeMillisecondes/(CLOCKS_PER_SEC))-(horlogeMinutes*60);
		horlogeMinutes=(horlogeMillisecondes/(CLOCKS_PER_SEC))/60;
		horlogeHeures=horlogeMinutes/60;


        if(page->niveau==2||page->niveau==1)
            clear_bitmap(page->collision);
        pause(page, img2);
        choixNiveaux(page, tuile, vaisseau, ennemis, acteurs);
        if(page->niveau!=1)
            vaisseau->imgcourante=0;



        screenActualisation(page, tuile, ennemis, vaisseau, acteurs);
        dessinerTableauBord(page, vaisseau);
        vaisseau->imgcourante = actualiserJoueur(vaisseau,acteurs, page);
        if(page->niveau==3||page->niveau==2||page->niveau==1||page->niveau==6)
            ajouterEnnemis(ennemis, page);
        if(page->niveau==1)
            ajouterEnnemisNiveau1(ennemis, page);

        if(key[KEY_N])
            vaisseau->vieActuelle = 0;


        gererCollisions(page, vaisseau);
        actualiserListeActeurs(acteurs, page);
        actualiserListeEnnemis(ennemis, page);
        animationExplosion(explosions, page);
        animationObjets(objets, page);
        collisionListeActeurs(ennemis, acteurs, vaisseau, page, tuile, explosions, objets);

        if(vaisseau->vieActuelle<=0)
            gameover(vaisseau, page);
        if(page->niveau!=1)
            tirEnnemis(ennemis, page, acteurs, vaisseau);
        else
             tirFixes(ennemis, page, acteurs, vaisseau);///*****************MODIF LAURE***********************///


        dessinerJoueur(vaisseau, page);
        dessinerListeEnnemis(page, ennemis, vaisseau);
        dessinerListeActeurs(page,acteurs);
        affichageEtTestcollision(page, &test, ennemis);

        //chrono
        tempsEcoule=horlogeSecondes;//actualise chrono
        page->temps=tempsEcoule%60;
        page->minutes=(tempsEcoule/60)%60;

        rest(10);
    }
    page->retourMenu = 1;
}
