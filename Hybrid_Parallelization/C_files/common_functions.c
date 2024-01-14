#include "GTO.h"

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sched.h>

/* TODO:
// - Understand why we should elevate M to the power of g and then to the power of 1/g
*/



void boundaryCheck(double max, double min, double *value) {
    if (*value > max)
        *value = max;
    else if (*value < min)
        *value = min;
}

void checkForUpdatePosition(Gorilla *GX, Gorilla *silverback, Gorilla *X) {
    FUNCTION(GX);

    if (GX->fitness < X->fitness) {
        memcpy(X, GX, sizeof(Gorilla));
        
        if (X->fitness < silverback->fitness)
            memcpy(silverback, X, sizeof(Gorilla));
    }
}

void exploitation(double C, double L, double lb, double ub, double M[], int gorilla_per_process, Gorilla GX[], Gorilla *silverback, Gorilla X[], int n_threads) {
    int j, k;
    Gorilla old_silverback;
    memcpy(&old_silverback, silverback, sizeof(Gorilla));
    if (C >= W){
       
        for (j = 0; j < gorilla_per_process; j++) {
            // tmp variables ot avoid data race conditions
            Gorilla tmp_GX = GX[j];
            Gorilla tmp_X = X[j];
            #pragma omp parallel default(none) shared(tmp_GX, tmp_X, j, C, L, old_silverback, ub,lb, M) private(k) num_threads(n_threads) 
            {
                #pragma omp for
                for (k = 0; k < DIM; k++) {
                    tmp_GX.coordinates[k] = L * fabs(M[k] / DIM) * (tmp_X.coordinates[k] - old_silverback.coordinates[k]) + tmp_X.coordinates[k];
                    boundaryCheck(ub, lb, &tmp_GX.coordinates[k]);
                }
            }
            #pragma omp barrier
            #pragma omp master
            {
                GX[j] = tmp_GX;
                checkForUpdatePosition(&GX[j], silverback, &X[j]);
            }
        }
    }
    else{
        for (j = 0; j < gorilla_per_process; j++) {
            double Q = 2 * rand01() - 1;
            // tmp variables ot avoid data race conditions
            Gorilla tmp_GX = GX[j];
            Gorilla tmp_X = X[j];
            if (rand01() >= 0.5)
                #pragma omp parallel default(none) shared(tmp_GX, tmp_X,old_silverback, j, Q, ub,lb) private(k) num_threads(n_threads) 
                {
                    #pragma omp for
                    for (k = 0; k < DIM; k++) {
                        tmp_GX.coordinates[k] = old_silverback.coordinates[k] - (old_silverback.coordinates[k] * Q - tmp_X.coordinates[k] * Q) * beta * randn();
                        boundaryCheck(ub, lb, &tmp_GX.coordinates[k]);
                    }
                }
            else {
                double E = randn();
                #pragma omp parallel default(none) shared(tmp_GX, tmp_X, j, Q, ub,lb, E, old_silverback) private(k) num_threads(n_threads) 
                {
                    #pragma omp for
                    for (k = 0; k < DIM; k++) {
                        tmp_GX.coordinates[k] = old_silverback.coordinates[k] - (old_silverback.coordinates[k] * Q - tmp_X.coordinates[k] * Q) * beta * E;
                        boundaryCheck(ub, lb, &tmp_GX.coordinates[k]);
                    }
                }
            }
            #pragma omp barrier
            #pragma omp master
            {
                GX[j] = tmp_GX;
                checkForUpdatePosition(&GX[j], silverback, &X[j]);
            }
        }
    }
}

void exploration(double C, double L, double lb, double ub, double M[], int gorilla_per_process, Gorilla GX[], Gorilla *silverback, Gorilla old_GX[], Gorilla X[], int n_threads) {
    int j, k, r = rand() % gorilla_per_process;
    memcpy(old_GX, GX, gorilla_per_process * sizeof(Gorilla)); // Create a deep copy of GX into old_GX
    for(j = 0; j < DIM; j++){
        M[j] = 0;
    }

    // tmp variables ot avoid data race conditions
    Gorilla X_r = X[r];
    Gorilla old_GX_r = old_GX[r];

    for (j = 0; j < gorilla_per_process; j++) {
        // tmp variables ot avoid data race conditions
        Gorilla tmp_GX = GX[j];
        Gorilla tmp_X = X[j];
        if (rand01() < p){
            #pragma omp parallel default(none) shared(tmp_GX, tmp_X, j, C, r, L, ub,lb, M) private(k) num_threads(n_threads)
            {
                # pragma omp for
                for(k = 0; k < DIM; k++) {
                    tmp_GX.coordinates[k] = (ub - lb) * rand01() + lb; // CHECKED
                    boundaryCheck(ub, lb, &tmp_GX.coordinates[k]);
                    M[k] += tmp_GX.coordinates[k];
                }
            }
        }
        else if (rand01() >= 0.5){
            #pragma omp parallel default(none) shared(tmp_GX, tmp_X, j, C, X_r, L, ub,lb, M) private(k) num_threads(n_threads)
            {
                # pragma omp for
                for (k = 0; k < DIM; k++) {
                    tmp_GX.coordinates[k] = (rand01() - C) * X_r.coordinates[k] + L * unifrnd(C) * tmp_X.coordinates[k]; // CHECKED
                    boundaryCheck(ub, lb, &tmp_GX.coordinates[k]);
                    M[k] += tmp_GX.coordinates[k];
                }
            }
        }
        else{
            #pragma omp parallel default(none) shared(tmp_GX, tmp_X, j, C, X_r, old_GX_r, L, ub,lb, M) private(k) num_threads(n_threads)
            {
                #pragma omp for
                for (k = 0; k < DIM; k++) {
                    tmp_GX.coordinates[k] = tmp_X.coordinates[k] - L * (L * (tmp_X.coordinates[k] - old_GX_r.coordinates[k]) + rand01() * (tmp_X.coordinates[k] - old_GX_r.coordinates[k])); // CHECKED
                    boundaryCheck(ub, lb, &tmp_GX.coordinates[k]);
                    M[k] += tmp_GX.coordinates[k];
                }
            }
        }
        #pragma omp barrier
        #pragma omp master
        {
            GX[j] = tmp_GX;
            checkForUpdatePosition(&GX[j], silverback, &X[j]);
        }
    }
}


void initialization(double *lb, double *ub, int gorilla_per_process, Gorilla GX[], Gorilla *silverback, Gorilla X[], int n_threads) {
    int i, j;
    silverback->fitness = INFINITY;
    switch(SELECTED_FUNCTION) {
        case 1:
            *lb = -100;
            *ub = 100;
            break;
        case 2:
            *lb = -5;
            *ub = 5;
            break;
        case 3:
            *lb = -5.12;
            *ub = 5.12;
            break;
        default:
            exit(1);
            break;
    }
    for (i = 0; i < gorilla_per_process; i++) {
        Gorilla tmp_GX = GX[i];
        Gorilla tmp_X = X[i];
        #pragma omp parallel default(none) shared(tmp_GX, tmp_X, ub,lb) private(j) num_threads(n_threads) 
        {
            # pragma omp for
            for (j = 0; j < DIM; j++) {
                tmp_X.coordinates[j] = rand01() * (*ub - *lb) + *lb;
                tmp_GX.coordinates[j] = tmp_X.coordinates[j]; // Copy values to GX
            }
        }
        #pragma omp barrier
        #pragma omp master
        {
            X[i] = tmp_X;
            GX[i] = tmp_GX;
            FUNCTION(&X[i]);
        }
        
        if (X[i].fitness < silverback->fitness)
            memcpy(silverback, X, sizeof(Gorilla));
    }
}

// Print search agents' positions
void printSearchAgentsData(int gorilla_per_process, int rank, long double silverback_fitness, Gorilla X[]) {
    int dim = (DIM > 4) ? 4 : DIM, i, j;

    for (i = 0; i < gorilla_per_process; i++) {
        char output[139] = "";
        sprintf(output, "%i. ", rank * gorilla_per_process + i);

        if (X[i].fitness == silverback_fitness)
            strcat(output, "Silverback: ");

        for (j = 0; j < dim; j++) {
            char buffer[21];
            sprintf(buffer, "%.15f ", X[i].coordinates[j]);
            strcat(output, buffer); // Append X values
        }
        
        char fitness_buffer[39];
        sprintf(fitness_buffer, "with fitness %.19Lf\n", X[i].fitness);
        strcat(output, fitness_buffer); // Append fitness value
        printf("%s", output);
    }
}

double rand01() {
    return (double)rand() / RAND_MAX;
}

double randn() {
    double s, u1, u2, v1;

    do {
        u1 = rand01() * 2.0 - 1.0;
        u2 = rand01() * 2.0 - 1.0;
        s = u1 * u1 + u2 * u2;
    } while (s >= 1.0 || s == 0.0);

    s = sqrt(-2.0 * log(s) / s);
    v1 = u1 * s;
    return v1;
}

// Function to generate a random number between -a and a
double unifrnd(double a) {
    return (2.0 * a * rand01()) - a;
}