#include "GTO.h"

void sphere(Gorilla *X) {
    X->fitness = 0.0;
    int i;

    for(i = 0; i < DIM; i++)
        X->fitness += pow(X->coordinates[i], 2);
}

void Styblinski_Tang(Gorilla *X) {
    X->fitness = 0.0;
    int i;

    for(i = 0; i < DIM; i++)
        X->fitness += pow(X->coordinates[i], 4) - 16 * pow(X->coordinates[i], 2) + 5 * X->coordinates[i];
        
    X->fitness /= 2;
}

void Rastrigin(Gorilla *X) {
    X->fitness = 10 * DIM;
    int i;

    for(i = 0; i < DIM; i++)
        X->fitness += pow(X->coordinates[i], 2) - 10 * cos(2 * M_PI * X->coordinates[i]);
}

void Schwefel(Gorilla *X) {
    int i;
    X->fitness = 418.9829 * DIM;
    for (i = 0; i < DIM; i++) {
        X ->fitness += (X->coordinates[i] * sin(sqrt(abs(X->coordinates[i]))));
    }
}

void Rana(Gorilla *X){
    int i;
    for (i = 0; i < DIM -1; i++) {
        X ->fitness += (X->coordinates[i] * cos(sqrt(abs(X->coordinates[i+1] + X->coordinates[i] + 1)))
                         * sin(sqrt(abs(X->coordinates[i+1] - X->coordinates[i] + 1))) 
                         + (1 + X->coordinates[i+1]) * cos(sqrt(abs(X->coordinates[i+1] - X->coordinates[i] + 1))) 
                         * sin(sqrt(abs(X->coordinates[i+1] +X->coordinates[i] + 1))));
    }
}