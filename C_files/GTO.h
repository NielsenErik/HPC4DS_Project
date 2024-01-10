#ifndef GTO_H
#define GTO_H

#define _GNU_SOURCE

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sched.h>
#ifdef _OPENMP
# include <omp.h>
#endif

#define beta 3
#define N 64
#define p 0.03
#define SELECTED_FUNCTION SPHERE
#define T 1000
#define W 0.8

// Define numeric values for each function
#define SPHERE 1
#define STYBLINSKI_TANG 2
#define RASTRIGIN 3
#define SCHWEFEL 4
#define RANA 5
// ---------------------------------------

// Define FUNCTION based on the function selection
#if SELECTED_FUNCTION == SPHERE
    #define FUNCTION sphere
#elif SELECTED_FUNCTION == STYBLINSKI_TANG
    #define FUNCTION Styblinski_Tang
#elif SELECTED_FUNCTION == SCHWEFEL
    #define FUNCTION Schwefel
#elif SELECTED_FUNCTION == RANA
    #define FUNCTION Rana
#else
    #define FUNCTION Rastrigin
#endif
// ------------------------------------------

typedef struct {
    double coordinates[DIM];
    long double fitness;
} Gorilla;

// optimization_functions.c ------
void sphere(Gorilla *X);
void Styblinski_Tang(Gorilla *X);
void Rastrigin(Gorilla *X);
void Rana(Gorilla *X);
void Schwefel(Gorilla *X);
// -------------------------------

// common_functions.c ---------------------------------------------------------------------------------------------------------------------------------------------
void boundaryCheck(double max, double min, double *value);
void checkForUpdatePosition(Gorilla *GX, Gorilla *silverback, Gorilla *X);
void exploitation(double C, double L, double lb, double ub, double M[], int gorilla_per_process, Gorilla GX[], Gorilla *silverback, Gorilla X[], int n_threads);
void exploration(double C, double L, double lb, double ub, double M[], int gorilla_per_process, Gorilla GX[], Gorilla *silverback, Gorilla old_GX[], Gorilla X[], int n_threads);
void initialization(double *lb, double *ub, int gorilla_per_process, Gorilla GX[], Gorilla *silverback, Gorilla X[], int n_threads);
void printSearchAgentsData(int gorilla_per_process, int rank, long double silverback_fitness, Gorilla X[]);
double rand01();
double randn();
double unifrnd(double a);
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif