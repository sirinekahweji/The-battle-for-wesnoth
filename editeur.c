/*
editeur.c
---------

Par mateo21, pour Le Site du Zér0 (www.siteduzero.com)

Rôle : gestion de l'éditeur de niveaux.
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "constantes.h"
#include "editeur.h"
#include "fichiers.h"

#define DOUBLE_CLICK_MAX_DELAY 1000


void editeur(SDL_Surface* ecran)
{
    SDL_Surface *mur = NULL, *caisse = NULL, *objectif = NULL, *mario = NULL;
    SDL_Rect position;
    SDL_Event event;

    int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
    int objetActuel = MUR, i = 0, j = 0;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};

    // Chargement des objets et du niveau
    mur = IMG_Load("mur.PNG");
    caisse = IMG_Load("caisse.PNG");
    objectif = IMG_Load("objectif.png");
    mario = IMG_Load("mario.gif");

    if (!chargerNiveau(carte))
        exit(EXIT_FAILURE);

    // Boucle infinie de l'éditeur
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                // On met l'objet actuellement choisi (mur, caisse...) à l'endroit du clic
                if(DoubleClickDetected() == 1){
                carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = objetActuel;
                clicGaucheEnCours = 1; // On active un booléen pour retenir qu'un bouton est
                }
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) // Le clic droit sert à effacer
            {
                                if(DoubleClickDetected() == 1){
                carte[event.button.x / TAILLE_BLOC][event.button.y /TAILLE_BLOC] = VIDE;
                clicDroitEnCours = 1;
                                }
            }
            break;

        case SDL_MOUSEBUTTONUP: // On désactive le booléen qui disait qu'un bouton était enfoncé
            if (event.button.button == SDL_BUTTON_LEFT)
                clicGaucheEnCours = 0;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                clicDroitEnCours = 0;
            break;


        case SDL_MOUSEMOTION:
            if (clicGaucheEnCours==1) // Si on déplace la souris et que le bouton gauche de la souris est enfoncé
            {
                carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = objetActuel;
            }
            else if (clicDroitEnCours==1) // Pareil pour le bouton droit de la souris
            {
                carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = VIDE;
            }
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;
            case SDLK_s:
                sauvegarderNiveau(carte);
                break;
            case SDLK_c:
                chargerNiveau(carte);
                break;
            case SDLK_KP1:
                objetActuel = MUR;
                break;
            case SDLK_KP2:
                objetActuel = CAISSE;
                break;
            case SDLK_KP3:
                objetActuel = OBJECTIF;
                break;
            case SDLK_KP4:
                objetActuel = MARIO;
                break;
            }
            break;
        }

        // Effacement de l'écran
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

        // Placement des objets à l'écran
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
                case OBJECTIF:
                    SDL_BlitSurface(objectif, NULL, ecran, &position);
                    break;
                case MARIO:
                    SDL_BlitSurface(mario, NULL, ecran, &position);
                    break;
                }
            }
        }

        // Mise à jour de l'écran
        SDL_Flip(ecran);
    }

    SDL_FreeSurface(mur);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(objectif);
    SDL_FreeSurface(mario);
}
