/*******************************************************
Nom ......... : main.c
Role ........ : Programme principal executant la lecture
                d'une image bitmap
Auteur ...... : Frédéric CHATRIE
Version ..... : V1.1 du 1/2/2021
Licence ..... : /

Compilation :
make veryclean
make
Pour exécuter, tapez : ./all
********************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Bmp2Matrix.h"
#include "index.h"

int main(int argc, char* argv[]){
    BMP bitmap;
    FILE* pFichier=NULL;

    pFichier=fopen("test_images/6.bmp", "rb");     //Ouverture du fichier contenant l'image
    if (pFichier==NULL) {
        printf("%s\n", "0_1.bmp");
        printf("Erreur dans la lecture du fichier\n");
    }
    LireBitmap(pFichier, &bitmap);
    fclose(pFichier);               //Fermeture du fichier contenant l'image

    ConvertRGB2Gray(&bitmap);
    //printf("%d\n", bitmap.mPixelsGray[10][10]);
    //printf("%d\n", bitmap.infoHeader.hauteur);

    /*********************Flaten bitmap***************************/
    double* image = (double*)malloc(sizeof(double) * 784);
    for(int i=0; i < 28 ; i++){
        for (int j = 0; j < 28; j++){
            image[j + i * 28] = (double)bitmap.mPixelsGray[i][j]/255.0;
        }
    }

    //*********************layers0_weights***********************//
    int lines = 784;
    int columns = 128;
    double** layers0_weights = (double**)malloc(lines * sizeof(double*));
    *layers0_weights = (double *)malloc(sizeof(double) * lines * columns);
    for (int r = 0; r < lines; r++) {
        layers0_weights[r] = (*layers0_weights + columns*r);
    }
    char file_0w[] = "layer0_weights.txt";
    txt2mat(file_0w, lines, columns, layers0_weights);
    
    //***********************layers0_biases**************************/
    int columns_0b = 128;
    double* layers0_bias = (double*)malloc(columns_0b * sizeof(double));
    char file_0b[] = "layer0_bias.txt";
    txt2mat_b(file_0b, columns_0b, layers0_bias);

    //***********************matmul and ReLU*************************//
    double *output1 = malloc(sizeof(double) * columns_0b);
    dot_matrix(image, layers0_weights, layers0_bias, lines, columns, output1);
    double *output = malloc(sizeof(double) * columns_0b);
    ReLU(output1, columns_0b ,output);
    
     //*********************layers1_weights***********************//
    int lines_1 = 128;
    int columns_1 = 10;
    double** layers1_weights = (double**)malloc(lines_1 * sizeof(double*));
    *layers1_weights = (double *)malloc(sizeof(double) * lines_1 * columns_1);
    for (int r = 0; r < lines_1; r++) {
        layers1_weights[r] = (*layers1_weights + columns_1*r);
    }
    char file_1w[] = "layer1_weights.txt";
    txt2mat(file_1w, lines_1, columns_1, layers1_weights);
    
    //***********************layers1_biases*************************//
    int columns_1b = 10;
    double* layers1_bias = (double*)malloc(columns_1b * sizeof(double));
    char file_1b[] = "layer1_bias.txt";
    txt2mat_b(file_1b, columns_1b, layers1_bias);

    //***********************matmul and softmax*************************//
    double *final_output = malloc(sizeof(double) * columns_1b);
    dot_matrix(output, layers1_weights, layers1_bias, lines_1, columns_1, final_output);
    double *final_output_softmax = malloc(sizeof(double) * columns_1b);
    softmax(final_output, columns_1b, final_output_softmax);
    
    for(int j=0; j<10 ; j++)
    {
        printf("%f", final_output_softmax[j]);
        printf(" ");
    }

    int N = argmax(final_output_softmax, 10);
    printf("\nThe predicted value is : %d  Accuracy : %.2f %%.", N, final_output_softmax[N]*100);
    DesallouerBMP(&bitmap);
    return 0;
}


