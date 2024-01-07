#include <stddef.h>
#include <sched.h>
#include <mpi.h>
#include "GTO.h"

void findGlobalSilverback(void *in, void *out, int *len, MPI_Datatype *dt) {
    Gorilla *in_data = (Gorilla *)in;
    Gorilla *out_data = (Gorilla *)out;

    if (in_data->fitness < out_data->fitness)
        memcpy(out_data, in_data, sizeof(Gorilla)); // Copy the entire struct from in_data to out_data
}

void updateGlobalSilverback(MPI_Datatype dt, MPI_Op myOp, Gorilla *silverback) {
    Gorilla global_silverback;
    MPI_Allreduce(silverback, &global_silverback, 1, dt, myOp, MPI_COMM_WORLD);
    memcpy(silverback, &global_silverback, sizeof(Gorilla)); // Copy the entire struct from global_silverback to silverback
}

int main() {
	MPI_Init(NULL, NULL);
    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();
    double lb, ub;
    int comm_sz, i, rank;
    MPI_Datatype MPI_GORILLA;
    MPI_Op myOp;
    Gorilla silverback;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    const int gorilla_per_process = N / comm_sz;
    Gorilla GX[gorilla_per_process], old_GX[gorilla_per_process], X[gorilla_per_process];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Type_create_struct(2, (int[]){DIM, 1}, (MPI_Aint[]){offsetof(Gorilla, coordinates), offsetof(Gorilla, fitness)}, (MPI_Datatype[]){MPI_DOUBLE, MPI_LONG_DOUBLE}, &MPI_GORILLA);
    MPI_Type_commit(&MPI_GORILLA);
    MPI_Op_create(findGlobalSilverback, 1, &myOp); 
    srand(time(NULL) + rank); // Seed the random number generator
    initialization(&lb, &ub, gorilla_per_process, GX, &silverback, X);
    updateGlobalSilverback(MPI_GORILLA, myOp, &silverback);
    //printSearchAgentsData(gorilla_per_process, rank, silverback.fitness, X);

    for (i = 0; i < T; i++) {
        double C, L, global_M[DIM], M[DIM];
        if (rank == 0)
            C = (cos(2.0 * rand01()) + 1) * (1.0 - (double)i / T), L = C * unifrnd(1);

        // Broadcast the data from the root process (rank 0) to all other processes
        MPI_Bcast(&C, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&L, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        // ------------------------------------------------------------------------      
        exploration(C, L, lb, ub, M, gorilla_per_process, GX, &silverback, old_GX, X);
        updateGlobalSilverback(MPI_GORILLA, myOp, &silverback);
        //printSearchAgentsData(gorilla_per_process, rank, silverback.fitness, X);
        MPI_Allreduce(M, global_M, DIM, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD); // Use MPI_Allreduce to compute the global sum array
        exploitation(C, L, lb, ub, global_M, gorilla_per_process, GX, &silverback, X);
        updateGlobalSilverback(MPI_GORILLA, myOp, &silverback);
        //printSearchAgentsData(gorilla_per_process, rank, silverback.fitness, X);
    }

    MPI_Op_free(&myOp);
    MPI_Type_free(&MPI_GORILLA);
    MPI_Barrier(MPI_COMM_WORLD);
    double t2 = MPI_Wtime();

    if (rank == 0)
        printf("%f, ", t2 - t1);

    MPI_Finalize();
    return 0;
}