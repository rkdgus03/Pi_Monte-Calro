#define _USE_MATH_DEFINES
#define l 4                      // Needle's length
#define d 8                      // Distance between lines
#define rep 1000000000           // Total number of needle throws

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <sprng_c.h>             // SPRNG 5 header for C interface

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);                     // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);       // Get rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size);       // Get total number of processes

    unsigned long long int local_rep = rep / size;  // Divide work among processes
    unsigned long long int local_n = 0, global_n = 0;
    double xi, cs, xf, pi = 0.0;

    // Initialize SPRNG stream
    int stream_id = rank; // Stream ID unique for each rank
    int *stream = init_sprng(stream_id, SPRNG_DEFAULT); // Use default generator type

    for (unsigned long long int a = 0; a < local_rep; a++) {
        // Generate random initial point within [0, d)
        xi = sprng(stream) * d;

        // Generate random angle and calculate cosine
        cs = cos(sprng(stream) * M_PI);

        // Determine if the needle intersects a line
        xf = xi + l * cs;
        if (fabs(cs) * l >= fmod(xi, d)) {
            local_n++;
        }
    }

    // Reduce local results to the root process
    MPI_Reduce(&local_n, &global_n, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Calculate pi on the root process
    if (rank == 0) {
        pi = (2.0 * (double)rep * l) / (global_n * d);
        printf("Estimated value of pi: %lf\n", pi);
    }

    // Free the SPRNG stream
    free_sprng(stream);

    MPI_Finalize();  // Finalize MPI
    return 0;
}
