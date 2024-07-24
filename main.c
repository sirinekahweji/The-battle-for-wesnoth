#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#define DOUBLE_CLICK_MAX_DELAY 1000

//derniere_code
#include<math.h>
#include<string.h>
#define ID_TYPE_TERRAIN_CHATEAU 1
#define ID_TYPE_TERRAIN_VILLAGE 16
#define ID_TYPE_TERRAIN_DONJON 17
#define MAX_ELOIGNEMENT_CHATEAU_DONJON 2
#define NB_LIGNES_CARTE 30
#define NB_COLONNES_CARTE 38
#define MAX_LIGNES_JOUEURS 5
#define MAX_LIGNES_UNITESJOUEURS 50
#define MAX_LIGNES_FICHES_TYPES_UNITES 10
#define MAX_LIGNES_UNITESMAGASIN 8
#define MAX_LIGNES_VILLAGES 14
#define NB_LIGNES_PERIODES 6
#define NB_UNITES_RANG_INF_SUP 3
#define MAX_ATTAQUES 3
#define NB_RESISTANCES 6
#define NB_TYPES_TERRAINS 17
#define MAX_CAPACITES 3
#define MAX_TRAITS 3
#define MAX_EFFETS_ATTAQUES_SUBITES 10
typedef struct {
 int idUniteMagasin;
 int idFicheTypeUnite;
 int idJoueurAutorise;
}UniteMagasin;
typedef struct {
 int idVillage;
 int ligne;
 int colonne;
 int idJoueurProprietaire;
}Village;
typedef struct{
 int numOrdre;
 char moment[20];
 int bonus[4];
}Periode;
typedef struct{
 int idTypeTerrain;
 char symboleTerrain;
 int codeAffichageTerrain;
 char nomTerrain[20];
} TypeTerrain;

typedef struct {
 int idAttaque;
 char nomAttaque[20];
 int degat;
 int nombreAttaques;
 int porteeAttaque;
 int typeAttaque ;
 int idAttaqueSpeciale1;
 int idAttaqueSpeciale2;
} Attaque;
typedef struct {
 int idTypeTerrain;
 int coutMouvement;
 int defense;
} RelationTerrain;
typedef struct {
 int idFicheTypeUnite;
 int race;
 char nom[20];
 int prix;
 int pvMax;
 int mouvementsMax;
 int xpRequise;
 int niveau;
 int alignement;
 int idFichesTypeUniteRangInf[NB_UNITES_RANG_INF_SUP];
 int idFichesTypeUniteRangSup[NB_UNITES_RANG_INF_SUP];
 Attaque attaques[MAX_ATTAQUES];
 int resistances[NB_RESISTANCES];
 RelationTerrain relationsTerrains[NB_TYPES_TERRAINS];
 int idCapacites[MAX_CAPACITES];
}FicheTypeUnite;
typedef struct {
 int idJoueur;
 char symbole;
 int or;
 int type;
 int groupe_allies;
 int nombreVillages;
 int nombreUnites;
 int entretien;
 int revenu;
}Joueur;
typedef struct {
 int idAttaqueSpeciale;
 int toursRestants;
} EffetAttaqueSubite;
typedef struct {
 int idUnite;
 int idFicheTypeUnite;
 int idJoueur;
 char nomUnite[20];
 int active;
 int rang;
 int ligne;
 int colonne;
 int traits[MAX_TRAITS];
 int pv;
 int pvMax;
 int mouvements;
 int mouvementsMax;
 int xp;
 int xpRequise;
 int niveau;
 int alignement;
 int finTour;
 EffetAttaqueSubite effetsAttaquesSubites[MAX_EFFETS_ATTAQUES_SUBITES];
}UniteJoueur;
typedef struct {
 int idUnite;
 int idTypeTerrain;
 int codeAffichageTerrain;
 char symboleJoueur;
 int marque;
}CelluleCarte;

typedef enum {TRAIT_NEANT, TRAIT_INTELLIGENT, TRAIT_RAPIDE, TRAIT_ROBUSTE, TRAIT_FORT,
TRAIT_HABILE, TRAIT_INTREPIDE, TRAIT_SAUVAGE, TRAIT_VIGOUREUX, TRAIT_AGE, TRAIT_IDIOT,
TRAIT_ELEMENTAL, TRAIT_FIDELE, TRAIT_MECANIQUE, TRAIT_LENT, TRAIT_MORTVIVANT,
TRAIT_FAIBLE} E_TRAITS;

int chercherIndiceJoueur(int idJoueur,int nbLignesReel,Joueur joueurs[MAX_LIGNES_JOUEURS])
{
  int i=0;
  for (i=0;i<nbLignesReel;i++)
  {
      if(idJoueur==joueurs[i].idJoueur)
      {
          return i;
      }
  }
  return -1;
}

int chercherIndiceFicheTypeUnite(int idFicheTypeUnite, int nbFichesTypesUnites, FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES])
{  int i=0;
  for (i=0;i<nbFichesTypesUnites;i++)
  {
      if(idFicheTypeUnite==fichesTypesUnites[i].idFicheTypeUnite)
      {
          return i;
      }
  }
  return -1;

}

int chercherIndiceRelationTerrain(int idTypeTerrain, RelationTerrain relationsTerrains[NB_TYPES_TERRAINS])
{int i=0;
   for(i=0;i<NB_TYPES_TERRAINS;i++)
    {
      if (idTypeTerrain==relationsTerrains[i].idTypeTerrain)
      {
        return i;
      }
    }
 return -1;
}

int chercherIndiceTypeTerrain(char symboleTerrain, TypeTerrain typesTerrains[NB_TYPES_TERRAINS])
{ int i=0;
  for (i=0;i<NB_TYPES_TERRAINS;i++)
  {
      if(symboleTerrain==typesTerrains[i].symboleTerrain)
      {

          return i;
      }
  }
  return -1;
}

int chercherIndiceUniteJoueur(int idUnite, int idJoueur, int active, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
{
    int i=0;
    for(i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
    {
        if((idUnite==unitesJoueurs[i].idUnite)&&(idJoueur==unitesJoueurs[i].idJoueur)&&(active==unitesJoueurs[i].active))
        {
            return i;
        }
    }

    return -1;
}

int chercherGroupeAllies(char symbolJoueur, int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS])
{
    int i=0;
    while(i<nbJoueurs)
    {
        if (joueurs[i].symbole == symbolJoueur)
        {
            return joueurs[i].groupe_allies;
        }
        i++;
    }
    return -1;
}
int DoubleClickDetected (void)
{
    static Uint32 LastClickTicks;
    Uint32 CurrentClickTicks;
    // The first time this function is called, LastClickTicks has not been initialised yet.
    if (! LastClickTicks)
    {
        LastClickTicks = SDL_GetTicks ();
        return 0;
    }
    else
    {
        CurrentClickTicks = SDL_GetTicks ();
        // If the period between the two clicks is smaller or equal to a pre-defined number, we report a DoubleClick event.
        if (CurrentClickTicks - LastClickTicks <= DOUBLE_CLICK_MAX_DELAY)
        {
            // Update LastClickTicks and signal a DoubleClick.
            LastClickTicks = CurrentClickTicks;
            return 1;
        }
        else
        {
            // Update LastClickTicks and signal a SingleClick.
            LastClickTicks = CurrentClickTicks;
            return 0;
        }
    }
}

int chercherIndiceJoueurHumain(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS])
 { int i=0;
   for (i=0;i<nbJoueurs;i++)
   {
       if (joueurs[i].type==1)
       {
           return i;
       }
   }
   return -1;
 }

 int chercherIndiceVillage(int ligne, int colonne, int nbVillages, Village villages[MAX_LIGNES_VILLAGES])
 { int i=0;
    for(i=0;i<nbVillages;i++)
    {
        if ((villages[i].colonne==colonne)&&(villages[i].ligne==ligne))
        {
            return i;
        }
    }
    return -1;
 }

int chercherIndiceUniteMagasin(int idUnite, int idJoueur, int nbUnitesMagasin, UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN])
{
    int i;
    for (i=0;i<nbUnitesMagasin;i++)
    {
        if((unitesMagasin[i].idUniteMagasin== idUnite)&&(unitesMagasin[i].idJoueurAutorise))
        {
            return i;
        }
    }
    return -1;
}

int chercherMaxIdUnitesJoueurs(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
{
    int max ,i;
    max=unitesJoueurs[0].idUnite;
    for (i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
    {
        if(unitesJoueurs[i].idUnite>max)
        {
            max=unitesJoueurs[i].idUnite;
        }

    }
    return max;
}

 int chercherIndiceChefUnitesJoueurs(int idJoueur, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
     int i;
    for (i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
    {
        if((unitesJoueurs[i].idJoueur==idJoueur)&&(unitesJoueurs[i].active==1)&&(unitesJoueurs[i].rang==1))
        {
            return i;
        }

    }
 }

 int chercherIndiceJoueurParSymbole(char symbole, int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS])
 {
     int i;
     for (i=0;i<nbJoueurs;i++)
     {
         if(joueurs[i].symbole==symbole)
         {
             return i;

         }
     }
     return -1;
 }

int possedeTrait(int trait, int traits[MAX_TRAITS])
{ int i=0;
 for (i=0;i<MAX_TRAITS;i++)
 {
     if (trait==traits[i])
     {
         return 1;
     }
 }
 return 0;
}

void appliquerTrait(int indiceUniteJoueur, int trait, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
{
    if (trait==TRAIT_INTELLIGENT)
     {
        unitesJoueurs[indiceUniteJoueur].xpRequise -=unitesJoueurs[indiceUniteJoueur].xpRequise*20/100;
     }
    if (trait==TRAIT_RAPIDE)
     {
        unitesJoueurs[indiceUniteJoueur].mouvements++;
        unitesJoueurs[indiceUniteJoueur].pv -= unitesJoueurs[indiceUniteJoueur].pv*5/100;
     }
    if (trait==TRAIT_ROBUSTE)
     {
        unitesJoueurs[indiceUniteJoueur].pv++;

     }
    if (trait==TRAIT_FORT)
     {
        unitesJoueurs[indiceUniteJoueur].pv +=1;

     }
    if (trait==TRAIT_VIGOUREUX)
     {
        unitesJoueurs[indiceUniteJoueur].pv++;

     }
    if (trait==TRAIT_AGE)
     {
        unitesJoueurs[indiceUniteJoueur].pv=8;
        unitesJoueurs[indiceUniteJoueur].mouvements ++;
    }
    if (trait==TRAIT_IDIOT)
     {
        unitesJoueurs[indiceUniteJoueur].xpRequise +=unitesJoueurs[indiceUniteJoueur].xpRequise*20/100;

     }
    if (trait==TRAIT_LENT)
     {
        unitesJoueurs[indiceUniteJoueur].xpRequise +=unitesJoueurs[indiceUniteJoueur].xpRequise*5/100;
        unitesJoueurs[indiceUniteJoueur].mouvements --;

     }
    if (trait==TRAIT_FAIBLE)
     {
        unitesJoueurs[indiceUniteJoueur].pv --;

     }
}

void miseAJourJoueurApresOccupationVillage(int indiceJoueur, Joueur joueurs[MAX_LIGNES_JOUEURS])
 {
     if ((joueurs[indiceJoueur].entretien-joueurs[indiceJoueur].nombreVillages)<0)
        {
            joueurs[indiceJoueur].revenu=2+joueurs[indiceJoueur].nombreVillages;
        }
        else
        {
           joueurs[indiceJoueur].revenu=2+joueurs[indiceJoueur].nombreVillages-(joueurs[indiceJoueur].entretien-joueurs[indiceJoueur].nombreVillages);
        }

 }

int chargerUnitesMagasinVersTableau(UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN], char*nomFichier)
{
    int i=0;
    FILE* UnitesMagasin=fopen(nomFichier,"r");
    while (!feof(UnitesMagasin))
    {
        fscanf(UnitesMagasin,"%d %d %d",&unitesMagasin[i].idUniteMagasin,&unitesMagasin[i].idFicheTypeUnite,&unitesMagasin[i].idJoueurAutorise);
        i++;
    }
    fclose(UnitesMagasin);
    return i;
}

int chargerVillagesVersTableau(Village villages[MAX_LIGNES_VILLAGES], char* nomFichier)
{
    int i=0;
    FILE* village=fopen(nomFichier,"r");
    while (!feof(village))
    {
        fscanf(village,"%d %d %d %d",&villages[i].idVillage,&villages[i].ligne,&villages[i].colonne,&villages[i].idJoueurProprietaire);
        i++;
    }
    fclose(village);
    return i;
}

 void chargerTypesTerrainsVersTableau(TypeTerrain typesTerrains[NB_TYPES_TERRAINS],char* nomFichier)
 {
   int i=0;
   FILE* terrains=fopen(nomFichier,"r");
   while (!feof(terrains))
    {
        fscanf(terrains,"%d %c %d %s /n",&typesTerrains[i].idTypeTerrain,&typesTerrains[i].symboleTerrain,&typesTerrains[i].codeAffichageTerrain,typesTerrains[i].nomTerrain);
        i++;
    }
   fclose(terrains);
 }

void chargerPeriodesVersTableau(Periode periodes[NB_LIGNES_PERIODES], char* nomFichier)
{  int i=0;
   FILE* Periodes=fopen(nomFichier,"r");
   while (!feof(Periodes))
    {
        fscanf(Periodes,"%d %s %d %d %d %d\n",&periodes[i].numOrdre,periodes[i].moment,&periodes[i].bonus[0],&periodes[i].bonus[1],&periodes[i].bonus[2],&periodes[i].bonus[3]);
      i++;
    }
   fclose(Periodes);
}

int chargerFichesTypesUnitesVersTableau(FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES], char* nomFichier)
{ int i=0,j=0;
    FILE* fichesTypesUnite=fopen(nomFichier,"r");
    while (!feof(fichesTypesUnite))
    {fscanf(fichesTypesUnite,"%d %d %s %d %d %d %d %d %d",&fichesTypesUnites[i].idFicheTypeUnite,
            &fichesTypesUnites[i].race,fichesTypesUnites[i].nom,&fichesTypesUnites[i].prix,&fichesTypesUnites[i].pvMax,
            &fichesTypesUnites[i].mouvementsMax,&fichesTypesUnites[i].xpRequise,&fichesTypesUnites[i].niveau,
            &fichesTypesUnites[i].alignement);
        for (j=0;j<3;j++)
        {
         fscanf(fichesTypesUnite,"%d",&fichesTypesUnites[i].idFichesTypeUniteRangInf[j]);
        }
        for(j=0;j<3;j++)
         {
          fscanf(fichesTypesUnite,"%d",&fichesTypesUnites[i].idFichesTypeUniteRangSup[j]);
         }
        for(j=0;j<3;j++)
         {
           fscanf(fichesTypesUnite,"%d %s %d %d %d %d %d %d",&fichesTypesUnites[i].attaques[j].idAttaque,
                  fichesTypesUnites[i].attaques[j].nomAttaque,&fichesTypesUnites[i].attaques[j].degat,
                  &fichesTypesUnites[i].attaques[j].nombreAttaques,&fichesTypesUnites[i].attaques[j].porteeAttaque,
                  &fichesTypesUnites[i].attaques[j].typeAttaque,&fichesTypesUnites[i].attaques[j].idAttaqueSpeciale1,
                  &fichesTypesUnites[i].attaques[j].idAttaqueSpeciale2);
         }
        for (j=0;j<6;j++)
        {
          fscanf(fichesTypesUnite,"%d ",&fichesTypesUnites[i].resistances[j]);
        }
        for (j=0;j<17;j++)
        {
         fscanf(fichesTypesUnite,"%d",&fichesTypesUnites[i].relationsTerrains[j].coutMouvement);
        }
        for (j=0;j<17;j++)
        {
         fscanf(fichesTypesUnite,"%d",&fichesTypesUnites[i].relationsTerrains[j].defense);
        }
        for(j=0;j<3;j++)
        {
         fscanf(fichesTypesUnite,"%d",&fichesTypesUnites[i].idCapacites[j]);
        }
     i++;
   }
    fclose(fichesTypesUnite);
    return i;
}

int chargerJoueursVersTableau(Joueur joueurs[MAX_LIGNES_JOUEURS], char* nomFichier)
 {int i=0;
    FILE* joueur=fopen(nomFichier,"r");
    while (!feof(joueur))
    {
        fscanf(joueur,"%d %c %d %d %d",&joueurs[i].idJoueur,&joueurs[i].symbole,&joueurs[i].or,&joueurs[i].type,&joueurs[i].groupe_allies);
        joueurs[i].nombreVillages=0;
        joueurs[i].nombreUnites=0;
        joueurs[i].entretien=0;
        joueurs[i].revenu=0;
        i++;
    }
    fclose(joueur);
    return i;
}

void chargerUniteJoueurVersTableau(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],char* nomFichier)
{   int i=0,j=0;
    FILE* Fichier=fopen(nomFichier,"r");
    while (!feof(Fichier))
    {
        fscanf(Fichier,"%d %d %d %s %d %d %d %d %d %d %d",&unitesJoueurs[i].idUnite,&unitesJoueurs[i].idFicheTypeUnite,&unitesJoueurs[i].idJoueur,unitesJoueurs[i].nomUnite,&unitesJoueurs[i].active,&unitesJoueurs[i].rang,&unitesJoueurs[i].ligne,&unitesJoueurs[i].colonne,&unitesJoueurs[i].traits[0],&unitesJoueurs[i].traits[1],&unitesJoueurs[i].traits[2]);
        unitesJoueurs[i].pv=0;
        unitesJoueurs[i].pvMax=0;
        unitesJoueurs[i].mouvements=0;
        unitesJoueurs[i].mouvementsMax=0;
        unitesJoueurs[i].xp=0;
        unitesJoueurs[i].xpRequise=0;
        unitesJoueurs[i].niveau=0;
        unitesJoueurs[i].alignement=0;
        unitesJoueurs[i].finTour=0;
        for(j=0;j<MAX_EFFETS_ATTAQUES_SUBITES;j++)
        {
          unitesJoueurs[i].effetsAttaquesSubites[j].idAttaqueSpeciale=0;
          unitesJoueurs[i].effetsAttaquesSubites[j].toursRestants=0;
        }

        i++;
    }
    fclose(Fichier);

}

void chargerCarteVersTableau(CelluleCarte carte[NB_LIGNES_CARTE][ NB_COLONNES_CARTE],TypeTerrain typesTerrains[NB_TYPES_TERRAINS], char* nomFichier)
{int i=0,j=0;
   char symbole;
   int indice;
    FILE* Carte=fopen(nomFichier,"r");
     for (i=0;i<NB_LIGNES_CARTE;i++)
    {
        for(j=0;j<NB_COLONNES_CARTE;j++)
        {
            fscanf(Carte,"%c ",&symbole);
            indice=chercherIndiceTypeTerrain(symbole,typesTerrains);
            carte[i][j].idTypeTerrain=typesTerrains[indice].idTypeTerrain;
            carte[i][j].codeAffichageTerrain=typesTerrains[indice].codeAffichageTerrain;
            carte[i][j].idUnite=0;
            carte[i][j].symboleJoueur=' ';
            carte[i][j].marque=0;
        }
    }
    fclose(Carte);
}

void chargerUnitesJoueursSauvegardeesVersTableau(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS], char* nomFichier)
{
    int i=0,j=0;
    FILE* Fichier=fopen(nomFichier,"r");

    while (i < MAX_LIGNES_UNITESJOUEURS )
    {

        fscanf(Fichier,"%d %d %d %s %d %d %d %d ",&unitesJoueurs[i].idUnite,&unitesJoueurs[i].idFicheTypeUnite,&unitesJoueurs[i].idJoueur,
               unitesJoueurs[i].nomUnite,&unitesJoueurs[i].active,&unitesJoueurs[i].rang,&unitesJoueurs[i].ligne,&unitesJoueurs[i].colonne);
        for (j=0;j<MAX_TRAITS;j++)
        {
            fscanf(Fichier,"%d ",&unitesJoueurs[i].traits[j]);
        }

        fscanf(Fichier,"%d %d %d %d %d %d %d %d %d ",&unitesJoueurs[i].pv,&unitesJoueurs[i].pvMax,&unitesJoueurs[i].mouvements,&unitesJoueurs[i].mouvementsMax,&unitesJoueurs[i].xp,&unitesJoueurs[i].xpRequise,&unitesJoueurs[i].niveau,&unitesJoueurs[i].alignement,&unitesJoueurs[i].finTour);

        for(j=0;j<MAX_EFFETS_ATTAQUES_SUBITES;j++)

        {
            fscanf(Fichier,"%d %d ",&unitesJoueurs[i].effetsAttaquesSubites[j].idAttaqueSpeciale,&unitesJoueurs[i].effetsAttaquesSubites[j].toursRestants);
        }
        i++;

    }

    fclose(Fichier);
}

void placerUnitesDansCarte(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS],
                           CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE])
{ int i=0, indice;
  for (i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
  {
      carte[unitesJoueurs[i].ligne][unitesJoueurs[i].colonne].idUnite=unitesJoueurs[i].idUnite;
      indice=chercherIndiceJoueur(unitesJoueurs[i].idJoueur,nbJoueurs,joueurs);
      carte[unitesJoueurs[i].ligne][unitesJoueurs[i].colonne].symboleJoueur=joueurs[indice].symbole;

  }

}

void completerInitialisationUnitesJoueurs(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],int nbFichesTypesUnites,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES])
{ int i=0,j=0,indice=0;
  for (i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
  {
      indice=chercherIndiceFicheTypeUnite(unitesJoueurs[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
      if (indice!=-1)
      {
      unitesJoueurs[i].pv=unitesJoueurs[i].pvMax=fichesTypesUnites[indice].pvMax;
      unitesJoueurs[i].mouvements=unitesJoueurs[i].mouvementsMax=fichesTypesUnites[indice].mouvementsMax;
      unitesJoueurs[i].xpRequise=fichesTypesUnites[indice].xpRequise;
      unitesJoueurs[i].xp=0;
      unitesJoueurs[i].finTour=0;
      unitesJoueurs[i].niveau=fichesTypesUnites[indice].niveau;
      unitesJoueurs[i].alignement=fichesTypesUnites[indice].alignement;
      }

      for(j=0;j<MAX_EFFETS_ATTAQUES_SUBITES;j++)
      {
          unitesJoueurs[i].effetsAttaquesSubites[j].idAttaqueSpeciale=0;
          unitesJoueurs[i].effetsAttaquesSubites[j].toursRestants=0;
      }

      for(j=0;j<MAX_TRAITS;j++)
      {
         appliquerTrait(i,unitesJoueurs[i].traits[j],unitesJoueurs);

      }
}
}

void completerInitialisationJoueurs(int nbJoueurs, int nbVillages,Joueur joueurs[MAX_LIGNES_JOUEURS], Village villages[MAX_LIGNES_VILLAGES], UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
{
    int i=0,j=0,k=0,possedtrait=0,nbvillages=0,nbunites=0;
    for (i=0;i<nbJoueurs;i++)
    {
        for (j=0;j<nbVillages;j++)
        {
            if (joueurs[i].idJoueur==villages[j].idJoueurProprietaire)
            {

                joueurs[i].nombreVillages++;

            }
        }

        for(k=0;k<MAX_LIGNES_UNITESJOUEURS;k++)
        {
               if (unitesJoueurs[k].idUnite!=0)
               {

                 if (joueurs[i].idJoueur==unitesJoueurs[k].idJoueur)
                  {

                    joueurs[i].nombreUnites++;
                    possedtrait=possedeTrait(TRAIT_FIDELE,unitesJoueurs[k].traits);
                     if (possedtrait==0)
                       {
                         joueurs[i].entretien+=unitesJoueurs[k].niveau;
                       }

                 }
              }
        }


        if ((joueurs[i].entretien-joueurs[i].nombreVillages)<0)
        {
            joueurs[i].revenu=2+joueurs[i].nombreVillages;
        }
        else
        {
           joueurs[i].revenu=2+joueurs[i].nombreVillages-(joueurs[i].entretien-joueurs[i].nombreVillages);
        }
    }

}

void afficherUnitesMagasin(int nbUnitesMagasin, UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN])
{   int i=0;
    printf("unites magasin:\n ");
    for(i=0;i<nbUnitesMagasin;i++)
    {
        printf("%d %d %d\n ",unitesMagasin[i].idUniteMagasin,unitesMagasin[i].idFicheTypeUnite,unitesMagasin[i].idJoueurAutorise);
    }
}

void afficherVillages(int nbVillages, Village villages[MAX_LIGNES_VILLAGES])
{   int i=0;
    printf("les villages:\n ");
    for(i=0;i<nbVillages;i++)
    {
        printf("%d %d %d %d\n",villages[i].idVillage,villages[i].ligne,villages[i].colonne,villages[i].idJoueurProprietaire);
    }
}

void afficherTypesTerrains(TypeTerrain typesTerrains[NB_TYPES_TERRAINS])
{   int i=0;
    printf("TypesTerrains:\n ");
    for(i=0;i<NB_TYPES_TERRAINS;i++)
    {
        printf("%d %c %d %s \n  ",typesTerrains[i].idTypeTerrain,typesTerrains[i].symboleTerrain,typesTerrains[i].codeAffichageTerrain,typesTerrains[i].nomTerrain);
    }
}

void afficherPeriodes(Periode periodes[NB_LIGNES_PERIODES])
{   int i=0;
    printf("Periodes:\n ");
    for(i=0;i<NB_LIGNES_PERIODES;i++)
    {
        printf("%d %s %d %d %d %d\n",periodes[i].numOrdre,periodes[i].moment,periodes[i].bonus[0],periodes[i].bonus[1],periodes[i].bonus[2],periodes[i].bonus[3]);
    }
}

void afficherFichesTypesUnites(int nbFichesTypesUnites, FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES])
{   int j=0,i=0;
    printf("FichesTypesUnites:\n ");
    for(i=0;i<nbFichesTypesUnites;i++)
    {
        printf("%d %d %s %d %d %d %d %d %d\n",fichesTypesUnites[i].idFicheTypeUnite,fichesTypesUnites[i].race,
               fichesTypesUnites[i].nom,fichesTypesUnites[i].prix,fichesTypesUnites[i].pvMax,fichesTypesUnites[i].mouvementsMax,
               fichesTypesUnites[i].xpRequise,fichesTypesUnites[i].niveau,fichesTypesUnites[i].alignement);
        for (j=0;j<3;j++)
        {
        printf("%d ",fichesTypesUnites[i].idFichesTypeUniteRangInf[j]);
        }
        printf("\n");
        for(j=0;j<3;j++)
         {
          printf("%d ",fichesTypesUnites[i].idFichesTypeUniteRangSup[j]);
         }
         printf("\n");
        for(j=0;j<3;j++)
         {
           printf("%d %s %d %d %d %d %d %d",fichesTypesUnites[i].attaques[j].idAttaque,fichesTypesUnites[i].attaques[j].nomAttaque,fichesTypesUnites[i].attaques[j].degat,fichesTypesUnites[i].attaques[j].nombreAttaques,fichesTypesUnites[i].attaques[j].porteeAttaque,fichesTypesUnites[i].attaques[j].typeAttaque,fichesTypesUnites[i].attaques[j].idAttaqueSpeciale1,fichesTypesUnites[i].attaques[j].idAttaqueSpeciale2);
         }
         printf("\n");
        for (j=0;j<6;j++)
        {
          printf("%d ",fichesTypesUnites[i].resistances[j]);
        }
        printf("\n");
        for (j=0;j<17;j++)
        {
        printf("%d ",fichesTypesUnites[i].relationsTerrains[j].coutMouvement);
        }
        printf("\n");
        for (j=0;j<17;j++)
        {
          printf("%d ",fichesTypesUnites[i].relationsTerrains[j].defense);
        }
        printf("\n");
        for(j=0;j<3;j++)
        {
          printf("%d ",fichesTypesUnites[i].idCapacites[j]);
        }
        printf("\n");
        printf("\n");
    }
}

void afficherJoueurs(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS])
{
    int i;
    printf("Joueurs:\n ");
    for(i=0;i<nbJoueurs;i++)
    {
        printf("%d %c %d %d %d %d %d %d %d\n",joueurs[i].idJoueur,joueurs[i].symbole,joueurs[i].or,joueurs[i].type,joueurs[i].groupe_allies,joueurs[i].nombreVillages,joueurs[i].nombreUnites,joueurs[i].entretien,joueurs[i].revenu);
    }
}

void afficherUniteJoueur( UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
{int i=0,j=0,k=0;
  printf("Unite Joueur:\n");
 for(i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
 {

    printf("%d %d %d %s %d %d %d %d ",unitesJoueurs[i].idUnite,unitesJoueurs[i].idFicheTypeUnite,
           unitesJoueurs[i].idJoueur,unitesJoueurs[i].nomUnite,unitesJoueurs[i].active,unitesJoueurs[i].rang,
           unitesJoueurs[i].ligne,unitesJoueurs[i].colonne);
    for(j=0;j<3;j++)
     {printf("%d ",unitesJoueurs[i].traits[j]);}

     printf("%d %d %d %d %d %d %d %d %d ",unitesJoueurs[i].pv,unitesJoueurs[i].pvMax,unitesJoueurs[i].mouvements,unitesJoueurs[i].mouvementsMax,unitesJoueurs[i].xp,unitesJoueurs[i].xpRequise,unitesJoueurs[i].niveau,unitesJoueurs[i].alignement,unitesJoueurs[i].finTour);
     for(j=0;j<MAX_EFFETS_ATTAQUES_SUBITES;j++)
        {
            printf("%d %d\n",unitesJoueurs[i].effetsAttaquesSubites[j].idAttaqueSpeciale,unitesJoueurs[i].effetsAttaquesSubites[j].toursRestants);
        }
 }
}

void afficherCarte(CelluleCarte carte[NB_LIGNES_CARTE][ NB_COLONNES_CARTE])
{ int i=0 ,j=0;
  printf(" La Carte:\n");
for (i=0;i<NB_LIGNES_CARTE;i++)
    {
        for(j=0;j<NB_COLONNES_CARTE;j++)
        {
            printf("%d %d %d %c %d|",carte[i][j].idUnite,carte[i][j].idTypeTerrain,carte[i][j].codeAffichageTerrain,carte[i][j].symboleJoueur,carte[i][j].marque);

        }
        printf("\n");
    }

}

void afficherUnitesMagasinUnJoueur(int idJoueur, int nbUnitesMagasin, int nbFichesTypesUnites,UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES])
 {
     int i,j,IndiceFicheTypeUnite;
     printf("\n**  les Unites disponible pour achat:  **\n");
        for (j=0;j<109;j++)
             {
                 printf("-");
             }
             printf("\n");
             printf("|idUnite   |race      |type                |prix      |pvMax     |mvtsMax   |xpRequise |niveau   |alignement|\n");

     for (i=0;i<MAX_LIGNES_UNITESMAGASIN;i++)
     {
         if(unitesMagasin[i].idJoueurAutorise==idJoueur)
         {
             IndiceFicheTypeUnite=chercherIndiceFicheTypeUnite(unitesMagasin[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
              for (j=0;j<109;j++)
             {
                 printf("-");
             }
               printf("\n");

             printf("|%-9d |%-9d | %-18s |%-9d |%-9d |%-9d |%-9d |%-9d|%-9d |\n",unitesMagasin[i].idUniteMagasin,fichesTypesUnites[IndiceFicheTypeUnite].race,
                    fichesTypesUnites[IndiceFicheTypeUnite].nom,fichesTypesUnites[IndiceFicheTypeUnite].prix,fichesTypesUnites[IndiceFicheTypeUnite].pvMax,
                    fichesTypesUnites[IndiceFicheTypeUnite].mouvementsMax,fichesTypesUnites[IndiceFicheTypeUnite].xpRequise,fichesTypesUnites[IndiceFicheTypeUnite].niveau,
                    fichesTypesUnites[IndiceFicheTypeUnite].alignement);

         }
     }
        for (j=0;j<109;j++)
             {
                 printf("-");
             }

 }


//false(sala7tha)
void sauvegarderVillages(int nb_lignes, Village villages[MAX_LIGNES_VILLAGES], char* nomFichier)
{
    FILE* Villages=fopen(nomFichier,"w");
    for (int i=0;i<nb_lignes; i++)
     {
            fprintf(Villages,"%d %d %d %d\n",villages[i].idVillage,villages[i].ligne,villages[i].colonne,villages[i].idJoueurProprietaire);
     }
    fclose(Villages);
}
//false(sala7tha)
void sauvegarderJoueurs(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS] , char* nomFichier)
{
    FILE* joueur=fopen(nomFichier,"w");
    for (int i=0;i<nbJoueurs; i++)
     {
            fprintf(joueur,"%d %c %d %d %d %d %d %d %d\n",joueurs[i].idJoueur,
                    joueurs[i].symbole,joueurs[i].or,joueurs[i].type,joueurs[i].groupe_allies,joueurs[i].nombreVillages,
                    joueurs[i].nombreUnites,joueurs[i].entretien,joueurs[i].revenu);
     }
    fclose(joueur);
}
//false(sala7tha)
void sauvegarderUniteJoueur(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS], char* nomFichier)
{
    int i=0,j=0;
    FILE* fichier = fopen(nomFichier, "w");
    for (i=0; i < MAX_LIGNES_UNITESJOUEURS; i++)
    {   if(unitesJoueurs[i].idUnite!=0)
        {
            fprintf(fichier, "%d %d %d %s %d %d %d %d ", unitesJoueurs[i].idUnite, unitesJoueurs[i].idFicheTypeUnite, unitesJoueurs[i].idJoueur,
                unitesJoueurs[i].nomUnite, unitesJoueurs[i].active, unitesJoueurs[i].rang, unitesJoueurs[i].ligne,
                unitesJoueurs[i].colonne);

        for (j = 0; j < 3; j++)
        {
            fprintf(fichier, "%d ", unitesJoueurs[i].traits[j]);
        }
        fprintf(fichier, "%d %d %d %d %d %d %d %d %d ",unitesJoueurs[i].pv, unitesJoueurs[i].pvMax, unitesJoueurs[i].mouvements,
                 unitesJoueurs[i].mouvementsMax,unitesJoueurs[i].xp, unitesJoueurs[i].xpRequise, unitesJoueurs[i].niveau,
                 unitesJoueurs[i].alignement,unitesJoueurs[i].finTour);
         for(j=0;j<MAX_EFFETS_ATTAQUES_SUBITES;j++)

        {
            fprintf(fichier,"%d %d ",unitesJoueurs[i].effetsAttaquesSubites[j].idAttaqueSpeciale,unitesJoueurs[i].effetsAttaquesSubites[j].toursRestants);
        }
        fprintf(fichier,"\n");


        }

    }

    fclose(fichier);
}

void initialiserNouveauJeu(int *nbfichesTypesUnites, int *nbJoueurs, int *nbUnitesMagasin,
 int *nbVillages,
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Joueur joueurs[MAX_LIGNES_JOUEURS],
 CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],
 Village villages[MAX_LIGNES_VILLAGES],
 TypeTerrain typesTerrains[NB_TYPES_TERRAINS],
 Periode periodes[NB_LIGNES_PERIODES],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
 char* unitesMagasin_original="unitesMagasin_original.txt";
 char* villages_original="villages_original.txt";
 char* typesTerrains_original="typesTerrains_original.txt";
 char* periodes_original="periodes_original.txt";
 char* joueurs_original="joueurs_original.txt";
 char* fichesTypesUnites_original="fichesTypesUnites_original.txt";
 char* unitesJoueurs_original="unitesJoueurs_original.txt";
 char* carte_original="carte_original.txt";


  *nbUnitesMagasin=chargerUnitesMagasinVersTableau(unitesMagasin,unitesMagasin_original);
  *nbVillages=chargerVillagesVersTableau(villages,villages_original);
 chargerTypesTerrainsVersTableau(typesTerrains,typesTerrains_original);
 chargerPeriodesVersTableau(periodes,periodes_original);
 *nbJoueurs=chargerJoueursVersTableau(joueurs,joueurs_original);
 chargerUniteJoueurVersTableau(unitesJoueurs,unitesJoueurs_original);
 chargerCarteVersTableau(carte,typesTerrains,carte_original);
 *nbfichesTypesUnites=chargerFichesTypesUnitesVersTableau(fichesTypesUnites,fichesTypesUnites_original);
  placerUnitesDansCarte( unitesJoueurs, *nbJoueurs,joueurs,carte);
  completerInitialisationUnitesJoueurs(unitesJoueurs,*nbfichesTypesUnites,fichesTypesUnites);
  completerInitialisationJoueurs(*nbJoueurs,*nbVillages,joueurs,villages,unitesJoueurs);
}

 void chargerJeuComplet(int *nbFichesTypesUnites, int *nbJoueurs, int *nbUnitesMagasin,
 int *nbVillages,
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Joueur joueurs[MAX_LIGNES_JOUEURS],
 CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],
 Village villages[MAX_LIGNES_VILLAGES],
 TypeTerrain typesTerrains[NB_TYPES_TERRAINS],
 Periode periodes[NB_LIGNES_PERIODES],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
   char* villages_original="villages_original.txt";
   char* typesTerrains_original="typesTerrains_original.txt";
   char* periodes_original="periodes_original.txt";
   char* joueurs_original="joueurs_original.txt";
   char* fichesTypesUnites_original="fichesTypesUnites_original.txt";
   char* unitesJoueurs_original="unitesJoueurs_original.txt";
   char* carte_original="carte_original.txt";
   char* JoueursSauvegardees="JoueursSauvegardees.txt";
   char* sauvegarderuniteJoueur="sauvegarderuniteJoueur.txt";
    chargerTypesTerrainsVersTableau(typesTerrains,typesTerrains_original);
    chargerPeriodesVersTableau(periodes,periodes_original);
    chargerCarteVersTableau(carte,typesTerrains,carte_original);
    *nbFichesTypesUnites=chargerFichesTypesUnitesVersTableau(fichesTypesUnites,fichesTypesUnites_original);
    *nbJoueurs=chargerJoueursVersTableau(joueurs,joueurs_original);
    *nbVillages=chargerVillagesVersTableau(villages,villages_original);

    //chargerUniteJoueurVersTableau(unitesJoueurs,unitesJoueurs_original);
    chargerUnitesJoueursSauvegardeesVersTableau( unitesJoueurs,sauvegarderuniteJoueur);
    placerUnitesDansCarte(unitesJoueurs,*nbJoueurs,joueurs,carte);
    //completerInitialisationJoueurs(*nbJoueurs,*nbVillages,joueurs,villages,unitesJoueurs);
 }

void afficherTableauxJeu(int nbLignesFichesTypesUnites, int nbLignesJoueurs,
 int nbLignesUnitesMagasin, int nbLignesVillages,
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Joueur joueurs[MAX_LIGNES_JOUEURS],
 CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],
 Village villages[MAX_LIGNES_VILLAGES],
 TypeTerrain typesTerrains[NB_TYPES_TERRAINS],
 Periode periodes[NB_LIGNES_PERIODES],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
     afficherUnitesMagasin(nbLignesUnitesMagasin,unitesMagasin);
     printf("\n");
    afficherVillages(nbLignesVillages,villages);
    printf("\n");
    afficherTypesTerrains(typesTerrains);
    printf("\n");
    afficherPeriodes(periodes);
    printf("\n");
    afficherJoueurs(nbLignesJoueurs,joueurs);
    printf("\n");
    afficherCarte(carte);
    printf("\n");
    afficherUniteJoueur(unitesJoueurs);
    printf("\n");
    afficherFichesTypesUnites(nbLignesFichesTypesUnites,fichesTypesUnites);
    printf("\n");

 }

void sauvegarderJeuComplet(int nbLignesVillages, int nbLignesJoueurs,
 Village villages[MAX_LIGNES_VILLAGES],
 Joueur joueurs[MAX_LIGNES_JOUEURS],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {    char* sauvegarderVillage="sauvegarderVillages.txt";
      char* sauvegarderJoueur="JoueursSauvegardees.txt";
      char* sauvegarderuniteJoueur="sauvegarderuniteJoueur.txt";
     sauvegarderVillages(nbLignesVillages,villages,sauvegarderVillage);
     sauvegarderJoueurs(nbLignesJoueurs,joueurs, sauvegarderJoueur);
     sauvegarderUniteJoueur(unitesJoueurs,sauvegarderuniteJoueur);
 }

void afficherJoueurJeu(int idJoueur, int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS])
 { int indice=0;
   int i=0;
   indice=chercherIndiceJoueur(idJoueur,nbJoueurs, joueurs);
   printf("***Joueur***\n");
   for(i=0;i<101;i++)
   {
     printf("-");
   }
   printf("\n");
   printf("| idJoueur | symbole  |    or     |   type   |grp_allies|nbVillages|nbrUnites |entretien |  revenu  |\n");
   for(i=0;i<101;i++)
   {
     printf("-");
   }
   printf("\n");
   printf("|     %d    |     %c    |     %d   |     %d    |     %d    |     %d    |     %d    |     %d    |     %d   |\n",joueurs[indice].idJoueur,joueurs[indice].symbole,joueurs[indice].or,joueurs[indice].type,joueurs[indice].groupe_allies,
          joueurs[indice].nombreVillages,joueurs[indice].nombreUnites,joueurs[indice].entretien,joueurs[indice].revenu);

   for(i=0;i<101;i++)
   {
     printf("-");
   }
 }

void afficherUnitesJoueursJeu(int idJoueur, int active, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {  int i=0,j=0,x;
    char nom[20];
     printf("\n***UnitesJoueurs***\n");
     for(j=0;j<199;j++)
         {
             printf("-");
         }
         printf("\n");
         printf("| idUnite  |idFichUnit| idJoueur | nomUnite |  active  |   rang   |  ligne   |  colonne |  traits  |    pv    |  pvMax   |   mvts   | mvts Max |    xp    | xpRequise|  niveau  |alignement| finTour  |\n");
    for(j=0;j<199;j++)
         {
             printf("-");
         }
         printf("\n");
     for(i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
     {
         if((unitesJoueurs[i].idJoueur==idJoueur)&&(unitesJoueurs[i].active=active))
         {


               printf("|    %2d    |   %3d    |     %d    | %6s   |    %d     |    %d     |    %2d    |    %2d    |    %2d    |    %2d    |    %2d    |    %2d    |    %2d    |    %-2d    |    %-3d   |    %d     |    %3d   |    %d     |\n",
                       unitesJoueurs[i].idUnite, unitesJoueurs[i].idFicheTypeUnite, unitesJoueurs[i].idJoueur, unitesJoueurs[i].nomUnite,
                       unitesJoueurs[i].active, unitesJoueurs[i].rang, unitesJoueurs[i].ligne+1, unitesJoueurs[i].colonne+1, unitesJoueurs[i].traits[0],
                       unitesJoueurs[i].pv, unitesJoueurs[i].pvMax, unitesJoueurs[i].mouvements,
                       unitesJoueurs[i].mouvementsMax, unitesJoueurs[i].xp, unitesJoueurs[i].xpRequise, unitesJoueurs[i].niveau,unitesJoueurs[i].alignement, unitesJoueurs[i].finTour);
              for(j=0;j<199;j++)
              {
                printf("-");
              }
              printf("\n");

         }
     }
 }

//afficherCarteJeu
void afficherCarteJeu(int numtour,SDL_Surface* ecran,  int nbFichesTypesUnites,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES], int uniteActuelle,Periode periode,int numJoueur,CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS],int nbVillages,
 Village villages[MAX_LIGNES_VILLAGES],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
     int i=0,j=0,k=0,s=1,indice=0,IndiceVillage=0 ,oractuelle=0;
     indice=chercherIndiceJoueurHumain(nbJoueurs,joueurs);
    SDL_Surface *village = NULL, *castle = NULL, *forest = NULL, *hills = NULL, *sand = NULL;
    SDL_Surface *cave=NULL, *CoastalReef= NULL, *DeepWater= NULL, *FakeShroud= NULL;
    SDL_Surface *Flat=NULL, *Frozen = NULL, *Fungus= NULL, *Mountains= NULL,*donjon=NULL;
    SDL_Surface *ShallowWater=NULL, *Swamp= NULL, *Unwalkable= NULL,*marquer=NULL,*pied=NULL,*barinfo=NULL,*or=NULL;

    SDL_Surface *u50= NULL, *u51= NULL, *u61= NULL, *u83= NULL, *u90= NULL,*cadre=NULL;
    SDL_Surface *u115=NULL, *u130= NULL, *u149= NULL, *u233= NULL,*bladeattaq=NULL;
    SDL_Surface *u235=NULL, *u11 = NULL,*periode1=NULL,*periode2=NULL,*periode3=NULL,*periode4=NULL,*periode5=NULL,*periode6=NULL;
     SDL_Surface *U50= NULL, *U51= NULL, *U61= NULL, *U83= NULL, *U90= NULL,*arcaneattaq=NULL;
    SDL_Surface *U115=NULL, *U130= NULL, *U149= NULL,*U233= NULL,*coldattaq=NULL;
    SDL_Surface *U235=NULL,*meleattaq=NULL,*rangdattaq=NULL,*pierceattaq=NULL,*impactattaq=NULL,*fireattaq=NULL;

    SDL_Rect position,positionattaq ,centre,positionJoueur,positionlogounite,positionunite,positionhp,positionxp,positionmp,posisitiontxt,positionLVL,positionnom,positionrace;
    SDL_Event event;
    char races[21][30]={"Bats","Drakes","DuneFolk","Dwarves","Elves","Falcons","Goblins","Gryphons","Horses","Humans","Mechanical","Merfolk","Monsters","Nagas","Ogres","Orcs","Saurian","Trolls","Undead","Wolves","Woses"};
    char textAttaq[20]="Attacks",txtx[10]="x",degat[10],nombre[10];
    centre.x=960;
    centre.y=520;
    positionhp.x=1035;
    positionhp.y=275;
    positionxp.x=1035;
    positionxp.y=300;
    positionmp.x=1035;
    positionmp.y=323;
    posisitiontxt.x=1047;
    posisitiontxt.y=286;
    positionLVL.x=1050;
    positionLVL.y=360;
    positionnom.x=970;
    positionnom.y=350;

    positionrace.x=965;
    positionrace.y=365;

    positionattaq.x=960;
    positionattaq.y=420;

    positionlogounite.x=955;
    positionlogounite.y=270;
    positionunite.x=962;
    positionunite.y=276;

    int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
    int objetActuel = 0, i1 = 0, j1 = 0;
    char numeroPeriode[30],nbor[30],nbvillages[30];
    char periodemax[30]="/6",textnbvillages[30]="/14";
    char hptxt[20]="HP";
    char xptxt[20]="XP";
    char mptxt[20]="MP";
    char Tour[10]="Tour ";
    char txt[10]="/";
    char leveltxt[10]="LVL";
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
    int indiceunitej,indiceuniteE,indiceunite;

          SDL_Color noir = {0, 0, 0}; //attention ce n'est pas un Uint32
          SDL_Color blanc = { 255, 255, 255 };
          SDL_Color gold = { 153, 0, 0 };
          SDL_Color vert = { 0, 255, 0 };
          SDL_Color bleu = { 0, 0, 255 };
           SDL_Color perple={153,51,255};
            SDL_Color beje={255,204,153};
      meleattaq=IMG_Load("melee_attack.PNG");
      rangdattaq=IMG_Load("ranged_attack.PNG");
      pierceattaq=IMG_Load("pierceattaq.PNG");
     impactattaq=IMG_Load("impactattaq.PNG");
     fireattaq=IMG_Load("fireattaq.PNG");
     coldattaq=IMG_Load("coldattaq.PNG");
     bladeattaq=IMG_Load("bladeattaq.PNG");
     arcaneattaq=IMG_Load("arcaneattaq.PNG");



      U50=IMG_Load("blademaster.PNG");
     U51=IMG_Load("arcanister.PNG");
     U61=IMG_Load("pathfinder.PNG");
     U83=IMG_Load("outrider.PNG");
     U90=IMG_Load("falcon.PNG");
     U115=IMG_Load("shocktrooper.PNG");
     U130=IMG_Load("bowman.PNG");
     U149=IMG_Load("general.PNG");
     U233=IMG_Load("Sauoracle.PNG");
     U235=IMG_Load("skirmisher.PNG");

     or=IMG_Load("or.PNG");
     barinfo=IMG_Load("barinfo.PNG");

     periode1=IMG_Load("periode1.PNG");
     periode2=IMG_Load("periode2.PNG");
     periode3=IMG_Load("periode3.PNG");
     periode4=IMG_Load("periode4.PNG");
     periode5=IMG_Load("periode5.PNG");
     periode6=IMG_Load("periode6.PNG");

     u50=IMG_Load("blademaster20_25.PNG");
     u51=IMG_Load("arcanister (1).PNG");
     u61=IMG_Load("pathfinder20_25.PNG");
     u83=IMG_Load("outrider20_25.PNG");
     u90=IMG_Load("falcon20_25.PNG");
     u115=IMG_Load("shocktrooper20_25.PNG");
     u130=IMG_Load("bowman20_25.PNG");
     u149=IMG_Load("general20_25.PNG");
     u233=IMG_Load("Sauoracle20_25.PNG");
     u235=IMG_Load("skirmisher20_25.PNG");

     cadre=IMG_Load("logounite.PNG");


      donjon=IMG_Load("donjonport.PNG");
     village = IMG_Load("village20_25.PNG");
    castle = IMG_Load("castle20_25.PNG");
    forest = IMG_Load("forest20_25.PNG");
    hills = IMG_Load("hills20_25.PNG");
    sand = IMG_Load("Sand20_25.PNG");
    cave = IMG_Load("cave20_25.PNG");
    CoastalReef = IMG_Load("Coastal Reef20_25.PNG");
    DeepWater = IMG_Load("Deep Water20_25.PNG");
    FakeShroud = IMG_Load("Fake Shroud20_25.PNG");
    Flat = IMG_Load("flat20_25.PNG");
    Frozen = IMG_Load("Frozen20_25.PNG");
    Fungus = IMG_Load("Fungus20_25.PNG");
    Mountains = IMG_Load("Mountains20_25.PNG");
    ShallowWater = IMG_Load("Shallow Water20_25.PNG");
    Swamp = IMG_Load("Swamp20_25.PNG");
    Unwalkable = IMG_Load("Unwalkable20_25.PNG");
    marquer = IMG_Load("marquerr.PNG");

    // Effacement de l'�cran
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 51, 102, 0));
    TTF_Init();
        font = TTF_OpenFont("angelina.ttf", 18);
        //TTF_Font* font1=TTF_OpenFont("HeadingNowTrial-45Medium.ttf", 24);
        TTF_Font* font1=TTF_OpenFont("123Marker.ttf",11);
        TTF_Font* font2=TTF_OpenFont("Dessin123.ttf", 16);
        TTF_Font* font3=TTF_OpenFont("Dessin123.ttf", 15);
         TTF_Font* font4=TTF_OpenFont("123Marker.ttf", 17);
         TTF_Font* font5=TTF_OpenFont("123Marker.ttf", 16);
        //TTF_Font* font2=TTF_OpenFont("Dessin123.ttf", 16);
        int indicejoueur=chercherIndiceJoueur(numJoueur,nbJoueurs,joueurs);



    for(j=0;j<NB_LIGNES_CARTE;j++){
        for(k=0;k<NB_COLONNES_CARTE;k++){
            position.x=k*25;
            position.y=j*20;
            if(carte[j][k].marque==1)
            {

                     if(carte[j][k].idUnite==0)
                     {
                              if (carte[j][k].idTypeTerrain==ID_TYPE_TERRAIN_VILLAGE)
                              {

                                  SDL_BlitSurface(village, NULL, ecran, &position);
                                  SDL_BlitSurface(marquer, NULL, ecran, &position);
                              }
                              else
                              {
                               switch(carte[j][k].idTypeTerrain)
                               {
                               case 1:
                               SDL_BlitSurface(castle, NULL, ecran, &position);
                               break;
                               case 2:
                               SDL_BlitSurface(cave, NULL, ecran, &position);
                               break;
                               case 3:
                               SDL_BlitSurface(CoastalReef, NULL, ecran, &position);
                               break;
                               case 4:
                               SDL_BlitSurface(DeepWater, NULL, ecran, &position);
                               break;
                               case 5:
                               SDL_BlitSurface(FakeShroud, NULL, ecran, &position);
                               break;
                               case 6:
                               SDL_BlitSurface(Flat, NULL, ecran, &position);
                               break;
                               case 7:
                               SDL_BlitSurface(forest, NULL, ecran, &position);
                               break;
                               case 8:
                               SDL_BlitSurface(Frozen, NULL, ecran, &position);
                               break;
                               case 9:
                               SDL_BlitSurface(Fungus, NULL, ecran, &position);
                               break;
                               case 10:
                               SDL_BlitSurface(hills, NULL, ecran, &position);
                               break;
                               case 11:
                               SDL_BlitSurface(Mountains, NULL, ecran, &position);
                               break;
                               case 12:
                               SDL_BlitSurface(sand, NULL, ecran, &position);
                               break;
                               case 13:
                               SDL_BlitSurface(ShallowWater, NULL, ecran, &position);
                               break;
                               case 14:
                               SDL_BlitSurface(Swamp, NULL, ecran, &position);
                               break;
                               case 15:
                               SDL_BlitSurface(Unwalkable, NULL, ecran, &position);
                               break;
                               case 16:
                               SDL_BlitSurface(village, NULL, ecran, &position);
                               break;
                              case 17:
                              SDL_BlitSurface(donjon, NULL, ecran, &position);
                              break;
                              }
                              SDL_BlitSurface(marquer, NULL, ecran, &position);
                             }
                    }
                    else
                    {
                        indiceunitej=chercherIndiceUniteJoueur(carte[j][k].idUnite,1,1,unitesJoueurs);
                        if(indiceunitej==-1)
                        {
                             indiceunitej=chercherIndiceUniteJoueur(carte[j][k].idUnite,1,0,unitesJoueurs);
                        }
                        indiceuniteE=chercherIndiceUniteJoueur(carte[j][k].idUnite,2,1,unitesJoueurs);
                        if(indiceuniteE==-1)
                        {
                          indiceuniteE=chercherIndiceUniteJoueur(carte[j][k].idUnite,2,0,unitesJoueurs);
                        }
                        if(indiceunitej==-1)
                        {
                            indiceunite=indiceuniteE;
                        }
                        else{
                            indiceunite=indiceunitej;
                        }

                          switch(carte[j][k].idTypeTerrain)
                               {
                               case 1:
                               SDL_BlitSurface(castle, NULL, ecran, &position);
                               break;
                               case 2:
                               SDL_BlitSurface(cave, NULL, ecran, &position);
                               break;
                               case 3:
                               SDL_BlitSurface(CoastalReef, NULL, ecran, &position);
                               break;
                               case 4:
                               SDL_BlitSurface(DeepWater, NULL, ecran, &position);
                               break;
                               case 5:
                               SDL_BlitSurface(FakeShroud, NULL, ecran, &position);
                               break;
                               case 6:
                               SDL_BlitSurface(Flat, NULL, ecran, &position);
                               break;
                               case 7:
                               SDL_BlitSurface(forest, NULL, ecran, &position);
                               break;
                               case 8:
                               SDL_BlitSurface(Frozen, NULL, ecran, &position);
                               break;
                               case 9:
                               SDL_BlitSurface(Fungus, NULL, ecran, &position);
                               break;
                               case 10:
                               SDL_BlitSurface(hills, NULL, ecran, &position);
                               break;
                               case 11:
                               SDL_BlitSurface(Mountains, NULL, ecran, &position);
                               break;
                               case 12:
                               SDL_BlitSurface(sand, NULL, ecran, &position);
                               break;
                               case 13:
                               SDL_BlitSurface(ShallowWater, NULL, ecran, &position);
                               break;
                               case 14:
                               SDL_BlitSurface(Swamp, NULL, ecran, &position);
                               break;
                               case 15:
                               SDL_BlitSurface(Unwalkable, NULL, ecran, &position);
                               break;
                               case 16:
                               SDL_BlitSurface(village, NULL, ecran, &position);
                               break;
                              case 17:
                              SDL_BlitSurface(donjon, NULL, ecran, &position);
                              break;
                              }

                                switch(unitesJoueurs[indiceunite].idFicheTypeUnite)
                                      {
                                         case 50:
                                         SDL_BlitSurface(u50, NULL, ecran, &position);
                                         break;
                                         case 51:
                                         SDL_BlitSurface(u51, NULL, ecran, &position);
                                         break;
                                         case 61:
                                         SDL_BlitSurface(u61, NULL, ecran, &position);
                                         break;
                                         case 83:
                                         SDL_BlitSurface(u83, NULL, ecran, &position);
                                         break;
                                         case 90:
                                         SDL_BlitSurface(u90, NULL, ecran, &position);
                                          break;
                                          case 115:
                                          SDL_BlitSurface(u115, NULL, ecran, &position);
                                           break;
                                           case 130:
                                           SDL_BlitSurface(u130, NULL, ecran, &position);
                                            break;
                                            case 149:
                                            SDL_BlitSurface(u149, NULL, ecran, &position);
                                           break;
                                          case 233:
                                          SDL_BlitSurface(u233, NULL, ecran, &position);
                                           break;
                                         case 235:
                                          SDL_BlitSurface(u235, NULL, ecran, &position);
                                           break;
                                      }

                             }
                      }

                else{
                     if(carte[j][k].idUnite==0)
                     {
                         if (carte[j][k].idTypeTerrain==ID_TYPE_TERRAIN_VILLAGE)
                         {
                             SDL_BlitSurface(village, NULL, ecran, &position);
                             //IndiceVillage=chercherIndiceVillage(j,k,nbVillages,villages);
                         }
                         else
                         {
                             //printf("|%c    ",carte[j][k].codeAffichageTerrain,carte[j][k].symboleJoueur,carte[j][k].idUnite);
                           switch(carte[j][k].idTypeTerrain)
                           {
                               case 1:
                               SDL_BlitSurface(castle, NULL, ecran, &position);
                               break;
                               case 2:
                               SDL_BlitSurface(cave, NULL, ecran, &position);
                               break;
                               case 3:
                               SDL_BlitSurface(CoastalReef, NULL, ecran, &position);
                               break;
                               case 4:
                               SDL_BlitSurface(DeepWater, NULL, ecran, &position);
                               break;
                               case 5:
                               SDL_BlitSurface(FakeShroud, NULL, ecran, &position);
                               break;
                               case 6:
                               SDL_BlitSurface(Flat, NULL, ecran, &position);
                               break;
                               case 7:
                               SDL_BlitSurface(forest, NULL, ecran, &position);
                               break;
                               case 8:
                               SDL_BlitSurface(Frozen, NULL, ecran, &position);
                               break;
                               case 9:
                               SDL_BlitSurface(Fungus, NULL, ecran, &position);
                               break;
                               case 10:
                               SDL_BlitSurface(hills, NULL, ecran, &position);
                               break;
                               case 11:
                               SDL_BlitSurface(Mountains, NULL, ecran, &position);
                               break;
                               case 12:
                               SDL_BlitSurface(sand, NULL, ecran, &position);
                               break;
                               case 13:
                               SDL_BlitSurface(ShallowWater, NULL, ecran, &position);
                               break;
                               case 14:
                               SDL_BlitSurface(Swamp, NULL, ecran, &position);
                               break;
                               case 15:
                               SDL_BlitSurface(Unwalkable, NULL, ecran, &position);
                               break;
                               case 16:
                               SDL_BlitSurface(village, NULL, ecran, &position);
                               break;
                              case 17:
                              SDL_BlitSurface(donjon, NULL, ecran, &position);
                              break;
                            }

                         }
                     }
                     else
                     {
                         //SDL_BlitSurface(Flat, NULL, ecran, &position);
                        //printf("|%c%c%-3d",carte[j][k].codeAffichageTerrain,carte[j][k].symboleJoueur,carte[j][k].idUnite);
                        switch(carte[j][k].idTypeTerrain)
                           {
                               case 1:
                               SDL_BlitSurface(castle, NULL, ecran, &position);
                               break;
                               case 2:
                               SDL_BlitSurface(cave, NULL, ecran, &position);
                               break;
                               case 3:
                               SDL_BlitSurface(CoastalReef, NULL, ecran, &position);
                               break;
                               case 4:
                               SDL_BlitSurface(DeepWater, NULL, ecran, &position);
                               break;
                               case 5:
                               SDL_BlitSurface(FakeShroud, NULL, ecran, &position);
                               break;
                               case 6:
                               SDL_BlitSurface(Flat, NULL, ecran, &position);
                               break;
                               case 7:
                               SDL_BlitSurface(forest, NULL, ecran, &position);
                               break;
                               case 8:
                               SDL_BlitSurface(Frozen, NULL, ecran, &position);
                               break;
                               case 9:
                               SDL_BlitSurface(Fungus, NULL, ecran, &position);
                               break;
                               case 10:
                               SDL_BlitSurface(hills, NULL, ecran, &position);
                               break;
                               case 11:
                               SDL_BlitSurface(Mountains, NULL, ecran, &position);
                               break;
                               case 12:
                               SDL_BlitSurface(sand, NULL, ecran, &position);
                               break;
                               case 13:
                               SDL_BlitSurface(ShallowWater, NULL, ecran, &position);
                               break;
                               case 14:
                               SDL_BlitSurface(Swamp, NULL, ecran, &position);
                               break;
                               case 15:
                               SDL_BlitSurface(Unwalkable, NULL, ecran, &position);
                               break;
                               case 16:
                               SDL_BlitSurface(village, NULL, ecran, &position);
                               break;
                              case 17:
                              SDL_BlitSurface(donjon, NULL, ecran, &position);
                              break;
                            }
                        indiceunitej=chercherIndiceUniteJoueur(carte[j][k].idUnite,1,1,unitesJoueurs);
                        if(indiceunitej==-1)
                        {
                             indiceunitej=chercherIndiceUniteJoueur(carte[j][k].idUnite,1,0,unitesJoueurs);
                        }
                        indiceuniteE=chercherIndiceUniteJoueur(carte[j][k].idUnite,2,1,unitesJoueurs);
                        if(indiceuniteE==-1)
                        {
                        indiceuniteE=chercherIndiceUniteJoueur(carte[j][k].idUnite,2,0,unitesJoueurs);
                        }
                        if(indiceunitej==-1)
                        {
                            indiceunite=indiceuniteE;
                        }
                        else{
                            indiceunite=indiceunitej;
                        }

                            switch(unitesJoueurs[indiceunite].idFicheTypeUnite)
                                      {
                                         case 50:
                                         SDL_BlitSurface(u50, NULL, ecran, &position);
                                         break;
                                         case 51:
                                         SDL_BlitSurface(u51, NULL, ecran, &position);
                                         break;
                                         case 61:
                                         SDL_BlitSurface(u61, NULL, ecran, &position);
                                         break;
                                         case 83:
                                         SDL_BlitSurface(u83, NULL, ecran, &position);
                                         break;
                                         case 90:
                                         SDL_BlitSurface(u90, NULL, ecran, &position);
                                          break;
                                          case 115:
                                          SDL_BlitSurface(u115, NULL, ecran, &position);
                                           break;
                                           case 130:
                                           SDL_BlitSurface(u130, NULL, ecran, &position);
                                            break;
                                            case 149:
                                            SDL_BlitSurface(u149, NULL, ecran, &position);
                                           break;
                                          case 233:
                                          SDL_BlitSurface(u233, NULL, ecran, &position);
                                           break;
                                         case 235:
                                          SDL_BlitSurface(u235, NULL, ecran, &position);
                                           break;
                                      }

                    }

           }

 }
}

                  /*char tour[30]="TOUR HUMANS";
                 texte= TTF_RenderText_Blended(font,tour,blanc);
                 SDL_BlitSurface(texte, NULL, ecran, &centre);*/


position.x=950;
position.y=0;
SDL_BlitSurface(barinfo, NULL, ecran, &position);
position.x=1030;
position.y=236;
SDL_BlitSurface(or, NULL, ecran, &position);
position.x=960;
position.y=240;
SDL_BlitSurface(village, NULL, ecran, &position);
    position.x=1065;
    position.y=242;
    oractuelle=joueurs[indicejoueur].or;
    if(oractuelle<=0){
        sprintf(nbor,"%d",0);
    }
    else{
        sprintf(nbor,"%d",oractuelle);
    }

    texte= TTF_RenderText_Blended(font,nbor,blanc);
    SDL_BlitSurface(texte, NULL, ecran, &position);
    position.x=992;
    position.y=242;
    sprintf(nbvillages,"%d",joueurs[indicejoueur].nombreVillages);
    strcat(nbvillages,textnbvillages);
    texte= TTF_RenderText_Blended(font,nbvillages,blanc);
    SDL_BlitSurface(texte, NULL, ecran, &position);



  position.x=952;
  position.y=180;
  switch(periode.numOrdre)
  {
    case 1:
        SDL_BlitSurface(periode1, NULL, ecran, &position);
        break;
    case 2:
        SDL_BlitSurface(periode2, NULL, ecran, &position);
        break;
    case 3:
        SDL_BlitSurface(periode3, NULL, ecran, &position);
        break;
    case 4:
        SDL_BlitSurface(periode4, NULL, ecran, &position);
        break;
    case 5:
        SDL_BlitSurface(periode5, NULL, ecran, &position);
        break;
    case 6:
        SDL_BlitSurface(periode6, NULL, ecran, &position);
        break;


  }
   SDL_BlitSurface(cadre, NULL, ecran, &positionlogounite);

    if(uniteActuelle!=0){

        int indiceDansUnitesJoueurs=chercherIndiceUniteJoueur(uniteActuelle,1,1,unitesJoueurs);
        int indicedansfichetypeunite=chercherIndiceFicheTypeUnite(unitesJoueurs[indiceDansUnitesJoueurs].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
        if(indiceDansUnitesJoueurs==-1)
        {
            indiceDansUnitesJoueurs=chercherIndiceUniteJoueur(uniteActuelle,2,1,unitesJoueurs);
            indicedansfichetypeunite=chercherIndiceFicheTypeUnite(unitesJoueurs[indiceDansUnitesJoueurs].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);

        }

                        switch(unitesJoueurs[indiceDansUnitesJoueurs].idFicheTypeUnite)
                        {
                                        case 50:
                                         SDL_BlitSurface(U50, NULL, ecran, &positionunite);
                                         break;
                                         case 51:
                                         SDL_BlitSurface(U51, NULL, ecran, &positionunite);
                                         break;
                                         case 61:
                                         SDL_BlitSurface(U61, NULL, ecran, &positionunite);
                                         break;
                                         case 83:
                                         SDL_BlitSurface(U83, NULL, ecran, &positionunite);
                                         break;
                                         case 90:
                                         SDL_BlitSurface(U90, NULL, ecran, &positionunite);
                                          break;
                                          case 115:
                                          SDL_BlitSurface(U115, NULL, ecran, &positionunite);
                                           break;
                                           case 130:
                                           SDL_BlitSurface(U130, NULL, ecran, &positionunite);
                                            break;
                                            case 149:
                                            SDL_BlitSurface(U149, NULL, ecran, &positionunite);
                                            break;
                                            case 233:
                                            SDL_BlitSurface(U233, NULL, ecran, &positionunite);
                                            break;
                                            case 235:
                                            SDL_BlitSurface(U235, NULL, ecran, &positionunite);
                                           break;

                           }

                       texte= TTF_RenderText_Blended(font1,hptxt,blanc);
                       SDL_BlitSurface(texte, NULL, ecran, &positionhp);

                       char hpunite[20];
                       sprintf(hpunite,"%d",unitesJoueurs[indiceDansUnitesJoueurs].pv);
                         texte= TTF_RenderText_Blended(font1,hpunite,vert);
                         positionhp.y+=11;
                       SDL_BlitSurface(texte, NULL, ecran, &positionhp);


                       texte= TTF_RenderText_Blended(font1,xptxt,blanc);
                       SDL_BlitSurface(texte, NULL, ecran, &positionxp);


                       sprintf(hpunite,"%d",unitesJoueurs[indiceDansUnitesJoueurs].xp);
                         texte= TTF_RenderText_Blended(font1,hpunite,perple);
                         positionxp.y+=11;
                       SDL_BlitSurface(texte, NULL, ecran, &positionxp);


                       texte= TTF_RenderText_Blended(font1,mptxt,blanc);
                       SDL_BlitSurface(texte, NULL, ecran, &positionmp);

                        sprintf(hpunite,"%d",unitesJoueurs[indiceDansUnitesJoueurs].mouvements);
                        texte= TTF_RenderText_Blended(font1,hpunite,blanc);
                        positionmp.y+=11;
                       SDL_BlitSurface(texte, NULL, ecran, &positionmp);

                       int indicefichetypeunite=chercherIndiceFicheTypeUnite(unitesJoueurs[indiceDansUnitesJoueurs].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);


                        sprintf(hpunite,"%d",fichesTypesUnites[indicefichetypeunite].xpRequise);
                         texte= TTF_RenderText_Blended(font1,hpunite,perple);
                         positionxp.x+=20;
                          SDL_BlitSurface(texte, NULL, ecran, &positionxp);


                        sprintf(hpunite,"%d",fichesTypesUnites[indicefichetypeunite].mouvementsMax);
                         texte= TTF_RenderText_Blended(font1,hpunite,blanc);
                         positionmp.x+=18;
                         SDL_BlitSurface(texte, NULL, ecran, &positionmp);

                       sprintf(hpunite,"%d",fichesTypesUnites[indicefichetypeunite].pvMax);
                         texte= TTF_RenderText_Blended(font1,hpunite,vert);
                         positionhp.x+=20;
                          SDL_BlitSurface(texte, NULL, ecran, &positionhp);




                        texte= TTF_RenderText_Blended(font1,txt,vert);
                        SDL_BlitSurface(texte, NULL, ecran, &posisitiontxt);
                        posisitiontxt.y+=25;
                        texte= TTF_RenderText_Blended(font1,txt,perple);
                        SDL_BlitSurface(texte, NULL, ecran, &posisitiontxt);
                         posisitiontxt.y+=23;
                        texte= TTF_RenderText_Blended(font1,txt,blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &posisitiontxt);

                          texte= TTF_RenderText_Blended(font1,leveltxt,blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &positionLVL);
                                         char level[10];
                        positionLVL.x+=23;
                        sprintf(level,"%d",unitesJoueurs[indiceDansUnitesJoueurs].niveau);

                        texte= TTF_RenderText_Blended(font1,level,blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &positionLVL);
                        char nom[30];
                         texte= TTF_RenderText_Blended(font3,unitesJoueurs[indiceDansUnitesJoueurs].nomUnite,blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &positionnom);
                        positionnom.y+=10;

                        int race=fichesTypesUnites[indicefichetypeunite].race;
                        texte= TTF_RenderText_Blended(font4,races[race-1],blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &positionrace);

                        positionattaq.y-=25;
                         texte= TTF_RenderText_Blended(font5,textAttaq,beje);
                        SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                        positionattaq.y+=25;



                    for(int j=0;j<MAX_ATTAQUES;j++){
                      if(fichesTypesUnites[indicefichetypeunite].attaques[j].idAttaque!=0){
                        switch(fichesTypesUnites[indicefichetypeunite].attaques[j].porteeAttaque)
                        {
                            case 0:
                                SDL_BlitSurface(meleattaq, NULL, ecran, &positionattaq);
                                 break;
                            case 1:
                                SDL_BlitSurface(rangdattaq, NULL, ecran, &positionattaq);
                                 break;

                        }
                        positionattaq.x+=20;
                    switch(fichesTypesUnites[indicefichetypeunite].attaques[j].typeAttaque)
                        {
                            case 0:
                                SDL_BlitSurface(bladeattaq, NULL, ecran, &positionattaq);
                                 break;
                            case 1:
                                SDL_BlitSurface(pierceattaq, NULL, ecran, &positionattaq);
                                 break;

                             case 2:
                                SDL_BlitSurface(impactattaq, NULL, ecran, &positionattaq);
                                 break;
                            case 3:
                                SDL_BlitSurface(fireattaq, NULL, ecran, &positionattaq);
                                 break;
                             case 4:
                                SDL_BlitSurface(coldattaq, NULL, ecran, &positionattaq);
                                 break;
                            case 5:
                                SDL_BlitSurface(arcaneattaq, NULL, ecran, &positionattaq);
                                 break;

                        }
                        positionattaq.x+=20;
                          sprintf(degat,"%d",fichesTypesUnites[indicefichetypeunite].attaques[j].degat);
                         texte= TTF_RenderText_Blended(font1,degat,blanc);
                          SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                           positionattaq.x+=14;
                            texte= TTF_RenderText_Blended(font1,txtx,blanc);
                          SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                           positionattaq.x+=8;
                            sprintf(nombre,"%d",fichesTypesUnites[indicefichetypeunite].attaques[j].nombreAttaques);
                         texte= TTF_RenderText_Blended(font1,nombre,blanc);
                          SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                          positionattaq.x+=10;
                          char nomattaq[20];
                          strcpy(nomattaq,fichesTypesUnites[indicefichetypeunite].attaques[j].nomAttaque);
                            texte= TTF_RenderText_Blended(font1,nomattaq,blanc);
                          SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                           positionattaq.x=960;
                          positionattaq.y=440;
                      }
                    }




                  }
        else{

             SDL_BlitSurface(U149, NULL, ecran, &positionunite);

                       texte= TTF_RenderText_Blended(font1,hptxt,blanc);
                       SDL_BlitSurface(texte, NULL, ecran, &positionhp);

                            char hpunite[20];
                       sprintf(hpunite,"%d",unitesJoueurs[6].pv);
                         texte= TTF_RenderText_Blended(font1,hpunite,vert);
                         positionhp.y+=11;
                       SDL_BlitSurface(texte, NULL, ecran, &positionhp);


                       int indicefichetypeunite=chercherIndiceFicheTypeUnite(unitesJoueurs[6].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
                       sprintf(hpunite,"%d",fichesTypesUnites[indicefichetypeunite].pvMax);
                         texte= TTF_RenderText_Blended(font1,hpunite,vert);
                         positionhp.x+=20;
                          SDL_BlitSurface(texte, NULL, ecran, &positionhp);





                       texte= TTF_RenderText_Blended(font1,xptxt,blanc);
                       SDL_BlitSurface(texte, NULL, ecran, &positionxp);

                        sprintf(hpunite,"%d",unitesJoueurs[6].xp);
                         texte= TTF_RenderText_Blended(font1,hpunite,perple);
                         positionxp.y+=11;
                       SDL_BlitSurface(texte, NULL, ecran, &positionxp);


                       sprintf(hpunite,"%d",fichesTypesUnites[indicefichetypeunite].xpRequise);
                         texte= TTF_RenderText_Blended(font1,hpunite,perple);
                         positionxp.x+=20;
                          SDL_BlitSurface(texte, NULL, ecran, &positionxp);




                       texte= TTF_RenderText_Blended(font1,mptxt,blanc);
                       SDL_BlitSurface(texte, NULL, ecran, &positionmp);

                        sprintf(hpunite,"%d",unitesJoueurs[6].mouvements);
                        texte= TTF_RenderText_Blended(font1,hpunite,blanc);
                        positionmp.y+=11;
                       SDL_BlitSurface(texte, NULL, ecran, &positionmp);



                         sprintf(hpunite,"%d",fichesTypesUnites[indicefichetypeunite].mouvementsMax);
                         texte= TTF_RenderText_Blended(font1,hpunite,blanc);
                         positionmp.x+=20;
                         SDL_BlitSurface(texte, NULL, ecran, &positionmp);


                        texte= TTF_RenderText_Blended(font1,txt,vert);
                        SDL_BlitSurface(texte, NULL, ecran, &posisitiontxt);
                        posisitiontxt.y+=25;
                        texte= TTF_RenderText_Blended(font1,txt,perple);
                        SDL_BlitSurface(texte, NULL, ecran, &posisitiontxt);
                         posisitiontxt.y+=23;
                        texte= TTF_RenderText_Blended(font1,txt,blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &posisitiontxt);

                         texte= TTF_RenderText_Blended(font1,leveltxt,blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &positionLVL);

                        char level[10];
                        positionLVL.x+=23;
                        sprintf(level,"%d",unitesJoueurs[6].niveau);

                        texte= TTF_RenderText_Blended(font1,level,blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &positionLVL);

                        char nom[30];
                         texte= TTF_RenderText_Blended(font3,unitesJoueurs[6].nomUnite,blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &positionnom);
                        positionnom.y+=10;


                        int race=fichesTypesUnites[indicefichetypeunite].race;
                        texte= TTF_RenderText_Blended(font4,races[race-1],blanc);
                        SDL_BlitSurface(texte, NULL, ecran, &positionrace);

                        positionattaq.y-=25;
                         texte= TTF_RenderText_Blended(font5,textAttaq,beje);
                        SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                        positionattaq.y+=25;

                        for(int j=0;j<MAX_ATTAQUES;j++){
                         if(fichesTypesUnites[indicefichetypeunite].attaques[j].idAttaque!=0){
                          switch(fichesTypesUnites[indicefichetypeunite].attaques[j].porteeAttaque)
                        {
                            case 0:
                                SDL_BlitSurface(meleattaq, NULL, ecran, &positionattaq);
                                 break;
                            case 1:
                                SDL_BlitSurface(rangdattaq, NULL, ecran, &positionattaq);
                                 break;

                        }
                        positionattaq.x+=20;
                    switch(fichesTypesUnites[indicefichetypeunite].attaques[j].typeAttaque)
                        {
                            case 0:
                                SDL_BlitSurface(bladeattaq, NULL, ecran, &positionattaq);
                                 break;
                            case 1:
                                SDL_BlitSurface(pierceattaq, NULL, ecran, &positionattaq);
                                 break;

                             case 2:
                                SDL_BlitSurface(impactattaq, NULL, ecran, &positionattaq);
                                 break;
                            case 3:
                                SDL_BlitSurface(fireattaq, NULL, ecran, &positionattaq);
                                 break;
                             case 4:
                                SDL_BlitSurface(coldattaq, NULL, ecran, &positionattaq);
                                 break;
                            case 5:
                                SDL_BlitSurface(arcaneattaq, NULL, ecran, &positionattaq);
                                 break;

                        }
                        positionattaq.x+=20;
                          sprintf(degat,"%d",fichesTypesUnites[indicefichetypeunite].attaques[j].degat);
                         texte= TTF_RenderText_Blended(font1,degat,blanc);
                          SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                           positionattaq.x+=14;
                            texte= TTF_RenderText_Blended(font1,txtx,blanc);
                          SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                           positionattaq.x+=8;
                            sprintf(nombre,"%d",fichesTypesUnites[indicefichetypeunite].attaques[j].nombreAttaques);
                         texte= TTF_RenderText_Blended(font1,nombre,blanc);
                          SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                          positionattaq.x+=10;
                          char nomattaq[20];
                          strcpy(nomattaq,fichesTypesUnites[indicefichetypeunite].attaques[j].nomAttaque);
                            texte= TTF_RenderText_Blended(font1,nomattaq,blanc);
                          SDL_BlitSurface(texte, NULL, ecran, &positionattaq);
                          positionattaq.x=960;
                          positionattaq.y=440;
                         }
                        }


        }
    position.x=1075;
    position.y=200;
    sprintf(numeroPeriode,"%d",periode.numOrdre);
    strcat(numeroPeriode,periodemax);
    texte= TTF_RenderText_Blended(font,numeroPeriode,blanc);
    SDL_BlitSurface(texte, NULL, ecran, &position);

    position.x=1000;
    position.y=555;
    char numtourr[5];
    sprintf(numtourr,"%d",numtour);
    strcat(Tour,numtourr);
    texte= TTF_RenderText_Blended(font5,Tour,blanc);
    SDL_BlitSurface(texte, NULL, ecran, &position);


    SDL_Flip(ecran);

  // D�sactivation de la r�p�tition des touches (remise � 0)
    SDL_EnableKeyRepeat(0, 0);

    // Lib�ration des surfaces charg�es
    SDL_FreeSurface(castle);
    SDL_FreeSurface(village);
    SDL_FreeSurface(sand);
    SDL_FreeSurface(hills);
    SDL_FreeSurface(Unwalkable);
    SDL_FreeSurface(Swamp);
    SDL_FreeSurface(ShallowWater);
    SDL_FreeSurface(Mountains);
    SDL_FreeSurface(Fungus);
    SDL_FreeSurface(Frozen);
    SDL_FreeSurface(cave);
    SDL_FreeSurface(forest);
    SDL_FreeSurface(Flat);
    SDL_FreeSurface(FakeShroud);
    SDL_FreeSurface(CoastalReef);
    SDL_FreeSurface(DeepWater);
    SDL_FreeSurface(u50);
    SDL_FreeSurface(u51);
    SDL_FreeSurface(u61);
    SDL_FreeSurface(u83);
    SDL_FreeSurface(u90);
    SDL_FreeSurface(u115);
    SDL_FreeSurface(u130);
    SDL_FreeSurface(u149);
    SDL_FreeSurface(u233);
    SDL_FreeSurface(u235);
    SDL_FreeSurface(marquer);
    SDL_FreeSurface(cadre);
    SDL_FreeSurface(U50);
    SDL_FreeSurface(U51);
    SDL_FreeSurface(U61);
    SDL_FreeSurface(U83);
    SDL_FreeSurface(U90);
    SDL_FreeSurface(U115);
    SDL_FreeSurface(U130);
    SDL_FreeSurface(U149);
    SDL_FreeSurface(U233);
    SDL_FreeSurface(U235);
    SDL_FreeSurface(periode1);
    SDL_FreeSurface(periode2);
    SDL_FreeSurface(periode3);
    SDL_FreeSurface(periode4);
    SDL_FreeSurface(periode5);
    SDL_FreeSurface(periode6);
    SDL_FreeSurface(or);
    SDL_FreeSurface(barinfo);
    TTF_CloseFont(font);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);
    TTF_CloseFont(font4);
    TTF_CloseFont(font5);
       SDL_FreeSurface(texte);







 }

void marquerDeplacementsPossibles(CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],int ligneCourante, int colonneCourante,int mouvements)
 {
     int lignemax,lignemin,colonnemax,colonnemin;
      int i=0,j=0;
     colonnemax=colonneCourante+mouvements;
     colonnemin=colonneCourante-mouvements;
     lignemax=ligneCourante+mouvements;
     lignemin=ligneCourante-mouvements;

     if(colonnemin<0)
     {
      colonnemin=0;
     }
     if(lignemin<0)
     {
         lignemin=0;
     }

     for(i=lignemin;i<=lignemax;i++)
     {
         for(j=colonnemin;j<=colonnemax;j++)
         {
             if((abs(i-ligneCourante))+(abs(j-colonneCourante))<= mouvements)
             {
                        if(i>=NB_LIGNES_CARTE ||j>=NB_COLONNES_CARTE)
                        {
                            //carte[i][j].marque=0;
                        }
                        else
                        {
                            carte[i][j].marque=1;

                        }
             }

           }
 }

 }

  void effacerDeplacementsPossibles(CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE])
 {  int i=0,j=0;
      for (i=0;i<NB_LIGNES_CARTE;i++)
    {
        for(j=0;j<NB_COLONNES_CARTE;j++)
        {

            carte[i][j].marque=0;
        }
    }
 }

void deplacerUnite(int idJoueur, int indiceDansUnitesJoueurs, int numUnite,
 int ligne_courante, int colonne_courante, int nouvelle_ligne, int nouvelle_colonne,
 int nbJoueurs, int nbVillages,
 CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 Joueur joueurs[MAX_LIGNES_JOUEURS],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],
 Village villages[MAX_LIGNES_VILLAGES])
 {
      int indiceJoueur,indiceVillage;

     if (carte[nouvelle_ligne][nouvelle_colonne].idTypeTerrain==ID_TYPE_TERRAIN_VILLAGE)
     {
          indiceVillage=chercherIndiceVillage(nouvelle_ligne,nouvelle_colonne,nbVillages,villages);
          if(villages[indiceVillage].idJoueurProprietaire!=idJoueur)
         {
               if(villages[indiceVillage].idJoueurProprietaire==0)
                    {
                        indiceJoueur=chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs);
                        joueurs[indiceJoueur].nombreVillages ++;
                        miseAJourJoueurApresOccupationVillage(indiceJoueur,joueurs);
                        villages[indiceVillage].idJoueurProprietaire=idJoueur;
                    }
                else
                {
                        indiceJoueur=chercherIndiceJoueur(villages[indiceVillage].idJoueurProprietaire,nbJoueurs,joueurs);
                        joueurs[indiceJoueur].nombreVillages --;
                        miseAJourJoueurApresOccupationVillage(indiceJoueur,joueurs);
                        villages[indiceVillage].idJoueurProprietaire=idJoueur;
                }


         }


     }

        carte[nouvelle_ligne][nouvelle_colonne].idUnite=carte[ligne_courante][colonne_courante].idUnite;
         carte[nouvelle_ligne][nouvelle_colonne].symboleJoueur=carte[ligne_courante][colonne_courante].symboleJoueur;
        carte[ligne_courante][colonne_courante].idUnite=0;
        carte[ligne_courante][colonne_courante].symboleJoueur=' ';

        unitesJoueurs[indiceDansUnitesJoueurs].ligne=nouvelle_ligne;
         unitesJoueurs[indiceDansUnitesJoueurs].colonne=nouvelle_colonne;
         unitesJoueurs[indiceDansUnitesJoueurs].mouvements -=(abs(nouvelle_ligne-ligne_courante)+abs(nouvelle_colonne-colonne_courante));



 }
void miseAJourJoueurApresAchatUnite(int indiceUniteJoueur, int indiceJoueur,
 int indiceFicheTypeUnite,
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],
 Joueur joueurs[MAX_LIGNES_JOUEURS],
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES])
 { int  possedtrait;

     joueurs[ indiceJoueur].or -= fichesTypesUnites[indiceFicheTypeUnite].prix;
     joueurs[ indiceJoueur].nombreUnites ++;
      possedtrait=possedeTrait(TRAIT_FIDELE,unitesJoueurs[indiceUniteJoueur].traits);
      if(possedeTrait==0)
      {
           joueurs[ indiceJoueur].entretien += unitesJoueurs[indiceUniteJoueur].niveau;

      }
     if ((joueurs[indiceJoueur].entretien-joueurs[indiceJoueur].nombreVillages)<0)
        {
            joueurs[indiceJoueur].revenu=2+joueurs[indiceJoueur].nombreVillages;
        }
        else
        {
           joueurs[indiceJoueur].revenu=2+joueurs[indiceJoueur].nombreVillages-(joueurs[indiceJoueur].entretien-joueurs[indiceJoueur].nombreVillages);
        }



 }

int initialiserNouvelleUniteJoueur(int indiceUniteJoueur, int indiceFicheTypeUnite, int idJoueur,int ligne, int colonne,
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES])
 {
      int max ,j,x,idTrait,i;
      char nomsUnites[10][20] = {"Evace","Fralie","Enked","Marios","Ivank","Blini","Jakie","Zucas","lian","Ithoah"};
      int numUnite;
      char nomUnite[20];
     numUnite=rand()%9;


      max=chercherMaxIdUnitesJoueurs(unitesJoueurs);
      unitesJoueurs[indiceUniteJoueur].idUnite=max+1;
      unitesJoueurs[indiceUniteJoueur].idJoueur= idJoueur;
        unitesJoueurs[indiceUniteJoueur].idFicheTypeUnite=fichesTypesUnites[indiceFicheTypeUnite].idFicheTypeUnite;

        strcpy(unitesJoueurs[indiceUniteJoueur].nomUnite,nomsUnites[numUnite]);

        unitesJoueurs[indiceUniteJoueur].active=1;
        unitesJoueurs[indiceUniteJoueur].rang=0;
        unitesJoueurs[indiceUniteJoueur].ligne=ligne;
        unitesJoueurs[indiceUniteJoueur].colonne=colonne;
        x=rand()%10;
        if(x==0)
        {    for (i=0;i<3;i++)
                {
                      idTrait=rand()%16;
                      unitesJoueurs[indiceUniteJoueur].traits[i]=idTrait;
                }
        }
        else
        {

              for (i=0;i<2;i++)
                {
                      idTrait=rand()%16;
                      unitesJoueurs[indiceUniteJoueur].traits[i]=idTrait;
                }
                unitesJoueurs[indiceUniteJoueur].traits[3]=idTrait;
        }


        unitesJoueurs[indiceUniteJoueur].pv=unitesJoueurs[indiceUniteJoueur].pvMax=fichesTypesUnites[indiceFicheTypeUnite].pvMax;
        unitesJoueurs[indiceUniteJoueur].mouvements=0;
        unitesJoueurs[indiceUniteJoueur].mouvementsMax=fichesTypesUnites[indiceFicheTypeUnite].mouvementsMax;
        unitesJoueurs[indiceUniteJoueur].xp=0;
        unitesJoueurs[indiceUniteJoueur].xpRequise=fichesTypesUnites[indiceFicheTypeUnite].xpRequise;
        unitesJoueurs[indiceUniteJoueur].niveau=fichesTypesUnites[indiceFicheTypeUnite].niveau;
        unitesJoueurs[indiceUniteJoueur].alignement=fichesTypesUnites[indiceFicheTypeUnite].alignement;
        unitesJoueurs[indiceUniteJoueur].finTour=1;
        for(j=0;j<MAX_EFFETS_ATTAQUES_SUBITES;j++)
        {
          unitesJoueurs[indiceUniteJoueur].effetsAttaquesSubites[j].idAttaqueSpeciale=0;
          unitesJoueurs[indiceUniteJoueur].effetsAttaquesSubites[j].toursRestants=0;
        }
 }
/* void confirmerAchat(int uniteActuelle,int a,int b,SDL_Surface* ecran,int idJoueur, int ligneDonjon, int colonneDonjon,
int nbUnitesMagasin, int nbJoueurs, int nbFichesTypesUnites, int nbVillages,
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],
 Joueur joueurs[MAX_LIGNES_JOUEURS],FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Village villages[MAX_LIGNES_VILLAGES],CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE])
 {

     int confirmer=0;
      SDL_Rect position;
      SDL_Event event;
      SDL_Surface  *menu = NULL;
      menu=IMG_Load("RecruitCom.PNG");
      position.x=550;
      position.y=250;
      int recruterr = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0,x=0,y=0;
     while (confirmer==0)
     {

             SDL_WaitEvent(&event);
        switch(event.type)
        {
          case SDL_QUIT:
            //continuer = 0;
            break;
          case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {   if(DoubleClickDetected() == 1)
                    {


                clicDroitEnCours = 1;
                }

                else{
                    int x=event.button.x/25;
                    int y=event.button.y/20;



                    clicGaucheEnCours = 1;

            }
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {


                         clicDroitEnCours = 1;

            }

            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                clicGaucheEnCours = 0;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                clicDroitEnCours = 0;
            break;
    }
          SDL_BlitSurface(menu, NULL, ecran, &position);
     }


 }*/

void acheterUnites(int uniteActuelle,int a,int b,SDL_Surface* ecran,int idJoueur, int ligneDonjon, int colonneDonjon, int nbUnitesMagasin, int nbJoueurs, int nbFichesTypesUnites, int nbVillages,
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],
 Joueur joueurs[MAX_LIGNES_JOUEURS],FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],Village villages[MAX_LIGNES_VILLAGES],
 CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE])
 {
     int ligne ,colonne,numUnite,IndiceJoueur, IndiceUniteMagasin,IndiceFicheTypeUnite,IndiceUniteJoueur,choix;
      SDL_Surface  *magasin = NULL,*u5 = NULL, *u6 = NULL,*u7 = NULL, *u8= NULL,*recruter=NULL,*meleeattack=NULL,*confirme=NULL,
      *rangedattack=NULL,*arcaneAttaque=NULL,*fireAttaque=NULL,*pierceAttaque=NULL,*coldAttaque=NULL,*impactAttaque=NULL,
      *bladeAttaque=NULL,*spearAttaque=NULL,*confirmation=NULL;
      SDL_Rect position,positionunites,positiontext,positionprix,psitiondonner,posicon,posniv,postitre,
      posdep,posxp,porace,pospv,center,Attaquespos,porteattaque,typeAttaquepos,degatpos,positioncomfirmation;
      FicheTypeUnite unitesAacheter[10];
      int idsUnitesMagasin[10];
      SDL_Event event;
      TTF_Font* font = NULL;
      SDL_Surface *texte=NULL;

      meleeattack=IMG_Load("melee_attack.PNG");
      rangedattack=IMG_Load("ranged_attack.PNG");
      arcaneAttaque=IMG_Load("arcaneAttaq.PNG");
      fireAttaque=IMG_Load("fireAttaq.PNG");
      pierceAttaque=IMG_Load("pierceAttaq.PNG");
      coldAttaque=IMG_Load("coldAttaq.PNG");
      impactAttaque=IMG_Load("impactAttaq.PNG");
      bladeAttaque=IMG_Load("bladeAttaq.PNG");
      spearAttaque=IMG_Load("spearAttaq.PNG");
       confirmation=IMG_Load("confirmerachat.PNG");

      position.x = 0;
      position.y = 0;
      positionunites.y=100;
      positionunites.x=500;
      positiontext.x=600;

      positiontext.y=110;

      positionprix.x=580;
      positionprix.y=140;

      psitiondonner.x=30;
      psitiondonner.y=150;

      posicon.x=50;
      posicon.y=80;

      posniv.x=50;
      posniv.y=180;
      posdep.x=40;
      posxp.x=40;
      pospv.x=40;
      porace.x=40;
      porace.y=250;
      pospv.y=280;
      posxp.y=310;
      posdep.y=340;
      center.x=300;
      center.y=400;
      Attaquespos.x=35;
      Attaquespos.y=380;
      porteattaque.x=35;
      porteattaque.y=420;
      typeAttaquepos.x=58;
      typeAttaquepos.y=420;
      degatpos.x=90;
      degatpos.y=420;
      positioncomfirmation.x=170;
      positioncomfirmation.y=190;

      FicheTypeUnite uniteSelec;
      //magasin = SDL_SetVideoMode(950,600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
      //SDL_WM_SetCaption("The Battle for Wesnoth : recruter", NULL);

       TTF_Init();

        font = TTF_OpenFont("angelina.ttf", 24);
        TTF_Font* font1=TTF_OpenFont("HeadingNowTrial-45Medium.ttf", 24);
        TTF_Font* font2=TTF_OpenFont("Semi Strong - Demo.ttf", 20);
        TTF_Font* font3 = TTF_OpenFont("angelina.ttf", 20);
        TTF_Font* font4=TTF_OpenFont("123Marker.ttf", 17);
        TTF_Font* font5=TTF_OpenFont("Dessin123.ttf",15);
         u5=IMG_Load("shocktrooper.PNG");
         u6=IMG_Load("Sauoracle.PNG");
         u7=IMG_Load("skirmisher.PNG");
         u8=IMG_Load("pathfinder.PNG");
         SDL_Color noir = {0, 0, 0}; //attention ce n'est pas un Uint32
         SDL_Color blanc = { 255, 255, 255 };
          SDL_Color gold = { 204, 204, 0 };
          SDL_Color vert = { 0, 255, 0 };
          SDL_Color bleu = { 0, 0, 255 };

          SDL_Rect positionText;


    int i,j,idunite;
    Attaque Attaques[3];
    char nom[50], prixtext[50]="",race[50],prix[30],pvmax[30],xpRequise[30],
    alignemen[30],mouvementmax[30],xprequise[30],niveau[30],niveautxt[30]="Level  ",
    racetxt[30]="Race   ",pvtxt[30]="HP : ",xptxt[30]="XP : ",deplac[30]="MP : 0/ ", textAttaque[30]="Attacks";
    char races[21][30]={"Bats","Drakes","DuneFolk","Dwarves","Elves","Falcons","Goblins","Gryphons","Horses","Humans","Mechanical","Merfolk","Monsters","Nagas","Ogres","Orcs","Saurian","Trolls","Undead","Wolves","Woses"};
  //code achteter
      int recruterr = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
    int objetActuel =0,selctionner=0;
    postitre.x=20;
    postitre.y=20;

while(recruterr==1){

        SDL_WaitEvent(&event);
        switch(event.type)
        {
          case SDL_QUIT:
            //continuer = 0;
            break;
          case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {   /*if(DoubleClickDetected() == 1)
                    {


                clicDroitEnCours = 1;
                }

                else{*/
                    int x=event.button.x/25;
                    int y=event.button.y/20;
                    //char test[30];
                    //sprintf(test,"%d",x);
                    //strcpy(niveautxt,test);
                    selctionner=1;
                        //uniteSelec=unitesAacheter[2];
                    //idunite=idsUnitesMagasin[2];

                    //texte= TTF_RenderText_Blended(font,test,blanc);
                    //SDL_BlitSurface(texte, NULL, ecran, &positioncomfirmation);
                    if((x==positionunites.x/25)||((x==positionunites.x/25)+10))
                        {

                            //uniteSelec=unitesAacheter[1];
                            //idunite=idsUnitesMagasin[1];
                            if(event.button.y/20==((positionunites.y/20)+1)) //||((event.button.y/20==positionunites.y/25)+10))
                            {
                                uniteSelec=unitesAacheter[0];
                                idunite=idsUnitesMagasin[0];
                                 recruter = IMG_Load("uniteaacheter1.PNG");
                            }
                            else if((event.button.y/20==(((positionunites.y+100)/20)+1))||(event.button.y/20==(((positionunites.y+100)/20)+1)))
                            {
                                uniteSelec=unitesAacheter[1];
                                idunite=idsUnitesMagasin[1];
                                 recruter = IMG_Load("uniteaacheter2.PNG");

                            }
                            else if((event.button.y/20==(((positionunites.y+200)/20)+1))||(event.button.y/20==(((positionunites.y+200)/20)+2)))
                            {
                                uniteSelec=unitesAacheter[2];
                                idunite=idsUnitesMagasin[2];
                                 recruter = IMG_Load("uniteaacheter3.PNG");

                            }
                            else if((event.button.y/20==(((positionunites.y+300)/20)+1))||(event.button.y/20==(((positionunites.y+300)/20)+2)))
                             {

                                uniteSelec=unitesAacheter[3];
                                idunite=idsUnitesMagasin[3];
                                 recruter = IMG_Load("uniteaacheter4.PNG");
                            }

                        }
                        //annuler
                        if((30<=x && x<=36)&&(28<=y && y<=29))
                        {
                            recruterr=0;
                           //afficherCarteJeu(ecran,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);

                             //strcpy(niveautxt,"annuler");

                        }
                        //ok
                        else if((18<=x && x<=26)&&(28<=y && y<=29))
                        {
                            //strcpy(niveautxt,"tstt222");
                            //strcpy(niveautxt,"ok");
                            //afficherCarteJeu(ecran,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
                            IndiceJoueur=chercherIndiceJoueur(idJoueur, nbJoueurs,joueurs);
                            if(joueurs[IndiceJoueur].or >= uniteSelec.prix){
                                 if((carte[b/20][a/25].idUnite==0)&&(carte[b/20][a/25].idTypeTerrain==ID_TYPE_TERRAIN_CHATEAU)&&((abs((b/20)-ligneDonjon)<=MAX_ELOIGNEMENT_CHATEAU_DONJON)&&(abs((a/25)-colonneDonjon)<=MAX_ELOIGNEMENT_CHATEAU_DONJON))){
                                  IndiceUniteJoueur=chercherIndiceUniteJoueur(0,0,0,unitesJoueurs);
                                  if(IndiceUniteJoueur!=-1)
                                  {
                                       //SDL_BlitSurface(confirmation, NULL, recruter, &positioncomfirmation);
                                           //SDL_BlitSurface(confirmation, NULL, recruter, &positioncomfirmation);
                                           //SDL_Delay(1500);

                                        IndiceFicheTypeUnite=chercherIndiceFicheTypeUnite(uniteSelec.idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
                                        initialiserNouvelleUniteJoueur(IndiceUniteJoueur, IndiceFicheTypeUnite,idJoueur,a/25,b/20,unitesJoueurs,fichesTypesUnites);
                                        miseAJourJoueurApresAchatUnite(IndiceUniteJoueur, IndiceJoueur, IndiceFicheTypeUnite,unitesJoueurs,joueurs,fichesTypesUnites);
                                        carte[b/20][a/25].idUnite= unitesJoueurs[IndiceUniteJoueur].idUnite;
                                        carte[b/20][a/25].symboleJoueur=joueurs[IndiceJoueur].symbole;
                                        recruterr=0;

                                        //int x=carte[b/20][a/25].idUnite;
                                        //joueurs[IndiceJoueur].nombreUnites;
                                        //char test[30];
                                        //strcpy(test,carte[b/20][a/25].symboleJoueur);
                                        //char testtt=carte[b/20][a/25].symboleJoueur;
                                        //sprintf(test,"%d",carte[b/20][a/25].idUnite);
                                        //strcpy(niveautxt,test);
                                        //strcpy(niveautxt,x);
                                        //afficherCarteJeu(ecran,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);

                                  }
                            }
                            }
                }



                    clicGaucheEnCours = 1;

            }
            else if (event.button.button == SDL_BUTTON_RIGHT) // Le clic droit sert � effacer
            {


                         clicDroitEnCours = 1;

                        }

            break;

        case SDL_MOUSEBUTTONUP: // On d�sactive le bool�en qui disait qu'un bouton �tait enfonc�
            if (event.button.button == SDL_BUTTON_LEFT)
                clicGaucheEnCours = 0;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                clicDroitEnCours = 0;
            break;
    }


        //affichage
        // Effacement de l'�cran
        //SDL_FillRect(magasin, NULL, SDL_MapRGB(magasin->format, 255, 255, 255));
        if(selctionner==0 )
        {
              recruter = IMG_Load("uniteaacheter1.PNG");
        }

        SDL_BlitSurface(recruter, NULL, ecran, &position);
   int u=0;
     for (i=0;i<MAX_LIGNES_UNITESMAGASIN;i++)
     {
         if(unitesMagasin[i].idJoueurAutorise==idJoueur)
         {



             IndiceFicheTypeUnite=chercherIndiceFicheTypeUnite(unitesMagasin[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
              //uniteSelec=fichesTypesUnites[IndiceFicheTypeUnite];
              unitesAacheter[u]=fichesTypesUnites[IndiceFicheTypeUnite];
              idsUnitesMagasin[u]=unitesMagasin[i].idUniteMagasin;


              sprintf(nom,"%s",fichesTypesUnites[IndiceFicheTypeUnite].nom);
              sprintf(prix,"%d",fichesTypesUnites[IndiceFicheTypeUnite].prix);

              strcat(prix,prixtext);


              texte= TTF_RenderText_Blended(font,nom,blanc);
              //idunite=unitesMagasin[i].idUniteMagasin;
              for(int j=0;j<MAX_ATTAQUES;j++)
              {
                  unitesAacheter[u].attaques[j]=fichesTypesUnites[IndiceFicheTypeUnite].attaques[j];
              }
              switch(unitesMagasin[i].idUniteMagasin)
                {
                    case 5:
                         SDL_BlitSurface(u5, NULL, ecran, &positionunites);

                         texte= TTF_RenderText_Blended(font4,prix,blanc);
                         SDL_BlitSurface(texte, NULL, ecran, &positionprix);
                         if(uniteSelec.idFicheTypeUnite==unitesAacheter[0].idFicheTypeUnite)
                         {
                           texte= TTF_RenderText_Blended(font,nom,gold);
                           SDL_BlitSurface(texte, NULL, ecran, &positiontext);
                         }
                         else{
                            texte= TTF_RenderText_Blended(font,nom,blanc);
                            SDL_BlitSurface(texte, NULL, ecran, &positiontext);
                         }
      {

      }
                    break;
                    case 6:
                          SDL_BlitSurface(u6, NULL, ecran, &positionunites);

                          texte= TTF_RenderText_Blended(font4,prix,blanc);
                          SDL_BlitSurface(texte, NULL, ecran, &positionprix);
                          if(uniteSelec.idFicheTypeUnite==unitesAacheter[1].idFicheTypeUnite)
                          {
                               texte= TTF_RenderText_Blended(font,nom,gold);
                              SDL_BlitSurface(texte, NULL, ecran, &positiontext);

                          }
                          else{
                               texte= TTF_RenderText_Blended(font,nom,blanc);
                            SDL_BlitSurface(texte, NULL, ecran, &positiontext);
                          }
                    break;
                    case 7:
                         SDL_BlitSurface(u7, NULL, ecran, &positionunites);

                         texte= TTF_RenderText_Blended(font4,prix,blanc);
                         SDL_BlitSurface(texte, NULL, ecran, &positionprix);
                          if(uniteSelec.idFicheTypeUnite==unitesAacheter[2].idFicheTypeUnite)
                          {

                                 texte= TTF_RenderText_Blended(font,nom,gold);
                                 SDL_BlitSurface(texte, NULL, ecran, &positiontext);
                          }
                          else
                          {
                                texte= TTF_RenderText_Blended(font,nom,blanc);
                               SDL_BlitSurface(texte, NULL, ecran, &positiontext);
                          }
                    break;
                    case 8:
                         SDL_BlitSurface(u8, NULL, ecran, &positionunites);

                         texte= TTF_RenderText_Blended(font4,prix,blanc);
                         SDL_BlitSurface(texte, NULL, ecran, &positionprix);
                           if(uniteSelec.idFicheTypeUnite==unitesAacheter[3].idFicheTypeUnite)
                           {
                                texte= TTF_RenderText_Blended(font,nom,gold);
                               SDL_BlitSurface(texte, NULL, ecran, &positiontext);
                           }
                          else{
                                   texte= TTF_RenderText_Blended(font,nom,blanc);
                                  SDL_BlitSurface(texte, NULL, ecran, &positiontext);
                          }
                    break;
                }
                positionunites.y+=100;
                positiontext.y+=100;
                positionprix.y+=100;
                u++;
         }
     }
      if(selctionner==0)
      {
           uniteSelec=unitesAacheter[0];
          idunite=idsUnitesMagasin[0];
      }




               sprintf(nom,"%s",uniteSelec.nom);
              sprintf(pvmax,"%d",uniteSelec.pvMax);
               sprintf(alignemen,"%d",uniteSelec.alignement);
              sprintf(mouvementmax,"%d",uniteSelec.mouvementsMax);
              sprintf(xprequise,"%d",uniteSelec.xpRequise);
              //sprintf(race,"%d",uniteSelec.race);
              sprintf(niveau,"%d",uniteSelec.niveau);
              strcat(niveautxt,niveau);
              strcat(pvtxt,pvmax);
              strcat(xptxt,xprequise);
              strcat(deplac,mouvementmax);
              strcat(racetxt,races[uniteSelec.race-1]);

              texte= TTF_RenderText_Blended(font1,nom,blanc);
              SDL_BlitSurface(texte, NULL, ecran, &psitiondonner);
               texte= TTF_RenderText_Blended(font,niveautxt,blanc);
              SDL_BlitSurface(texte, NULL, ecran, &posniv);
              texte= TTF_RenderText_Blended(font2,racetxt,blanc);
              SDL_BlitSurface(texte, NULL, ecran, &porace);
               texte= TTF_RenderText_Blended(font,pvtxt,vert);
              SDL_BlitSurface(texte, NULL, ecran, &pospv);
                texte= TTF_RenderText_Blended(font,xptxt,bleu);
              SDL_BlitSurface(texte, NULL, ecran, &posxp);
               texte= TTF_RenderText_Blended(font,deplac,blanc);
              SDL_BlitSurface(texte, NULL, ecran, &posdep);
              switch(idunite)
                {
                    case 5:
                         SDL_BlitSurface(u5, NULL, ecran, &posicon);

                    break;
                    case 6:
                          SDL_BlitSurface(u6, NULL, ecran, &posicon);

                    break;
                    case 7:
                         SDL_BlitSurface(u7, NULL, ecran, &posicon);

                    break;
                    case 8:
                         SDL_BlitSurface(u8, NULL, ecran, &posicon);

                    break;
                }
//while(continuer==1){
    for(int j=0;j<3;j++)
    {
        if(uniteSelec.attaques[j].idAttaque!=0)
        {

             //sprintf(race,"%d",uniteSelec.race);
             switch(uniteSelec.attaques[j].porteeAttaque)
             {
              case 0:
                    SDL_BlitSurface(meleeattack, NULL, ecran, &porteattaque);

                break;
               case 1:
                   SDL_BlitSurface(rangedattack, NULL, ecran, &porteattaque);
                break;

             }
             porteattaque.y+=30;
             switch(uniteSelec.attaques[j].typeAttaque)
             {

                case 0:
                     SDL_BlitSurface(bladeAttaque, NULL, ecran, &typeAttaquepos);
                    break;
                case 1:
                    SDL_BlitSurface(pierceAttaque, NULL, ecran, &typeAttaquepos);
                    break;
                case 2:
                    SDL_BlitSurface(impactAttaque, NULL, ecran, &typeAttaquepos);
                    break;
                case 3:
                    SDL_BlitSurface(fireAttaque, NULL, ecran, &typeAttaquepos);
                    break;
                case 4:
                    SDL_BlitSurface(coldAttaque, NULL, ecran, &typeAttaquepos);
                    break;
                case 5:
                    SDL_BlitSurface(arcaneAttaque, NULL, ecran, &typeAttaquepos);
                    break;

             }
             typeAttaquepos.y+=30;
             char degatAttq[10];
              sprintf(degatAttq,"%d",uniteSelec.attaques[j].degat);
              //strcat(racetxt,races[uniteSelec.race-1]);
              texte= TTF_RenderText_Blended(font,degatAttq,blanc);
              SDL_BlitSurface(texte, NULL, ecran, &degatpos);
            texte= TTF_RenderText_Blended(font1,"x",blanc);
             degatpos.x+=25;
             degatpos.y-=8;
              SDL_BlitSurface(texte, NULL, ecran, &degatpos);
              degatpos.x+=25;
             degatpos.y+=8;
              char nbattaque[10];
              sprintf(nbattaque,"%d",uniteSelec.attaques[j].nombreAttaques);
              texte= TTF_RenderText_Blended(font,nbattaque,blanc);
              SDL_BlitSurface(texte, NULL, ecran, &degatpos);
              degatpos.x+=25;
              //degatpos.y-=5;
               texte= TTF_RenderText_Blended(font,uniteSelec.attaques[j].nomAttaque,blanc);
              SDL_BlitSurface(texte, NULL, ecran, &degatpos);

               degatpos.x=90;
              degatpos.y+=30;
        }
    }


    char titre[30]="Recruit Unit";
    texte= TTF_RenderText_Blended(font1,titre,gold);
    SDL_BlitSurface(texte, NULL, ecran, &postitre);
        postitre.x=560;
        postitre.y=559;
        strcpy(titre,"Recruit");
    texte= TTF_RenderText_Blended(font2,titre,blanc);
    SDL_BlitSurface(texte, NULL, ecran, &postitre);
         postitre.x=790;
        postitre.y=559;
         strcpy(titre,"cancel");
    texte= TTF_RenderText_Blended(font2,titre,blanc);
    SDL_BlitSurface(texte, NULL, ecran, &postitre);

    texte= TTF_RenderText_Blended(font1,textAttaque,blanc);
    SDL_BlitSurface(texte, NULL, ecran, &Attaquespos);



     SDL_Flip(ecran);
    postitre.x=20;
    postitre.y=20;
     positionunites.y=100;
      positionunites.x=500;
        positiontext.x=600;
      positiontext.y=110;
      positionprix.x=580;
      positionprix.y=140;
      psitiondonner.x=30;
      psitiondonner.y=150;
      posicon.x=50;
      posicon.y=80;
      posniv.x=60;
      posniv.y=180;
      posdep.x=40;
      posxp.x=40;
      pospv.x=40;
      porace.x=40;
      porace.y=250;
      pospv.y=280;
      posxp.y=310;
      posdep.y=340;
      center.x=300;
      center.y=300;
       porteattaque.y=420;
      typeAttaquepos.x=60;
      typeAttaquepos.y=420;
      degatpos.x=90;
      degatpos.y=420;
      strcpy(niveautxt,"Level");
      strcpy(racetxt,"Race ");
      strcpy(pvtxt,"HP :  ");
      strcpy(xptxt,"XP : ");
      strcpy(deplac,"MP : 0/ ");

}

            //afficherJoueurJeu(idJoueur, nbJoueurs,joueurs);
           //afficherUnitesMagasinUnJoueur(idJoueur,nbUnitesMagasin,nbFichesTypesUnites,unitesMagasin,fichesTypesUnites);
          /* do{
           printf("\n     **    Menu achat unites     **\n     ***********************************************************\n     ** 1-Acheter des nouvelles unites **\n     ** 2-Revenir au menu precedent **\n     ***********************************************************\n");
           printf("\n     donner votre choix 1/2");
           scanf("%d",&choix);
           }while(choix!=1 &&  choix!=2);
           if(choix==1)
           {


             do{
                printf("\n     Entrez la ligne de la case ou vous voulez placer l'unite achetee:");
                scanf("%d",&ligne);
                printf("\n     Entrez la colonne de la case ou vous voulez placer l'unite achetee:");
                scanf("%d",&colonne);
                }while((carte[ligne-1][colonne-1].idUnite!=0)||(carte[ligne-1][colonne-1].idTypeTerrain!=ID_TYPE_TERRAIN_CHATEAU)||((abs((ligne-1)-ligneDonjon)>MAX_ELOIGNEMENT_CHATEAU_DONJON)&&(abs((colonne-1)-colonneDonjon)>MAX_ELOIGNEMENT_CHATEAU_DONJON)));


            do{
                    printf("\n     Entrez le  numero de l'unite que vous voulez acheter :");
                    scanf("%d",&numUnite);
                IndiceUniteMagasin=chercherIndiceUniteMagasin(numUnite, idJoueur,nbUnitesMagasin,unitesMagasin);
            }while( IndiceUniteMagasin==-1);
              IndiceFicheTypeUnite=chercherIndiceFicheTypeUnite(unitesMagasin[IndiceUniteMagasin].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
              IndiceJoueur=chercherIndiceJoueur(idJoueur, nbJoueurs,joueurs);
                 if(joueurs[IndiceJoueur].or >fichesTypesUnites[IndiceFicheTypeUnite].prix)
                 {
                  IndiceUniteJoueur=chercherIndiceUniteJoueur(0,0,0,unitesJoueurs);
                  if(IndiceUniteJoueur==-1)
                  {
                      printf("\n     le nombre maximale d'unites est attande");
                  }
                  else
                  {
                      printf("\n     Confirmez l'achat (oui:1,non=0)");
                      scanf("%d",&confirmerAchat);
                      if(confirmerAchat)
                      {

                      initialiserNouvelleUniteJoueur(IndiceUniteJoueur, IndiceFicheTypeUnite,idJoueur,ligne-1,colonne-1,unitesJoueurs,fichesTypesUnites);
                      miseAJourJoueurApresAchatUnite(IndiceUniteJoueur, IndiceJoueur, IndiceFicheTypeUnite,unitesJoueurs,joueurs,fichesTypesUnites);
                      carte[ligne-1][colonne-1].idUnite= unitesJoueurs[IndiceUniteJoueur].idUnite;
                       carte[ligne-1][colonne-1].symboleJoueur=joueurs[IndiceJoueur].symbole;
                      }
                  }

                  }
                 else

                 {
                     printf("\n     Votre Or est insuffsant\n");
                 }




           }*/


     //SDL_Flip(ecran);
    //SDL_Delay (6000);

//SDL_FreeSurface(texte); //On oublie toujours pas
    //TTF_CloseFont(font);
    SDL_FreeSurface(recruter);
    SDL_FreeSurface(u5);
    SDL_FreeSurface(u6);
    SDL_FreeSurface(u7);
    SDL_FreeSurface(u8);
    SDL_FreeSurface(meleeattack);
    SDL_FreeSurface(rangedattack);
    SDL_FreeSurface(fireAttaque);
    SDL_FreeSurface(pierceAttaque);
    SDL_FreeSurface(coldAttaque);
    SDL_FreeSurface(impactAttaque);
    SDL_FreeSurface(bladeAttaque);
    SDL_FreeSurface(spearAttaque);
    TTF_CloseFont(font);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);
    TTF_CloseFont(font4);
    TTF_CloseFont(font5);
   SDL_FreeSurface(texte);

 }



int calculerCodeAttaque(int groupe_allies_joueur, int ligne_courante, int colonne_courante,int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS],
 CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE] )
 {
   int CodeAttaque=0;
   int GroupeAllies;
   if(colonne_courante+1<= NB_COLONNES_CARTE)
   {

      GroupeAllies=chercherGroupeAllies(carte[ligne_courante][colonne_courante+1].symboleJoueur,nbJoueurs,joueurs);

       if((carte[ligne_courante][colonne_courante+1].idUnite!=0)&&(GroupeAllies!=-1)&&(GroupeAllies!=groupe_allies_joueur))
       {
           CodeAttaque++;

       }
   }

   if(colonne_courante-1>=0)
   {    GroupeAllies=chercherGroupeAllies(carte[ligne_courante][colonne_courante-1].symboleJoueur,nbJoueurs,joueurs);
        if((carte[ligne_courante][colonne_courante-1].idUnite!=0)&&(GroupeAllies!=-1)&&(GroupeAllies!=groupe_allies_joueur))
       {
           CodeAttaque+=10;
       }
   }

   if(ligne_courante-1 <= NB_LIGNES_CARTE)
    {
     GroupeAllies=chercherGroupeAllies(carte[ligne_courante-1][colonne_courante].symboleJoueur,nbJoueurs,joueurs);

    if((carte[ligne_courante-1][colonne_courante].idUnite!=0)&&(GroupeAllies!=-1)&&(GroupeAllies!=groupe_allies_joueur))
       {
           CodeAttaque+=100;
       }
   }

   if(ligne_courante+1 >=0)
   {

        GroupeAllies=chercherGroupeAllies(carte[ligne_courante+1][colonne_courante].symboleJoueur,nbJoueurs,joueurs);
      if((carte[ligne_courante+1][colonne_courante].idUnite!=0)&&(GroupeAllies!=-1)&&(GroupeAllies!=groupe_allies_joueur))
       {
           CodeAttaque+=1000;
       }
   }
   return CodeAttaque;

 }

int verifierUniteAttaquable(int codeAttaque, int ligne_courante, int colonne_courante, int ligne_ennemie, int colonne_ennemie)
{
 int droite, gauche,bas,haut;
 haut=codeAttaque/1000;
 bas=(codeAttaque%1000)/100;
 gauche=(codeAttaque%100)/10;
 droite=codeAttaque%10;

 if(((droite==1)&&(ligne_courante==ligne_ennemie)&&(colonne_courante+1==colonne_ennemie))||
    ((gauche==1)&&(ligne_courante==ligne_ennemie)&&(colonne_courante-1==colonne_ennemie))||
    ((bas==1)&&(ligne_courante-1==ligne_ennemie)&&(colonne_courante==colonne_ennemie))||
    ((haut==1)&&(ligne_courante+1==ligne_ennemie)&&(colonne_courante==colonne_ennemie)))
 {

     return 1;
 }
 else
{

    return 0;

 }

}
int determinerPossibilitesAttaque(int numJoueur, int numEnnemi,int indiceficheTypeUniteJoueur, int indiceficheTypeUniteEnnemi,
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],int indiceDansUnitesJoueurs, int indiceEnnemiDansUnitesJoueurs,
 Attaque attaques[10][2], int resistances[10][2])
 {
     int i=0,j=0,typeattaque=0,nbAttaques=0;
     for (i=0;i<MAX_ATTAQUES;i++)
     {

        if(fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaque!=0)
        {
             for (j=0;j<MAX_ATTAQUES;j++)
             {


                 if(fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].porteeAttaque==fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].porteeAttaque)
                 {

                     attaques[i][0].degat=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].degat;
                     attaques[i][0].idAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaque;
                     attaques[i][0].idAttaqueSpeciale1=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaqueSpeciale1;
                     attaques[i][0].idAttaqueSpeciale2=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaqueSpeciale2;
                      strcpy(attaques[i][0].nomAttaque,fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].nomAttaque);
                    attaques[i][0].nombreAttaques=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].nombreAttaques;
                    attaques[i][0].porteeAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].porteeAttaque;
                    attaques[i][0].typeAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].typeAttaque;
                    //ennemi
                    attaques[i][1].degat=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].degat;
                     attaques[i][1].idAttaque=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].idAttaque;
                     attaques[i][1].idAttaqueSpeciale1=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].idAttaqueSpeciale1;
                     attaques[i][1].idAttaqueSpeciale2=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].idAttaqueSpeciale2;
                    strcpy(attaques[i][1].nomAttaque,fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].nomAttaque);
                    attaques[i][1].nombreAttaques=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].nombreAttaques;
                    attaques[i][1].porteeAttaque=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].porteeAttaque;
                    attaques[i][1].typeAttaque=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].typeAttaque;
                    typeattaque=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].typeAttaque;
                    resistances[i][1]= fichesTypesUnites[indiceficheTypeUniteJoueur].resistances[typeattaque];
                    typeattaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].typeAttaque;
                     resistances[i][0]=fichesTypesUnites[indiceficheTypeUniteEnnemi].resistances[typeattaque];




                 }
                 else
                 {

                     attaques[i][0].degat=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].degat;
                     attaques[i][0].idAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaque;
                     attaques[i][0].idAttaqueSpeciale1=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaqueSpeciale1;
                     attaques[i][0].idAttaqueSpeciale2=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaqueSpeciale2;
                      strcpy(attaques[i][0].nomAttaque,fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].nomAttaque);
                    attaques[i][0].nombreAttaques=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].nombreAttaques;
                    attaques[i][0].porteeAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].porteeAttaque;
                    attaques[i][0].typeAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].typeAttaque;
                    //ennemi
                    attaques[i][1].degat=0;
                     attaques[i][1].idAttaque=0;
                     attaques[i][1].idAttaqueSpeciale1=0;
                     attaques[i][1].idAttaqueSpeciale2=0;
                    strcpy(attaques[i][1].nomAttaque," ");
                    attaques[i][1].nombreAttaques=0;
                    attaques[i][1].porteeAttaque=0;
                    attaques[i][1].typeAttaque=0;
                    typeattaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].typeAttaque;
                    resistances[i][0]=fichesTypesUnites[indiceficheTypeUniteEnnemi].resistances[typeattaque];
                     resistances[i][1]=0;

                 }

             }


         nbAttaques++;
        }


     }
     return nbAttaques;
 }

int choisirAttaque(SDL_Surface* ecran,int uniteActuelle,int numJoueur, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS] ,int numEnnemi,int indiceficheTypeUniteJoueur, int indiceficheTypeUniteEnnemi,
FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],int indiceDansUnitesJoueurs, int indiceEnnemiDansUnitesJoueurs,
int defenseJoueur, int defenseEnnemi, int nbrAttaques, Attaque attaques[10][2], int resistances[10][2], Attaque attaqueChoisie[2],
int resistanceAuxAttaquesChoisis[2], Periode periode, int alignementJoueur, int alignementEnnemi)
{
   int i=0,choix;
    int degatE,degatJ;
    SDL_Surface  *menuAttaque = NULL,*menuAttaque2=NULL,*uJ = NULL, *uE = NULL,*attaque1 = NULL, *attaque2= NULL,*attaque=NULL,*AttaqueSelc=NULL,*menuAttaque1=NULL;
    SDL_Surface *u50=NULL,*u51=NULL,*u61=NULL,*u83=NULL,*u90=NULL,*u115=NULL,*u130=NULL,*u149=NULL,*u232=NULL,*u235=NULL;
    SDL_Surface *fireattaq=NULL,*impactattaq=NULL,*pierceattaq=NULL,*spearattaq=NULL,*ranged_attack=NULL,*coldattaq=NULL,*bladeattaq=NULL,*arcaneattaq=NULL;
    SDL_Rect position,positionuniteJ,positionuniteE ,centre,posattaque,posattaquE,typeattaque,ponomattaqueJ,ponomattaqueE,positionattaquechoisi;
    SDL_Rect testpo,positionnomJ,positionnomE,polevel,degatpo,nbpos,degattxtpo,resistancepo,purcentagepo,poxp,pohp,potxt,poxpmax,pohpmax,raceposition;
    SDL_Event event;
    char races[21][30]={"Bats","Drakes","DuneFolk","Dwarves","Elves","Falcons","Goblins","Gryphons","Horses","Humans","Mechanical","Merfolk","Monsters","Nagas","Ogres","Orcs","Saurian","Trolls","Undead","Wolves","Woses"};
    position.x=225;
    position.y=50;
    positionattaquechoisi.x=10;
    positionattaquechoisi.y=260;
    int continuer=1;
    centre.x=500;
    centre.y=400;
    posattaque.x=15;
    posattaque.y=270;
    positionuniteJ.x=60;
    positionuniteJ.y=100;
    testpo.x=140;
    testpo.y=60;
    poxp.x=207;
    poxp.y=194;
    pohp.x=200;
    pohp.y=172;

    poxpmax.x=221;
    poxpmax.y=194;

    pohpmax.x=221;
    pohpmax.y=172;

    potxt.x=217;
    potxt.y=172;
    positionnomJ.x=120;
    positionnomJ.y=60;

    raceposition.x=175;
    raceposition.y=133;


    positionnomE.x=260;
    positionnomE.y=60;

    polevel.x=230;
    polevel.y=99;

    positionuniteE.x=360;
    positionuniteE.y=100;
    posattaquE.x=420;
    posattaquE.y=270;
    typeattaque.x=230;
    typeattaque.y=290;
    ponomattaqueJ.x=80;
    ponomattaqueJ.y=270;
    degatpo.x=80;
    degatpo.y=295;
    degattxtpo.x=93;
    degattxtpo.y=293;
    ponomattaqueE.x=370;
    ponomattaqueE.y=270;
    nbpos.x=105;
    nbpos.y=295;
    resistancepo.x=80;
    resistancepo.y=310;
    purcentagepo.x=100;
    purcentagepo.y=310;

      int  clicGaucheEnCours = 0, clicDroitEnCours = 0;
      int objetActuel =0,selctionner=0;
      char nomattaqueJ[30],nomJ[30],nomE[30];
      int attaque1selec=0;
      int attaque2selec=0;



    TTF_Init();
        fireattaq=IMG_Load("fireAttaque.PNG");
        impactattaq=IMG_Load("impactAttaque.PNG");
        pierceattaq=IMG_Load("pierceAttaque.PNG");
        spearattaq=IMG_Load("spearAttaque.PNG");
        ranged_attack=IMG_Load("ranged_attack.PNG");
        coldattaq=IMG_Load("coldAttaque.PNG");
        bladeattaq=IMG_Load("bladeAttaque.PNG");
        arcaneattaq=IMG_Load("arcaneAttaque.PNG");
         TTF_Font* font = TTF_OpenFont("angelina.ttf", 20);
        TTF_Font* font1=TTF_OpenFont("123Marker.ttf", 17);
        TTF_Font* font2=TTF_OpenFont("Dessin123.ttf", 16);
         menuAttaque=IMG_Load("attackMenu.PNG");
         menuAttaque1=IMG_Load("choisirattaque (2).PNG");
         menuAttaque2=IMG_Load("attaqueselc2.PNG");

         //uJ=IMG_Load("Sauoracle.PNG");
         //uE=IMG_Load("skirmisher.PNG");
        u50=IMG_Load("blademaster.PNG");
     u51=IMG_Load("arcanister.PNG");
     u61=IMG_Load("pathfinder.PNG");
     u83=IMG_Load("outrider.PNG");
     u90=IMG_Load("falcon.PNG");
     u115=IMG_Load("shocktrooper.PNG");
     u130=IMG_Load("bowman.PNG");
     u149=IMG_Load("general.PNG");
     u232=IMG_Load("Sauoracle.PNG");
     u235=IMG_Load("skirmisher.PNG");

         SDL_Color noir = {0, 0, 0}; //attention ce n'est pas un Uint32
         SDL_Color blanc = { 255, 255, 255 };
          SDL_Color gold = { 204, 204, 0 };
          SDL_Color vert = { 0, 153, 0};
          SDL_Color bleu = { 0, 102, 204 };
          SDL_Color beje = { 255, 229, 204};
          SDL_Color gris = { 224, 224, 224 };
           SDL_Color vertclair = { 153, 153, 0 };
            SDL_Color rouge = { 153, 0, 0 };
            SDL_Color marron = { 255, 229, 204 };
           SDL_Surface* texte = NULL;


int sirine=1;


   int iattaque=0;
   //while(iattaque<nbrAttaques){

   while(continuer==1)
    {
            SDL_BlitSurface(menuAttaque, NULL, ecran, &position);

        SDL_WaitEvent(&event);
        switch(event.type)
        {

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {

                   int x=event.button.x;
                     int y=event.button.y;
                   if( y > 318 && y < 382)
                   {


                          attaque1selec=1;
                          attaque2selec=0;
                          menuAttaque=IMG_Load("choisirattaque (2).PNG");
                           attaqueChoisie[0].idAttaque=attaques[0][0].idAttaque;
                           attaqueChoisie[0].degat=attaques[0][0].degat;
                           attaqueChoisie[0].idAttaqueSpeciale1=attaques[0][0].idAttaqueSpeciale1;
                           attaqueChoisie[0].idAttaqueSpeciale2=attaques[0][0].idAttaqueSpeciale2;
                           strcpy( attaqueChoisie[0].nomAttaque,attaques[0][0].nomAttaque);
                           attaqueChoisie[0].nombreAttaques=attaques[0][0].nombreAttaques;
                           attaqueChoisie[0].porteeAttaque=attaques[0][0].porteeAttaque;
                           attaqueChoisie[0].typeAttaque=attaques[0][0].typeAttaque;
                           resistanceAuxAttaquesChoisis[0]=resistances[0][0];

                           attaqueChoisie[1].idAttaque=attaques[0][1].idAttaque;
                            attaqueChoisie[1].degat=attaques[0][1].degat;
                            attaqueChoisie[1].idAttaqueSpeciale1=attaques[0][1].idAttaqueSpeciale1;
                            attaqueChoisie[1].idAttaqueSpeciale2=attaques[0][1].idAttaqueSpeciale2;
                            strcpy( attaqueChoisie[1].nomAttaque,attaques[0][1].nomAttaque);
                            attaqueChoisie[1].nombreAttaques=attaques[0][1].nombreAttaques;
                            attaqueChoisie[1].porteeAttaque=attaques[0][1].porteeAttaque;
                            attaqueChoisie[1].typeAttaque=attaques[0][1].typeAttaque;
                             resistanceAuxAttaquesChoisis[1]=resistances[0][1];
                             //sirine=0;


                   }
                   else if(y > 398 && y < 451)
                   {

                       attaque2selec=1;
                       attaque1selec=0;
                       menuAttaque=IMG_Load("attaqueselc2.PNG");
                            attaqueChoisie[0].idAttaque=attaques[1][0].idAttaque;
                           attaqueChoisie[0].degat=attaques[1][0].degat;
                           attaqueChoisie[0].idAttaqueSpeciale1=attaques[1][0].idAttaqueSpeciale1;
                           attaqueChoisie[0].idAttaqueSpeciale2=attaques[1][0].idAttaqueSpeciale2;
                           strcpy( attaqueChoisie[0].nomAttaque,attaques[1][0].nomAttaque);
                           attaqueChoisie[0].nombreAttaques=attaques[1][0].nombreAttaques;
                           attaqueChoisie[0].porteeAttaque=attaques[1][0].porteeAttaque;
                           attaqueChoisie[0].typeAttaque=attaques[1][0].typeAttaque;
                           resistanceAuxAttaquesChoisis[0]=resistances[1][0];

                           attaqueChoisie[1].idAttaque=attaques[1][1].idAttaque;
                            attaqueChoisie[1].degat=attaques[1][1].degat;
                            attaqueChoisie[1].idAttaqueSpeciale1=attaques[1][1].idAttaqueSpeciale1;
                            attaqueChoisie[1].idAttaqueSpeciale2=attaques[1][1].idAttaqueSpeciale2;
                            strcpy( attaqueChoisie[1].nomAttaque,attaques[1][1].nomAttaque);
                            attaqueChoisie[1].nombreAttaques=attaques[1][1].nombreAttaques;
                            attaqueChoisie[1].porteeAttaque=attaques[1][1].porteeAttaque;
                            attaqueChoisie[1].typeAttaque=attaques[1][1].typeAttaque;
                             resistanceAuxAttaquesChoisis[1]=resistances[1][1];
                              positionattaquechoisi.y-=80;
                             //sirine=0;


                   }
                   //annuler
                   else if(x>625 && y>500)
                   {
                           attaqueChoisie[0].idAttaque=0;
                           resistanceAuxAttaquesChoisis[0]=0;

                           continuer=0;

                   }
                   //attaquer
                   else if(x<600 && y>500)
                   {
                        continuer=0;


                   }


                clicGaucheEnCours = 1;

            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {





                 clicDroitEnCours = 1;

            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                clicGaucheEnCours = 0;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                clicDroitEnCours = 0;
            break;
        }
           /* if(attaque1selec==1)
             {

                 SDL_BlitSurface(menuAttaque1, NULL, ecran, &position);
                  //attaque2selec=0;
             }
             else if(attaque2selec==1)
             {
                  SDL_BlitSurface(menuAttaque2, NULL, ecran, &position);
                  //attaque1selec=0;
             }
             else{
                 SDL_BlitSurface(menuAttaque, NULL, ecran, &position);
             }*/


            /*if(attaque1selec==1)
             {
                      // menuAttaque=IMG_Load("choisirattaque (2).PNG");
                     //menuAttaque1=IMG_Load("choisirattaque (2).PNG");
                     //menuAttaque2=IMG_Load("attaqueselc2.PNG");
                 SDL_BlitSurface(menuAttaque1, NULL, ecran, &position);
                  //attaque2selec=0;
                   //SDL_BlitSurface(menuAttaque, NULL, ecran, &position);
             }
             else if(attaque2selec==1)
             {
                  //menuAttaque=IMG_Load("attaqueselc2.PNG");
                  SDL_BlitSurface(menuAttaque2, NULL, ecran, &position);
                  //attaque1selec=0;
             }
             else{
                   menuAttaque=IMG_Load("attackMenu.PNG");
                 SDL_BlitSurface(menuAttaque, NULL, ecran, &position);
             }*/


   if(sirine==1||attaque1selec==1||attaque2selec==1){
  for(i=0;i<nbrAttaques;i++)
  {
              attaque1selec=0;
              attaque2selec=0;

              char degat[10];
               char degattxt[30]="x";
               char nb[10];
                char nbattaq[10];
                 char resistance[10];
               char pourcentage[10]="%";



            if(attaques[0][0].idAttaque!=0)
          {


              degatJ=attaques[0][0].degat+((attaques[0][0].degat/100)*periode.bonus[alignementJoueur]);

           switch(attaques[0][0].porteeAttaque)
          {
                    case 0:
                    texte= TTF_RenderText_Blended(font,"melee",beje);
                   SDL_BlitSurface(texte, NULL,menuAttaque, &typeattaque);
                    break;
                    case 1:
                    texte= TTF_RenderText_Blended(font,"ranged",beje);
                    SDL_BlitSurface(texte, NULL,menuAttaque , &typeattaque);
                    break;

          }
          typeattaque.y+=70;


               sprintf(degat,"%d",degatJ);
                texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degatpo);
                degatpo.x+=300;

                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degattxtpo);
                 degattxtpo.x+=300;

               sprintf(nbattaq,"%d",attaques[0][0].nombreAttaques);
                texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, menuAttaque,&nbpos);
               nbpos.x+=300;

          sprintf(resistance,"%d",resistances[0][0]);
          texte= TTF_RenderText_Blended(font1,resistance,vertclair);
          SDL_BlitSurface(texte, NULL, menuAttaque,&resistancepo);
           resistancepo.x+=300;



          texte= TTF_RenderText_Blended(font1,pourcentage,vertclair);
          SDL_BlitSurface(texte, NULL, menuAttaque,&purcentagepo);
          purcentagepo.x+=305;
          if( attaqueChoisie[0].idAttaque==attaques[0][0].idAttaque)
          {
              texte= TTF_RenderText_Blended(font1,attaques[0][0].nomAttaque,gold);
          }
          else{
            texte= TTF_RenderText_Blended(font1,attaques[0][0].nomAttaque,blanc);
          }

          texte= TTF_RenderText_Blended(font1,attaques[0][0].nomAttaque,blanc);
          SDL_BlitSurface(texte, NULL, menuAttaque, &ponomattaqueJ);
          ponomattaqueJ.y+=70;


        switch(attaques[0][0].typeAttaque)
          {
              case 0:
                   SDL_BlitSurface(bladeattaq, NULL, menuAttaque, &posattaque);
                    break;
               case 1:
                   SDL_BlitSurface(pierceattaq, NULL, menuAttaque, &posattaque);
                    break;
                case 2:
                   SDL_BlitSurface(impactattaq, NULL, menuAttaque, &posattaque);
                    break;
                case 3:
                   SDL_BlitSurface(fireattaq, NULL, menuAttaque, &posattaque);
                    break;
                 case 4:
                   SDL_BlitSurface(coldattaq, NULL, menuAttaque, &posattaque);
                    break;
                 case 5:
                   SDL_BlitSurface(arcaneattaq, NULL, menuAttaque, &posattaque);
                    break;
          }

           posattaque.y+=75;

          }


            if(attaques[0][1].idAttaque!=0)
            {
                degatE=attaques[0][1].degat+((attaques[0][1].degat/100)*periode.bonus[alignementEnnemi]);
            switch(attaques[0][1].typeAttaque)
          {
              case 0:
                   SDL_BlitSurface(bladeattaq, NULL, menuAttaque, &posattaquE);
                    break;
               case 1:
                   SDL_BlitSurface(pierceattaq, NULL, menuAttaque, &posattaquE);
                    break;
                case 2:
                   SDL_BlitSurface(impactattaq, NULL, menuAttaque, &posattaquE);
                    break;
                case 3:
                   SDL_BlitSurface(fireattaq, NULL, menuAttaque, &posattaquE);
                    break;
                 case 4:
                   SDL_BlitSurface(coldattaq, NULL, menuAttaque, &posattaquE);
                    break;
                 case 5:
                   SDL_BlitSurface(arcaneattaq, NULL, menuAttaque, &posattaquE);
                    break;
          }
           posattaquE.y+=75;
                sprintf(degat,"%d",degatE);
                 texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degatpo);
                degatpo.x-=300;
                degatpo.y+=72;

                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degattxtpo);
                 degattxtpo.x-=300;
                degattxtpo.y+=72;

               sprintf(nbattaq,"%d",attaques[0][1].nombreAttaques);
               texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, menuAttaque,&nbpos);
               nbpos.x-=300;
               nbpos.y+=72;

            sprintf(resistance,"%d",resistances[0][1]);
          texte= TTF_RenderText_Blended(font1,resistance,vertclair);
          SDL_BlitSurface(texte, NULL, menuAttaque,&resistancepo);
          resistancepo.x-=300;
            resistancepo.y+=70;

           texte= TTF_RenderText_Blended(font1,pourcentage,vertclair);
          SDL_BlitSurface(texte, NULL, menuAttaque,&purcentagepo);
           purcentagepo.x-=305;
            purcentagepo.y+=70;

                texte= TTF_RenderText_Blended(font1,attaques[0][1].nomAttaque,blanc);
                  SDL_BlitSurface(texte, NULL, menuAttaque, &ponomattaqueE);
                  ponomattaqueE.y+=70;


           }


           else
            {

                sprintf(degat,"%d",0);
                 texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degatpo);


                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degattxtpo);


               sprintf(nbattaq,"%d",0);
               texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, menuAttaque,&nbpos);


            sprintf(resistance,"%d",resistances[0][1]);
          texte= TTF_RenderText_Blended(font1,resistance,rouge);
          SDL_BlitSurface(texte, NULL, menuAttaque,&resistancepo);


           texte= TTF_RenderText_Blended(font1,pourcentage,rouge);
          SDL_BlitSurface(texte, NULL, menuAttaque,&purcentagepo);


             posattaquE.y+=75;
              ponomattaqueE.y+=70;
               purcentagepo.y+=70;
               resistancepo.y+=70;
               nbpos.y+=72;
              degattxtpo.y+=72;
              degatpo.y+=72;
              degatpo.x-=300;
               purcentagepo.x-=305;
               resistancepo.x-=300;
                nbpos.x-=300;
                degattxtpo.x-=300;

           }




           //attque2
          if(attaques[1][0].idAttaque!=0)
          {

             degatJ=attaques[1][0].degat+((attaques[1][0].degat/100)*periode.bonus[alignementJoueur]);

          switch(attaques[1][0].porteeAttaque)
          {
                    case 0:
                    texte= TTF_RenderText_Blended(font,"melee",beje);
                   SDL_BlitSurface(texte, NULL,menuAttaque, &typeattaque);
                    break;
                    case 1:
                    texte= TTF_RenderText_Blended(font,"ranged",beje);
                    SDL_BlitSurface(texte, NULL,menuAttaque , &typeattaque);
                    break;

          }


        switch(attaques[1][0].typeAttaque)
          {
              case 0:
                   SDL_BlitSurface(bladeattaq, NULL, menuAttaque, &posattaque);
                    break;
               case 1:
                   SDL_BlitSurface(pierceattaq, NULL, menuAttaque, &posattaque);
                    break;
                case 2:
                   SDL_BlitSurface(impactattaq, NULL, menuAttaque, &posattaque);
                    break;
                case 3:
                   SDL_BlitSurface(fireattaq, NULL, menuAttaque, &posattaque);
                    break;
                 case 4:
                   SDL_BlitSurface(coldattaq, NULL, menuAttaque, &posattaque);
                    break;
                 case 5:
                   SDL_BlitSurface(arcaneattaq, NULL, menuAttaque, &posattaque);
                    break;
          }

                sprintf(degat,"%d",degatJ);
                texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degatpo);
                degatpo.x+=300;

                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degattxtpo);
                 degattxtpo.x+=300;

                sprintf(nbattaq,"%d",attaques[1][0].nombreAttaques);
                texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, menuAttaque,&nbpos);
                nbpos.x+=300;

            sprintf(resistance,"%d",resistances[1][0]);
          texte= TTF_RenderText_Blended(font1,resistance,vertclair);
          SDL_BlitSurface(texte, NULL, menuAttaque,&resistancepo);
           resistancepo.x+=300;

            texte= TTF_RenderText_Blended(font1,pourcentage,vertclair);
          SDL_BlitSurface(texte, NULL, menuAttaque,&purcentagepo);
          purcentagepo.x+=305;
          if(attaqueChoisie[0].idAttaque==attaques[1][0].idAttaque)
          {
               texte= TTF_RenderText_Blended(font1,attaques[1][0].nomAttaque,gold);
          }
          else
          {
             texte= TTF_RenderText_Blended(font1,attaques[1][0].nomAttaque,blanc);
          }
             texte= TTF_RenderText_Blended(font1,attaques[1][0].nomAttaque,blanc);
             SDL_BlitSurface(texte, NULL, menuAttaque, &ponomattaqueJ);
          }
             if(attaques[1][1].idAttaque!=0)
             {
                 degatE=attaques[1][1].degat+((attaques[1][1].degat/100)*periode.bonus[alignementEnnemi]);

                sprintf(degat,"%d",degatE);
                 texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degatpo);

                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degattxtpo);


               sprintf(nbattaq,"%d",attaques[1][1].nombreAttaques);
               texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, menuAttaque,&nbpos);

            sprintf(resistance,"%d",resistances[1][1]);
          texte= TTF_RenderText_Blended(font1,resistance,vertclair);
          SDL_BlitSurface(texte, NULL, menuAttaque,&resistancepo);

            texte= TTF_RenderText_Blended(font1,pourcentage,vertclair);
           SDL_BlitSurface(texte, NULL, menuAttaque,&purcentagepo);


                texte= TTF_RenderText_Blended(font1,attaques[1][1].nomAttaque,blanc);
                  SDL_BlitSurface(texte, NULL, menuAttaque, &ponomattaqueE);

        switch(attaques[1][1].typeAttaque)
          {
              case 0:
                   SDL_BlitSurface(bladeattaq, NULL, menuAttaque, &posattaquE);
                    break;
               case 1:
                   SDL_BlitSurface(pierceattaq, NULL, menuAttaque, &posattaquE);
                    break;
                case 2:
                   SDL_BlitSurface(impactattaq, NULL, menuAttaque, &posattaquE);
                    break;
                case 3:
                   SDL_BlitSurface(fireattaq, NULL, menuAttaque, &posattaquE);
                    break;
                 case 4:
                   SDL_BlitSurface(coldattaq, NULL, menuAttaque, &posattaquE);
                    break;
                 case 5:
                   SDL_BlitSurface(arcaneattaq, NULL, menuAttaque, &posattaquE);
                    break;
          }


          }

            else
            {

                sprintf(degat,"%d",0);
                 texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degatpo);


                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, menuAttaque, &degattxtpo);


               sprintf(nbattaq,"%d",0);
               texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, menuAttaque,&nbpos);


            sprintf(resistance,"%d",resistances[0][1]);
          texte= TTF_RenderText_Blended(font1,resistance,rouge);
          SDL_BlitSurface(texte, NULL, menuAttaque,&resistancepo);


           texte= TTF_RenderText_Blended(font1,pourcentage,rouge);
          SDL_BlitSurface(texte, NULL, menuAttaque,&purcentagepo);

          }

           texte= TTF_RenderText_Blended(font1,fichesTypesUnites[indiceficheTypeUniteJoueur].nom,blanc);
           SDL_BlitSurface(texte, NULL, menuAttaque, &positionnomJ);

          texte= TTF_RenderText_Blended(font1,fichesTypesUnites[indiceficheTypeUniteEnnemi].nom,blanc);
         SDL_BlitSurface(texte, NULL, menuAttaque, &positionnomE);


         char level[10];
        sprintf(level,"%d",fichesTypesUnites[indiceficheTypeUniteJoueur].niveau);
        texte= TTF_RenderText_Blended(font1,level,gris);
         SDL_BlitSurface(texte, NULL, menuAttaque, &polevel);

          polevel.x=280;
        sprintf(level,"%d",fichesTypesUnites[indiceficheTypeUniteEnnemi].niveau);
        texte= TTF_RenderText_Blended(font1,level,gris);
         SDL_BlitSurface(texte, NULL, menuAttaque, &polevel);
          polevel.x=230;
          char xp[10];
          char hp[10];
          char txt[5]="/";
          char xpmax[10];
          char hptxt[10];
         sprintf(hptxt,"%d",fichesTypesUnites[indiceficheTypeUniteJoueur].pvMax);
         texte= TTF_RenderText_Blended(font2,hptxt,vert);
         SDL_BlitSurface(texte, NULL, menuAttaque, &pohpmax);
         pohpmax.x+=90;
            sprintf(hptxt,"%d",fichesTypesUnites[indiceficheTypeUniteEnnemi].pvMax);
         texte= TTF_RenderText_Blended(font2,hptxt,vert);
         SDL_BlitSurface(texte, NULL, menuAttaque, &pohpmax);


         sprintf(xpmax,"%d",fichesTypesUnites[indiceficheTypeUniteJoueur].xpRequise);
         texte= TTF_RenderText_Blended(font2,xpmax,bleu);
         SDL_BlitSurface(texte, NULL, menuAttaque, &poxpmax);
         poxpmax.x+=80;
         sprintf(xpmax,"%d",fichesTypesUnites[indiceficheTypeUniteEnnemi].xpRequise);
         texte= TTF_RenderText_Blended(font2,xpmax,bleu);
         SDL_BlitSurface(texte, NULL, menuAttaque, &poxpmax);




         texte= TTF_RenderText_Blended(font2,txt,vert);
         SDL_BlitSurface(texte, NULL, menuAttaque, &potxt);
            potxt.x+=85;
         texte= TTF_RenderText_Blended(font2,txt,vert);
         SDL_BlitSurface(texte, NULL, menuAttaque, &potxt);
          potxt.x-=86;
           potxt.y+=23;
        texte= TTF_RenderText_Blended(font2,txt,bleu);
         SDL_BlitSurface(texte, NULL, menuAttaque, &potxt);
           potxt.x+=78;
        texte= TTF_RenderText_Blended(font2,txt,bleu);
         SDL_BlitSurface(texte, NULL, menuAttaque, &potxt);



        sprintf(hp,"%d",unitesJoueurs[indiceDansUnitesJoueurs].pv);
        texte= TTF_RenderText_Blended(font2,hp,vert);
         SDL_BlitSurface(texte, NULL, menuAttaque, &pohp);



       sprintf(xp,"%d",unitesJoueurs[indiceDansUnitesJoueurs].xp);
        texte= TTF_RenderText_Blended(font2,xp,bleu);
         SDL_BlitSurface(texte, NULL, menuAttaque, &poxp);

         pohp.x+=85;
         poxp.x+=78;
        sprintf(hp,"%d",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv);
        texte= TTF_RenderText_Blended(font2,hp,vert);
         SDL_BlitSurface(texte, NULL, menuAttaque, &pohp);

           sprintf(xp,"%d",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].xp);
        texte= TTF_RenderText_Blended(font2,xp,bleu);
         SDL_BlitSurface(texte, NULL, menuAttaque, &poxp);

         char race[30];
           char race2[20];
           strcpy(race,races[fichesTypesUnites[indiceficheTypeUniteJoueur].race-1]);
          texte= TTF_RenderText_Blended(font1,race,marron);
         SDL_BlitSurface(texte, NULL, menuAttaque, &raceposition);
         raceposition.x+=80;
        strcpy(race2,races[fichesTypesUnites[indiceficheTypeUniteEnnemi].race-1]);
          texte= TTF_RenderText_Blended(font1,race2,marron);
         SDL_BlitSurface(texte, NULL, menuAttaque, &raceposition);


          switch(fichesTypesUnites[indiceficheTypeUniteJoueur].idFicheTypeUnite)
          {
                                        case 50:
                                         SDL_BlitSurface(u50, NULL, menuAttaque, &positionuniteJ);
                                         break;
                                         case 51:
                                         SDL_BlitSurface(u51, NULL, menuAttaque, &positionuniteJ);
                                         break;
                                         case 61:
                                         SDL_BlitSurface(u61, NULL, menuAttaque, &positionuniteJ);
                                         break;
                                         case 83:
                                         SDL_BlitSurface(u83, NULL, menuAttaque, &positionuniteJ);
                                         break;
                                         case 90:
                                         SDL_BlitSurface(u90, NULL, menuAttaque, &positionuniteJ);
                                          break;
                                          case 115:
                                          SDL_BlitSurface(u115, NULL, menuAttaque, &positionuniteJ);
                                           break;
                                           case 130:
                                           SDL_BlitSurface(u130, NULL, menuAttaque, &positionuniteJ);
                                            break;
                                            case 149:
                                            SDL_BlitSurface(u149, NULL, menuAttaque, &positionuniteJ);
                                           break;
                                          case 232:
                                          SDL_BlitSurface(u232, NULL, menuAttaque, &positionuniteJ);
                                           break;
                                         case 235:
                                          SDL_BlitSurface(u235, NULL, menuAttaque, &positionuniteJ);
                                           break;

          }
          switch(fichesTypesUnites[indiceficheTypeUniteEnnemi].idFicheTypeUnite )
          {
                                         case 50:
                                         SDL_BlitSurface(u50, NULL, menuAttaque, &positionuniteE);
                                         break;
                                         case 51:
                                         SDL_BlitSurface(u51, NULL, menuAttaque, &positionuniteE);
                                         break;
                                         case 61:
                                         SDL_BlitSurface(u61, NULL, menuAttaque, &positionuniteE);
                                         break;
                                         case 83:
                                         SDL_BlitSurface(u83, NULL, menuAttaque, &positionuniteE);
                                         break;
                                         case 90:
                                         SDL_BlitSurface(u90, NULL, menuAttaque, &positionuniteE);
                                          break;
                                          case 115:
                                          SDL_BlitSurface(u115, NULL, menuAttaque, &positionuniteE);
                                           break;
                                           case 130:
                                           SDL_BlitSurface(u130, NULL, menuAttaque, &positionuniteE);
                                            break;
                                            case 149:
                                            SDL_BlitSurface(u149, NULL, menuAttaque, &positionuniteE);
                                           break;
                                          case 232:
                                          SDL_BlitSurface(u232, NULL, menuAttaque, &positionuniteE);
                                           break;
                                         case 235:
                                          SDL_BlitSurface(u235, NULL, menuAttaque, &positionuniteE);
                                           break;

          }



              position.x=225;
    position.y=50;
    continuer=1;
    centre.x=500;
    centre.y=400;
    posattaque.x=15;
    posattaque.y=270;
    positionuniteJ.x=60;
    positionuniteJ.y=100;
    testpo.x=70;
    testpo.y=80;
    poxp.x=207;
    poxp.y=194;
    pohp.x=200;
    pohp.y=172;

    poxpmax.x=221;
    poxpmax.y=194;

    pohpmax.x=221;
    pohpmax.y=172;

    potxt.x=217;
    potxt.y=172;
    positionnomJ.x=120;
    positionnomJ.y=60;

    raceposition.x=175;
    raceposition.y=133;


    positionnomE.x=260;
    positionnomE.y=60;

    polevel.x=230;
    polevel.y=99;

    positionuniteE.x=360;
    positionuniteE.y=100;
    posattaquE.x=420;
    posattaquE.y=270;
    typeattaque.x=230;
    typeattaque.y=290;
    ponomattaqueJ.x=80;
    ponomattaqueJ.y=270;
    degatpo.x=80;
    degatpo.y=295;
    degattxtpo.x=93;
    degattxtpo.y=293;
    ponomattaqueE.x=370;
    ponomattaqueE.y=270;
    nbpos.x=105;
    nbpos.y=295;
    resistancepo.x=80;
    resistancepo.y=310;
    purcentagepo.x=100;
    purcentagepo.y=310;
    sirine=0;

  }
}

        SDL_Flip(ecran);


}
    SDL_FreeSurface(menuAttaque);
    SDL_FreeSurface(u50);
    SDL_FreeSurface(u61);
    SDL_FreeSurface(u51);
    SDL_FreeSurface(u83);
    SDL_FreeSurface(u130);
    SDL_FreeSurface(u115);
    SDL_FreeSurface(u232);
    SDL_FreeSurface(u235);
    SDL_FreeSurface(u90);
    SDL_FreeSurface(u51);
    SDL_FreeSurface(u149);
    SDL_FreeSurface(ranged_attack);
    SDL_FreeSurface(fireattaq);
    SDL_FreeSurface(pierceattaq);
    SDL_FreeSurface(coldattaq);
    SDL_FreeSurface(impactattaq);
    SDL_FreeSurface(bladeattaq);
    SDL_FreeSurface(spearattaq);
    TTF_CloseFont(font);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);





}



void miseAJourJoueurApresMortUnite(int indiceUniteJoueur, int indiceJoueur,
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],Joueur joueurs[MAX_LIGNES_JOUEURS])
 {
     int possedtrait;
     possedtrait=possedeTrait(TRAIT_FIDELE,unitesJoueurs[indiceUniteJoueur].traits);
     if(possedtrait==0)
     {
        joueurs[indiceJoueur].entretien -=unitesJoueurs[indiceUniteJoueur].niveau;
        if ((joueurs[indiceJoueur].entretien-joueurs[indiceJoueur].nombreVillages)<0)
        {
            joueurs[indiceJoueur].revenu=2+joueurs[indiceJoueur].nombreVillages;
        }
        else
        {
           joueurs[indiceJoueur].revenu=2+joueurs[indiceJoueur].nombreVillages-(joueurs[indiceJoueur].entretien-joueurs[indiceJoueur].nombreVillages);
        }

     }

 }

int Max (int a , int b)
 {
     if(a<b)
     {
         return b;
     }
     else
     {
        return a;
     }
 }
 void attaque(int numtour,SDL_Surface* ecran,  int nbFichesTypesUnites,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],int uniteActuelle,int nbVillages,Village villages[MAX_LIGNES_VILLAGES],int indiceDansUnitesJoueurs, int indiceEnnemiDansUnitesJoueurs, int indiceJoueur, int indiceJoueurEnnemi,
 int ligneJoueur, int colonneJoueur, int ligneEnnemi, int colonneEnnemi,CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],Joueur joueurs[MAX_LIGNES_JOUEURS],int defenseJoueur, int defenseEnnemi,
  Attaque attaqueChoisie[2],int resistanceAuxAttaquesChoisis[2],Periode periode, int alignementJoueur, int alignementEnnemi)
 {

       int degatJouer =0;
       int degatEnnemi=0;
       int max ,nbattaqesEnnemi,nbattaqesJoueur;
       nbattaqesEnnemi=attaqueChoisie[1].nombreAttaques;
       nbattaqesJoueur=attaqueChoisie[0].nombreAttaques;
       max=Max(nbattaqesEnnemi,nbattaqesJoueur);
       degatJouer=(attaqueChoisie[0].degat*(100-resistanceAuxAttaquesChoisis[0])*(100+periode.bonus[alignementJoueur]))/10000;
       degatEnnemi=(attaqueChoisie[1].degat*(100-resistanceAuxAttaquesChoisis[1])*(100+periode.bonus[alignementEnnemi]))/10000;
       char nomJ[10],nomE[10];
         //int k=0;
        SDL_Surface *u50=NULL,*u51=NULL,*u61=NULL,*u83=NULL,*u90=NULL,*u115=NULL,*u130=NULL,*u149=NULL,*u232=NULL,*u235=NULL,*resume=NULL;
         SDL_Rect position,positionuniteJ,positiontxt,positionresume,postitre,positionAttack1,positionAttack4,positionAttack2,positionAttack3;
          int posx=colonneEnnemi*25;
          int posy=ligneEnnemi*20;
         position.x=posx;
         position.y=posy;
         positiontxt.x=50;
         positiontxt.y=100;
         positionresume.x=230;
         positionresume.y=40;
         postitre.x=390;
         postitre.y=70;
         positionAttack1.x=250;
         positionAttack1.y=130;
         positionAttack2.x=250;
         positionAttack2.y=230;
         positionAttack3.x=250;
         positionAttack3.y=330;
         positionAttack4.x=250;
         positionAttack4.y=430;
            char titre[30]="Attack Information";
            char Attq1[20]="Attack 1/4 ";
             char Attq2[20]="Attack 2/4 ";
              char Attq3[20]="Attack 3/4 ";
               char Attq4[20]="Attack 4/4 ";
            TTF_Init();
             int x=0;
             int y=0;
             int z=0;
             int t=0;
             int n=0;
             int m=0,a4=0;
             int defanceJ=0,defanceE=0;
            int defanceJ1=0,defanceE1=0;
            int defanceJ2=0,defanceE2=0;
             int defanceJ3=0,defanceE3=0;
              int defanceJ4=0,defanceE4=0;


        TTF_Font* font1=TTF_OpenFont("HeadingNowTrial-45Medium.ttf", 30);
        TTF_Font* font2=TTF_OpenFont("Semi Strong - Demo.ttf", 25);
        TTF_Font* font = TTF_OpenFont("angelina.ttf", 30);
        TTF_Font* font3=TTF_OpenFont("123Marker.ttf", 25);
         TTF_Font* font5=TTF_OpenFont("123Marker.ttf", 13);
        TTF_Font* font4=TTF_OpenFont("Dessin123.ttf", 25);

     u50=IMG_Load("blademaster20_25.PNG");
     u51=IMG_Load("arcanister (1).PNG");
     u61=IMG_Load("pathfinder20_25.PNG");
     u83=IMG_Load("outrider20_25.PNG");
     u90=IMG_Load("falcon20_25.PNG");
     u115=IMG_Load("shocktrooper20_25.PNG");
     u130=IMG_Load("bowman20_25.PNG");
     u149=IMG_Load("general20_25.PNG");
     u232=IMG_Load("Sauoracle20_25.PNG");
     u235=IMG_Load("skirmisher20_25.PNG");
     //resume=IMG_Load("Two_Brothers_M1P1.PNG");
    resume=IMG_Load("attaquerr.PNG");

         SDL_Color noir = {0, 0, 0}; //attention ce n'est pas un Uint32
         SDL_Color blanc = { 255, 255, 255 };
          SDL_Color gold = { 204, 204, 0 };
          SDL_Color vert = { 0, 153, 0};
          SDL_Color bleu = { 0, 102, 204 };
          SDL_Color beje = { 255, 229, 204};
          SDL_Color gris = { 224, 224, 224 };
           SDL_Color vertclair = { 153, 153, 0 };
            SDL_Color rouge = { 153, 0, 0 };
            SDL_Color marron = { 255, 229, 204 };
           SDL_Surface* texte = NULL;

       int dJ,dE;
       int i=0,j=0;
       int indiceficheTypeUniteJoueur;
       int idJ=carte[ligneJoueur][colonneJoueur].idUnite;
       int attaquer=1;
       int pvAttq1J=0;
        int pvAttq1E=0;
        int degat1J=0;
        int degat1E=0;

        int pvAttq2E=0;
         int pvAttq2J=0;
        int degat2J=0;
        int degat2E=0;


        int pvAttq3J=0;
        int pvAttq3E=0;
        int degat3J=0;
        int degat3E=0;
        int pvAttq4J=0;
        int pvAttq4E=0;
        int degat4J=0;
        int degat4E=0;
             /*char maxtxt[30]="Dansssssssssss attaquessss";
              //sprintf(maxtxt,"%d",max);
           texte= TTF_RenderText_Blended(font2,maxtxt,blanc);
            SDL_BlitSurface(texte, NULL, ecran, &positiontxt);*/

       if(attaqueChoisie[0].idAttaque!=0){
       while(j<max){
            carte[ligneJoueur][colonneJoueur].idUnite=0;
            afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,uniteActuelle,periode,unitesJoueurs[indiceDansUnitesJoueurs].idJoueur,carte,2,joueurs,nbVillages,villages,unitesJoueurs);
                switch(unitesJoueurs[indiceDansUnitesJoueurs].idFicheTypeUnite)
                     {
                                        case 50:
                                         SDL_BlitSurface(u50, NULL, ecran, &position);
                                         break;
                                         case 51:
                                         SDL_BlitSurface(u51, NULL, ecran, &position);
                                         break;
                                         case 61:
                                         SDL_BlitSurface(u61, NULL, ecran, &position);
                                         break;
                                         case 83:
                                         SDL_BlitSurface(u83, NULL, ecran, &position);
                                         break;
                                         case 90:
                                         SDL_BlitSurface(u90, NULL, ecran, &position);
                                          break;
                                          case 115:
                                          SDL_BlitSurface(u115, NULL, ecran, &position);
                                           break;
                                           case 130:
                                           SDL_BlitSurface(u130, NULL, ecran, &position);
                                            break;
                                            case 149:
                                            SDL_BlitSurface(u149, NULL, ecran, &position);
                                           break;
                                          case 232:
                                          SDL_BlitSurface(u232, NULL, ecran, &position);
                                           break;
                                         case 235:
                                          SDL_BlitSurface(u235, NULL, ecran, &position);
                                           break;

                         }

            SDL_Flip(ecran);
            SDL_Delay(900);
            j++;
       }

                    while((i<max)&&unitesJoueurs[indiceDansUnitesJoueurs].pv!=0 && unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv!=0)
                         {




                                  dE=rand()%100+1;
                               if(dE>defenseEnnemi)
                                {
                                     int pv=unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-degatJouer;
                                      if(pv>0)
                                      {
                                          if(degatJouer!=0)
                                               {

                                                     unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer;


                                                 }

                                      }
                                    else
                                     {
                                        unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv=0;

                                     }
                              }
                              else
                              {
                                  defanceE=1;

                              }


                        dJ=rand()%100+1;
                       if(dJ>defenseJoueur)
                      {

                                 int pv=unitesJoueurs[indiceDansUnitesJoueurs].pv-degatEnnemi;
                                  if(pv>0)
                                 {
                                      if(degatEnnemi!=0)
                                       {

                                              unitesJoueurs[indiceDansUnitesJoueurs].pv-=degatEnnemi;

                                        }

                                   }
                                  else
                                 {
                                    unitesJoueurs[indiceDansUnitesJoueurs].pv=0;

                                }
                      }
                      else{
                            defanceJ=1;

                      }

                         if(i==0)
                         {
                                   pvAttq1J=unitesJoueurs[indiceDansUnitesJoueurs].pv;
                                   pvAttq1E=unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv;
                                    if(defanceE==1)
                                   {
                                       defanceE1=1;
                                   }
                                   if(defanceJ==1)
                                   {

                                       defanceJ1=1;
                                   }


                         }
                         if(i==1)
                         {
                                    pvAttq2J=unitesJoueurs[indiceDansUnitesJoueurs].pv;
                                   pvAttq2E=unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv;
                                   if(defanceE==1)
                                   {
                                       defanceE2=1;
                                   }
                                   if(defanceJ==1)
                                   {

                                       defanceJ2=1;
                                   }

                         }
                         if(i==2 && pvAttq2E>0)
                         {
                                    pvAttq3J=unitesJoueurs[indiceDansUnitesJoueurs].pv;
                                   pvAttq3E=unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv;
                                  if(defanceE==1)
                                   {
                                       defanceE3=1;
                                   }
                                   if(defanceJ==1)
                                   {

                                       defanceJ3=1;
                                   }


                         }
                         if(i==3&&pvAttq3E>0)
                         {
                                    pvAttq4J=unitesJoueurs[indiceDansUnitesJoueurs].pv;
                                   pvAttq4E=unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv;
                                  if(defanceE==1)
                                   {
                                       defanceE4=1;
                                   }
                                   if(defanceJ==1)
                                   {

                                       defanceJ4=1;
                                   }


                         }


                             i+=1;
                             defanceE=0;
                             defanceJ=0;
                         }

                         if(i>=1 ){
                          //int w=0;

                         //for(int k=0;k<3;k++)
                                  //{
                                                         /*char degat[10];
                                                         sprintf(degat,"%d",degat3E);
                                                         texte= TTF_RenderText_Blended(font5,degat,blanc);
                                                         postitre.x-=10;
                                                         SDL_BlitSurface(texte, NULL, resume, &postitre);
                                                         postitre.x+=10;*/

                                 while(i>1){
                                                 char txtJ[200];
                                                 char txtE[200];
                                                  char txtJ2[200];
                                                  char txtE2[200];
                                                  char txtdefanceE[200];
                                                   char txtdefanceJ[200];

                                            if(joueurs[indiceJoueur].idJoueur==1){
                                               strcpy(txtJ,"Human player unit inflicts on enemy unit damage of ");
                                               strcpy(txtE,"Enemy player unit inflicts on human unit damage of ");
                                                strcpy(txtJ2,"Human player unit does not inflict damage on  enemy unit(damage=0)");
                                               strcpy(txtE2,"Enemy player unit does not inflict damage on  human unit(damage=0)");
                                                strcpy(txtdefanceE,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                               strcpy(txtdefanceJ,"Enemy player unit does not inflict damage on  human unit(because of its defense)");

                                            }
                                            else
                                            {

                                               strcpy(txtE,"Human player unit inflicts on enemy unit damage of ");
                                               strcpy(txtJ,"Enemy player unit inflicts on human unit damage of ");
                                                strcpy(txtE2,"Human player unit does not inflict damage on  enemy unit(damage=0) ");
                                               strcpy(txtJ2,"Enemy player unit does not inflict damage on  human unit(damage=0) ");
                                                strcpy(txtdefanceJ,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                               strcpy(txtdefanceE,"Enemy player unit does not inflict damage on  human unit(because of its defense)");


                                            }
                                     SDL_BlitSurface(resume, NULL, ecran, &positionresume);

                                     texte= TTF_RenderText_Blended(font1,titre,gold);
                                     SDL_BlitSurface(texte, NULL, ecran, &postitre);
                                if(max>=1){
                                     texte= TTF_RenderText_Blended(font3,Attq1,rouge);
                                     SDL_BlitSurface(texte, NULL, ecran, &positionAttack1);

                                            if(degatJouer!=0)
                                            {


                                                     if(defanceE1!=1){
                                                          if(x==0){
                                                          positionAttack1.x-=235;
                                                         char degat[10];
                                                         sprintf(degat,"%d",degatJouer);
                                                         strcat(txtJ,degat);
                                                         texte= TTF_RenderText_Blended(font5,txtJ,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack1);
                                                           positionAttack1.x+=235;
                                                            if(joueurs[indiceJoueur].idJoueur==1){
                                                           strcpy(txtJ,"Human player unit inflicts on enemy unit damage of ");
                                                            }
                                                            else{
                                                                 strcpy(txtJ,"Enemy player unit inflicts on human unit damage of" );
                                                            }

                                                         x=1;
                                                         }
                                                       }
                                                       else{


                                                                  positionAttack1.x-=235;
                                                         texte= TTF_RenderText_Blended(font5,txtdefanceE,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack1);
                                                           positionAttack1.x+=235;
                                                            if(joueurs[indiceJoueur].idJoueur==1)
                                                            {
                                                                strcpy(txtdefanceE,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                                            }
                                                            else{
                                                                     strcpy(txtdefanceE,"Enemy player unit does not inflict damage on  human unit(because of its defense) ");
                                                            }
                                                       }


                                            }
                                            else{
                                                            positionAttack1.x-=235;
                                                         texte= TTF_RenderText_Blended(font5,txtJ2,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack1);
                                                           positionAttack1.x+=235;
                                                            if(joueurs[indiceJoueur].idJoueur==1)
                                                            {
                                                                strcpy(txtdefanceE,"Human player unit does not inflict damage on  enemy unit(damage=0)");
                                                            }
                                                            else{
                                                                     strcpy(txtdefanceE,"Enemy player unit does not inflict damage on  human unit(damage=0) ");
                                                            }

                                            }
                                        if(degatEnnemi!=0)
                                            {

                                                       if(defanceJ1!=1){
                                                          if(y==0){
                                                         positionAttack1.x-=235;
                                                           positionAttack1.y+=20;

                                                            char degatE[10];
                                                         sprintf(degatE,"%d",degatEnnemi);
                                                         strcat(txtE,degatE);
                                                         texte= TTF_RenderText_Blended(font5,txtE,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack1);
                                                         y=1;
                                                            positionAttack1.x+=235;
                                                             positionAttack1.y-=20;


                                                            if(joueurs[indiceJoueur].idJoueur==1){

                                                             strcpy(txtE,"Enemy player unit inflicts on human unit damage of ");
                                                            }
                                                            else{

                                                                   strcpy(txtE,"Human player unit inflicts on enemy unit damage of ");
                                                            }

                                                         }
                                                       }
                                                       else{

                                                              positionAttack1.x-=235;
                                                              positionAttack1.y+=20;
                                                         texte= TTF_RenderText_Blended(font5,txtdefanceJ,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack1);
                                                           positionAttack1.x+=235;
                                                             positionAttack1.y-=20;
                                                         if(joueurs[indiceJoueur].idJoueur==1)
                                                            {
                                                                strcpy(txtdefanceJ,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                                            }
                                                            else{
                                                                     strcpy(txtdefanceJ,"Enemy player unit does not inflict damage on  human unit(because of its defense) ");
                                                            }
                                                       }

                                            }
                                            else{
                                                            positionAttack1.x-=235;
                                                              positionAttack1.y+=20;
                                                         texte= TTF_RenderText_Blended(font5,txtE2,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack1);
                                                           positionAttack1.x+=235;
                                                             positionAttack1.y-=20;


                                            }
                                     }


                                          if(max>=2&&pvAttq1E>0) {
                                           texte= TTF_RenderText_Blended(font3,Attq2,rouge);
                                            SDL_BlitSurface(texte, NULL, ecran, &positionAttack2);

                                            if(degatJouer!=0){

                                                         if(defanceE2!=1){
                                                           if(z==0){
                                                         positionAttack2.x-=235;
                                                         char degatt[10];
                                                         sprintf(degatt,"%d",degatJouer);
                                                         strcat(txtJ,degatt);
                                                         texte= TTF_RenderText_Blended(font5,txtJ,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack2);
                                                          positionAttack2.x+=235;
                                                             if(joueurs[indiceJoueur].idJoueur==1){
                                                            strcpy(txtJ,"Human player unit inflicts on enemy unit damage of ");
                                                            }
                                                            else{
                                                                 strcpy(txtJ,"Enemy player unit inflicts on human unit damage of" );
                                                            }
                                                         z=1;
                                                           }
                                                         }
                                                         else{
                                                                 positionAttack2.x-=235;
                                                         texte= TTF_RenderText_Blended(font5,txtdefanceE,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack2);
                                                          positionAttack2.x+=235;
                                                            if(joueurs[indiceJoueur].idJoueur==1)
                                                            {
                                                                strcpy(txtdefanceE,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                                            }
                                                            else{
                                                                     strcpy(txtdefanceE,"Enemy player unit does not inflict damage on  human unit(because of its defense) ");
                                                            }


                                                         }



                                            }
                                            else{

                                                          positionAttack2.x-=235;
                                                         texte= TTF_RenderText_Blended(font5,txtJ2,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack2);
                                                          positionAttack2.x+=235;



                                            }
                                            if(degatEnnemi!=0)
                                            {

                                                          if(defanceJ2!=1){
                                                          if(t==0){
                                                            positionAttack2.x-=235;
                                                           positionAttack2.y+=20;
                                                        char degat2[10];
                                                         sprintf(degat2,"%d",degatEnnemi);
                                                         strcat(txtE,degat2);
                                                         texte= TTF_RenderText_Blended(font5,txtE,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack2);
                                                           positionAttack2.x+=235;
                                                             positionAttack2.y-=20;
                                                           t=1;
                                                          if(joueurs[indiceJoueur].idJoueur==1){

                                                             strcpy(txtE,"Enemy player unit inflicts on human unit damage of ");
                                                            }
                                                            else{

                                                                   strcpy(txtE,"Human player unit inflicts on enemy unit damage of ");
                                                            }
                                                          }
                                                          }
                                                          else{
                                                            positionAttack2.x-=235;
                                                              positionAttack2.y+=20;
                                                         texte= TTF_RenderText_Blended(font5,txtdefanceJ,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack2);
                                                           positionAttack2.x+=235;
                                                             positionAttack2.y-=20;
                                                            if(joueurs[indiceJoueur].idJoueur==1)
                                                            {
                                                                strcpy(txtdefanceJ,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                                            }
                                                            else{
                                                                     strcpy(txtdefanceJ,"Enemy player unit does not inflict damage on  human unit(because of its defense) ");
                                                            }
                                                          }


                                            }
                                            else{
                                                            positionAttack2.x-=235;
                                                              positionAttack2.y+=20;
                                                         texte= TTF_RenderText_Blended(font5,txtE2,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack2);
                                                           positionAttack2.x+=235;
                                                             positionAttack2.y-=20;


                                            }
                                          }

                                           if(max>=3&&pvAttq2E>0){
                                                texte= TTF_RenderText_Blended(font3,Attq3,rouge);
                                                 SDL_BlitSurface(texte, NULL, ecran, &positionAttack3);

                                             if(degatJouer!=0 ){

                                                         if(defanceE3!=1){
                                                         if(m==0){
                                                            positionAttack3.x-=235;
                                                          char degat[10];
                                                         sprintf(degat,"%d",degatJouer);
                                                         strcat(txtJ,degat);
                                                         texte= TTF_RenderText_Blended(font5,txtJ,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack3);
                                                         m=1;
                                                          positionAttack3.x+=235;
                                                          if(joueurs[indiceJoueur].idJoueur==1){
                                                            strcpy(txtJ,"Human player unit inflicts on enemy unit damage of ");
                                                            }
                                                            else{
                                                                 strcpy(txtJ,"Enemy player unit inflicts on human unit damage of" );
                                                            }
                                                         }
                                                         }
                                                         else{
                                                                positionAttack3.x-=235;
                                                         texte= TTF_RenderText_Blended(font5,txtdefanceE,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack3);
                                                           positionAttack3.x+=235;
                                                           if(joueurs[indiceJoueur].idJoueur==1)
                                                            {
                                                                strcpy(txtdefanceE,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                                            }
                                                            else{
                                                                     strcpy(txtdefanceE,"Enemy player unit does not inflict damage on  human unit(because of its defense) ");
                                                            }
                                                         }

                                            }
                                            else{
                                                            positionAttack3.x-=235;
                                                         texte= TTF_RenderText_Blended(font5,txtJ2,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack3);
                                                           positionAttack3.x+=235;

                                            }
                                            if(degatEnnemi!=0 )
                                            {
                                                       if(defanceJ3!=1){
                                                            if(n==0){
                                                            positionAttack3.x-=235;
                                                           positionAttack3.y+=20;
                                                          char degat[10];
                                                         sprintf(degat,"%d",degatEnnemi);
                                                         strcat(txtE,degat);

                                                         texte= TTF_RenderText_Blended(font5,txtE,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack3);
                                                         n=1;
                                                            positionAttack3.x+=235;
                                                            positionAttack3.y-=20;
                                                            if(joueurs[indiceJoueur].idJoueur==1){

                                                             strcpy(txtE,"Enemy player unit inflicts on human unit damage of ");
                                                            }
                                                            else{

                                                                   strcpy(txtE,"Human player unit inflicts on enemy unit damage of ");
                                                            }
                                                            }
                                                       }
                                                       else
                                                       {
                                                            positionAttack3.x-=235;
                                                              positionAttack3.y+=20;

                                                         texte= TTF_RenderText_Blended(font5,txtdefanceJ,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack3);
                                                           positionAttack3.x+=235;
                                                             positionAttack3.y-=20;
                                                              if(joueurs[indiceJoueur].idJoueur==1)
                                                            {
                                                                strcpy(txtdefanceJ,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                                            }
                                                            else{
                                                                     strcpy(txtdefanceJ,"Enemy player unit does not inflict damage on  human unit(because of its defense) ");
                                                            }

                                                       }

                                            }
                                            else{

                                                            positionAttack3.x-=235;
                                                              positionAttack3.y+=20;

                                                         texte= TTF_RenderText_Blended(font5,txtE2,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack3);
                                                           positionAttack3.x+=235;
                                                             positionAttack3.y-=20;

                                            }
                                           }
                                           if(max==4&&pvAttq3E>0){
                                            texte= TTF_RenderText_Blended(font3,Attq4,rouge);
                                                 SDL_BlitSurface(texte, NULL, ecran, &positionAttack4);

                                             if(degatJouer!=0 ){

                                                         if(defanceE4!=1){
                                                         if(a4==0){
                                                            positionAttack4.x-=235;
                                                          char degat[10];
                                                         sprintf(degat,"%d",degatJouer);
                                                         strcat(txtJ,degat);
                                                         texte= TTF_RenderText_Blended(font5,txtJ,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack4);
                                                         a4=1;
                                                          positionAttack4.x+=235;
                                                          if(joueurs[indiceJoueur].idJoueur==1){
                                                            strcpy(txtJ,"Human player unit inflicts on enemy unit damage of ");
                                                            }
                                                            else{
                                                                 strcpy(txtJ,"Enemy player unit inflicts on human unit damage of" );
                                                            }
                                                         }
                                                         }
                                                         else{
                                                                positionAttack4.x-=235;
                                                         texte= TTF_RenderText_Blended(font5,txtdefanceE,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack4);
                                                           positionAttack4.x+=235;
                                                           if(joueurs[indiceJoueur].idJoueur==1)
                                                            {
                                                                strcpy(txtdefanceE,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                                            }
                                                            else{
                                                                     strcpy(txtdefanceE,"Enemy player unit does not inflict damage on  human unit(because of its defense) ");
                                                            }
                                                         }

                                            }
                                            else{
                                                            positionAttack4.x-=235;
                                                         texte= TTF_RenderText_Blended(font5,txtJ2,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack4);
                                                           positionAttack4.x+=235;

                                            }
                                            if(degatEnnemi!=0 )
                                            {
                                                       if(defanceJ4!=1){
                                                            if(n==0){
                                                            positionAttack4.x-=235;
                                                           positionAttack4.y+=20;
                                                          char degat[10];
                                                         sprintf(degat,"%d",degatEnnemi);
                                                         strcat(txtE,degat);

                                                         texte= TTF_RenderText_Blended(font5,txtE,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack4);
                                                         n=1;
                                                            positionAttack4.x+=235;
                                                            positionAttack4.y-=20;
                                                            if(joueurs[indiceJoueur].idJoueur==1){

                                                             strcpy(txtE,"Enemy player unit inflicts on human unit damage of ");
                                                            }
                                                            else{

                                                                   strcpy(txtE,"Human player unit inflicts on enemy unit damage of ");
                                                            }
                                                            }
                                                       }
                                                       else
                                                       {
                                                            positionAttack4.x-=235;
                                                              positionAttack4.y+=20;

                                                         texte= TTF_RenderText_Blended(font5,txtdefanceJ,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack4);
                                                           positionAttack4.x+=235;
                                                             positionAttack4.y-=20;
                                                              if(joueurs[indiceJoueur].idJoueur==1)
                                                            {
                                                                strcpy(txtdefanceJ,"Human player unit does not inflict damage on  enemy unit(because of its defense)");
                                                            }
                                                            else{
                                                                     strcpy(txtdefanceJ,"Enemy player unit does not inflict damage on  human unit(because of its defense) ");
                                                            }

                                                       }

                                            }
                                            else{

                                                            positionAttack4.x-=235;
                                                              positionAttack4.y+=20;

                                                         texte= TTF_RenderText_Blended(font5,txtE2,blanc);
                                                         SDL_BlitSurface(texte, NULL, resume, &positionAttack4);
                                                           positionAttack4.x+=235;
                                                             positionAttack4.y-=20;

                                            }
                                           }
                                       SDL_Flip(ecran);
                                       SDL_Delay(400);

                                       i++;

                                       if(i==10)
                                       {
                                            i=0;
                                       }


                                 }
                         }







        /*char maxtxt[30];
              sprintf(maxtxt,"%d",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv);
           texte= TTF_RenderText_Blended(font2,maxtxt,blanc);
            SDL_BlitSurface(texte, NULL, ecran, &positiontxt);
            positiontxt.y+=50;*/


         carte[ligneJoueur][colonneJoueur].idUnite=idJ;

       if(unitesJoueurs[indiceDansUnitesJoueurs].pv==0)
       {
           unitesJoueurs[indiceDansUnitesJoueurs].idUnite=0;
           unitesJoueurs[indiceDansUnitesJoueurs].idJoueur=0;
           unitesJoueurs[indiceDansUnitesJoueurs].active=0;
           unitesJoueurs[indiceDansUnitesJoueurs].alignement=0;
           unitesJoueurs[indiceDansUnitesJoueurs].colonne=0;
           unitesJoueurs[indiceDansUnitesJoueurs].finTour=1;
           unitesJoueurs[indiceDansUnitesJoueurs].ligne=0;
           unitesJoueurs[indiceDansUnitesJoueurs].mouvements=0;
           unitesJoueurs[indiceDansUnitesJoueurs].mouvementsMax=0;
           unitesJoueurs[indiceDansUnitesJoueurs].niveau=0;
           unitesJoueurs[indiceDansUnitesJoueurs].rang=0;
           miseAJourJoueurApresMortUnite(indiceDansUnitesJoueurs,indiceJoueur,unitesJoueurs,joueurs);
           carte[ligneJoueur][colonneJoueur].idUnite=0;
           carte[ligneJoueur][colonneJoueur].symboleJoueur=' ';



       }
        if(unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv==0)
       {
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idJoueur=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].active=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].colonne=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].finTour=1;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].ligne=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].mouvements=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].mouvementsMax=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].niveau=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].rang=0;
           miseAJourJoueurApresMortUnite(indiceEnnemiDansUnitesJoueurs,indiceJoueurEnnemi,unitesJoueurs,joueurs);
           carte[ligneEnnemi][colonneEnnemi].idUnite=0;
           carte[ligneEnnemi][colonneEnnemi].symboleJoueur=' ';



       }
       }

     /* if(attaqueChoisie[0].idAttaque!=0){

          char maxtxt[30];
           sprintf(maxtxt,"%d",max);
               //SDL_BlitSurface(u50, NULL, ecran, &position);
           texte= TTF_RenderText_Blended(font2,maxtxt,blanc);
            SDL_BlitSurface(texte, NULL, ecran, &position);
           while(i<max)
           {


             carte[ligneJoueur][colonneJoueur].idUnite=0;


          switch(unitesJoueurs[indiceDansUnitesJoueurs].idFicheTypeUnite)
          {
                                        case 50:
                                         SDL_BlitSurface(u50, NULL, ecran, &position);
                                         break;
                                         case 51:
                                         SDL_BlitSurface(u51, NULL, ecran, &position);
                                         break;
                                         case 61:
                                         SDL_BlitSurface(u61, NULL, ecran, &position);
                                         break;
                                         case 83:
                                         SDL_BlitSurface(u83, NULL, ecran, &position);
                                         break;
                                         case 90:
                                         SDL_BlitSurface(u90, NULL, ecran, &position);
                                          break;
                                          case 115:
                                          SDL_BlitSurface(u115, NULL, ecran, &position);
                                           break;
                                           case 130:
                                           SDL_BlitSurface(u130, NULL, ecran, &position);
                                            break;
                                            case 149:
                                            SDL_BlitSurface(u149, NULL, ecran, &position);
                                           break;
                                          case 232:
                                          SDL_BlitSurface(u232, NULL, ecran, &position);
                                           break;
                                         case 235:
                                          SDL_BlitSurface(u235, NULL, ecran, &position);
                                           break;

                }
                SDL_Delay(100);
                 SDL_Flip(ecran);
              //i++;
           }
       }


       /*if(joueurs[indiceJoueur].idJoueur==1)
       {
           strcpy(nomJ,"Humain");
           strcpy(nomE,"Ennemi");

       }
       else{

           strcpy(nomJ,"Ennemi");
            strcpy(nomE,"Humain");

       }
       if(attaqueChoisie[0].idAttaque!=0)
       {
       printf("lunite %d (pv:%d) de Joueur %s attaque l'unite %d de Joueur %s (pv:%d)\n",unitesJoueurs[indiceDansUnitesJoueurs].idUnite,
       unitesJoueurs[indiceDansUnitesJoueurs].pv,nomJ,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv);
       }
       while((i<=max)&&unitesJoueurs[indiceDansUnitesJoueurs].pv!=0&&unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv!=0)
       {
            printf("l'attaque %d/%d\n",i,max);
            dE=rand()%100+1;
           if(dE>defenseEnnemi)
           {
               int pv=unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-degatJouer;
              if(pv>0)
              {
                  if(degatJouer!=0)
                  {

                      unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer;
                      printf(" lunite %d de joueur %s inflige au l'unite %d de Joueur %s un degat de %d (sante restante %d)\n",unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,
                           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,degatJouer,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv);
                  }
                 else
                 {
                    printf("lunite %d de joueur %s  n'inflige pas un degat au l'unite %d de Joueur %s(degat=0)",unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,
                   unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE );
                 }
              }
              else
              {
                  unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv=0;
                printf(" lunite %d de joueur %s inflige au l'unite %d de Joueur %s un degat de %d (sante restante %d)\n",unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,
                    unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,degatJouer,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv);
              }
           }
           else
            {
             printf("lunite %d de joueur %s  n'inflige pas un degat au l'unite %d de Joueur %s ( a cause de son defense)\n",unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,
                   unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE );
           }
           dJ=rand()%100+1;
           if(dJ>defenseJoueur)
           {

               int pv=unitesJoueurs[indiceDansUnitesJoueurs].pv-degatEnnemi;
               if(pv>0)
               {
                   if(degatEnnemi!=0)
                   {

                       unitesJoueurs[indiceDansUnitesJoueurs].pv-=degatEnnemi;
                      printf(" lunite %d de joueur %s inflige au l'unite %d de Joueur %s un degat de %d (sante restante %d)\n",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,
                       unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,degatEnnemi,unitesJoueurs[indiceDansUnitesJoueurs].pv);
                   }
                   else
                    {
                      printf("lunite %d de joueur %s  n'inflige pas un degat au l'unite %d de Joueur %s ( degat=0)\n",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,
                      unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ );
                   }
               }
               else
                {
                  unitesJoueurs[indiceDansUnitesJoueurs].pv=0;
                  printf(" lunite %d de joueur %s inflige au l'unite %d de Joueur %s un degat de %d (sante restante %d)\n",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,
                    unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,degatEnnemi,unitesJoueurs[indiceDansUnitesJoueurs].pv);
               }
           }
           else
            {
             printf("lunite %d de joueur %s  n'inflige pas un degat au l'unite %d de Joueur %s ( a cause de son defense)\n",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,
                   unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ );
           }
           i++;
       }
       if(unitesJoueurs[indiceDansUnitesJoueurs].pv==0)
       {
           unitesJoueurs[indiceDansUnitesJoueurs].idUnite=0;
           unitesJoueurs[indiceDansUnitesJoueurs].idJoueur=0;
           unitesJoueurs[indiceDansUnitesJoueurs].active=0;
           unitesJoueurs[indiceDansUnitesJoueurs].alignement=0;
           unitesJoueurs[indiceDansUnitesJoueurs].colonne=0;
           unitesJoueurs[indiceDansUnitesJoueurs].finTour=1;
           unitesJoueurs[indiceDansUnitesJoueurs].ligne=0;
           unitesJoueurs[indiceDansUnitesJoueurs].mouvements=0;
           unitesJoueurs[indiceDansUnitesJoueurs].mouvementsMax=0;
           unitesJoueurs[indiceDansUnitesJoueurs].niveau=0;
           unitesJoueurs[indiceDansUnitesJoueurs].rang=0;
           miseAJourJoueurApresMortUnite(indiceDansUnitesJoueurs,indiceJoueur,unitesJoueurs,joueurs);
           carte[ligneJoueur][colonneJoueur].idUnite=0;
           carte[ligneJoueur][colonneJoueur].symboleJoueur=' ';



       }
        if(unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv==0)
       {
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idJoueur=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].active=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].colonne=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].finTour=1;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].ligne=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].mouvements=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].mouvementsMax=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].niveau=0;
           unitesJoueurs[indiceEnnemiDansUnitesJoueurs].rang=0;
           miseAJourJoueurApresMortUnite(indiceEnnemiDansUnitesJoueurs,indiceJoueurEnnemi,unitesJoueurs,joueurs);
           carte[ligneEnnemi][colonneEnnemi].idUnite=0;
           carte[ligneEnnemi][colonneEnnemi].symboleJoueur=' ';



       }

 */
    SDL_FreeSurface(resume);
    SDL_FreeSurface(u50);
    SDL_FreeSurface(u61);
    SDL_FreeSurface(u51);
    SDL_FreeSurface(u83);
    SDL_FreeSurface(u130);
    SDL_FreeSurface(u115);
    SDL_FreeSurface(u90);
    SDL_FreeSurface(u149);
    SDL_FreeSurface(u232);
    SDL_FreeSurface(u235);
    TTF_CloseFont(font);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);
    TTF_CloseFont(font4);
    TTF_CloseFont(font5);
    SDL_FreeSurface(texte);


 }

int verifierGagnant(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS], UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
     int indiceJoueur,i=0,j=0;
    indiceJoueur=chercherIndiceJoueurHumain(nbJoueurs,joueurs);
    //int JoueurHumainGagnant=1;
    int JoueurHumainExiste=0;
    int JoueurEnnemiExiste=0;
    for(i=0;i<nbJoueurs;i++)
    {

        if(joueurs[indiceJoueur].groupe_allies==joueurs[i].groupe_allies)
        {
            if(joueurs[i].nombreUnites==0)
            {

                return 2;

            }
            else
            {
                for(j=0;j<MAX_LIGNES_UNITESJOUEURS;j++)
                {

                    if((unitesJoueurs[j].rang==1)&&(unitesJoueurs[j].idJoueur==joueurs[indiceJoueur].idJoueur))
                       {

                            JoueurHumainExiste=1;
                       }
                }
               /*while((j<MAX_LIGNES_UNITESJOUEURS)&&(unitesJoueurs[j].rang!=1)&&(unitesJoueurs[j].idJoueur==joueurs[indiceJoueur].idJoueur))
               {
                   j++;
               }
               printf("j1=%d\n",j);
               if(j==MAX_LIGNES_UNITESJOUEURS)
               {
                     printf("return 2 2222222222\n");
                   return 2;

               }
               else
               {  printf("JoueurHumainExiste=1\n");
                  JoueurHumainExiste=1;

               }*/
            }
        }
        else
        {

            if(joueurs[i].nombreUnites==0)
            {

                return 1;

            }
            else
            {
                for(j=0;j<MAX_LIGNES_UNITESJOUEURS;j++)
                {

                    if((unitesJoueurs[j].rang==1)&&(unitesJoueurs[j].idJoueur!=joueurs[indiceJoueur].idJoueur))
                       {

                            JoueurEnnemiExiste=1;
                       }
                }
            /*while((j<MAX_LIGNES_UNITESJOUEURS)&&(unitesJoueurs[j].rang!=1)&&(unitesJoueurs[j].idJoueur!=joueurs[indiceJoueur].idJoueur))
               {
                   j++;
                   printf("unitesJoueurs[j].idUnite=%d\n",unitesJoueurs[j].idUnite);
               }
               printf("j=%d\n",j);
               printf("MAX_LIGNES_UNITESJOUEURS=%d\n",MAX_LIGNES_UNITESJOUEURS);
               if(j==MAX_LIGNES_UNITESJOUEURS)
               {
                   printf("JoueurEnnemiExiste=0\n");
                   JoueurEnnemiExiste=0;
               }
               else
               {
                   printf("JoueurEnnemiExiste=1\n");
                  JoueurEnnemiExiste=1;
               }*/

        }

    }
    }
    //printf("JoueurHumainGagnant=%d\n",JoueurHumainGagnant);
    if((JoueurHumainExiste==1)&&(JoueurEnnemiExiste==1))
    {

         return 0;

    }
    else if(JoueurHumainExiste==1)
    {
         return 1;

    }
    else
    {
        return 2;
    }
 }

int verifierFinTours(int idJoueur, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
{    int i=0;
    for(i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
    {
        if(unitesJoueurs[i].idJoueur==idJoueur&&unitesJoueurs[i].finTour==0)
        {
            return 0;
        }
    }
    return 1;
}
void reinitialiserTours(int idJoueur, int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS],
UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
{
    int i =0,indice;
    for (i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
    {
        unitesJoueurs[i].finTour=0;
         unitesJoueurs[i].mouvements=unitesJoueurs[i].mouvementsMax;

    }
    indice=chercherIndiceJoueur(idJoueur, nbJoueurs,joueurs);
    joueurs[indice].or +=  joueurs[indice].revenu;

}


int jouerTourHumain(int numtour,SDL_Surface* ecran,int idJoueur, int groupeAlliesJoueur, int nbFichesTypesUnites, int nbJoueurs,
 int nbUnitesMagasin , int nbVillages,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Joueur joueurs[MAX_LIGNES_JOUEURS],CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],Village villages[MAX_LIGNES_VILLAGES],TypeTerrain typesTerrains[NB_TYPES_TERRAINS],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],Periode periode
 )
 {
     int possibiliter=0;
      int FinTour=0,FinTourforcee=0,IndiceUniteJoueur,ligne_courante,colonne_courante;
      int deplacer,ligneAdeplacer,x=0,ColonneAdeplacer,indiceJoueurEnnemi=0;
      int IndiceChefUnitesJoueurs=0,choix,CodeAttaque=0,UniteAttaquable,nbAttaques;
      int ligne_ennemie,colonne_ennemie,indiceficheTypeUniteEnnemi,Gagnant=0,indiceEnnemiDansUnitesJoueurs,indiceficheTypeUniteJoueur=0;
      int defenseEnnemi,defenseJoueur;
      Attaque attaques[10][2];
       int resistances[10][2];
       Attaque attaqueChoisie[2];
        int resistanceAuxAttaquesChoisis[2];
       int indiceJoueur=0;
      int attaquer=0,cartemarquer=0;
      int iduniteAdeplacer=-1;
    int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
    int objetActuel ,xobjetActuel=0,yobjetActuel=0, i = 0, j = 0,acheter=0,possibiliterAttaque=-1;
    SDL_Rect position,positionJoueur,positionadeplacer,centre;
    SDL_Event event;
    TTF_Font* font = NULL;
    SDL_Surface* texte = NULL;
     SDL_Surface *u50=NULL,*u51=NULL,*u61=NULL,*u83=NULL,*u90=NULL,*u115=NULL,*u130=NULL,*u149=NULL,*u232=NULL,*u235=NULL;
    centre.x=960;
    centre.y=540;
    position.x=970;
    position.y=280;
    int x1=0,y1=0,unitequiattaquey=0,unitequiattaquex=0;
    int idJattaque=0,ChoisirAttaque=0;
    int uniteActuelle=0;

     SDL_Rect positiongagner;
     positiongagner.x=150;
     positiongagner.y=100;
     SDL_Surface  *joueurgagner = NULL,*ennemigagner=NULL;
      joueurgagner = IMG_Load("Human.PNG");
       ennemigagner = IMG_Load("Enemy.PNG");



        TTF_Init();
        font = TTF_OpenFont("angelina.ttf", 30);

        SDL_Color noir = {0, 0, 0}; //attention ce n'est pas un Uint32
        SDL_Color blanc = { 255, 255, 255 };
        SDL_Color gold = { 153, 0, 0 };
        SDL_Rect positionText;



      Gagnant=verifierGagnant(nbJoueurs,joueurs,unitesJoueurs);
          FinTour=verifierFinTours(idJoueur,unitesJoueurs);


   /*if(Gagnant==1)
    {

        afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,0,periode,idJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          SDL_BlitSurface(joueurgagner, NULL, ecran, &positiongagner);
            SDL_Flip(ecran);
           SDL_Delay(4000);
    }
    else if(Gagnant==2){

          afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,0,periode,idJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          SDL_BlitSurface(ennemigagner, NULL, ecran, &positiongagner);
           SDL_Flip(ecran);
            SDL_Delay(4000);

    }*/
  while((FinTour==0)&&(FinTourforcee==0)&&(Gagnant==0)){

        afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,uniteActuelle,periode,idJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                sauvegarderJeuComplet(nbVillages,nbJoueurs,villages,joueurs,unitesJoueurs);
                SDL_Quit();
               break;
             case SDL_MOUSEBUTTONDOWN:
             if (event.button.button == SDL_BUTTON_LEFT)
              {

                if( DoubleClickDetected() == 1){

                  if(990< event.button.x &&  event.button.x<1065 &&  event.button.y<595 &&  event.button.y>585)
                     {

                         FinTourforcee=1;

                     }
                     else{

                        int y = event.button.y / 20;
                            int x = event.button.x / 25;
                    IndiceUniteJoueur= chercherIndiceUniteJoueur(carte[yobjetActuel][xobjetActuel].idUnite, 1, 1, unitesJoueurs);


                if((unitesJoueurs[IndiceUniteJoueur].ligne==y)&&(unitesJoueurs[IndiceUniteJoueur].colonne==x))
               {
                    effacerDeplacementsPossibles(carte);
               }
                else if (carte[y][x].marque == 1 ){
                     IndiceUniteJoueur= chercherIndiceUniteJoueur(carte[yobjetActuel][xobjetActuel].idUnite, 1, 1, unitesJoueurs);

               if(unitesJoueurs[ IndiceUniteJoueur].mouvements>0){
             if((unitesJoueurs[IndiceUniteJoueur].ligne!=y)&&(unitesJoueurs[IndiceUniteJoueur].colonne!=x)&&(carte[y][x].idUnite==0)){


                        deplacerUnite(idJoueur,IndiceUniteJoueur ,carte[yobjetActuel][xobjetActuel].idUnite , yobjetActuel, xobjetActuel,
                                                  y, x, nbJoueurs, nbVillages,
                                                  carte, joueurs, unitesJoueurs, villages);
                                                   effacerDeplacementsPossibles(carte);
                                                  unitequiattaquey=y;
                                                 unitequiattaquex=x;
                               CodeAttaque=calculerCodeAttaque(groupeAlliesJoueur,y,x,nbJoueurs,joueurs,carte);

                               if(CodeAttaque!=0)
                               {
                                   possibiliterAttaque=1;
                                   idJattaque=carte[unitequiattaquey][unitequiattaquex].idUnite;
                               }
             }
               }
                                   /*char test[30];
                                    sprintf(test,"%d",CodeAttaque);
                                    texte= TTF_RenderText_Blended(font,test,blanc);
                                    SDL_BlitSurface(texte, NULL, ecran, &centre);
                               }
             }
               }



               }


                                  }
                //carte[event.button.x / 25][event.button.y / 20].idUnite = objetActuel;
                   /* positionJoueur.x=event.button.x;
                    positionJoueur.y=event.button.y;
                      int x=event.button.x/25;
                     int y=event.button.y/20;
                     int ligneadeplacer=unitesJoueurs[carte[y][x].idUnite].ligne;
                     int colonneadeplacer=unitesJoueurs[carte[y][x].idUnite].colonne;

                    IndiceChefUnitesJoueurs=chercherIndiceChefUnitesJoueurs(idJoueur,unitesJoueurs);
                    /*

                     else{

                            int mar=carte[ligneadeplacer][colonneadeplacer].marque;

                        IndiceUniteJoueur=chercherIndiceUniteJoueur(carte[yobjetActuel][xobjetActuel].idUnite,1,1,unitesJoueurs);
                         if(mar==1)
                          {
                                 char test[30];
                                sprintf(test,"%d",xobjetActuel);
                                    texte= TTF_RenderText_Blended(font,test,blanc);
                                    SDL_BlitSurface(texte, NULL, ecran, &centre);

                          if(unitesJoueurs[ IndiceUniteJoueur].mouvements>0)
                          {


                            if((unitesJoueurs[IndiceUniteJoueur].ligne!=y)&&(unitesJoueurs[IndiceUniteJoueur].colonne!=x)&&(carte[y][x].idUnite==0)){
                               deplacerUnite(1,IndiceUniteJoueur,carte[yobjetActuel][xobjetActuel].idUnite,yobjetActuel,xobjetActuel,ligneadeplacer,colonneadeplacer,nbJoueurs,nbVillages,carte,joueurs,unitesJoueurs, villages);
                               effacerDeplacementsPossibles(carte);
                               unitequiattaquey=y;
                               unitequiattaquex=x;

                               CodeAttaque=calculerCodeAttaque(groupeAlliesJoueur,y,x,nbJoueurs,joueurs,carte);

                               if(CodeAttaque!=0)
                               {
                                   possibiliterAttaque=1;
                                   idJattaque=carte[unitequiattaquey][unitequiattaquex].idUnite;
                                   /*char test[30];
                                    sprintf(test,"%d",CodeAttaque);
                                    texte= TTF_RenderText_Blended(font,test,blanc);
                                    SDL_BlitSurface(texte, NULL, ecran, &centre);
                               }

                           }

                          }

                     }
                           //effacerDeplacementsPossibles(carte);



                  }*/
                }
 }}
                else{


                    positionJoueur.x=event.button.x;
                     positionJoueur.y=event.button.y;
                     int x=positionJoueur.x/25;
                     int y=positionJoueur.y/20;
                    if(carte[y][x].idUnite!=0)
                     {

                           uniteActuelle=carte[y][x].idUnite;

                     }
                      IndiceUniteJoueur = chercherIndiceUniteJoueur(carte[y][x].idUnite, 1, 1, unitesJoueurs);
                     /*if(cartemarquer==1)
                     {
                         effacerDeplacementsPossibles(carte);
                         cartemarquer=0;
                     }*/
                if(IndiceUniteJoueur!=-1)
                   {
                        yobjetActuel = unitesJoueurs[IndiceUniteJoueur].ligne;
                         xobjetActuel = unitesJoueurs[IndiceUniteJoueur].colonne;
                        int mvt = unitesJoueurs[IndiceUniteJoueur].mouvements;
                           if(mvt>0)
                           {


                                marquerDeplacementsPossibles(carte,yobjetActuel,xobjetActuel,unitesJoueurs[ IndiceUniteJoueur].mouvements);
                                 cartemarquer=1;

                           }

                }

                     /*positionJoueur.x=event.button.x;
                     positionJoueur.y=event.button.y;
                     int x=positionJoueur.x/25;
                     int y=positionJoueur.y/20;
                     objetActuel=carte[y][x].idUnite;

                     IndiceUniteJoueur=chercherIndiceUniteJoueur(carte[y][x].idUnite,1,1,unitesJoueurs);
                      xobjetActuel=unitesJoueurs[IndiceUniteJoueur].colonne;
                     yobjetActuel=unitesJoueurs[IndiceUniteJoueur].ligne;
                     if(IndiceUniteJoueur!=-1)
                     {
                         if(unitesJoueurs[ IndiceUniteJoueur].mouvements>0)
                         {
                              marquerDeplacementsPossibles(carte,yobjetActuel,xobjetActuel,unitesJoueurs[ IndiceUniteJoueur].mouvements);
                         }

                     }*/
                     //deplacer unite
                  /*else if(mar==1)
                     {
                        IndiceUniteJoueur=chercherIndiceUniteJoueur(carte[yobjetActuel][xobjetActuel].idUnite,1,1,unitesJoueurs);

                          if(unitesJoueurs[ IndiceUniteJoueur].mouvements>0)
                          {
                            if((yobjetActuel!=y)&&(xobjetActuel!=x)&&(carte[y][x].idUnite==0)){
                               deplacerUnite(1,IndiceUniteJoueur,objetActuel,yobjetActuel,xobjetActuel,y,x,nbJoueurs,nbVillages,carte,joueurs,unitesJoueurs, villages);
                               effacerDeplacementsPossibles(carte);
                               unitequiattaquey=y;
                               unitequiattaquex=x;

                               CodeAttaque=calculerCodeAttaque(groupeAlliesJoueur,y,x,nbJoueurs,joueurs,carte);

                               if(CodeAttaque!=0)
                               {
                                   possibiliterAttaque=1;
                                   idJattaque=carte[unitequiattaquey][unitequiattaquex].idUnite;
                                   /*char test[30];
                                    sprintf(test,"%d",CodeAttaque);
                                    texte= TTF_RenderText_Blended(font,test,blanc);
                                    SDL_BlitSurface(texte, NULL, ecran, &centre);
                               }

                           }

                          }

                     }*/
                }
                clicGaucheEnCours = 1;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) // Le clic droit sert � effacer
            {

                //recruter des unites
                if(DoubleClickDetected() == 1){
                               positionJoueur.x=event.button.x/25;
                    positionJoueur.y=event.button.y/20;
                    IndiceChefUnitesJoueurs=chercherIndiceChefUnitesJoueurs(idJoueur,unitesJoueurs);

                    if(carte[unitesJoueurs[IndiceChefUnitesJoueurs].ligne][unitesJoueurs[IndiceChefUnitesJoueurs].colonne].idTypeTerrain==ID_TYPE_TERRAIN_DONJON )
                       {
                              //acheter=1;
                              acheterUnites(uniteActuelle,event.button.x,event.button.y,ecran,idJoueur,unitesJoueurs[IndiceChefUnitesJoueurs].ligne,unitesJoueurs[IndiceChefUnitesJoueurs].colonne,nbUnitesMagasin,  nbJoueurs, nbFichesTypesUnites,  nbVillages,
                              unitesMagasin,unitesJoueurs,joueurs,fichesTypesUnites,villages,carte);

                               //afficherCarteJeu(ecran,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
                       }



                    }
                 else if(possibiliterAttaque==1){
                   //deplacer unite
                    positionJoueur.x=event.button.x;
                    positionJoueur.y=event.button.y;

                  int x=positionJoueur.x/25;
                  int y=positionJoueur.y/20;
                  /*int mar=carte[y][x].marque;
                  IndiceUniteJoueur=chercherIndiceUniteJoueur(carte[yobjetActuel][xobjetActuel].idUnite,1,1,unitesJoueurs);
                  if(mar==1)
                     {
                          if(unitesJoueurs[ IndiceUniteJoueur].mouvements>0)
                          {
                            if((yobjetActuel!=y)&&(xobjetActuel!=x)&&(carte[y][x].idUnite==0)){
                               deplacerUnite(1,IndiceUniteJoueur,objetActuel,yobjetActuel,xobjetActuel,y,x,nbJoueurs,nbVillages,carte,joueurs,unitesJoueurs, villages);
                               effacerDeplacementsPossibles(carte);
                               unitequiattaquey=y;
                               unitequiattaquex=x;

                               CodeAttaque=calculerCodeAttaque(groupeAlliesJoueur,y,x,nbJoueurs,joueurs,carte);

                               if(CodeAttaque!=0)
                               {
                                   possibiliterAttaque=1;
                                   idJattaque=carte[unitequiattaquey][unitequiattaquex].idUnite;
                                   /*char test[30];
                                    sprintf(test,"%d",CodeAttaque);
                                    texte= TTF_RenderText_Blended(font,test,blanc);
                                    SDL_BlitSurface(texte, NULL, ecran, &centre);
                               }

                           }

                          }

                     }*/
                      if( carte[y][x].symboleJoueur=='E')
                     {
                                IndiceUniteJoueur=chercherIndiceUniteJoueur(idJattaque,1,1,unitesJoueurs);
                                 ligne_courante=unitesJoueurs[IndiceUniteJoueur].ligne;
                                 colonne_courante=unitesJoueurs[ IndiceUniteJoueur].colonne;
                                  UniteAttaquable=verifierUniteAttaquable(CodeAttaque,ligne_courante,colonne_courante,y,x);
                              if(UniteAttaquable==1)
                               {


                                    indiceEnnemiDansUnitesJoueurs=chercherIndiceUniteJoueur(carte[y][x].idUnite,2,1,unitesJoueurs);
                                     indiceficheTypeUniteEnnemi=chercherIndiceFicheTypeUnite(unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);

                                      indiceJoueurEnnemi=chercherIndiceJoueur(2,nbJoueurs,joueurs);

                                      indiceficheTypeUniteJoueur=chercherIndiceFicheTypeUnite(unitesJoueurs[IndiceUniteJoueur].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);



                                    defenseEnnemi=fichesTypesUnites[indiceficheTypeUniteEnnemi].relationsTerrains[carte[y][x].idTypeTerrain].defense;


                                        defenseJoueur=fichesTypesUnites[indiceficheTypeUniteJoueur].relationsTerrains[carte[ligne_courante][colonne_courante].idTypeTerrain].defense ;

                                         nbAttaques=determinerPossibilitesAttaque(idJoueur,carte[y][x].idUnite, indiceficheTypeUniteJoueur,
                                           indiceficheTypeUniteEnnemi,fichesTypesUnites, IndiceUniteJoueur, indiceEnnemiDansUnitesJoueurs,attaques,resistances);
                                            ChoisirAttaque=1;
                                           if(ChoisirAttaque==1){
                                            choisirAttaque(ecran,uniteActuelle,idJoueur,unitesJoueurs,carte[y][x].idUnite ,indiceficheTypeUniteJoueur,  indiceficheTypeUniteEnnemi,
                                             fichesTypesUnites,IndiceUniteJoueur,indiceEnnemiDansUnitesJoueurs, defenseJoueur,defenseEnnemi, nbAttaques,  attaques,resistances, attaqueChoisie,
                                              resistanceAuxAttaquesChoisis,  periode, unitesJoueurs[IndiceUniteJoueur].alignement,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement );
                                               ChoisirAttaque=0;
                                               }ligne_ennemie=y;
                                               colonne_ennemie=x;
                                                attaquer=1;





                }

                }
                 }



            else{

                    positionJoueur.x=event.button.x/25;
                    positionJoueur.y=event.button.y/20;
                    IndiceChefUnitesJoueurs=chercherIndiceChefUnitesJoueurs(idJoueur,unitesJoueurs);

                    if(carte[unitesJoueurs[IndiceChefUnitesJoueurs].ligne][unitesJoueurs[IndiceChefUnitesJoueurs].colonne].idTypeTerrain==ID_TYPE_TERRAIN_DONJON )
                       {
                              //acheter=1;
                              acheterUnites(uniteActuelle,event.button.x,event.button.y,ecran,idJoueur,unitesJoueurs[IndiceChefUnitesJoueurs].ligne,unitesJoueurs[IndiceChefUnitesJoueurs].colonne,nbUnitesMagasin,  nbJoueurs, nbFichesTypesUnites,  nbVillages,
                              unitesMagasin,unitesJoueurs,joueurs,fichesTypesUnites,villages,carte);

                               //afficherCarteJeu(ecran,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
                       }

            }
            clicDroitEnCours = 1;
            }
            break;
        case SDL_MOUSEBUTTONUP: // On d�sactive le bool�en qui disait qu'un bouton �tait enfonc�
            if (event.button.button == SDL_BUTTON_LEFT)
                {clicGaucheEnCours = 0;}
            else if (event.button.button == SDL_BUTTON_RIGHT)
                {clicDroitEnCours = 0;}
            break;

        }


        if(attaquer==1)
        {
                  //afficherCarteJeu(ecran,periode,idJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);

                 attaque(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,uniteActuelle,nbVillages,villages,IndiceUniteJoueur, indiceEnnemiDansUnitesJoueurs,indiceJoueur,indiceJoueurEnnemi,
                                                     ligne_courante,colonne_courante, ligne_ennemie, colonne_ennemie,carte,unitesJoueurs,
                                                    joueurs, defenseJoueur, defenseEnnemi, attaqueChoisie,resistanceAuxAttaquesChoisis,periode,
                                                    unitesJoueurs[IndiceUniteJoueur].alignement,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement );
                                                    attaquer=0;
        Gagnant=verifierGagnant(nbJoueurs,joueurs,unitesJoueurs);
     if(Gagnant==1)
    {
         afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,0,periode,idJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          SDL_BlitSurface(joueurgagner, NULL, ecran, &positiongagner);
            SDL_Flip(ecran);
           SDL_Delay(5000);
    }
    else if(Gagnant==2){

          afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,0,periode,idJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          SDL_BlitSurface(ennemigagner, NULL, ecran, &positiongagner);
           SDL_Flip(ecran);
            SDL_Delay(5000);

    }


        }
SDL_Flip(ecran);
FinTour=verifierFinTours(idJoueur,unitesJoueurs);
 Gagnant=verifierGagnant( nbJoueurs,joueurs,unitesJoueurs);

 }
    SDL_FreeSurface(joueurgagner);
    SDL_FreeSurface(ennemigagner);
       SDL_FreeSurface(texte);
    TTF_CloseFont(font);
 return Gagnant;
 }

void acheterUnitesAuto(int idJoueur, int ligneDonjon, int colonneDonjon, int nbUnitesMagasin,
 int nbJoueurs, int nbFichesTypesUnites,
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],
 Joueur joueurs[MAX_LIGNES_JOUEURS],
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE] )
 {
         int ligne ,colonne,numUnite,IndiceJoueur, IndiceUniteMagasin,IndiceFicheTypeUnite,IndiceUniteJoueur;
            afficherJoueurJeu(idJoueur, nbJoueurs,joueurs);
           afficherUnitesMagasinUnJoueur(idJoueur,nbUnitesMagasin,nbFichesTypesUnites,unitesMagasin,fichesTypesUnites);
             int i=0,j=0;

           int UnitesMagasinUnJoueur[20];

     for (i=0;i<MAX_LIGNES_UNITESMAGASIN;i++)
     {
         if(unitesMagasin[i].idJoueurAutorise==idJoueur)
         {

            UnitesMagasinUnJoueur[j]=unitesMagasin[i].idUniteMagasin;
            j++;

         }
     }

         do{
            ligne=rand()%30;
            colonne=rand()%38;
           }while((carte[ligne][colonne].idUnite!=0)||(carte[ligne][colonne].idTypeTerrain!=ID_TYPE_TERRAIN_CHATEAU)||((abs(ligne-ligneDonjon)>MAX_ELOIGNEMENT_CHATEAU_DONJON)&&(abs(colonne-colonneDonjon)>MAX_ELOIGNEMENT_CHATEAU_DONJON)));
          //printf("ligne de nouvelle unite= %d\n",ligne+1);
          //printf("colonne de nouvelle unite= %d\n",colonne+1);
            numUnite=UnitesMagasinUnJoueur[rand()%j];
            //printf("unite choisite a acheter = %d\n",numUnite);
            IndiceUniteMagasin=chercherIndiceUniteMagasin(numUnite, idJoueur,nbUnitesMagasin,unitesMagasin);
              IndiceFicheTypeUnite=chercherIndiceFicheTypeUnite(unitesMagasin[IndiceUniteMagasin].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);

              IndiceJoueur=chercherIndiceJoueur(idJoueur, nbJoueurs,joueurs);
                if(joueurs[IndiceJoueur].or >fichesTypesUnites[IndiceFicheTypeUnite].prix)
                 {
                  IndiceUniteJoueur=chercherIndiceUniteJoueur(0,0,0,unitesJoueurs);
                      initialiserNouvelleUniteJoueur(IndiceUniteJoueur, IndiceFicheTypeUnite,idJoueur,ligne,colonne,unitesJoueurs,fichesTypesUnites);
                      miseAJourJoueurApresAchatUnite(IndiceUniteJoueur, IndiceJoueur, IndiceFicheTypeUnite,unitesJoueurs,joueurs,fichesTypesUnites);
                      carte[ligne][colonne].idUnite= unitesJoueurs[IndiceUniteJoueur].idUnite;
                       carte[ligne][colonne].symboleJoueur=joueurs[IndiceJoueur].symbole;
                        //printf("achat effectuer avec succer\n");

                }
                else {
                    //printf("\n Or est insuffsant !\n");
                }


}

void deplacementAutomatiqueUnite(int indiceUnite, int ligne_courante, int colonne_courante,
 int groupeAlliesJoueur, int nbJoueurs, int nbVillages,
 CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 Joueur joueurs[MAX_LIGNES_JOUEURS],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],
 Village villages[MAX_LIGNES_VILLAGES] )
 {
      int indiceEnnemi,indiceVillage,indiceJoueur,nouvelle_ligne=0,nouvelle_colonne=0;
      int lignemax=0,lignemin=0,colonnemax=0,colonnemin=0;
      int i=0,j=0,mouvements=unitesJoueurs[indiceUnite].mouvements;
     colonnemax=colonne_courante+mouvements;
     colonnemin=colonne_courante-mouvements;
     lignemax=ligne_courante+mouvements;
     lignemin=ligne_courante-mouvements;
     printf("l Ennemi veut deplacer l unite %d\n",unitesJoueurs[indiceUnite].idUnite);
     if(colonnemin<0)
     {
      colonnemin=0;
     }
     if(lignemin<0)
     {
         lignemin=0;
     }
     if(lignemax>=NB_LIGNES_CARTE)
     {
         lignemax=NB_LIGNES_CARTE-1;
     }
     if(colonnemax>=NB_COLONNES_CARTE)
     {
         colonnemax=NB_COLONNES_CARTE-1;

     }


    for(i=lignemin;i<=lignemax;i++)
     {
         for(j=colonnemin;j<=colonnemax;j++)
         {
            if(unitesJoueurs[carte[i][j].idUnite].idJoueur!=unitesJoueurs[indiceUnite].idJoueur)
           {
                if((carte[i+1][j].marque==1)&&(carte[i+1][j].idUnite==0))
                  {
                    //printf("dans if if\n");
                    nouvelle_ligne=i+1;
                    nouvelle_colonne=j;

                  }
                else  if (carte[i][j+1].marque==1&&(carte[i][j+1].idUnite==0))
                  {
                    //printf("dans else1\n");
                    nouvelle_ligne=i;
                    nouvelle_colonne=j+1;

                  }
                  else if(carte[i-1][j].marque==1&&(carte[i-1][j].idUnite==0))
                  {
                    //printf("dans else\n");
                    nouvelle_ligne=i-1;
                    nouvelle_colonne=j;

                  }
                  else if(carte[i][j-1].marque==1&&(carte[i][j-1].idUnite==0)){


                    nouvelle_ligne=i;
                    nouvelle_colonne=j-1;


                   }

                  }
           }
 }
     if((nouvelle_ligne==0)&&(nouvelle_colonne==0)&&(carte[lignemax][colonnemax].idUnite==0))
    {
        nouvelle_ligne=lignemax;
        nouvelle_colonne=colonnemax;
    }



        printf("ligne a deplacer :%d\n",nouvelle_ligne+1);
        printf("colonne a deplacer :%d\n",nouvelle_colonne+1);

     if (carte[nouvelle_ligne][nouvelle_colonne].idTypeTerrain==ID_TYPE_TERRAIN_VILLAGE)
     {
          indiceVillage=chercherIndiceVillage(nouvelle_ligne,nouvelle_colonne,nbVillages,villages);

          if(villages[indiceVillage].idJoueurProprietaire!=unitesJoueurs[indiceUnite].idJoueur)
         {
               if(villages[indiceVillage].idJoueurProprietaire==0)
                    {
                        indiceEnnemi=chercherIndiceJoueur(unitesJoueurs[indiceUnite].idJoueur,nbJoueurs,joueurs);
                        joueurs[indiceEnnemi].nombreVillages ++;
                        miseAJourJoueurApresOccupationVillage(indiceEnnemi,joueurs);
                        villages[indiceVillage].idJoueurProprietaire=unitesJoueurs[indiceUnite].idJoueur;
                    }
                else
                {
                        indiceJoueur=chercherIndiceJoueur(villages[indiceVillage].idJoueurProprietaire,nbJoueurs,joueurs);
                        joueurs[indiceJoueur].nombreVillages --;
                        miseAJourJoueurApresOccupationVillage(indiceJoueur,joueurs);
                        villages[indiceVillage].idJoueurProprietaire=unitesJoueurs[indiceUnite].idJoueur;
                }


         }


     }

        carte[nouvelle_ligne][nouvelle_colonne].idUnite=carte[ligne_courante][colonne_courante].idUnite;
         carte[nouvelle_ligne][nouvelle_colonne].symboleJoueur=carte[ligne_courante][colonne_courante].symboleJoueur;
        carte[ligne_courante][colonne_courante].idUnite=0;
        carte[ligne_courante][colonne_courante].symboleJoueur=' ';

        unitesJoueurs[indiceUnite].ligne=nouvelle_ligne;
         unitesJoueurs[indiceUnite].colonne=nouvelle_colonne;
         unitesJoueurs[indiceUnite].mouvements -=abs(nouvelle_ligne-ligne_courante)+abs(nouvelle_colonne-colonne_courante);

 }

int choixAutomatiqueUniteEnnemie( int codeAttaque, int ligne_courante, int colonne_courante,
 int nbJoueurs, CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 Joueur joueurs[MAX_LIGNES_JOUEURS],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
     int droite=0, gauche=0,bas=0,haut=0,idUniteh=0,idUniteb=0,idUniteg=0,idUnited=0,IndiceUniteJoueur=0;
     int  pvhaut=0, pvbas=0, pvgauche=0,pvdroite=0;
     int min=0,idUnite=0;

     haut=codeAttaque/1000;
     bas=(codeAttaque%1000)/100;
     gauche=(codeAttaque%100)/10;
     droite=codeAttaque%10;

     if(haut==1)
     {
         idUniteh=carte[ligne_courante+1][colonne_courante].idUnite;
         IndiceUniteJoueur=chercherIndiceUniteJoueur(idUniteh,2,1,unitesJoueurs);
         pvhaut=unitesJoueurs[IndiceUniteJoueur].pv;
         min=pvhaut;

     }
    if(bas==1)
     {
         idUniteb=carte[ligne_courante-1][colonne_courante].idUnite;
         IndiceUniteJoueur=chercherIndiceUniteJoueur(idUniteb,1,1,unitesJoueurs);
         pvbas=unitesJoueurs[IndiceUniteJoueur].pv;
          min=pvbas;

     }
    if(gauche==1)
     {
         idUniteg=carte[ligne_courante][colonne_courante-1].idUnite;
         IndiceUniteJoueur=chercherIndiceUniteJoueur(idUniteg,2,1,unitesJoueurs);
         pvgauche=unitesJoueurs[IndiceUniteJoueur].pv;
          min=pvgauche;

     }
    if(droite==1)
     {
         idUnited=carte[ligne_courante][colonne_courante+1].idUnite;
         IndiceUniteJoueur=chercherIndiceUniteJoueur(idUnited,2,1,unitesJoueurs);
         pvdroite=unitesJoueurs[IndiceUniteJoueur].pv;
          min=pvdroite;

     }
      if(haut==1)
      {
        min=pvhaut;
        idUnite=idUniteh;
      }

     if( pvbas<=min && bas==1)
     {
         min=pvbas;
         idUnite=idUniteb;
     }
     if( pvgauche<=min && gauche==1)
     {

         min=pvgauche;
         idUnite=idUniteg;
     }
     if(pvdroite<=min && droite==1)
     {
         min=pvdroite;
         idUnite=idUnited;
     }
    return  idUnite;

 }

int choisirAttaqueAutomatiquement(SDL_Surface* ecran,UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],int numJoueur, int numEnnemi,int indiceficheTypeUniteJoueur, int indiceficheTypeUniteEnnemi,
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],int indiceDansUnitesJoueurs, int indiceEnnemiDansUnitesJoueurs,
 int defenseJoueur, int defenseEnnemi, int nbrAttaques, Attaque attaques[10][2],int resistances[10][2], Attaque attaqueChoisie[2],
 int resistanceAuxAttaquesChoisis[2], Periode periode, int alignementJoueur,int alignementEnnemi)
 {

   int i=0,choix;
    int degatE,degatJ;
    SDL_Surface  *menuAttaque = NULL,*menuAttaque2=NULL,*uJ = NULL, *uE = NULL,*attaque1 = NULL, *attaque2= NULL,*attaque=NULL,*AttaqueSelc=NULL,*menuAttaque1=NULL;
    SDL_Surface *u50=NULL,*u51=NULL,*u61=NULL,*u83=NULL,*u90=NULL,*u115=NULL,*u130=NULL,*u149=NULL,*u232=NULL,*u235=NULL;
    SDL_Surface *fireattaq=NULL,*impactattaq=NULL,*pierceattaq=NULL,*spearattaq=NULL,*ranged_attack=NULL,*coldattaq=NULL,*bladeattaq=NULL,*arcaneattaq=NULL;
    SDL_Rect position,positionuniteJ,positionuniteE ,centre,posattaque,posattaquE,typeattaque,ponomattaqueJ,ponomattaqueE,positionattaquechoisi;
    SDL_Rect testpo,positionnomJ,positionnomE,polevel,degatpo,nbpos,degattxtpo,resistancepo,purcentagepo,poxp,pohp,potxt,poxpmax,pohpmax,raceposition;
    SDL_Event event;
    char races[21][30]={"Bats","Drakes","DuneFolk","Dwarves","Elves","Falcons","Goblins","Gryphons","Horses","Humans","Mechanical","Merfolk","Monsters","Nagas","Ogres","Orcs","Saurian","Trolls","Undead","Wolves","Woses"};
    position.x=225;
    position.y=50;
    positionattaquechoisi.x=10;
    positionattaquechoisi.y=260;
    int continuer=1;
    centre.x=500;
    centre.y=400;
    posattaque.x=238;
    posattaque.y=320;

    posattaquE.x=650;
    posattaquE.y=320;

    positionuniteJ.x=290;
    positionuniteJ.y=200;
    testpo.x=170;
    testpo.y=70;
    poxp.x=430;
    poxp.y=244;
    pohp.x=424;
    pohp.y=220;

    poxpmax.x=448;
    poxpmax.y=244;

    pohpmax.x=450;
    pohpmax.y=220;

    potxt.x=443;
    potxt.y=220;
    positionnomJ.x=320;
    positionnomJ.y=120;
     positionnomE.x=480;
    positionnomE.y=120;

    raceposition.x=390;
    raceposition.y=180;




    polevel.x=455;
    polevel.y=150;

    positionuniteE.x=600;
    positionuniteE.y=200;

    typeattaque.x=455;
    typeattaque.y=342;

    ponomattaqueJ.x=300;
    ponomattaqueJ.y=325;
    degatpo.x=300;
    degatpo.y=350;
    degattxtpo.x=315;
    degattxtpo.y=350;

    nbpos.x=330;
    nbpos.y=350;
    resistancepo.x=300;
    resistancepo.y=365;

    purcentagepo.x=330;
    purcentagepo.y=365;

    ponomattaqueE.x=580;
    ponomattaqueE.y=325;

      int  clicGaucheEnCours = 0, clicDroitEnCours = 0;
      int objetActuel =0,selctionner=0;
      char nomattaqueJ[30],nomJ[30],nomE[30];
      int attaque1selec=0;
      int attaque2selec=0;



    TTF_Init();
        fireattaq=IMG_Load("fireAttaque.PNG");
        impactattaq=IMG_Load("impactAttaque.PNG");
        pierceattaq=IMG_Load("pierceAttaque.PNG");
        spearattaq=IMG_Load("spearAttaque.PNG");
        ranged_attack=IMG_Load("ranged_attack.PNG");
        coldattaq=IMG_Load("coldAttaque.PNG");
        bladeattaq=IMG_Load("bladeAttaque.PNG");
        arcaneattaq=IMG_Load("arcaneAttaque.PNG");
         TTF_Font* font = TTF_OpenFont("angelina.ttf", 20);
        TTF_Font* font1=TTF_OpenFont("123Marker.ttf", 17);
        TTF_Font* font2=TTF_OpenFont("Dessin123.ttf", 16);

         //menuAttaque1=IMG_Load("choisirattaque (2).PNG");
         //menuAttaque2=IMG_Load("attaqueselc2.PNG");

         //uJ=IMG_Load("Sauoracle.PNG");
         //uE=IMG_Load("skirmisher.PNG");
     u50=IMG_Load("blademaster.PNG");
     u51=IMG_Load("arcanister.PNG");
     u61=IMG_Load("pathfinder.PNG");
     u83=IMG_Load("outrider.PNG");
     u90=IMG_Load("falcon.PNG");
     u115=IMG_Load("shocktrooper.PNG");
     u130=IMG_Load("bowman.PNG");
     u149=IMG_Load("general.PNG");
     u232=IMG_Load("Sauoracle.PNG");
     u235=IMG_Load("skirmisher.PNG");

         SDL_Color noir = {0, 0, 0}; //attention ce n'est pas un Uint32
         SDL_Color blanc = { 255, 255, 255 };
          SDL_Color gold = { 204, 204, 0 };
          SDL_Color vert = { 0, 153, 0};
          SDL_Color bleu = { 0, 102, 204 };
          SDL_Color beje = { 255, 229, 204};
          SDL_Color gris = { 224, 224, 224 };
           SDL_Color vertclair = { 153, 153, 0 };
            SDL_Color rouge = { 153, 0, 0 };
            SDL_Color marron = { 255, 229, 204 };
           SDL_Surface* texte = NULL;


int sirine=1;


   int iattaque=0;



   //while(iattaque<nbrAttaques){
      choix=rand()%nbrAttaques+1;


    {

        attaqueChoisie[0].idAttaque=attaques[choix-1][0].idAttaque;
        attaqueChoisie[0].degat=attaques[choix-1][0].degat+((attaques[choix-1][0].degat/100)*periode.bonus[alignementJoueur]);
          //attaqueChoisie[0].degat=attaques[choix-1][0].degat;
        attaqueChoisie[0].idAttaqueSpeciale1=attaques[choix-1][0].idAttaqueSpeciale1;
        attaqueChoisie[0].idAttaqueSpeciale2=attaques[choix-1][0].idAttaqueSpeciale2;
        strcpy( attaqueChoisie[0].nomAttaque,attaques[choix-1][0].nomAttaque);
        attaqueChoisie[0].nombreAttaques=attaques[choix-1][0].nombreAttaques;
        attaqueChoisie[0].porteeAttaque=attaques[choix-1][0].porteeAttaque;
        attaqueChoisie[0].typeAttaque=attaques[choix-1][0].typeAttaque;
        resistanceAuxAttaquesChoisis[0]=resistances[choix-1][0];

        attaqueChoisie[1].idAttaque=attaques[choix-1][1].idAttaque;
        attaqueChoisie[1].degat=attaques[choix-1][1].degat+((attaques[choix-1][1].degat/100)*periode.bonus[alignementJoueur]);
        //attaqueChoisie[1].degat=attaques[choix-1][1].degat;
        attaqueChoisie[1].idAttaqueSpeciale1=attaques[choix-1][1].idAttaqueSpeciale1;
        attaqueChoisie[1].idAttaqueSpeciale2=attaques[choix-1][1].idAttaqueSpeciale2;
        strcpy( attaqueChoisie[1].nomAttaque,attaques[choix-1][1].nomAttaque);
        attaqueChoisie[1].nombreAttaques=attaques[choix-1][1].nombreAttaques;
        attaqueChoisie[1].porteeAttaque=attaques[choix-1][1].porteeAttaque;
        attaqueChoisie[1].typeAttaque=attaques[choix-1][1].typeAttaque;
        resistanceAuxAttaquesChoisis[1]=resistances[choix-1][1];
        //return 1;
    }

    if(choix==1)
    {
              menuAttaque=IMG_Load("choisirattaque (2).PNG");

    }
    else if(choix==2)
    {
          menuAttaque=IMG_Load("attaqueselc2.PNG");
    }
    else{
         menuAttaque=IMG_Load("attackMenu.PNG");
    }
     SDL_BlitSurface(menuAttaque, NULL, ecran, &position);

  // while(continuer==1)
    //{

               //char tst[20];
                // sprintf(tst,"%d",continuer);
                //texte= TTF_RenderText_Blended(font2,tst,blanc);
                //SDL_BlitSurface(texte, NULL, menuAttaque, &polevel);

   if(sirine==1){


  //for(i=0;i<nbrAttaques;i++)
  //{


              char degat[10];
               char degattxt[30]="x";
               char nb[10];
                char nbattaq[10];
                 char resistance[10];
               char pourcentage[10]="%";



            if(attaques[0][0].idAttaque!=0)
          {


              degatJ=attaques[0][0].degat+((attaques[0][0].degat/100)*periode.bonus[alignementJoueur]);

           switch(attaques[0][0].porteeAttaque)
          {
                    case 0:
                    texte= TTF_RenderText_Blended(font,"melee",beje);
                   SDL_BlitSurface(texte, NULL,ecran, &typeattaque);
                    break;
                    case 1:
                    texte= TTF_RenderText_Blended(font,"ranged",beje);
                    SDL_BlitSurface(texte, NULL,ecran , &typeattaque);
                    break;

          }
          typeattaque.y+=70;


               sprintf(degat,"%d",degatJ);
                texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degatpo);
                degatpo.x+=300;

                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degattxtpo);
                 degattxtpo.x+=300;

               sprintf(nbattaq,"%d",attaques[0][0].nombreAttaques);
                texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, ecran,&nbpos);
               nbpos.x+=300;

          sprintf(resistance,"%d",resistances[0][0]);
          texte= TTF_RenderText_Blended(font1,resistance,vertclair);
          SDL_BlitSurface(texte, NULL, ecran,&resistancepo);
           resistancepo.x+=300;



          texte= TTF_RenderText_Blended(font1,pourcentage,vertclair);
          SDL_BlitSurface(texte, NULL, ecran,&purcentagepo);
          purcentagepo.x+=290;
          if( attaqueChoisie[0].idAttaque==attaques[0][0].idAttaque)
          {
              texte= TTF_RenderText_Blended(font1,attaques[0][0].nomAttaque,gold);
          }
          else{
            texte= TTF_RenderText_Blended(font1,attaques[0][0].nomAttaque,blanc);
          }

          texte= TTF_RenderText_Blended(font1,attaques[0][0].nomAttaque,blanc);
          SDL_BlitSurface(texte, NULL, ecran, &ponomattaqueJ);
          ponomattaqueJ.y+=70;


        switch(attaques[0][0].typeAttaque)
          {
              case 0:
                   SDL_BlitSurface(bladeattaq, NULL, ecran, &posattaque);
                    break;
               case 1:
                   SDL_BlitSurface(pierceattaq, NULL, ecran, &posattaque);
                    break;
                case 2:
                   SDL_BlitSurface(impactattaq, NULL, ecran, &posattaque);
                    break;
                case 3:
                   SDL_BlitSurface(fireattaq, NULL, ecran, &posattaque);
                    break;
                 case 4:
                   SDL_BlitSurface(coldattaq, NULL, ecran, &posattaque);
                    break;
                 case 5:
                   SDL_BlitSurface(arcaneattaq, NULL, ecran, &posattaque);
                    break;
          }

           posattaque.y+=77;

          }

            if(attaques[0][1].idAttaque!=0)
            {
                degatE=attaques[0][1].degat+((attaques[0][1].degat/100)*periode.bonus[alignementEnnemi]);
            switch(attaques[0][1].typeAttaque)
          {
              case 0:
                   SDL_BlitSurface(bladeattaq, NULL, ecran, &posattaquE);
                    break;
               case 1:
                   SDL_BlitSurface(pierceattaq, NULL, ecran, &posattaquE);
                    break;
                case 2:
                   SDL_BlitSurface(impactattaq, NULL, ecran, &posattaquE);
                    break;
                case 3:
                   SDL_BlitSurface(fireattaq, NULL, ecran, &posattaquE);
                    break;
                 case 4:
                   SDL_BlitSurface(coldattaq, NULL, ecran, &posattaquE);
                    break;
                 case 5:
                   SDL_BlitSurface(arcaneattaq, NULL, ecran, &posattaquE);
                    break;
          }
           posattaquE.y+=77;
                sprintf(degat,"%d",degatE);
                 texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degatpo);
                degatpo.x-=300;
                degatpo.y+=72;

                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degattxtpo);
                 degattxtpo.x-=300;
                degattxtpo.y+=72;

               sprintf(nbattaq,"%d",attaques[0][1].nombreAttaques);
               texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, ecran,&nbpos);
               nbpos.x-=300;
               nbpos.y+=72;

            sprintf(resistance,"%d",resistances[0][1]);
          texte= TTF_RenderText_Blended(font1,resistance,vertclair);
          SDL_BlitSurface(texte, NULL, ecran,&resistancepo);
          resistancepo.x-=300;
            resistancepo.y+=70;

           texte= TTF_RenderText_Blended(font1,pourcentage,vertclair);
          SDL_BlitSurface(texte, NULL, ecran,&purcentagepo);
           purcentagepo.x-=290;
            purcentagepo.y+=70;

                texte= TTF_RenderText_Blended(font1,attaques[0][1].nomAttaque,blanc);
                  SDL_BlitSurface(texte, NULL, ecran, &ponomattaqueE);
                  ponomattaqueE.y+=70;


           }


           else
            {

                sprintf(degat,"%d",0);
                 texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degatpo);


                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degattxtpo);


               sprintf(nbattaq,"%d",0);
               texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, ecran,&nbpos);


            sprintf(resistance,"%d",resistances[0][1]);
          texte= TTF_RenderText_Blended(font1,resistance,rouge);
          SDL_BlitSurface(texte, NULL, ecran,&resistancepo);


           texte= TTF_RenderText_Blended(font1,pourcentage,rouge);
          SDL_BlitSurface(texte, NULL, ecran,&purcentagepo);


             posattaquE.y+=75;
              ponomattaqueE.y+=70;
               purcentagepo.y+=70;
               resistancepo.y+=70;
               nbpos.y+=72;
              degattxtpo.y+=72;
              degatpo.y+=72;
              degatpo.x-=300;
               purcentagepo.x-=290;
               resistancepo.x-=300;
                nbpos.x-=300;
                degattxtpo.x-=300;

           }




           //attque2
          if(attaques[1][0].idAttaque!=0)
          {

             degatJ=attaques[1][0].degat+((attaques[1][0].degat/100)*periode.bonus[alignementJoueur]);

          switch(attaques[1][0].porteeAttaque)
          {
                    case 0:
                    texte= TTF_RenderText_Blended(font,"melee",beje);
                   SDL_BlitSurface(texte, NULL,ecran, &typeattaque);
                    break;
                    case 1:
                    texte= TTF_RenderText_Blended(font,"ranged",beje);
                    SDL_BlitSurface(texte, NULL,ecran , &typeattaque);
                    break;

          }


        switch(attaques[1][0].typeAttaque)
          {
              case 0:
                   SDL_BlitSurface(bladeattaq, NULL, ecran, &posattaque);
                    break;
               case 1:
                   SDL_BlitSurface(pierceattaq, NULL, ecran, &posattaque);
                    break;
                case 2:
                   SDL_BlitSurface(impactattaq, NULL, ecran, &posattaque);
                    break;
                case 3:
                   SDL_BlitSurface(fireattaq, NULL, ecran, &posattaque);
                    break;
                 case 4:
                   SDL_BlitSurface(coldattaq, NULL, ecran, &posattaque);
                    break;
                 case 5:
                   SDL_BlitSurface(arcaneattaq, NULL, ecran, &posattaque);
                    break;
          }

                sprintf(degat,"%d",degatJ);
                texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degatpo);
                degatpo.x+=300;

                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degattxtpo);
                 degattxtpo.x+=300;

                sprintf(nbattaq,"%d",attaques[1][0].nombreAttaques);
                texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, ecran,&nbpos);
                nbpos.x+=300;

            sprintf(resistance,"%d",resistances[1][0]);
          texte= TTF_RenderText_Blended(font1,resistance,vertclair);
          SDL_BlitSurface(texte, NULL, ecran,&resistancepo);
           resistancepo.x+=300;

            texte= TTF_RenderText_Blended(font1,pourcentage,vertclair);
          SDL_BlitSurface(texte, NULL, ecran,&purcentagepo);
          purcentagepo.x+=290;
          if(attaqueChoisie[0].idAttaque==attaques[1][0].idAttaque)
          {
               texte= TTF_RenderText_Blended(font1,attaques[1][0].nomAttaque,gold);
          }
          else
          {
             texte= TTF_RenderText_Blended(font1,attaques[1][0].nomAttaque,blanc);
          }
             texte= TTF_RenderText_Blended(font1,attaques[1][0].nomAttaque,blanc);
             SDL_BlitSurface(texte, NULL, ecran, &ponomattaqueJ);
          }
             if(attaques[1][1].idAttaque!=0)
             {
                 degatE=attaques[1][1].degat+((attaques[1][1].degat/100)*periode.bonus[alignementEnnemi]);

                sprintf(degat,"%d",degatE);
                 texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degatpo);

                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degattxtpo);


               sprintf(nbattaq,"%d",attaques[1][1].nombreAttaques);
               texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, ecran,&nbpos);

            sprintf(resistance,"%d",resistances[1][1]);
          texte= TTF_RenderText_Blended(font1,resistance,vertclair);
          SDL_BlitSurface(texte, NULL, ecran,&resistancepo);

            texte= TTF_RenderText_Blended(font1,pourcentage,vertclair);
           SDL_BlitSurface(texte, NULL, ecran,&purcentagepo);


                texte= TTF_RenderText_Blended(font1,attaques[1][1].nomAttaque,blanc);
                  SDL_BlitSurface(texte, NULL, ecran, &ponomattaqueE);

        switch(attaques[1][1].typeAttaque)
          {
              case 0:
                   SDL_BlitSurface(bladeattaq, NULL, ecran, &posattaquE);
                    break;
               case 1:
                   SDL_BlitSurface(pierceattaq, NULL, ecran, &posattaquE);
                    break;
                case 2:
                   SDL_BlitSurface(impactattaq, NULL, ecran, &posattaquE);
                    break;
                case 3:
                   SDL_BlitSurface(fireattaq, NULL, ecran, &posattaquE);
                    break;
                 case 4:
                   SDL_BlitSurface(coldattaq, NULL, ecran, &posattaquE);
                    break;
                 case 5:
                   SDL_BlitSurface(arcaneattaq, NULL, ecran, &posattaquE);
                    break;
          }


          }

            else
            {

                sprintf(degat,"%d",0);
                 texte= TTF_RenderText_Blended(font2,degat,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degatpo);


                texte= TTF_RenderText_Blended(font2,degattxt,gris);
                SDL_BlitSurface(texte, NULL, ecran, &degattxtpo);


               sprintf(nbattaq,"%d",0);
               texte= TTF_RenderText_Blended(font2,nbattaq,gris);
               SDL_BlitSurface(texte, NULL, ecran,&nbpos);


            sprintf(resistance,"%d",resistances[0][1]);
          texte= TTF_RenderText_Blended(font1,resistance,rouge);
          SDL_BlitSurface(texte, NULL, ecran,&resistancepo);


           texte= TTF_RenderText_Blended(font1,pourcentage,rouge);
          SDL_BlitSurface(texte, NULL, ecran,&purcentagepo);

          }

           texte= TTF_RenderText_Blended(font1,fichesTypesUnites[indiceficheTypeUniteJoueur].nom,blanc);
           SDL_BlitSurface(texte, NULL, ecran, &positionnomJ);

          texte= TTF_RenderText_Blended(font1,fichesTypesUnites[indiceficheTypeUniteEnnemi].nom,blanc);
         SDL_BlitSurface(texte, NULL, ecran, &positionnomE);


         char level[10];
        sprintf(level,"%d",fichesTypesUnites[indiceficheTypeUniteJoueur].niveau);
        texte= TTF_RenderText_Blended(font1,level,gris);
         SDL_BlitSurface(texte, NULL, ecran, &polevel);

          polevel.x+=50;
        sprintf(level,"%d",fichesTypesUnites[indiceficheTypeUniteEnnemi].niveau);
        texte= TTF_RenderText_Blended(font1,level,gris);
         SDL_BlitSurface(texte, NULL, ecran, &polevel);
          polevel.x=230;
          char xp[10];
          char hp[10];
          char txt[5]="/";
          char xpmax[10];
          char hptxt[10];
         sprintf(hptxt,"%d",fichesTypesUnites[indiceficheTypeUniteJoueur].pvMax);
         texte= TTF_RenderText_Blended(font2,hptxt,vert);
         SDL_BlitSurface(texte, NULL, ecran, &pohpmax);
         pohpmax.x+=90;
            sprintf(hptxt,"%d",fichesTypesUnites[indiceficheTypeUniteEnnemi].pvMax);
         texte= TTF_RenderText_Blended(font2,hptxt,vert);
         SDL_BlitSurface(texte, NULL, ecran, &pohpmax);


         sprintf(xpmax,"%d",fichesTypesUnites[indiceficheTypeUniteJoueur].xpRequise);
         texte= TTF_RenderText_Blended(font2,xpmax,bleu);
         SDL_BlitSurface(texte, NULL, ecran, &poxpmax);
         poxpmax.x+=80;
         sprintf(xpmax,"%d",fichesTypesUnites[indiceficheTypeUniteEnnemi].xpRequise);
         texte= TTF_RenderText_Blended(font2,xpmax,bleu);
         SDL_BlitSurface(texte, NULL, ecran, &poxpmax);




         texte= TTF_RenderText_Blended(font2,txt,vert);
         SDL_BlitSurface(texte, NULL, ecran, &potxt);
            potxt.x+=85;
         texte= TTF_RenderText_Blended(font2,txt,vert);
         SDL_BlitSurface(texte, NULL, ecran, &potxt);
          potxt.x-=86;
           potxt.y+=23;
        texte= TTF_RenderText_Blended(font2,txt,bleu);
         SDL_BlitSurface(texte, NULL, ecran, &potxt);
           potxt.x+=78;
        texte= TTF_RenderText_Blended(font2,txt,bleu);
         SDL_BlitSurface(texte, NULL, ecran, &potxt);



        sprintf(hp,"%d",unitesJoueurs[indiceDansUnitesJoueurs].pv);
        texte= TTF_RenderText_Blended(font2,hp,vert);
         SDL_BlitSurface(texte, NULL, ecran, &pohp);



       sprintf(xp,"%d",unitesJoueurs[indiceDansUnitesJoueurs].xp);
        texte= TTF_RenderText_Blended(font2,xp,bleu);
         SDL_BlitSurface(texte, NULL, ecran, &poxp);

         pohp.x+=85;
         poxp.x+=78;
        sprintf(hp,"%d",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv);
        texte= TTF_RenderText_Blended(font2,hp,vert);
         SDL_BlitSurface(texte, NULL, ecran, &pohp);

           sprintf(xp,"%d",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].xp);
        texte= TTF_RenderText_Blended(font2,xp,bleu);
         SDL_BlitSurface(texte, NULL, ecran, &poxp);

         char race[30];
          char race1[30];
           strcpy(race,races[fichesTypesUnites[indiceficheTypeUniteJoueur].race-1]);
          texte= TTF_RenderText_Blended(font1,race,marron);
         SDL_BlitSurface(texte, NULL, ecran, &raceposition);
         raceposition.x+=100;

        strcpy(race1,races[fichesTypesUnites[indiceficheTypeUniteEnnemi].race-1]);
          texte= TTF_RenderText_Blended(font1,race1,marron);
         SDL_BlitSurface(texte, NULL, ecran, &raceposition);


          switch(fichesTypesUnites[indiceficheTypeUniteJoueur].idFicheTypeUnite)
          {
                                        case 50:
                                         SDL_BlitSurface(u50, NULL, ecran, &positionuniteJ);
                                         break;
                                         case 51:
                                         SDL_BlitSurface(u51, NULL, ecran, &positionuniteJ);
                                         break;
                                         case 61:
                                         SDL_BlitSurface(u61, NULL, ecran, &positionuniteJ);
                                         break;
                                         case 83:
                                         SDL_BlitSurface(u83, NULL, ecran, &positionuniteJ);
                                         break;
                                         case 90:
                                         SDL_BlitSurface(u90, NULL, ecran, &positionuniteJ);
                                          break;
                                          case 115:
                                          SDL_BlitSurface(u115, NULL, ecran, &positionuniteJ);
                                           break;
                                           case 130:
                                           SDL_BlitSurface(u130, NULL, ecran, &positionuniteJ);
                                            break;
                                            case 149:
                                            SDL_BlitSurface(u149, NULL, ecran, &positionuniteJ);
                                           break;
                                          case 232:
                                          SDL_BlitSurface(u232, NULL, ecran, &positionuniteJ);
                                           break;
                                         case 235:
                                          SDL_BlitSurface(u235, NULL, ecran, &positionuniteJ);
                                           break;

          }
          switch(fichesTypesUnites[indiceficheTypeUniteEnnemi].idFicheTypeUnite )
          {
                                         case 50:
                                         SDL_BlitSurface(u50, NULL, ecran, &positionuniteE);
                                         break;
                                         case 51:
                                         SDL_BlitSurface(u51, NULL, ecran, &positionuniteE);
                                         break;
                                         case 61:
                                         SDL_BlitSurface(u61, NULL, ecran, &positionuniteE);
                                         break;
                                         case 83:
                                         SDL_BlitSurface(u83, NULL, ecran, &positionuniteE);
                                         break;
                                         case 90:
                                         SDL_BlitSurface(u90, NULL, ecran, &positionuniteE);
                                          break;
                                          case 115:
                                          SDL_BlitSurface(u115, NULL, ecran, &positionuniteE);
                                           break;
                                           case 130:
                                           SDL_BlitSurface(u130, NULL, ecran, &positionuniteE);
                                            break;
                                            case 149:
                                            SDL_BlitSurface(u149, NULL, ecran, &positionuniteE);
                                           break;
                                          case 232:
                                          SDL_BlitSurface(u232, NULL, ecran, &positionuniteE);
                                           break;
                                         case 235:
                                          SDL_BlitSurface(u235, NULL, ecran, &positionuniteE);
                                           break;

          }

     SDL_Flip(ecran);
    SDL_Delay(2000);
    sirine=0;

}
   SDL_FreeSurface(texte);
    SDL_FreeSurface(menuAttaque);
    SDL_FreeSurface(u50);
    SDL_FreeSurface(u61);
    SDL_FreeSurface(u51);
    SDL_FreeSurface(u83);
    SDL_FreeSurface(u130);
    SDL_FreeSurface(u115);
    SDL_FreeSurface(u90);
    SDL_FreeSurface(u149);
    SDL_FreeSurface(u232);
    SDL_FreeSurface(u235);
    SDL_FreeSurface(bladeattaq);
    SDL_FreeSurface(arcaneattaq);
    SDL_FreeSurface(coldattaq);
    SDL_FreeSurface(ranged_attack);
    SDL_FreeSurface(spearattaq);
    SDL_FreeSurface(pierceattaq);
    SDL_FreeSurface(impactattaq);
    SDL_FreeSurface(fireattaq);
    TTF_CloseFont(font);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);

 }

int jouerTourEnnemi(int numtour,SDL_Surface* ecran,int numJoueur, int groupeAlliesJoueur, int nbFichesTypesUnites, int nbJoueurs,
 int nbUnitesMagasin, int nbVillages,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Joueur joueurs[MAX_LIGNES_JOUEURS],CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],Village villages[MAX_LIGNES_VILLAGES],
 TypeTerrain typesTerrains[NB_TYPES_TERRAINS],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],Periode periode)
 {

      int FinTour=0,FinTourforcee=0,IndiceUniteJoueur,ligne_courante,colonne_courante;
      int deplacer,ligneAdeplacer,ColonneAdeplacer,x,indiceJoueurEnnemi,idUniteEnnemie;
      int IndiceChefUnitesJoueurs,choix,CodeAttaque,UniteAttaquable,nbAttaques;
      int ligne_ennemie,colonne_ennemie,indiceficheTypeUniteEnnemi,Gagnant=0,indiceEnnemiDansUnitesJoueurs,indiceficheTypeUniteJoueur=0;
      int defenseEnnemi,defenseJoueur,indiceUniteEnnemie;
      SDL_Rect positiongagner;
     positiongagner.x=150;
     positiongagner.y=100;
     SDL_Surface  *joueurgagner = NULL,*ennemigagner=NULL;


      Attaque attaques[10][2];
      int resistances[10][2];
      Attaque attaqueChoisie[2];
      int resistanceAuxAttaquesChoisis[2];
      int attaquer,i=0;
      int choixs[15];
      int taillechoixs=0;
      int index=0;
       int j=2;
       int nbdeplacement=2;
        int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
       SDL_Rect position,positionText;
       SDL_Surface* texte = NULL;
        TTF_Font* font = NULL;

       SDL_Event event;
       position.x=400;
       position.y=500;

         TTF_Init();
        font = TTF_OpenFont("HeadingNowTrial-45Medium.ttf", 30);

        SDL_Color noir = {0, 0, 0}; //attention ce n'est pas un Uint32
        SDL_Color blanc = { 255, 255, 255 };
        SDL_Color gold = { 153, 0, 0 };
        //texte = TTF_RenderText_Blended(font,"Tour Ennemi", blanc);
        //SDL_BlitSurface(texte, NULL, ecran, &positionText);
             joueurgagner = IMG_Load("Human.PNG");
       ennemigagner = IMG_Load("Enemy.PNG");
       //code
      for(i=0;i<15;i++)
        {
            choixs[i]=0;
        }

      for (i=1;i<MAX_LIGNES_UNITESJOUEURS;i++)
      {

          if(unitesJoueurs[i].idJoueur==2)
          {
              choixs[j]=unitesJoueurs[i].idUnite;
              j++;
          }
      }
     taillechoixs=sizeof(choixs)/sizeof(choixs[0]);
     int nbboucle=0;
       FinTour=verifierFinTours(numJoueur,unitesJoueurs);
        Gagnant=verifierGagnant( nbJoueurs,joueurs,unitesJoueurs);

      while((FinTour==0)&&( FinTourforcee==0)&&(Gagnant==0)&& (nbdeplacement==2))
        {

        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                sauvegarderJeuComplet(nbVillages,nbJoueurs,villages,joueurs,unitesJoueurs);
                SDL_Quit();
               break;
        }
          afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,0,periode,numJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          //SDL_Delay (1500) ;


          //afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
        do{
            index=rand()%taillechoixs;
            x=choixs[index];
          //x=-1;
         }while((chercherIndiceUniteJoueur(x,2,1,unitesJoueurs)==-1));

     //printf("Num joueur a jouer =%d\n",x);
    while(joueurs[1].or > 110)
    {
        //printf("L'ennemi veut acheter des nouvelles unites\n");
        IndiceChefUnitesJoueurs=chercherIndiceChefUnitesJoueurs(numJoueur,unitesJoueurs);

        if(carte[unitesJoueurs[IndiceChefUnitesJoueurs].ligne][unitesJoueurs[IndiceChefUnitesJoueurs].colonne].idTypeTerrain==ID_TYPE_TERRAIN_DONJON )
        {
            //SDL_Delay(1000);
             acheterUnitesAuto(numJoueur,unitesJoueurs[IndiceChefUnitesJoueurs].ligne,unitesJoueurs[IndiceChefUnitesJoueurs].colonne ,nbUnitesMagasin,
                     nbJoueurs,nbFichesTypesUnites,unitesMagasin,unitesJoueurs,joueurs,fichesTypesUnites,carte);
            //afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
            //afficherCarteJeu(ecran,periode,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);



        }


    }
    if(x!=0){
   if((chercherIndiceUniteJoueur(x,2,1,unitesJoueurs)!=-1))
    {

          IndiceUniteJoueur=chercherIndiceUniteJoueur(x,2,1,unitesJoueurs);
          ligne_courante=unitesJoueurs[IndiceUniteJoueur].ligne;
          colonne_courante=unitesJoueurs[ IndiceUniteJoueur].colonne;

          marquerDeplacementsPossibles(carte,ligne_courante,colonne_courante,unitesJoueurs[IndiceUniteJoueur].mouvements);
           //SDL_Delay(3000);
         afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,0,periode,numJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);

         // afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
           // printf("deplacer unite %d de l'ennemi\n",x);
            deplacementAutomatiqueUnite(IndiceUniteJoueur,ligne_courante,colonne_courante,groupeAlliesJoueur,nbJoueurs,nbVillages,carte,joueurs,unitesJoueurs,villages);
             //SDL_Delay(1000);
            effacerDeplacementsPossibles(carte);
//           afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);

            CodeAttaque=calculerCodeAttaque(groupeAlliesJoueur,unitesJoueurs[IndiceUniteJoueur].ligne,unitesJoueurs[ IndiceUniteJoueur].colonne,nbJoueurs,joueurs,carte);

            if(CodeAttaque!=0)
            {
                //printf("Cette unite a la possibilite d'attaquer\n");

                idUniteEnnemie=choixAutomatiqueUniteEnnemie( CodeAttaque,unitesJoueurs[IndiceUniteJoueur].ligne,unitesJoueurs[ IndiceUniteJoueur].colonne ,nbJoueurs,carte,joueurs,unitesJoueurs);
                //printf("idUniteEnnemie a Attaquer =%d\n",idUniteEnnemie);
                indiceUniteEnnemie=chercherIndiceUniteJoueur(idUniteEnnemie,1,1,unitesJoueurs);
                ligne_ennemie=unitesJoueurs[indiceUniteEnnemie].ligne;
                colonne_ennemie=unitesJoueurs[ indiceUniteEnnemie].colonne;

                UniteAttaquable=verifierUniteAttaquable(CodeAttaque,unitesJoueurs[IndiceUniteJoueur].ligne,unitesJoueurs[IndiceUniteJoueur].colonne,ligne_ennemie,colonne_ennemie);

                if(UniteAttaquable==1)
                {
                    //printf("ligne de ennemi a attaquer : %d\n",ligne_ennemie);
                    //printf("colonne de ennemi a attaquer : %d\n",colonne_ennemie);
                    indiceEnnemiDansUnitesJoueurs=chercherIndiceUniteJoueur(carte[ligne_ennemie][colonne_ennemie].idUnite,1,1,unitesJoueurs);
                    indiceficheTypeUniteEnnemi=chercherIndiceFicheTypeUnite(unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
                     indiceJoueurEnnemi=chercherIndiceJoueur(1,nbJoueurs,joueurs);
                     indiceficheTypeUniteJoueur=chercherIndiceFicheTypeUnite(unitesJoueurs[IndiceUniteJoueur].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);

                     defenseEnnemi=fichesTypesUnites[indiceficheTypeUniteEnnemi].relationsTerrains[carte[ligne_ennemie][colonne_ennemie].idTypeTerrain].defense;
                     defenseJoueur=fichesTypesUnites[indiceficheTypeUniteJoueur].relationsTerrains[carte[unitesJoueurs[IndiceUniteJoueur].ligne][unitesJoueurs[ IndiceUniteJoueur].colonne].idTypeTerrain].defense ;

                     nbAttaques=determinerPossibilitesAttaque(numJoueur,carte[unitesJoueurs[IndiceUniteJoueur].ligne][unitesJoueurs[ IndiceUniteJoueur].colonne].idUnite, indiceficheTypeUniteJoueur,
                            indiceficheTypeUniteEnnemi,fichesTypesUnites, IndiceUniteJoueur, indiceEnnemiDansUnitesJoueurs,attaques,resistances);

                      int indiceDansUnitesJoueurs=(chercherIndiceUniteJoueur(x,numJoueur,1,unitesJoueurs));

                     choisirAttaqueAutomatiquement(ecran,unitesJoueurs,numJoueur,idUniteEnnemie, indiceficheTypeUniteJoueur,indiceficheTypeUniteEnnemi,
                           fichesTypesUnites, indiceDansUnitesJoueurs,indiceEnnemiDansUnitesJoueurs,
                         defenseJoueur, defenseEnnemi, nbAttaques,attaques,resistances,attaqueChoisie,resistanceAuxAttaquesChoisis,periode,
                           unitesJoueurs[IndiceUniteJoueur].alignement, unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement );

                      attaque(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,unitesJoueurs[ IndiceUniteJoueur].idUnite,nbVillages,villages,IndiceUniteJoueur, indiceEnnemiDansUnitesJoueurs,numJoueur,indiceJoueurEnnemi,
                       unitesJoueurs[IndiceUniteJoueur].ligne , unitesJoueurs[IndiceUniteJoueur].colonne,ligne_ennemie,colonne_ennemie,carte,unitesJoueurs,
                         joueurs, defenseJoueur, defenseEnnemi, attaqueChoisie,resistanceAuxAttaquesChoisis,periode,
                         unitesJoueurs[IndiceUniteJoueur].alignement,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement );
                          Gagnant=verifierGagnant( nbJoueurs,joueurs,unitesJoueurs);

   if(Gagnant==1)
    {

        afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,0,periode,numJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          SDL_BlitSurface(joueurgagner, NULL, ecran, &positiongagner);
            SDL_Flip(ecran);
           SDL_Delay(5000);
    }
    else if(Gagnant==2){

          afficherCarteJeu(numtour,ecran,nbFichesTypesUnites,fichesTypesUnites,0,periode,numJoueur,carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          SDL_BlitSurface(ennemigagner, NULL, ecran, &positiongagner);
           SDL_Flip(ecran);
            SDL_Delay(5000);

    }
                }
            }
        else{
             effacerDeplacementsPossibles(carte);
       }
    }

    }
    else{

    }
    if(nbboucle==3)
    {
        FinTourforcee=1;
    }
    nbboucle++;
        FinTour=verifierFinTours(numJoueur,unitesJoueurs);
      Gagnant=verifierGagnant( nbJoueurs,joueurs,unitesJoueurs);
 //nbdeplacement++;
}
    SDL_FreeSurface(texte); //On oublie toujours pas
    TTF_CloseFont(font);
    SDL_FreeSurface(joueurgagner);
    SDL_FreeSurface(ennemigagner);
   return Gagnant;
 }



void deroulerJeu(SDL_Surface* ecran,int nbFichesTypesUnite, int nbJoueurs,int nbUnitesMagasin,int nbVillages,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Joueur joueurs[MAX_LIGNES_JOUEURS], CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],
 Village villages[MAX_LIGNES_VILLAGES],TypeTerrain typesTerrains[NB_TYPES_TERRAINS],Periode periodes[NB_LIGNES_PERIODES],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {

     int numtour=1,i,resultat=0;
    Periode periodeCorante;
    periodeCorante=periodes[numtour%6];
     SDL_Rect positiongagner;
     positiongagner.x=300;
     positiongagner.y=200;
     //SDL_Surface  *joueurgagner = NULL,*ennemigagner=NULL;
      //joueurgagner = IMG_Load("Human Player Win!.png");
       //ennemigagner = IMG_Load("Human_Player_Win (2).png");

    while(resultat==0){
        //printf("****** Tour: %d ******\n",numtour);
     for (i=0;i<nbJoueurs;i++)
     {



             if(joueurs[i].type==1)
              {
                 if(i!=0)
                 {
                      reinitialiserTours(joueurs[i].idJoueur, nbJoueurs,joueurs,unitesJoueurs);
                 }

                  //printf("*** Tour Humain *****\n ");
                   periodeCorante=periodes[numtour%6];
                   resultat=jouerTourHumain(numtour,ecran,joueurs[i].idJoueur,joueurs[i].groupe_allies,nbFichesTypesUnite,nbJoueurs,nbUnitesMagasin,nbVillages,
                        fichesTypesUnites,joueurs,carte,unitesMagasin,villages,typesTerrains,unitesJoueurs,periodeCorante);



              }
              else
              {
                /*if(chercherGroupeAllies(joueurs[i].symbole,nbJoueurs,joueurs)==1)
                {
                    printf("***** Tour GroupeAllies*****\n");
                    numtour++;
                }*/
                //else
                //{
                   if(joueurs[i].type==2)
                   {
                       if(i!=0)
                       {


                            reinitialiserTours(joueurs[i].idJoueur, nbJoueurs,joueurs,unitesJoueurs);
                       }

                        //printf("***** Tour Enemie *****\n");
                         periodeCorante=periodes[numtour%6];
                        resultat=jouerTourEnnemi(numtour,ecran,joueurs[i].idJoueur,joueurs[i].groupe_allies,nbFichesTypesUnite,nbJoueurs,nbUnitesMagasin,
                            nbVillages,fichesTypesUnites,joueurs,carte,unitesMagasin,villages,typesTerrains,unitesJoueurs,periodeCorante);

                       }

                }

    /*if(resultat==1)
    {


          SDL_BlitSurface(joueurgagner, NULL, ecran, &positiongagner);
    }
    else if(resultat==2){

          SDL_BlitSurface(ennemigagner, NULL, ecran, &positiongagner);

    }*/


             }
        //printf("resultat=%d",resultat);


    //}
    numtour++;
     //SDL_Flip(ecran);
     //SDL_Delay(2000);
    //periodeCorante=periodes[];
    }


 }

int main(int argc, char *argv[])
 {

      CelluleCarte carte[NB_LIGNES_CARTE][ NB_COLONNES_CARTE];
      UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN];
      Village villages[MAX_LIGNES_VILLAGES];
      TypeTerrain typesTerrains[NB_TYPES_TERRAINS];
      Periode periodes[NB_LIGNES_PERIODES];
      Joueur joueurs[MAX_LIGNES_JOUEURS];
      UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS];
      FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES];
      int nbfichesTypesUnites=0,nbJoueurs=0,nbUnitesMagasin=0,nbVillages=0;


      SDL_Surface *ecran = NULL, *menu = NULL,*quitter=NULL;
      SDL_Rect positionMenu,positionquitter;
      SDL_Event event;
      TTF_Font* font = NULL;
      SDL_Surface* texte = NULL;
      int indiceOption=0;
      int continuer = 1;
      positionquitter.x=400;
      positionquitter.y=240;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_WM_SetIcon(IMG_Load("battle.PNG"), NULL); // L'ic�ne doit �tre charg�e avant SDL_SetVideoMode
    ecran = SDL_SetVideoMode(1100,600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("The Battle for Wesnoth", NULL);
//Backgroundmenu  24taillefont
    menu = IMG_Load("dernier.png");

    positionMenu.x = 0;
    positionMenu.y = 0;

    TTF_Init();
        font = TTF_OpenFont("HeadingNowTrial-45Medium.ttf", 34);

        SDL_Color noir = {0, 0, 0}; //attention ce n'est pas un Uint32
        SDL_Color blanc = { 255, 255, 255 };
        //SDL_Color gold = { 153, 0, 0 };
         SDL_Color gold = { 255, 255, 51};
         SDL_Color bleu = { 0, 0, 51};
        SDL_Rect positionText;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
              sauvegarderJeuComplet(nbVillages,nbJoueurs,villages,joueurs,unitesJoueurs);
               continuer = 0;
               break;
           case SDL_KEYDOWN:
              switch(event.key.keysym.sym)
              {
                   case SDLK_ESCAPE:
                    sauvegarderJeuComplet(nbVillages,nbJoueurs,villages,joueurs,unitesJoueurs);
 // Veut arr�ter le jeu
                    continuer = 0;
                   break;
                   case SDLK_KP_ENTER: case SDLK_RETURN: // Veut arr�ter le jeu
                   if(indiceOption==0)
                    {

                      initialiserNouveauJeu(&nbfichesTypesUnites,&nbJoueurs,&nbUnitesMagasin,&nbVillages,fichesTypesUnites,joueurs,carte,unitesMagasin,villages,typesTerrains,periodes,unitesJoueurs);
                    //sauvegarderJeuComplet(nbVillages,nbJoueurs,villages,joueurs,unitesJoueurs);
                     deroulerJeu(ecran,nbfichesTypesUnites, nbJoueurs, nbUnitesMagasin, nbVillages, fichesTypesUnites,joueurs,  carte, unitesMagasin,villages, typesTerrains, periodes,unitesJoueurs);
                     sauvegarderJeuComplet(nbVillages,nbJoueurs,villages,joueurs,unitesJoueurs);
                   }

                    else if(indiceOption==1)
                     {
                         chargerJeuComplet(&nbfichesTypesUnites,&nbJoueurs,&nbUnitesMagasin,&nbVillages,fichesTypesUnites,joueurs,carte,unitesMagasin,villages,typesTerrains,periodes,unitesJoueurs);

                               deroulerJeu(ecran,nbfichesTypesUnites, nbJoueurs, nbUnitesMagasin, nbVillages, fichesTypesUnites,joueurs,  carte, unitesMagasin,villages, typesTerrains, periodes,unitesJoueurs);

                         sauvegarderJeuComplet(nbVillages,nbJoueurs,villages,joueurs,unitesJoueurs);

                     }
                    if(indiceOption==2)
                   {
                    //editeur(ecran);
                     sauvegarderJeuComplet(nbVillages,nbJoueurs,villages,joueurs,unitesJoueurs);
                    continuer=0;
                   }
                   break;
                   case SDLK_LEFT:
                   if(indiceOption>0)
                    {
                    indiceOption--;
                    }
                    break;
                   case SDLK_RIGHT:
                   if(indiceOption<2)
                   {
                    indiceOption++;
                   }
                   break;
            }
            break;
        }
         SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        // attachement du menu � l'�cran
        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);

        if(font != 0)
        {
             char* phrase[3];
            phrase[0]= "New Game";
            phrase[1]= "Load Full Game";

            phrase[2]= "Quit";
            //phrase[2]= "hii";
            int x[3];
            x[0]=240;
            x[1]=490;
            x[2]=803;
            int i, x_depart=2;
            SDL_Color couleur_a_utiliser;

            for(i=0; i<3; i++)
            {
                if(i==indiceOption)
                {

                    couleur_a_utiliser=gold;
                }
                else
                {
                    couleur_a_utiliser=bleu;
                }
                texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser) ;
                positionText.x =x[i];
                positionText.y = 455;
                SDL_BlitSurface(texte, NULL, ecran, &positionText);

            }


        }

        SDL_Flip(ecran);
    }

    SDL_FreeSurface(texte); //On oublie toujours pas
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
 }













