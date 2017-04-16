#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RandBool (rand() % 2)

typedef int **Matrix;

Matrix TransiveClosure(Matrix mat, int n);
void Merge(Matrix *A, Matrix *B, int n);
void Union(Matrix *A, Matrix *B, int n);
Matrix CopyMatrix(Matrix mat, int n);
Matrix Floyd(Matrix mat, int n);
Matrix NewMatrix(int n);
void MatrixInitialize(Matrix mat, int n);
void PrintMatrix(Matrix mat, int n);

void PrintMatrixToFile(FILE *f, Matrix mat, int n);

int main()
{
	Matrix mat = NULL;
	clock_t timeBegin, timeEnd;
	int n;
	FILE *f = NULL;
    /*open file*/
	if (!(f = fopen("./res.txt", "w+")))
	{
		printf("Cannot open file\n");
		exit(0);
	}

	for (n = 2; n <= 1000; n++)
	{
		mat = NewMatrix(n);
		MatrixInitialize(mat, n);
		PrintMatrixToFile(f, mat, n);

		/*user the algorithm to calculate matrix of */
		timeBegin = clock();
        mat = TransiveClosure(mat, n);//Floyd(mat, n);
		timeEnd = clock();

		fprintf(f, "Changed to:\n");
		PrintMatrixToFile(f, mat, n);
		fprintf(f, "Time Cost:%ld\n", timeEnd - timeBegin);
	}
	fclose(f);
	return 0;
}

/*the algorithm of O(n^4)*/
Matrix TransiveClosure(Matrix mat, int n)
{
	Matrix tempMatrix, finalMatrix;
	tempMatrix = CopyMatrix(mat, n);
	finalMatrix = CopyMatrix(mat, n);
	int i;
	for (i = 1; i<n; i++)
	{
		Merge(&tempMatrix, &mat, n);
		Union(&finalMatrix, &tempMatrix, n);
	}
	return finalMatrix;
}

/*Floyd algorithm*/
Matrix Floyd(Matrix mat, int n)
{
	int i, j, k;
	for (i = 1; i < n; i++)
		for (j = 0; j < n; j++)
			for (k = 0; k < n; k++)
				if (k == 0)
				{
					mat[i][j] = mat[i][j] || (mat[i][k] && mat[k][j]);
				}
	return mat;
}

Matrix NewMatrix(int n)
{
	Matrix mat = (Matrix)malloc(sizeof(int *)*n);
	int i;
	for (i = 0; i < n; i++)
	{
		mat[i] = (int *)malloc(sizeof(int) * n);
	}
	return mat;
}

void MatrixInitialize(Matrix mat, int n)
{
	int i, j;
	srand(time(NULL));
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			mat[i][j] = RandBool;
		}
	}
}

void PrintMatrix(Matrix mat, int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j< n; j++)
		{
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void Merge(Matrix *A ,Matrix *B, int n)
{
	int i, j, k;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
		    for(k = 0;k<n;k++)
            {
                (*A)[i][j] = (*A)[i][j] || ((*A)[i][k] && (*B)[k][j]);
            }

		}
	}
}

void Union(Matrix *A, Matrix *B, int n)
{
    int i,j;
    for(i = 0; i<n; i++)
    {
        for(j = 0; j<n; j++)
        {
            (*A)[i][j] = (*A)[i][j] || (*B)[i][j];
        }
    }
}

Matrix CopyMatrix(Matrix mat, int n)
{
    Matrix dstMatrix = NewMatrix(n);
    int i,j;
    for(i = 0; i<n; i++)
    {
        for(j = 0; j<n;j++)
        {
            dstMatrix[i][j] = mat[i][j];
        }
    }
    return dstMatrix;
}


void PrintMatrixToFile(FILE *f, Matrix mat, int n)
{
	int i, j;
	fprintf(f, "Matrix size:%d\n", n);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j< n; j++)
		{
			fprintf(f, "%d ", mat[i][j]);
		}
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
}
