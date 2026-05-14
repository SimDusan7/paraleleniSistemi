#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#define m 2
#define n 3
#define k 4
void main(int argc, char* argv[]) {
	int A[m][n], B[n][k], C[m][k],niza[m], nizb[k], local_c[m][k], c[m][k];
	int rank, size, i, j;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Datatype vector, column, dt;
	MPI_Type_vector(m, 1, m, MPI_INT, &vector);
	MPI_Type_commit(&vector);
	MPI_Type_create_resized(vector, 0, 1 * sizeof(int), &column);
	MPI_Type_commit(&column);
	if (rank == 0) {
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				A[i][j] = i + j;
			}
		}
		for (i = 0; i < n; i++) {
			for (j = 0; j < k; j++) {
				B[i][j] = 1 + j - i;
			}
		}
	}
	MPI_Scatter(A, 1, column, niza, m, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(B, k, MPI_INT, nizb, k, MPI_INT, 0, MPI_COMM_WORLD);
	for (i = 0; i < m; i++) {
		for (j = 0; j < k; j++) {
			local_c[i][j] = niza[i] * nizb[j];
		}
	}
	for (i = 0; i < m; i++)
		for (j = 0; j < k; j++)
			c[i][j] = 0;
	MPI_Reduce(&local_c[0][0], &c[0][0], m * k, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		for (i = 0; i < m; i++)
		{
			for (j = 0; j < k; j++) {
				printf("%d ", c[i][j]);
			}
			printf("\n");
		}
	}
		MPI_Finalize();

}