//oktobar II 2025
//#include<mpi.h>
//#include<stdio.h>
//#include<math.h>
//#include <random>
//#define n 3
//#define k 6
//#define m 4
//void main(int argc, char* argv[]) {
//	int i, j, size, rank, A[n][k], B[k][m], C[n][m], * mycols, * myrows;
//	int l;
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	MPI_Datatype col, colR, row, rowR;
//	if (rank == 0) {
//		for (i = 0; i < n; i++) {
//			for (j = 0; j < k; j++) {
//				A[i][j] = rand() % 10;
//			}
//		}
//		for (i = 0; i < k; i++) {
//			for (j = 0; j < m; j++) {
//				B[i][j] = rand() % 10;
//			}
//		}
//	}
//	l = k / size;
//
//	//Distribucija A
//	mycols = (int*)malloc(n * l * sizeof(int));
//	MPI_Type_vector(n * l, 1, size, MPI_INT, &col);
//	MPI_Type_commit(&col);
//
//	MPI_Type_create_resized(col, 0, 1 * sizeof(int), &colR);
//	MPI_Type_commit(&colR);
//
//	MPI_Scatter(A, 1, colR, mycols, n * l, MPI_INT, 0, MPI_COMM_WORLD);
//
//	//Distribucija B
//	myrows = (int*)malloc(m * l * sizeof(int));
//	MPI_Type_vector(l, m, m * size, MPI_INT, &row);
//	MPI_Type_commit(&row);
//
//	MPI_Type_create_resized(row, 0, m * sizeof(int), &rowR);
//	MPI_Type_commit(&rowR);
//
//	MPI_Scatter(B, 1, rowR, myrows, m * l, MPI_INT, 0, MPI_COMM_WORLD);
//
//	//Izracunavanje
//	int* local_c = (int*)malloc(n * m * sizeof(int));
//	for (i = 0; i < n * m; i++) {
//		local_c[i] = 0;
//	}
//
//	for (i = 0; i < n; i++) {
//		for (j = 0; j < m; j++) {
//			for (int iter = 0; iter < l; iter++) {
//				local_c[i * m + j] += mycols[i * l + iter] * myrows[iter * m + j];
//			}
//		}
//	}
//
//	MPI_Reduce(local_c, C, n * m, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
//	if (rank == 0) {
//		for (i = 0; i < n; i++) {
//			for (j = 0; j < m; j++) {
//				printf("%d", C[i][j]);
//			}
//		}
//	}
//	MPI_Finalize();
//
//
//}

#include<mpi.h>
#include<stdio.h>
#include<math.h>
#include <random>
#define r 3
#define m 6
#define k 4
void main(int argc, char* argv[]) {
	
	int rank, size, i, j, l, A[r][m], B[m][k], C[r][k], * myrows, * local_c;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Datatype rows, rowsR, recvRows, recvRowsR;
	if (rank == 0) {
		for (i = 0; i < r; i++) {
			for (j = 0; j < m; j++) {
				A[i][j] = rand() % 10;
			}
		}
		for (i = 0; i < m; i++) {
			for (j = 0; j < k; j++) {
				B[i][j] = rand() % 10;
			}
		}
	}

	l = r / size;
	myrows = (int*)malloc(l * m * sizeof(int));
	MPI_Type_vector(l, m, size * m, MPI_INT, &rows);
	MPI_Type_commit(&rows);

	MPI_Type_create_resized(rows, 0, m * sizeof(int), &rowsR);
	MPI_Type_commit(&rowsR);

	MPI_Scatter(A, 1, rowsR, myrows, l * m, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(B, m * k, MPI_INT, 0, MPI_COMM_WORLD);

	//izracunavanje
	local_c = (int*)malloc(l * k * sizeof(int));
	for (int i = 0; i < l * k; i++) {
		local_c[i] = 0;
	}

	for (i = 0; i < l; i++) {
		for (j = 0; j < k; j++) {
			for (int iter = 0; iter < m; iter++) {
				local_c[i * k + j] += myrows[i * m + iter] * B[iter][j];
			}
		}
	}

	MPI_Type_vector(l, k, k * size, MPI_INT, &recvRows);
	MPI_Type_commit(&recvRows);

	MPI_Type_create_resized(recvRows, 0, k, &recvRowsR);
	MPI_Type_commit(&recvRowsR);

	MPI_Gather(local_c, l * k, MPI_INT, C, 1, recvRowsR, 0, MPI_COMM_WORLD);


}