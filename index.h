#ifndef INDEX_H
#define INDEX_H

void txt2mat(char*, int, int, double**);
void txt2mat_b(char*, int, double*);
void dot_matrix(double*, double **, double*, int, int, double*);
void ReLU(double*, int,double*);
void softmax(double*, int, double*);
int argmax(double*, int);

#endif