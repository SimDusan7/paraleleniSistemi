#include<mpi.h>
#include<stdio.h>

void main(int argc, char* argv[]) {
	struct {
		double value;
		int rank;
	}in[30], out[30];
	int rank, i, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	for (i = 0; i < 30; i++) {
		in[i].value = double(rank + i);
		in[i].rank = rank;
	}
	MPI_Reduce(in, out, 30, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		for (i = 0; i < 30; i++) {
			printf("Out value: %lf\n",out[i].value);
			printf("Out rank: %d", out[i].rank);
		}
	}
	MPI_Finalize();

}