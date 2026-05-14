#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#define m 4
#define n 3
void main(int argc, char* argv[]) {
	struct { 
		int val; 
		int rank; 
	}min, gmin;
	MPI_Init(&argc, &argv);
	float a[m][n], b[n],x[m], y[m],z, local_c[m], c[m];
	int i, j, rank, size;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0) {
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				a[i][j] = i + j;
			}
		}
		for (i = 0; i < n; i++) {
			b[i] = 1;
		}
	}
	if (rank == 0) {
		for (i = 0; i < m; i++) {
			x[i] = a[i][0];
		}
		for (j = 1; j < size; j++) {
			for (i = 0; i < m; i++) {
				y[i] = a[i][j];
			}
			MPI_Send(y, m, MPI_FLOAT, j, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(x, m, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
	}
	MPI_Scatter(&b[0], 1, MPI_FLOAT, &z, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	for (i = 0; i < m; i++) {
		local_c[i] = x[i] * z;
	}
	min.val = INT_MAX;
	for (i = 0; i < m; i++) {
		if (x[i] < min.val) {
			min.val = x[i];
			min.rank = rank;
		}
	}
	MPI_Reduce(&min, &gmin, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&gmin, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	MPI_Reduce(local_c, c, m, MPI_FLOAT, MPI_SUM, gmin.rank, MPI_COMM_WORLD);
	if (rank == gmin.rank) {
		for (i = 0; i < m; i++) {
			printf("c[%d]=%f\n", i, c[i]);
			printf("gmin=%d", gmin.val);
		}
	}
	MPI_Finalize();
}