/*
jeu.c
-----

Par mateo21, pour Le Site du Zér0 (www.siteduzero.com)

Rôle : fonctions du jeu.
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "constantes.h"
#include "jeu.h"
#include "fichiers.h"


void jouer(SDL_Surface* ecran)
{
    SDL_Surface *mur = NULL, *caisse = NULL, *caisseOK = NULL, *objectif = NULL, *marioActuel = NULL;
    SDL_Rect position, positionJoueur;
    SDL_Event event;

    int continuer = 1, objectifsRestants = 0, i = 0, j = 0;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};

    // Chargement des sprites (décors, personnage...)
    mur = IMG_Load("mur.PNG");
    caisse = IMG_Load("caisse.PNG");
    caisseOK = IMG_Load("caisse_ok.PNG");
    objectif = IMG_Load("objectif.png");

    marioActuel = IMG_Load("mario.gif"); // Mario sera dirigé vers le bas au départ

    // Chargement du niveau
    if (!chargerNiveau(carte))
        exit(EXIT_FAILURE); // On arrête le jeu si on n'a pas pu charger le niveau

    // Recherche de la position de Mario au départ
    for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
        {
            if (carte[i][j] == MARIO) // Si Mario se trouve à cette position sur la carte
            {
                positionJoueur.x = i;
                positionJoueur.y = j;
                carte[i][j] = VIDE;
            }
        }
    }

    // Activation de la répétition des touches
    SDL_EnableKeyRepeat(100, 100);

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_UP:
                deplacerJoueur(carte, &positionJoueur, HAUT);
                break;
            case SDLK_DOWN:
                deplacerJoueur(carte, &positionJoueur, BAS);
                break;
            case SDLK_RIGHT:
                deplacerJoueur(carte, &positionJoueur, DROITE);
                break;
            case SDLK_LEFT:
                deplacerJoueur(carte, &positionJoueur, GAUCHE);
                break;
            }
            break;
        }

        // Effacement de l'écran
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

        // Placement des objets à l'écran
        objectifsRestants = 0;

        for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
        {
            for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
            {
                position.x = i * TAILLE_BLOC;
                position.y = j * TAILLE_BLOC;

                switch(carte[i][j])
                {
                case MUR:
                    SDL_BlitSurface(mur, NULL, ecran, &position);
                    break;
                case CAISSE:
                    SDL_BlitSurface(caisse, NULL, ecran, &position);
                    break;
                case CAISSE_OK:
                    SDL_BlitSurface(caisseOK, NULL, ecran, &position);
                    break;
                case OBJECTIF:
                    SDL_BlitSurface(objectif, NULL, ecran, &position);
                    objectifsRestants = 1;
                    break;
                }
            }
        }

        // Si on n'a trouvé aucun objectif sur la carte, c'est qu'on a gagné
        if (!objectifsRestants)
            continuer = 0;

        // On place le joueur à la bonne position
        position.x = positionJoueur.x * TAILLE_BLOC;
        position.y = positionJoueur.y * TAILLE_BLOC;

        SDL_BlitSurface(marioActuel, NULL, ecran, &position);


        // ajouter une couleur de sélection semi-transparente
        //SDL_Rect position_rec = {position.x,  position.y, TAILLE_BLOC, TAILLE_BLOC};
        SDL_Surface *Case_a_remplir = NULL;
        Case_a_remplir = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, 32, 0, 0, 0, 0);
        SDL_FillRect(Case_a_remplir, NULL, SDL_MapRGB(ecran->format, 255, 0, 0));
        SDL_SetAlpha(Case_a_remplir, SDL_SRCALPHA, 100);
        SDL_BlitSurface(Case_a_remplir, NULL, ecran, &position);



        SDL_Flip(ecran);
    }

    // Désactivation de la répétition des touches (remise à 0)
    SDL_EnableKeyRepeat(0, 0);

    // Libération des surfaces chargées
    SDL_FreeSurface(mur);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(caisseOK);
    SDL_FreeSurface(objectif);

}

void deplacerJoueur(int carte[][NB_BLOCS_HAUTEUR], SDL_Rect *pos, int direction)
{
    switch(direction)
    {
    case HAUT:
        if (pos->y - 1 < 0) // Si le joueur dépasse l'écran, on arrête
            break;
        if (carte[pos->x][pos->y - 1] == MUR) // S'il y a un mur, on arrête
            break;
        // Si on veut pousser une caisse, il faut vérifier qu'il n'y a pas de mur derrière (ou une autre caisse, ou la limite du monde)
        if ((carte[pos->x][pos->y - 1] == CAISSE || carte[pos->x][pos->y - 1] == CAISSE_OK) &&
                (pos->y - 2 < 0 || carte[pos->x][pos->y - 2] == MUR ||
                 carte[pos->x][pos->y - 2] == CAISSE || carte[pos->x][pos->y - 2] == CAISSE_OK))
            break;

        // Si on arrive là, c'est qu'on peut déplacer le joueur !
        // On vérifie d'abord s'il y a une caisse à déplacer
        deplacerCaisse(&carte[pos->x][pos->y - 1], &carte[pos->x][pos->y - 2]);

        pos->y--; // On peut enfin faire monter le joueur (oufff !)
        break;


    case BAS:
        if (pos->y + 1 >= NB_BLOCS_HAUTEUR)
            break;
        if (carte[pos->x][pos->y + 1] == MUR)
            break;

        if ((carte[pos->x][pos->y + 1] == CAISSE || carte[pos->x][pos->y + 1] == CAISSE_OK) &&
                (pos->y + 2 >= NB_BLOCS_HAUTEUR || carte[pos->x][pos->y + 2] == MUR ||
                 carte[pos->x][pos->y + 2] == CAISSE || carte[pos->x][pos->y + 2] == CAISSE_OK))
            break;


        deplacerCaisse(&carte[pos->x][pos->y + 1], &carte[pos->x][pos->y + 2]);

        pos->y++;
        break;


    case GAUCHE:
        if (pos->x - 1 < 0)
            break;
        if (carte[pos->x - 1][pos->y] == MUR)
            break;

        if ((carte[pos->x - 1][pos->y] == CAISSE || carte[pos->x - 1][pos->y] == CAISSE_OK) &&
                (pos->x - 2 < 0 || carte[pos->x - 2][pos->y] == MUR ||
                 carte[pos->x - 2][pos->y] == CAISSE || carte[pos->x - 2][pos->y] == CAISSE_OK))
            break;


        deplacerCaisse(&carte[pos->x - 1][pos->y], &carte[pos->x - 2][pos->y]);

        pos->x--;
        break;


    case DROITE:
        if (pos->x + 1 >= NB_BLOCS_LARGEUR)
            break;
        if (carte[pos->x + 1][pos->y] == MUR)
            break;

        if ((carte[pos->x + 1][pos->y] == CAISSE || carte[pos->x + 1][pos->y] == CAISSE_OK) &&
                (pos->x + 2 >= NB_BLOCS_LARGEUR || carte[pos->x + 2][pos->y] == MUR ||
                 carte[pos->x + 2][pos->y] == CAISSE || carte[pos->x + 2][pos->y] == CAISSE_OK))
            break;

        deplacerCaisse(&carte[pos->x + 1][pos->y], &carte[pos->x + 2][pos->y]);

        pos->x++;
        break;
    }
}

void deplacerCaisse(int *premiereCase, int *secondeCase)
{
    if (*premiereCase == CAISSE || *premiereCase == CAISSE_OK)
    {
        if (*secondeCase == OBJECTIF)
            *secondeCase = CAISSE_OK;
        else
            *secondeCase = CAISSE;

        if (*premiereCase == CAISSE_OK)
            *premiereCase = OBJECTIF;
        else
            *premiereCase = VIDE;
    }
}

