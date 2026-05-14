#include <mpi.h>
#include<stdio.h>
#include <stdlib.h>
#define n 3
void main(int argc, char* argv[]) {
	float a[n][n], b[n],c[n];
	int size, rank, i, j;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				a[i][j] = i + j;
			}
			b[i] = 1;
		}
	}
	float local_a[n];
	float local_c = 0;
	MPI_Scatter(&a[0][0], n, MPI_FLOAT, local_a, n, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Bcast(b, n, MPI_FLOAT, 0, MPI_COMM_WORLD);

	for (i = 0; i < n; i++) {
		local_c += local_a[i] * b[i];
	}

	MPI_Gather(&local_c, 1, MPI_FLOAT, c, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		for (i = 0; i < n; i++) {
			printf("%f", c[i]);

			printf("\n");
		}
	}
	MPI_Finalize();


}