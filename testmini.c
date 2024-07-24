#include<stdio.h>
#include<stdlib.h>
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
        fscanf(terrains,"%d %c %d %s\n",&typesTerrains[i].idTypeTerrain,&typesTerrains[i].symboleTerrain,&typesTerrains[i].codeAffichageTerrain,typesTerrains[i].nomTerrain);
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
void sauvegarderUnitesMagasin(int nbUnitesMagasin, UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN], char* nomFichier)
{
    FILE* UnitesMagasin=fopen(nomFichier,"w");
    for (int i=0;i< nbUnitesMagasin; i++)
     {
        fprintf(UnitesMagasin, "%d %d %d \n",unitesMagasin[i].idUniteMagasin,unitesMagasin[i].idFicheTypeUnite,unitesMagasin[i].idJoueurAutorise);
     }
    fclose(UnitesMagasin);
}
void sauvegarderVillages(int nb_lignes, Village villages[MAX_LIGNES_VILLAGES], char* nomFichier)
{
    FILE* Villages=fopen(nomFichier,"w");
    for (int i=0;i< nb_lignes; i++)
     {
         fprintf(Villages, "%d %d %d %d \n",villages[i].idVillage,villages[i].ligne,villages[i].colonne,villages[i].idJoueurProprietaire);
     }
    fclose(Villages);
}
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
{
    int i=0;
   for (i=0;i<nbFichesTypesUnites;i++)
   {
      if(idFicheTypeUnite==fichesTypesUnites[i].idFicheTypeUnite)
      {
          return i;
      }
   }
  return -1;

}
int chercherIndiceTypeTerrain(char symboleTerrain, TypeTerrain typesTerrains[NB_TYPES_TERRAINS])
{
   int i=0;
  for (i=0;i<NB_TYPES_TERRAINS;i++)
  {
      if(symboleTerrain==typesTerrains[i].symboleTerrain)
      {

          return i;
      }
  }
  return -1;
}

int chercherIndiceRelationTerrain(int idTypeTerrain, RelationTerrain relationsTerrains[NB_TYPES_TERRAINS])
{   int i=0;
   for(i=0;i<NB_TYPES_TERRAINS;i++)
    {
      if (idTypeTerrain==relationsTerrains[i].idTypeTerrain)
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

 int chargerJoueursVersTableau(Joueur joueurs[MAX_LIGNES_JOUEURS], char* nomFichier)
 {
    int i=0;
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
void afficherJoueurs(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS])
{
    int i;
    printf("Joueurs:\n ");
    for(i=0;i<nbJoueurs;i++)
    {
        printf("%d %c %d %d %d %d %d %d %d\n",joueurs[i].idJoueur,joueurs[i].symbole,joueurs[i].or,joueurs[i].type,joueurs[i].groupe_allies,joueurs[i].nombreVillages,joueurs[i].nombreUnites,joueurs[i].entretien,joueurs[i].revenu);
    }
}

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
int chargerFichesTypesUnitesVersTableau(FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES], char* nomFichier)
{
    int i=0,j=0;
    FILE* fichesTypesUnite=fopen(nomFichier,"r");
    while (!feof(fichesTypesUnite))
    {
        fscanf(fichesTypesUnite,"%d %d %s %d %d %d %d %d %d",&fichesTypesUnites[i].idFicheTypeUnite,
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
void chargerUniteJoueurVersTableau(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],char* nomFichier)
{   int i=0,j=0;
    FILE* Fichier=fopen(nomFichier,"r");
    while (!feof(Fichier))
    {
        fscanf(Fichier,"%d %d %d %s %d %d %d %d %d %d %d",&unitesJoueurs[i].idUnite,&unitesJoueurs[i].idFicheTypeUnite,
               &unitesJoueurs[i].idJoueur,unitesJoueurs[i].nomUnite,&unitesJoueurs[i].active,&unitesJoueurs[i].rang,
               &unitesJoueurs[i].ligne,&unitesJoueurs[i].colonne,&unitesJoueurs[i].traits[0],&unitesJoueurs[i].traits[1],
               &unitesJoueurs[i].traits[2]);
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
void afficherUniteJoueur( UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
{
   int i=0,j=0,k=0;
   printf("Unite Joueur:\n");
   for(i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
   {

        printf("%d %d %d %s %d %d %d %d ",unitesJoueurs[i].idUnite,unitesJoueurs[i].idFicheTypeUnite,
           unitesJoueurs[i].idJoueur,unitesJoueurs[i].nomUnite,unitesJoueurs[i].active,unitesJoueurs[i].rang,
           unitesJoueurs[i].ligne,unitesJoueurs[i].colonne);
       for(j=0;j<3;j++)
           {
               printf("%d ",unitesJoueurs[i].traits[j]);
           }
        printf("%d %d %d %d %d %d %d %d %d ",unitesJoueurs[i].pv,unitesJoueurs[i].pvMax,unitesJoueurs[i].mouvements,unitesJoueurs[i].mouvementsMax,unitesJoueurs[i].xp,unitesJoueurs[i].xpRequise,unitesJoueurs[i].niveau,unitesJoueurs[i].alignement,unitesJoueurs[i].finTour);
       for(j=0;j<MAX_EFFETS_ATTAQUES_SUBITES;j++)
        {
             printf("%d %d\n",unitesJoueurs[i].effetsAttaquesSubites[j].idAttaqueSpeciale,unitesJoueurs[i].effetsAttaquesSubites[j].toursRestants);
        }
   }
}
void sauvegarderUniteJoueur(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS], char* nomFichier)
{
    int i=0,j=0;
    FILE* fichier = fopen(nomFichier, "w");

    for (i=0; i < MAX_LIGNES_UNITESJOUEURS; i++)
    {   if(unitesJoueurs[i].idUnite!=0)
        {
            fprintf(fichier,"%d %d %d %s %d %d %d %d ", unitesJoueurs[i].idUnite, unitesJoueurs[i].idFicheTypeUnite, unitesJoueurs[i].idJoueur,
                unitesJoueurs[i].nomUnite, unitesJoueurs[i].active, unitesJoueurs[i].rang, unitesJoueurs[i].ligne,
                unitesJoueurs[i].colonne);

        for (j = 0; j < 3; j++)
        {
            fprintf(fichier,"%d ", unitesJoueurs[i].traits[j]);

        }
        fprintf(fichier,"%d %d %d %d %d %d %d\n", unitesJoueurs[i].pvMax, unitesJoueurs[i].mouvements, unitesJoueurs[i].mouvementsMax,
                unitesJoueurs[i].xp, unitesJoueurs[i].xpRequise, unitesJoueurs[i].niveau, unitesJoueurs[i].finTour);

        }
    }

    fclose(fichier);
}

void chargerCarteVersTableau(CelluleCarte carte[NB_LIGNES_CARTE][ NB_COLONNES_CARTE],TypeTerrain typesTerrains[NB_TYPES_TERRAINS], char* nomFichier)
{  int i=0,j=0;
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
void afficherCarte(CelluleCarte carte[NB_LIGNES_CARTE][ NB_COLONNES_CARTE])
{ int i=0 ,j=0;
  printf("   La Carte :\n");
  for (i=0;i<NB_LIGNES_CARTE;i++)
    {
        for(j=0;j<NB_COLONNES_CARTE;j++)
        {
            printf("|%-2d%d %d %-2c%d",carte[i][j].idUnite,carte[i][j].idTypeTerrain,carte[i][j].codeAffichageTerrain,carte[i][j].symboleJoueur,carte[i][j].marque);
        }

        printf("|\n");
    }
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
void chargerUnitesJoueursSauvegardeesVersTableau(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS], char* nomFichier)
{
    int i=0,j=0;
    FILE* Fichier=fopen(nomFichier,"r");
    while (i < MAX_LIGNES_UNITESJOUEURS )
    {
        fscanf(Fichier,"%d %d %d %s %d %d %d %d",&unitesJoueurs[i].idUnite,&unitesJoueurs[i].idFicheTypeUnite,&unitesJoueurs[i].idJoueur,unitesJoueurs[i].nomUnite,
               &unitesJoueurs[i].active,&unitesJoueurs[i].rang,&unitesJoueurs[i].ligne,&unitesJoueurs[i].colonne);
        for (j=0;j<MAX_TRAITS;j++)
        {
            fscanf(Fichier,"%d",&unitesJoueurs[i].traits[j]);
        }
        fscanf(Fichier,"%d %d %d %d %d %d %d %d %d",&unitesJoueurs[i].pv,&unitesJoueurs[i].pvMax,&unitesJoueurs[i].mouvements,&unitesJoueurs[i].mouvementsMax,&unitesJoueurs[i].xp,&unitesJoueurs[i].xpRequise,&unitesJoueurs[i].niveau,&unitesJoueurs[i].alignement,&unitesJoueurs[i].finTour);
        for(j=0;j<MAX_EFFETS_ATTAQUES_SUBITES;j++)
        {
            fscanf(Fichier,"%d %d",&unitesJoueurs[i].effetsAttaquesSubites[j].idAttaqueSpeciale,&unitesJoueurs[i].effetsAttaquesSubites[j].toursRestants);
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
      if (indice!=-1)
      {
      carte[unitesJoueurs[i].ligne][unitesJoueurs[i].colonne].symboleJoueur=joueurs[indice].symbole;
      }
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
 *nbfichesTypesUnites=chargerFichesTypesUnitesVersTableau(fichesTypesUnites,fichesTypesUnites_original);
 chargerCarteVersTableau(carte,typesTerrains,carte_original);
  placerUnitesDansCarte( unitesJoueurs,*nbJoueurs,joueurs,carte);
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
    char* unitesMagasin_original="unitesMagasin_original.txt";
   char* villages_original="villages_original.txt";
   char* typesTerrains_original="typesTerrains_original.txt";
   char* periodes_original="periodes_original.txt";
   char* joueurs_original="joueurs_original.txt";
   char* fichesTypesUnites_original="fichesTypesUnites_original.txt";
   char* unitesJoueurs_original="unitesJoueurs_original.txt";
   char* carte_original="carte_original.txt";
   char* UnitesJoueursSauvegardees="sauvegarderuniteJoueur.txt";
    *nbUnitesMagasin=chargerUnitesMagasinVersTableau(unitesMagasin,unitesMagasin_original);
    chargerTypesTerrainsVersTableau(typesTerrains,typesTerrains_original);
    chargerPeriodesVersTableau(periodes,periodes_original);
    *nbFichesTypesUnites=chargerFichesTypesUnitesVersTableau(fichesTypesUnites,fichesTypesUnites_original);
    *nbJoueurs=chargerJoueursVersTableau(joueurs,joueurs_original);
    *nbVillages=chargerVillagesVersTableau(villages,villages_original);
    chargerUniteJoueurVersTableau(unitesJoueurs,unitesJoueurs_original);
    chargerCarteVersTableau(carte,typesTerrains,carte_original);
    chargerUnitesJoueursSauvegardeesVersTableau( unitesJoueurs,UnitesJoueursSauvegardees);
    placerUnitesDansCarte(unitesJoueurs,*nbJoueurs,joueurs,carte);
    completerInitialisationJoueurs(*nbJoueurs,*nbVillages,joueurs,villages,unitesJoueurs);
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
 {
    char* sauvegarderVillage="sauvegarderVillages.txt";
    char* sauvegarderJoueur="JoueursSauvegardees.txt";
    char* sauvegarderuniteJoueur="sauvegarderuniteJoueur.txt";
    sauvegarderVillages(nbLignesVillages,villages,sauvegarderVillage);
    sauvegarderJoueurs(nbLignesJoueurs,joueurs, sauvegarderJoueur);
    sauvegarderUniteJoueur(unitesJoueurs,sauvegarderuniteJoueur);
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
 {  int i=0;
    for(i=0;i<nbVillages;i++)
    {
        if ((villages[i].colonne==colonne)&&(villages[i].ligne==ligne))
        {
            return i;
        }
    }
    return -1;
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
 void afficherJoueurJeu(int idJoueur, int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS])
 {
   int indice=10;
   int i=0;
   indice=chercherIndiceJoueur(idJoueur,nbJoueurs, joueurs);
   printf("***Joueur***\n");
   for(i=0;i<101;i++)
   {
       printf("-");
   }
   printf("\n");
   printf("| idJoueur | symbole  |     or    |   type   |grp_allies|nbVillages|nbrUnites |entretien |  revenu  |\n");
   for(i=0;i<101;i++)
   {
       printf("-");
   }
   printf("\n");
   printf("|     %d    |     %c    |     %3d   |     %d    |     %2d   |     %d    |     %-2d   |     %d    |     %2d   |\n",joueurs[indice].idJoueur,joueurs[indice].symbole,joueurs[indice].or,joueurs[indice].type,joueurs[indice].groupe_allies,
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
 void afficherCarteJeu(CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS],int nbVillages,
 Village villages[MAX_LIGNES_VILLAGES], UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
     int i=0,j=0,k=0,s=1,indice=0,IndiceVillage=0;
     printf("dans afficher carte\n");
     indice=chercherIndiceJoueurHumain(nbJoueurs,joueurs);
     for(i=0;i<nbJoueurs;i++)
     {
         if(joueurs[i].groupe_allies!=joueurs[indice].groupe_allies)
         {
              afficherJoueurJeu(joueurs[i].idJoueur,nbJoueurs,joueurs);
              afficherUnitesJoueursJeu(joueurs[i].idJoueur,1,unitesJoueurs);

         }
     }
     printf("***Carte Jeu ***\n ");
     printf("\n");
    for(s=0;s<39;s++)
     {
         printf("%-6d",s);
     }
     printf("\n");
    for(i=0;i<231;i++)
     {
         printf("-");
     }
     printf("\n");
    for(j=0;j<NB_LIGNES_CARTE;j++)
      {
         printf("%3d",j+1);

          for(k=0;k<NB_COLONNES_CARTE;k++)
           {
                if (carte[j][k].marque==1)
                 {
                     if(carte[j][k].idUnite==0)
                     {
                         if (carte[j][k].idTypeTerrain==ID_TYPE_TERRAIN_VILLAGE)
                         {
                             IndiceVillage=chercherIndiceVillage(j,k,nbVillages,villages);
                             printf("|%c %d++",carte[j][k].codeAffichageTerrain,villages[IndiceVillage].idJoueurProprietaire);
                         }
                         else
                         {
                            printf("|%c ++ ",carte[j][k].codeAffichageTerrain);
                         }
                    }
                    else
                    {
                        printf("|%c%c%-3d",carte[j][k].codeAffichageTerrain,carte[j][k].symboleJoueur,carte[j][k].idUnite);
                    }
                  }
                else{
                     if(carte[j][k].idUnite==0)
                     {
                         if (carte[j][k].idTypeTerrain==ID_TYPE_TERRAIN_VILLAGE)
                         {
                             IndiceVillage=chercherIndiceVillage(j,k,nbVillages,villages);
                             printf("|%c %d  ",carte[j][k].codeAffichageTerrain,villages[IndiceVillage].idJoueurProprietaire);
                         }
                         else
                         {
                             printf("|%c    ",carte[j][k].codeAffichageTerrain,carte[j][k].symboleJoueur,carte[j][k].idUnite);
                         }
                     }
                     else
                     {
                        printf("|%c%c%-3d",carte[j][k].codeAffichageTerrain,carte[j][k].symboleJoueur,carte[j][k].idUnite);
                     }
                }

           }
           printf("|\n");
         for(i=0;i<231;i++)
           {
             printf("-");
           }
      printf("\n");
         }

     printf("\n");

     for(i=0;i<nbJoueurs;i++)
     {
         if(joueurs[i].groupe_allies==joueurs[indice].groupe_allies)
         {
              afficherJoueurJeu(joueurs[i].idJoueur,nbJoueurs,joueurs);
              afficherUnitesJoueursJeu(joueurs[i].idJoueur,1,unitesJoueurs);
         }
     }
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
void afficherUnitesMagasinUnJoueur(int idJoueur, int nbUnitesMagasin, int nbFichesTypesUnites,UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES])
 {
     int i=0,j=0,IndiceFicheTypeUnite=0;
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
              printf("\n");

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

/* int chercherIndiceChefUnitesJoueurs(int idJoueur, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
     int i;
    for (i=0;i<MAX_LIGNES_UNITESJOUEURS;i++)
    {
        if((unitesJoueurs[i].idJoueur==idJoueur)&&(unitesJoueurs[i].active==1)&&(unitesJoueurs[i].rang==1))
        {
            return i;
        }

    }
 }*/


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


 void acheterUnites(int idJoueur, int ligneDonjon, int colonneDonjon, int nbUnitesMagasin, int nbJoueurs, int nbFichesTypesUnites, int nbVillages,
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],
 Joueur joueurs[MAX_LIGNES_JOUEURS],FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],Village villages[MAX_LIGNES_VILLAGES],
 CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE])
 {  int ligne ,colonne,numUnite,IndiceJoueur, IndiceUniteMagasin,IndiceFicheTypeUnite,IndiceUniteJoueur,choix,confirmerAchat;


            afficherJoueurJeu(idJoueur, nbJoueurs,joueurs);
           afficherUnitesMagasinUnJoueur(idJoueur,nbUnitesMagasin,nbFichesTypesUnites,unitesMagasin,fichesTypesUnites);
           do{
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
     int i=0,j=0,typeattaque,nbAttaques=0;
     for (i=0;i<MAX_ATTAQUES;i++)
     {

        if(fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaque!=0)
        {
             for (j=0;j<MAX_ATTAQUES;j++)
             {


                 if(fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].porteeAttaque==fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].porteeAttaque)
                 {

                     attaques[j][0].degat=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].degat;
                     attaques[j][0].idAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaque;
                     attaques[j][0].idAttaqueSpeciale1=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaqueSpeciale1;
                     attaques[j][0].idAttaqueSpeciale2=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaqueSpeciale2;
                      strcpy(attaques[j][0].nomAttaque,fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].nomAttaque);
                    attaques[j][0].nombreAttaques=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].nombreAttaques;
                    attaques[j][0].porteeAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].porteeAttaque;
                    attaques[j][0].typeAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].typeAttaque;
                    //ennemi
                    attaques[j][1].degat=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].degat;
                     attaques[j][1].idAttaque=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].idAttaque;
                     attaques[j][1].idAttaqueSpeciale1=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].idAttaqueSpeciale1;
                     attaques[j][1].idAttaqueSpeciale2=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].idAttaqueSpeciale2;
                    strcpy(attaques[j][1].nomAttaque,fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].nomAttaque);
                    attaques[j][1].nombreAttaques=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].nombreAttaques;
                    attaques[j][1].porteeAttaque=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].porteeAttaque;
                    attaques[j][1].typeAttaque=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].typeAttaque;
                    typeattaque=fichesTypesUnites[indiceficheTypeUniteEnnemi].attaques[j].typeAttaque;
                    resistances[j][1]= fichesTypesUnites[indiceficheTypeUniteJoueur].resistances[typeattaque];
                    typeattaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].typeAttaque;
                     resistances[j][0]=fichesTypesUnites[indiceficheTypeUniteEnnemi].resistances[typeattaque];




                 }
                 else
                 {

                     attaques[j][0].degat=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].degat;
                     attaques[j][0].idAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaque;
                     attaques[j][0].idAttaqueSpeciale1=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaqueSpeciale1;
                     attaques[j][0].idAttaqueSpeciale2=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].idAttaqueSpeciale2;
                      strcpy(attaques[j][0].nomAttaque,fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].nomAttaque);
                    attaques[j][0].nombreAttaques=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].nombreAttaques;
                    attaques[j][0].porteeAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].porteeAttaque;
                    attaques[j][0].typeAttaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].typeAttaque;
                    //ennemi
                    attaques[j][1].degat=0;
                     attaques[j][1].idAttaque=0;
                     attaques[j][1].idAttaqueSpeciale1=0;
                     attaques[j][1].idAttaqueSpeciale2=0;
                    strcpy(attaques[j][1].nomAttaque," ");
                    attaques[j][1].nombreAttaques=0;
                    attaques[j][1].porteeAttaque=0;
                    attaques[j][1].typeAttaque=0;
                    typeattaque=fichesTypesUnites[indiceficheTypeUniteJoueur].attaques[i].typeAttaque;
                    resistances[j][0]=fichesTypesUnites[indiceficheTypeUniteEnnemi].resistances[typeattaque];
                     resistances[j][1]=0;

                 }

             }


         nbAttaques++;
        }


     }
     return nbAttaques;
 }
int choisirAttaque(int numJoueur, int numEnnemi,int indiceficheTypeUniteJoueur, int indiceficheTypeUniteEnnemi,
FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],int indiceDansUnitesJoueurs, int indiceEnnemiDansUnitesJoueurs,
int defenseJoueur, int defenseEnnemi, int nbrAttaques, Attaque attaques[10][2], int resistances[10][2], Attaque attaqueChoisie[2],
int resistanceAuxAttaquesChoisis[2], Periode periode, int alignementJoueur, int alignementEnnemi)
{
    int i=0,choix;
    int degatE,degatJ;
    printf(" Priode : %s\n",periode.moment);

    for (i=0;i<100;i++)
    {
        printf("-");
    }
    printf("\n");
    printf("|Vos                    Bonus                   |Ennemi                       Bonus                |\n");
    printf("|Defanse :  %2d         d'alignement:%2d           |Defanse :  %2d               d'alignement:%2d      |\n",defenseJoueur,
    periode.bonus[alignementJoueur],defenseEnnemi,periode.bonus[alignementEnnemi]);
    for (i=0;i<100;i++)
    {
        printf("-");
    }
    printf("\n");
    for(i=0;i<nbrAttaques;i++)
    {

          degatE=attaques[i][1].degat+((attaques[i][1].degat/100)*periode.bonus[alignementEnnemi]);
           degatJ=attaques[i][0].degat+((attaques[i][0].degat/100)*periode.bonus[alignementJoueur]);

        printf("|%d- %10s     %d  X %d  (resistance %2d %% )  <-- %d --> %10s     %d X %d  (resistance %2d %% )|\n",
               i+1,attaques[i][0].nomAttaque,attaques[i][0].degat,attaques[i][0].nombreAttaques,resistances[i][0],
               i,attaques[i][1].nomAttaque,attaques[i][1].degat,attaques[i][1].nombreAttaques,resistances[i][1]);
    }


    printf("|0-Annler l'attaque !                                                                              |\n");
    for (i=0;i<100;i++)
    {
        printf("-");
    }
    printf("\n");
    do{
    printf("Choisir un numero d'attaque ou 0 pour l'annuler : \n");
    scanf("%d",&choix);
    }while(choix<0);
    if(choix==0)
    {
        printf("  l'attaque est annler !\n");
      attaqueChoisie[0].idAttaque=0;
      resistanceAuxAttaquesChoisis[0]=0;
      return 0;
    }
    else
    {
        attaqueChoisie[0].idAttaque=attaques[choix-1][0].idAttaque;
        attaqueChoisie[0].degat=attaques[choix-1][0].degat;
        attaqueChoisie[0].idAttaqueSpeciale1=attaques[choix-1][0].idAttaqueSpeciale1;
        attaqueChoisie[0].idAttaqueSpeciale2=attaques[choix-1][0].idAttaqueSpeciale2;
        strcpy( attaqueChoisie[0].nomAttaque,attaques[choix-1][0].nomAttaque);
        attaqueChoisie[0].nombreAttaques=attaques[choix-1][0].nombreAttaques;
        attaqueChoisie[0].porteeAttaque=attaques[choix-1][0].porteeAttaque;
        attaqueChoisie[0].typeAttaque=attaques[choix-1][0].typeAttaque;
        resistanceAuxAttaquesChoisis[0]=resistances[choix-1][0];

        attaqueChoisie[1].idAttaque=attaques[choix-1][1].idAttaque;
        attaqueChoisie[1].degat=attaques[choix-1][1].degat;
        attaqueChoisie[1].idAttaqueSpeciale1=attaques[choix-1][1].idAttaqueSpeciale1;
        attaqueChoisie[1].idAttaqueSpeciale2=attaques[choix-1][1].idAttaqueSpeciale2;
        strcpy( attaqueChoisie[1].nomAttaque,attaques[choix-1][1].nomAttaque);
        attaqueChoisie[1].nombreAttaques=attaques[choix-1][1].nombreAttaques;
        attaqueChoisie[1].porteeAttaque=attaques[choix-1][1].porteeAttaque;
        attaqueChoisie[1].typeAttaque=attaques[choix-1][1].typeAttaque;
        resistanceAuxAttaquesChoisis[1]=resistances[choix-1][1];
        return 1;
    }
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
 /*int Max (int a , int b)
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
 void attaque(int indiceDansUnitesJoueurs, int indiceEnnemiDansUnitesJoueurs, int indiceJoueur, int indiceJoueurEnnemi,
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
       int dJ=0,dE=0;
       int i=1;
       if(joueurs[indiceJoueur].idJoueur==1)
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
                printf("unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer= %d\n",(unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer));
               if( (unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer)>0)
               {   printf("unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer= %d\n",(unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer));
                   unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer;
                   printf("degatJouer=%d\n",degatJouer);
                    printf("unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer=%d\n",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv-=degatJouer);
               printf(" lunite %d de joueur %s inflige au l'unite %d de Joueur %s un degat de %d (sante restante E %d)\n",unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,
                    unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,degatJouer,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv);
               }
               else
               {
                unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv=0;

                printf(" lunite %d de joueur %s inflige au l'unite %d de Joueur %s un degat de %d (sante restante E %d)\n",unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,
                    unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,degatJouer,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].pv);
                   // miseAJourJoueurApresMortUnite(indiceEnnemiDansUnitesJoueurs, indiceJoueur,unitesJoueurs,joueurs);
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
                if((unitesJoueurs[indiceDansUnitesJoueurs].pv-=degatEnnemi)>0)
                {
                 unitesJoueurs[indiceDansUnitesJoueurs].pv-=degatEnnemi;
                printf(" lunite %d de joueur %s inflige au l'unite %d de Joueur %s un degat de %d (sante restante J %d)\n",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,
                    unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,degatEnnemi,unitesJoueurs[indiceDansUnitesJoueurs].pv);
                }
                else{
                    unitesJoueurs[indiceDansUnitesJoueurs].pv=0;
                   printf(" lunite %d de joueur %s inflige au l'unite %d de Joueur %s un degat de %d (sante restante J %d)\n",unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idUnite,nomE,
                    unitesJoueurs[indiceDansUnitesJoueurs].idUnite,nomJ,degatEnnemi,unitesJoueurs[indiceDansUnitesJoueurs].pv);
                     //miseAJourJoueurApresMortUnite(indiceEnnemiDansUnitesJoueurs, indiceJoueur,unitesJoueurs,joueurs);
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
           miseAJourJoueurApresMortUnite(indiceEnnemiDansUnitesJoueurs,indiceJoueurEnnemi,unitesJoueurs,joueurs);
           carte[ligneEnnemi][colonneEnnemi].idUnite=0;
           carte[ligneEnnemi][colonneEnnemi].symboleJoueur=' ';


       }



 }*/
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
 void attaque(int indiceDansUnitesJoueurs, int indiceEnnemiDansUnitesJoueurs, int indiceJoueur, int indiceJoueurEnnemi,
 int ligneJoueur, int colonneJoueur, int ligneEnnemi, int colonneEnnemi,CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],Joueur joueurs[MAX_LIGNES_JOUEURS],int defenseJoueur, int defenseEnnemi,
  Attaque attaqueChoisie[2],int resistanceAuxAttaquesChoisis[2],Periode periode, int alignementJoueur, int alignementEnnemi)
 {

       int degatJouer ;
       int degatEnnemi;
       int max ,nbattaqesEnnemi,nbattaqesJoueur;
       nbattaqesEnnemi=attaqueChoisie[1].nombreAttaques;
       nbattaqesJoueur=attaqueChoisie[0].nombreAttaques;
       max=Max(nbattaqesEnnemi,nbattaqesJoueur);
       degatJouer=(attaqueChoisie[0].degat*(100-resistanceAuxAttaquesChoisis[0])*(100+periode.bonus[alignementJoueur]))/10000;
       degatEnnemi=(attaqueChoisie[1].degat*(100-resistanceAuxAttaquesChoisis[1])*(100+periode.bonus[alignementEnnemi]))/10000;
       char nomJ[10],nomE[10];

       int dJ,dE;
       int i=1;
       if(joueurs[indiceJoueur].idJoueur==1)
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

 int jouerTourHumain(int idJoueur, int groupeAlliesJoueur, int nbFichesTypesUnites, int nbJoueurs,
 int nbUnitesMagasin , int nbVillages,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Joueur joueurs[MAX_LIGNES_JOUEURS],CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],Village villages[MAX_LIGNES_VILLAGES],TypeTerrain typesTerrains[NB_TYPES_TERRAINS],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],Periode periode
 )
 {
      int FinTour=0,FinTourforcee=0,IndiceUniteJoueur,ligne_courante,colonne_courante;
      int deplacer,ligneAdeplacer,ColonneAdeplacer,x,indiceJoueurEnnemi;
      int IndiceChefUnitesJoueurs,choix,CodeAttaque,UniteAttaquable,nbAttaques;
      int ligne_ennemie,colonne_ennemie,indiceficheTypeUniteEnnemi,Gagnant,indiceEnnemiDansUnitesJoueurs,indiceficheTypeUniteJoueur=0;
      int defenseEnnemi,defenseJoueur;
      Attaque attaques[10][2];
       int resistances[10][2];
       Attaque attaqueChoisie[2];
        int resistanceAuxAttaquesChoisis[2];
       int indiceJoueur=0;
      int attaquer;



  while((FinTour==0)&&( FinTourforcee==0)&&(Gagnant==0)){


      indiceJoueur=chercherIndiceJoueurHumain(nbJoueurs,joueurs);
      afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
     do
    {
          printf("Donner le numero d'unite a jouer ou 0 si vous voulez forcer la fin du tour ou -1 si vous vouler acheter de nouvelles unites:");
          scanf("%d",&x);
    }while((chercherIndiceUniteJoueur(x,1,1,unitesJoueurs)==-1)&&x<-1);

    if(x==-1)
    {
        IndiceChefUnitesJoueurs=chercherIndiceChefUnitesJoueurs(idJoueur,unitesJoueurs);


        if(carte[unitesJoueurs[IndiceChefUnitesJoueurs].ligne][unitesJoueurs[IndiceChefUnitesJoueurs].colonne].idTypeTerrain==ID_TYPE_TERRAIN_DONJON )
        {


                acheterUnites( idJoueur,unitesJoueurs[IndiceChefUnitesJoueurs].ligne,unitesJoueurs[IndiceChefUnitesJoueurs].colonne,nbUnitesMagasin,  nbJoueurs, nbFichesTypesUnites,  nbVillages,
                              unitesMagasin,unitesJoueurs,joueurs,fichesTypesUnites,villages,carte);


        }
        else
        {
            printf("le ChefUnitesJoueurs n est pas dans son Donjon");
        }

    }
    else if(x==0)
    {
        printf(" FinTourforcee Tour Humain\n");
        FinTourforcee=1;

    }
    else
    {
          IndiceUniteJoueur=chercherIndiceUniteJoueur(x,1,1,unitesJoueurs);
          ligne_courante=unitesJoueurs[IndiceUniteJoueur].ligne;
           colonne_courante=unitesJoueurs[ IndiceUniteJoueur].colonne;
             marquerDeplacementsPossibles(carte,ligne_courante,colonne_courante,unitesJoueurs[ IndiceUniteJoueur].mouvements);

            afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);

           do{
             printf("\n     voulez vous deplacer l'unite? (1:oui/0:non):");
             scanf("%d",&deplacer);
            }while((deplacer!=1) && (deplacer!=0));
            if(deplacer==1)
            {
              if(unitesJoueurs[ IndiceUniteJoueur].mouvements>0)
              {
              do{
                 printf("\n     donner la ligne de la case a jouer (depuis les cases possibles marquees de +):");
                 scanf("%d",&ligneAdeplacer);
                 printf("\n     donner la colonne de la case a jouer (depuis les cases possibles marquees de +):");
                 scanf("%d",&ColonneAdeplacer);
                }while((carte[ligneAdeplacer-1][ColonneAdeplacer-1].marque==0)||(carte[ligneAdeplacer-1][ColonneAdeplacer-1].idUnite!=0));

               deplacerUnite(1,IndiceUniteJoueur,x,ligne_courante,colonne_courante, ligneAdeplacer-1, ColonneAdeplacer-1,nbJoueurs,nbVillages,carte,joueurs,unitesJoueurs, villages);
               effacerDeplacementsPossibles(carte);
               afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
                CodeAttaque=calculerCodeAttaque(groupeAlliesJoueur,ligneAdeplacer-1, ColonneAdeplacer-1 ,nbJoueurs,joueurs,carte);

                if(CodeAttaque!=0)
                {
                do{
                    printf("\n     Cette unite a la possibilite d'attaquer!\n\n  Voulez vous attaquer une unite ennemi(1:oui/0:non)");
                    scanf("%d",&attaquer);
                    }while(attaquer!=0 && attaquer!=1);
                if(attaquer==1)
                    {

                printf("\n     donner la ligne de l'ennemie a attaquer:");
                scanf("%d",&ligne_ennemie);
                printf("\n     donner la colonne de l'ennemie a attaquer:");
                scanf("%d",&colonne_ennemie);
                printf("\n");
                UniteAttaquable=verifierUniteAttaquable(CodeAttaque,ligneAdeplacer-1, ColonneAdeplacer-1,ligne_ennemie-1,colonne_ennemie-1);


                if(UniteAttaquable==1)
                {
                    indiceEnnemiDansUnitesJoueurs=chercherIndiceUniteJoueur(carte[ligne_ennemie-1][colonne_ennemie-1].idUnite,2,1,unitesJoueurs);
                    indiceficheTypeUniteEnnemi=chercherIndiceFicheTypeUnite(unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
                     indiceJoueurEnnemi=chercherIndiceJoueur(2,nbJoueurs,joueurs);
                     indiceficheTypeUniteJoueur=chercherIndiceFicheTypeUnite(unitesJoueurs[IndiceUniteJoueur].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);

                     defenseEnnemi=fichesTypesUnites[indiceficheTypeUniteEnnemi].relationsTerrains[carte[ligne_ennemie-1][colonne_ennemie-1].idTypeTerrain].defense;
                     defenseJoueur=fichesTypesUnites[indiceficheTypeUniteJoueur].relationsTerrains[carte[ligneAdeplacer-1][ColonneAdeplacer-1].idTypeTerrain].defense ;

                     nbAttaques=determinerPossibilitesAttaque(idJoueur,carte[ligne_ennemie-1][colonne_ennemie-1].idUnite, indiceficheTypeUniteJoueur,
                            indiceficheTypeUniteEnnemi,fichesTypesUnites, IndiceUniteJoueur, indiceEnnemiDansUnitesJoueurs,attaques,resistances);


                    choisirAttaque(idJoueur, carte[ligne_ennemie-1][colonne_ennemie-1].idUnite ,indiceficheTypeUniteJoueur,  indiceficheTypeUniteEnnemi,
                            fichesTypesUnites,IndiceUniteJoueur,  indiceEnnemiDansUnitesJoueurs, defenseJoueur,defenseEnnemi, nbAttaques,  attaques,resistances, attaqueChoisie,
                              resistanceAuxAttaquesChoisis,  periode, unitesJoueurs[IndiceUniteJoueur].alignement,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement );
                    attaque(IndiceUniteJoueur, indiceEnnemiDansUnitesJoueurs,indiceJoueur,indiceJoueurEnnemi,
                        ligneAdeplacer-1 , ColonneAdeplacer-1, ligne_ennemie-1, colonne_ennemie-1,carte,unitesJoueurs,
                         joueurs, defenseJoueur, defenseEnnemi, attaqueChoisie,resistanceAuxAttaquesChoisis,periode,
                         unitesJoueurs[IndiceUniteJoueur].alignement,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement );


                }
                    }
         }
            }
            else
            {
                printf(" tu ne peut pas deplacer (mouvement= 0 )\n");
            }
        }
         else{
                 effacerDeplacementsPossibles(carte);
                }

    }
      FinTour=verifierFinTours(idJoueur,unitesJoueurs);
//printf("FinTour=%d\n",FinTour);
 Gagnant=verifierGagnant( nbJoueurs,joueurs,unitesJoueurs);
    /*if(Gagnant==1)
    {

        printf("*** joueur Humain a gagner ! ***\n");
    }
    else if (Gagnant==2){
        printf("*** joueur Ennemie a gagner ! ***\n");
    }*/
 }
//printf("FinTour=%d\n",FinTour);
 return Gagnant;
 }

 //tourEnnemi

int choisirAttaqueAutomatiquement(int numJoueur, int numEnnemi,int indiceficheTypeUniteJoueur, int indiceficheTypeUniteEnnemi,
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],int indiceDansUnitesJoueurs, int indiceEnnemiDansUnitesJoueurs,
 int defenseJoueur, int defenseEnnemi, int nbrAttaques, Attaque attaques[10][2],int resistances[10][2], Attaque attaqueChoisie[2],
 int resistanceAuxAttaquesChoisis[2], Periode periode, int alignementJoueur,int alignementEnnemi)
 {
    int i=0,choix;
    int degatE,degatJ;
    printf(" Priode : %s\n",periode.moment);
    for (i=0;i<100;i++)
    {
        printf("-");
    }
    printf("\n");
    printf("|Vos                    Bonus                   |Ennemi                       Bonus                |\n");
    printf("|Defanse :  %2d         d'alignement:%2d           |Defanse :  %2d               d'alignement:%2d      |\n",
    defenseEnnemi,
    periode.bonus[alignementEnnemi],defenseJoueur,periode.bonus[alignementJoueur]);
    for (i=0;i<100;i++)
    {
        printf("-");
    }
    printf("\n");
    for(i=0;i<nbrAttaques;i++)
    {

          degatJ=attaques[i][1].degat+((attaques[i][1].degat/100)*periode.bonus[alignementEnnemi]);
           degatE=attaques[i][0].degat+((attaques[i][0].degat/100)*periode.bonus[alignementJoueur]);

        printf("|%d- %10s     %d  X %d  (resistance %2d %% )  <-- %d --> %10s     %d X %d  (resistance %2d %% )|\n",
                i+1,attaques[i][1].nomAttaque,attaques[i][1].degat,attaques[i][1].nombreAttaques,resistances[i][1],
               i,attaques[i][0].nomAttaque,attaques[i][0].degat,attaques[i][0].nombreAttaques,resistances[i][0]);
    }
    for (i=0;i<100;i++)
    {
        printf("-");
    }
    printf("\n");

    choix=rand()%nbrAttaques+1;

    {
       printf("l'attaque choisit est %d\n",choix);
        attaqueChoisie[0].idAttaque=attaques[choix-1][0].idAttaque;
        attaqueChoisie[0].degat=attaques[choix-1][0].degat;
        attaqueChoisie[0].idAttaqueSpeciale1=attaques[choix-1][0].idAttaqueSpeciale1;
        attaqueChoisie[0].idAttaqueSpeciale2=attaques[choix-1][0].idAttaqueSpeciale2;
        strcpy( attaqueChoisie[0].nomAttaque,attaques[choix-1][0].nomAttaque);
        attaqueChoisie[0].nombreAttaques=attaques[choix-1][0].nombreAttaques;
        attaqueChoisie[0].porteeAttaque=attaques[choix-1][0].porteeAttaque;
        attaqueChoisie[0].typeAttaque=attaques[choix-1][0].typeAttaque;
        resistanceAuxAttaquesChoisis[0]=resistances[choix-1][0];

        attaqueChoisie[1].idAttaque=attaques[choix-1][1].idAttaque;
        attaqueChoisie[1].degat=attaques[choix-1][1].degat;
        attaqueChoisie[1].idAttaqueSpeciale1=attaques[choix-1][1].idAttaqueSpeciale1;
        attaqueChoisie[1].idAttaqueSpeciale2=attaques[choix-1][1].idAttaqueSpeciale2;
        strcpy( attaqueChoisie[1].nomAttaque,attaques[choix-1][1].nomAttaque);
        attaqueChoisie[1].nombreAttaques=attaques[choix-1][1].nombreAttaques;
        attaqueChoisie[1].porteeAttaque=attaques[choix-1][1].porteeAttaque;
        attaqueChoisie[1].typeAttaque=attaques[choix-1][1].typeAttaque;
        resistanceAuxAttaquesChoisis[1]=resistances[choix-1][1];
        return 1;
    }

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
          printf("ligne de nouvelle unite= %d\n",ligne+1);
          printf("colonne de nouvelle unite= %d\n",colonne+1);
            numUnite=UnitesMagasinUnJoueur[rand()%j];
            printf("unite choisite a acheter = %d\n",numUnite);
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
                        printf("achat effectuer avec succer\n");

                }
                else {
                    printf("\n Or est insuffsant !\n");
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
                    //break;
                  }
                else  if (carte[i][j+1].marque==1&&(carte[i][j+1].idUnite==0))
                  {
                    //printf("dans else1\n");
                    nouvelle_ligne=i;
                    nouvelle_colonne=j+1;
                     //break;
                  }
                  else if(carte[i-1][j].marque==1&&(carte[i-1][j].idUnite==0))
                  {
                    //printf("dans else\n");
                    nouvelle_ligne=i-1;
                    nouvelle_colonne=j;
                    // break;
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

 int jouerTourEnnemi(int numJoueur, int groupeAlliesJoueur, int nbFichesTypesUnites, int nbJoueurs,
 int nbUnitesMagasin, int nbVillages,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Joueur joueurs[MAX_LIGNES_JOUEURS],CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],Village villages[MAX_LIGNES_VILLAGES],
 TypeTerrain typesTerrains[NB_TYPES_TERRAINS],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],Periode periode)
 {

      int FinTour=0,FinTourforcee=0,IndiceUniteJoueur,ligne_courante,colonne_courante;
      int deplacer,ligneAdeplacer,ColonneAdeplacer,x,indiceJoueurEnnemi,idUniteEnnemie;
      int IndiceChefUnitesJoueurs,choix,CodeAttaque,UniteAttaquable,nbAttaques;
      int ligne_ennemie,colonne_ennemie,indiceficheTypeUniteEnnemi,Gagnant,indiceEnnemiDansUnitesJoueurs,indiceficheTypeUniteJoueur=0;
      int defenseEnnemi,defenseJoueur,indiceUniteEnnemie;

      Attaque attaques[10][2];
      int resistances[10][2];
      Attaque attaqueChoisie[2];
      int resistanceAuxAttaquesChoisis[2];
      int attaquer,i=0;
      int choixs[15];
      int taillechoixs=0;
      int index=0;
       int j=2;
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
     printf("FinTour=%d,FinTourforcee=%d,Gagnant=%d\n",FinTour,FinTourforcee,Gagnant);
      while((FinTour==0)&&( FinTourforcee=0)&&(Gagnant==0))
        {
         // afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          //afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          //afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
          printf("apres carte jeux\n");
        do{
            index=rand()%taillechoixs;
            x=choixs[index];
          //x=-1;
         }while((chercherIndiceUniteJoueur(x,2,1,unitesJoueurs)==-1));

     printf("Num joueur a jouer =%d\n",x);
     while(joueurs[1].or > 20)
    {
        printf("L'ennemi veut acheter des nouvelles unites\n");
        IndiceChefUnitesJoueurs=chercherIndiceChefUnitesJoueurs(numJoueur,unitesJoueurs);

        if(carte[unitesJoueurs[IndiceChefUnitesJoueurs].ligne][unitesJoueurs[IndiceChefUnitesJoueurs].colonne].idTypeTerrain==ID_TYPE_TERRAIN_DONJON )
        {
             acheterUnitesAuto(numJoueur,unitesJoueurs[IndiceChefUnitesJoueurs].ligne,unitesJoueurs[IndiceChefUnitesJoueurs].colonne ,nbUnitesMagasin,
                     nbJoueurs,nbFichesTypesUnites,unitesMagasin,unitesJoueurs,joueurs,fichesTypesUnites,carte);
            afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);

        }
        else
        {
            printf("Lennemi n'a pas reussit a acheter des des nouvelles unites(le ChefUnitesJoueurs n est pas dans son Donjon)\n");
        }

    }

   if(chercherIndiceUniteJoueur(x,2,1,unitesJoueurs)!=-1)
    {

          IndiceUniteJoueur=chercherIndiceUniteJoueur(x,2,1,unitesJoueurs);
          ligne_courante=unitesJoueurs[IndiceUniteJoueur].ligne;
          colonne_courante=unitesJoueurs[ IndiceUniteJoueur].colonne;

          marquerDeplacementsPossibles(carte,ligne_courante,colonne_courante,unitesJoueurs[IndiceUniteJoueur].mouvements);
          afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);
           // printf("deplacer unite %d de l'ennemi\n",x);
            deplacementAutomatiqueUnite(IndiceUniteJoueur,ligne_courante,colonne_courante,groupeAlliesJoueur,nbJoueurs,nbVillages,carte,joueurs,unitesJoueurs,villages);
            effacerDeplacementsPossibles(carte);
           afficherCarteJeu(carte,nbJoueurs,joueurs,nbVillages,villages,unitesJoueurs);

            CodeAttaque=calculerCodeAttaque(groupeAlliesJoueur,unitesJoueurs[IndiceUniteJoueur].ligne,unitesJoueurs[ IndiceUniteJoueur].colonne,nbJoueurs,joueurs,carte);
            if(CodeAttaque!=0)
            {
                printf("Cette unite a la possibilite d'attaquer\n");

                idUniteEnnemie=choixAutomatiqueUniteEnnemie( CodeAttaque,unitesJoueurs[IndiceUniteJoueur].ligne,unitesJoueurs[ IndiceUniteJoueur].colonne ,nbJoueurs,carte,joueurs,unitesJoueurs);
                printf("idUniteEnnemie a Attaquer =%d\n",idUniteEnnemie);
                indiceUniteEnnemie=chercherIndiceUniteJoueur(idUniteEnnemie,1,1,unitesJoueurs);
                ligne_ennemie=unitesJoueurs[indiceUniteEnnemie].ligne;
                colonne_ennemie=unitesJoueurs[ indiceUniteEnnemie].colonne;

                UniteAttaquable=verifierUniteAttaquable(CodeAttaque,unitesJoueurs[IndiceUniteJoueur].ligne,unitesJoueurs[IndiceUniteJoueur].colonne,ligne_ennemie,colonne_ennemie);

                if(UniteAttaquable==1)
                {
                    printf("ligne de ennemi a attaquer : %d\n",ligne_ennemie);
                    printf("colonne de ennemi a attaquer : %d\n",colonne_ennemie);
                    indiceEnnemiDansUnitesJoueurs=chercherIndiceUniteJoueur(carte[ligne_ennemie][colonne_ennemie].idUnite,1,1,unitesJoueurs);
                    indiceficheTypeUniteEnnemi=chercherIndiceFicheTypeUnite(unitesJoueurs[indiceEnnemiDansUnitesJoueurs].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);
                     indiceJoueurEnnemi=chercherIndiceJoueur(1,nbJoueurs,joueurs);
                     indiceficheTypeUniteJoueur=chercherIndiceFicheTypeUnite(unitesJoueurs[IndiceUniteJoueur].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites);

                     defenseEnnemi=fichesTypesUnites[indiceficheTypeUniteEnnemi].relationsTerrains[carte[ligne_ennemie][colonne_ennemie].idTypeTerrain].defense;
                     defenseJoueur=fichesTypesUnites[indiceficheTypeUniteJoueur].relationsTerrains[carte[unitesJoueurs[IndiceUniteJoueur].ligne][unitesJoueurs[ IndiceUniteJoueur].colonne].idTypeTerrain].defense ;

                     nbAttaques=determinerPossibilitesAttaque(numJoueur,carte[unitesJoueurs[IndiceUniteJoueur].ligne][unitesJoueurs[ IndiceUniteJoueur].colonne].idUnite, indiceficheTypeUniteJoueur,
                            indiceficheTypeUniteEnnemi,fichesTypesUnites, IndiceUniteJoueur, indiceEnnemiDansUnitesJoueurs,attaques,resistances);

                      int indiceDansUnitesJoueurs=(chercherIndiceUniteJoueur(x,numJoueur,1,unitesJoueurs));
                     choisirAttaqueAutomatiquement(numJoueur,idUniteEnnemie, indiceficheTypeUniteJoueur,indiceficheTypeUniteEnnemi,
                           fichesTypesUnites, indiceDansUnitesJoueurs,indiceEnnemiDansUnitesJoueurs,
                         defenseJoueur, defenseEnnemi, nbAttaques,attaques,resistances,attaqueChoisie,resistanceAuxAttaquesChoisis,periode,
                           unitesJoueurs[IndiceUniteJoueur].alignement, unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement );

                    attaque(IndiceUniteJoueur, indiceEnnemiDansUnitesJoueurs,numJoueur,indiceJoueurEnnemi,
                       unitesJoueurs[IndiceUniteJoueur].ligne , unitesJoueurs[IndiceUniteJoueur].colonne,ligne_ennemie,colonne_ennemie,carte,unitesJoueurs,
                         joueurs, defenseJoueur, defenseEnnemi, attaqueChoisie,resistanceAuxAttaquesChoisis,periode,
                         unitesJoueurs[IndiceUniteJoueur].alignement,unitesJoueurs[indiceEnnemiDansUnitesJoueurs].alignement );

                }
            }
        else{
             effacerDeplacementsPossibles(carte);
       }
    }
        FinTour=verifierFinTours(numJoueur,unitesJoueurs);
      Gagnant=verifierGagnant( nbJoueurs,joueurs,unitesJoueurs);

}

   return Gagnant;
 }
void deroulerJeu(int nbFichesTypesUnite, int nbJoueurs,int nbUnitesMagasin,int nbVillages,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],
 Joueur joueurs[MAX_LIGNES_JOUEURS], CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],
 Village villages[MAX_LIGNES_VILLAGES],TypeTerrain typesTerrains[NB_TYPES_TERRAINS],Periode periodes[NB_LIGNES_PERIODES],
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
 {
     int numtour=1,i,resultat;
    Periode periodeCorante;
    do{
        printf("****** Tour: %d ******\n",numtour);
     for (i=0;i<nbJoueurs;i++)
     {

              periodeCorante=periodes[numtour%6];

             if(joueurs[i].type==1)
              {
                 if(i!=0)
                 {
                      reinitialiserTours(joueurs[i].idJoueur, nbJoueurs,joueurs,unitesJoueurs);
                 }

                  printf("*** Tour Humain *****\n ");
                resultat=jouerTourHumain(joueurs[i].idJoueur,joueurs[i].groupe_allies,nbFichesTypesUnite,nbJoueurs,nbUnitesMagasin,nbVillages,
                        fichesTypesUnites,joueurs,carte,unitesMagasin,villages,typesTerrains,unitesJoueurs,periodeCorante);



              }
              else
              {
                if(chercherGroupeAllies(joueurs[i].symbole,nbJoueurs,joueurs)==1)
                {
                    printf("***** Tour GroupeAllies*****\n");
                    numtour++;
                }
                else
                {
                   if(joueurs[i].type==2)
                   {
                       if(i!=0)
                       {
                            reinitialiserTours(joueurs[i].idJoueur, nbJoueurs,joueurs,unitesJoueurs);
                       }

                        printf("***** Tour Enemie *****\n");
                        resultat=jouerTourEnnemi(joueurs[i].idJoueur,joueurs[i].groupe_allies,nbFichesTypesUnite,nbJoueurs,nbUnitesMagasin,
                            nbVillages,fichesTypesUnites,joueurs,carte,unitesMagasin,villages,typesTerrains,unitesJoueurs,periodeCorante);

                       }

                }


             }
        //printf("resultat=%d",resultat);
    if(resultat==1)
    {

        printf("*** joueur Humain a gagner ! ***\n\n");
        break;
    }
    else if(resultat==2){
        printf("*** joueur Ennemie a gagner ! ***\n\n");
        break;
    }


    }
    numtour++;
    }while(resultat==0);


 }
 int main()
 {
 CelluleCarte carte[NB_LIGNES_CARTE][ NB_COLONNES_CARTE];
 UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN];
 Village villages[MAX_LIGNES_VILLAGES];
 TypeTerrain typesTerrains[NB_TYPES_TERRAINS];
 Periode periodes[NB_LIGNES_PERIODES];
 Joueur joueurs[MAX_LIGNES_JOUEURS];
 UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS];
 FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES];

 int choix;
 int nbfichesTypesUnites=0,nbJoueurs=0,nbUnitesMagasin=0,nbVillages=0;

 do{
    printf("\n****** Battle for wesnoth ******\n ");
    printf("********************************\n");
    printf(" 1:NouveauJeu\n 2:chargerJeuComplet\n 0:QUITER\n");
    printf("********************************\n");
    printf("Donner votre choix ");
    scanf("%d",&choix);
 switch (choix)
 {
     case 1:
             initialiserNouveauJeu(&nbfichesTypesUnites,&nbJoueurs,&nbUnitesMagasin,&nbVillages
                                   ,fichesTypesUnites,joueurs,carte,unitesMagasin,villages,typesTerrains,periodes,unitesJoueurs);

            afficherTableauxJeu(nbfichesTypesUnites,nbJoueurs,nbUnitesMagasin,nbVillages,fichesTypesUnites
                                ,joueurs,carte,unitesMagasin,villages,typesTerrains,periodes,unitesJoueurs);
             sauvegarderJeuComplet(nbVillages,nbJoueurs,villages,joueurs,unitesJoueurs);
             break;
    case 2:chargerJeuComplet(&nbfichesTypesUnites,&nbJoueurs,&nbUnitesMagasin,&nbVillages,fichesTypesUnites,joueurs,carte,unitesMagasin,villages,typesTerrains,periodes,unitesJoueurs);

            afficherUnitesMagasin(nbUnitesMagasin,unitesMagasin);
            afficherTableauxJeu(nbfichesTypesUnites,nbJoueurs,nbUnitesMagasin,nbVillages,fichesTypesUnites,joueurs,carte,unitesMagasin,villages,typesTerrains,periodes,unitesJoueurs);
             break;
 }
 }while(choix<=0||choix>2);


    deroulerJeu( nbfichesTypesUnites, nbJoueurs, nbUnitesMagasin, nbVillages, fichesTypesUnites,joueurs,  carte, unitesMagasin,villages, typesTerrains, periodes,unitesJoueurs);


return 0;
}


