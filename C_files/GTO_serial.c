#include "GTO.h"

int main() {
	MPI_Init(NULL, NULL);
    double t1 = MPI_Wtime();
    double lb, ub;
    int i;
    Gorilla silverback, GX[N], old_GX[N], X[N];
    srand(time(NULL)); // Seed the random number generator
    initialization(&lb, &ub, N, GX, &silverback, X);
    //printSearchAgentsData(N, 0, silverback.fitness, X);

    for (i = 0; i < T; i++) {
        double C, L, M[DIM];
        C = (cos(2.0 * rand01()) + 1) * (1.0 - (double)i / T), L = C * unifrnd(1);
        exploration(C, L, lb, ub, M, N, GX, &silverback, old_GX, X);
        //printSearchAgentsData(N, 0, silverback.fitness, X);
        exploitation(C, L, lb, ub, M, N, GX, &silverback, X);
        //printSearchAgentsData(N, 0, silverback.fitness, X);
    }
    
    double t2 = MPI_Wtime();
    printf("%f, ", t2 - t1);
    MPI_Finalize();
    return 0;
}