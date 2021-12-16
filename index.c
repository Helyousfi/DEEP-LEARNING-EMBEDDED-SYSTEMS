#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "index.h"
#include <math.h>

void txt2mat(char* file, int lines, int columns, double** mat){
    FILE* fp = fopen(file, "r");
    size_t len = 1000 * lines;
    char* str = malloc(sizeof(char) * len);
    if(fp == NULL)
    {
        printf("Cannot open file input.txt");
        return;
    }
    int line = 0;
    int column = 0;

    while(fgets(str, len , fp) != NULL)
    {
        str[strcspn(str, "\n")] = 0;
        char delim[] = " ";

        char *ptr = strtok(str, delim);
        column = 0;
        while(ptr != NULL)
        {
            double x = atof(ptr);
            mat[line][column] = x;
            ptr = strtok(NULL, delim);
            column = column + 1;
        }
        line = line + 1;
    }
    free(str);
}


void txt2mat_b(char* file, int columns, double* mat){
    FILE* fp = fopen(file, "r");
    size_t len = 25*128;
    char* str = malloc(sizeof(char) * len);
    if(fp == NULL)
    {
        printf("Cannot open file input.txt");
        return;
    }
    int column = 0;

    while(fgets(str, len , fp) != NULL)
    {
        str[strcspn(str, "\n")] = 0;
        char delim[] = " ";

        char *ptr = strtok(str, delim);
        column = 0;
        while(ptr != NULL)
        {
            double x = atof(ptr);
            mat[column] = x;
            ptr = strtok(NULL, delim);
            column = column + 1;
        }
    }
    free(str);
}


void dot_matrix(double* input, double **weights, double* biases, int lines, int columns, double* output){
    for(int j=0; j < columns;j++)
    {
        output[j] = 0;
        for(int i=0; i < lines; i++)
        {
            output[j] = output[j] +  input[i] * weights[i][j];
        }
        output[j] = output[j] + biases[j];
    }
}


void ReLU(double* input, int size ,double* output){
    for(int i=0; i<size; i++)
    {
        if(input[i]<0){
            output[i] = 0;
        }
        else{
            output[i] = input[i];
        }
    }
}

void softmax(double* input, int size, double* output){
    double somme = 0;
    for(int i=0; i<size; i++){
        somme = somme + exp(input[i]);
    }
    for(int i=0; i<size; i++){
        output[i] = exp(input[i])/somme;
    }
  
}

int argmax(double* tab, int N)
{
    double max = tab[0];
    double argmax = 0;
    for(int i=1; i<N; i++)
    {
        if(tab[i]>max)
        {
            max = tab[i];
            argmax = i;
        }
    }
    return argmax;
}