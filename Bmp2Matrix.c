/*******************************************************
Nom ......... : Bmp2Matrix.c
Role ........ : Permet la conversion et la lecture des
                images bitmap
Auteur ...... : Frédéric CHATRIE
Version ..... : V1.1 du 1/2/2021
Licence ..... : /

Compilation : /
Pour exécuter, tapez : /
********************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "Bmp2Matrix.h"

// Fonction permettant de convertir une image lue en RBG vers un signal en niveau de gris accessible depuis la structure BMP
void ConvertRGB2Gray(BMP* pBitmap){
  int i,j;
  for (i=0;i<pBitmap->infoHeader.hauteur;i++) {
    for (j=0;j<pBitmap->infoHeader.largeur;j++) {
      pBitmap->mPixelsGray[i][j]=(pBitmap->mPixels[i][j].rouge+pBitmap->mPixels[i][j].vert+pBitmap->mPixels[i][j].bleu)/3;
    }
  }
}

// Fonction permettant l'allocation de l'espace mémoire nécessaire dans une structure à la fois pour le signal RGB et en niveau de gris
void AllouerBMP(BMP* pBitmap){
  int i;
  pBitmap->mPixels=(Pixels**) malloc(pBitmap->infoHeader.hauteur * sizeof(Pixels*));  //Allocation memoire des lignes de la matrice
  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    pBitmap->mPixels[i]=(Pixels*) malloc(pBitmap->infoHeader.largeur * sizeof(Pixels));   //Allocation memoire des colonnes de la matrice

  pBitmap->mPixelsGray=(unsigned char**) malloc(pBitmap->infoHeader.hauteur * sizeof(unsigned char*));  //Allocation memoire des lignes de la matrice
  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    pBitmap->mPixelsGray[i]=(unsigned char*) malloc(pBitmap->infoHeader.largeur * sizeof(unsigned char));   //Allocation memoire des colonnes de la matrice

}

// Fonction permettant de desallouer l'espace mémoire nécessaire à la fois pour le signal RGB et en niveau de gris
void DesallouerBMP(BMP* pBitmap){
  int i;

  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    free(pBitmap->mPixels[i]);   //Desallocation memoire des colonnes de la matrice

  free(pBitmap->mPixels);  //Desallocation memoire des lignes de la matrice

  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    free(pBitmap->mPixelsGray[i]);   //Desallocation memoire des colonnes de la matrice

  free(pBitmap->mPixelsGray);  //Desallocation memoire des lignes de la matrice

}

// Fonction permettant la lecture d'une image bitmap. Elle vient remplir la structure de données avec l'en-tête, l'info et la matrice de pixels
void LireBitmap(FILE* pFichier,BMP* pBitmap){
       long curseur;
       curseur=LireHeaderBitmap(pFichier,pBitmap);
       LireInfoHeaderBitmap(pFichier,pBitmap,curseur);
       AllouerBMP(pBitmap);
       LirePixelsBitmap(pFichier,pBitmap);
}

// Fonction permettant de lire l'en-tête du bitmap et qui vient stocker les valeurs dans la structure
long LireHeaderBitmap(FILE* pFichier,BMP* pBitmap){
  long curseur;
  fseek(pFichier, 0, SEEK_SET);       //place le curseur au debut du fichier
  fread(pBitmap->header.signature, 1, 2, pFichier); //lit et place la signature dans la structure
  fread(&(pBitmap->header.tailleFichier), 1, 4, pFichier); //lit et place la taille du fichier dans la structure
  fread(&(pBitmap->header.reserved), 1, 4, pFichier);
  fread(&(pBitmap->header.offset), 1, 4, pFichier);
  curseur=ftell(pFichier);
  return curseur;
}

// Fonction permettant de lire les informations du bitmap et qui vient stocker les valeurs dans la structure
void LireInfoHeaderBitmap(FILE* pFichier,BMP* pBitmap, long curseur){
  fseek(pFichier, curseur, SEEK_SET);       //place le curseur au bon endroit du fichier
  fread(&(pBitmap->infoHeader.tailleHeader), 1, 4, pFichier);    //lit et place la taille de l'en-tete dans la structure
  fread(&(pBitmap->infoHeader.largeur), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.hauteur), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.trames), 1, 2, pFichier);
  fread(&(pBitmap->infoHeader.bits), 1, 2, pFichier);
  fread(&(pBitmap->infoHeader.compression), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.tailleImage), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.xResolution), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.yResolution), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.nbCouleurs), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.couleursImportantes), 1, 4, pFichier);

}

// Fonction permettant de lire et stocker la matrice de pixels dans la structure
void LirePixelsBitmap(FILE* pFichier,BMP* pBitmap){
  int i,j,padding;
  size_t verif;
  long cur,end;

  fseek(pFichier, pBitmap->header.offset, SEEK_SET);       //place le curseur au bon endroit du fichier
  padding=4-((pBitmap->infoHeader.largeur*3)%4);  //Calcul du padding si le nombre d'octets n'est pas multiple de 4 nécessaire au format bitmap
  if (padding==4)
      padding=0;

  //Attention, dans un bitmap le bleu et rouge sont inversés
  //De plus, la lecture des pixels sur l'axe des ordonnées matriciel est différent de celui des coordonnées cartésiennes
  for (i=pBitmap->infoHeader.hauteur-1;i>=0;i--) {
    for (j=0;j<pBitmap->infoHeader.largeur;j++) {
      verif = fread(&(pBitmap->mPixels[i][j].bleu), 1, 1, pFichier);
      if (verif!=1)
        printf("Probleme de lecture pixel bleu\n");
      verif = fread(&(pBitmap->mPixels[i][j].vert), 1, 1, pFichier);
      if (verif!=1)
        printf("Probleme de lecture pixel vert\n");
      verif = fread(&(pBitmap->mPixels[i][j].rouge), 1, 1, pFichier);    //lit et place les pixels rouges dans la structure
      if (verif!=1)
        printf("Probleme de lecture pixel rouge\n");
    }
    fseek(pFichier, padding, SEEK_CUR);
  }

cur=ftell(pFichier);
fseek(pFichier, 0, SEEK_END);
end=ftell(pFichier);

if (cur==end)
  printf("Le fichier est bien termine\n");

}
