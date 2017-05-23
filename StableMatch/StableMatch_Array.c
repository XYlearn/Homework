#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SIZE 2000
void StableMatch(int malePref[][SIZE], int femalePref[][SIZE], int n);
void randPref(int pref[][SIZE], int n);
int getFreeMale(int *freeMale, int n);
int getFemale(int *malePref, int n);
int findPref(int *pref, int male1, int male2, int n);
void deleteFemale(int *pref, int female, int n);
/*make output*/
void printMatch(int *match, int n);
void printPref(int pref[][SIZE], int n);

int malePref[SIZE][SIZE], femalePref[SIZE][SIZE];
int main()
{
	clock_t begin, end, cost;
	int i;
	FILE *fp;

	if ((fp = fopen("./out.txt", "a+")) == NULL)
	{
		printf("cant open the file.");
		exit(0);
	}

	srand(time(NULL));
	cost = 0;
	for (i = 0; i < 20; i++)
	{
		randPref(malePref, SIZE);
		randPref(femalePref, SIZE);
		begin = clock();
		StableMatch(malePref, femalePref, SIZE);
		end = clock();
		cost += end - begin;
	}
	printf("%d %ld\n", SIZE, cost);
	fprintf(fp, "%d %ld\n", SIZE, cost);
	return 0;
}

void randPref(int pref[][SIZE], int n)
{
	int i, j, k, temp;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			pref[i][j] = j;
		}
		for (j = n - 1; j > 0; j--)
		{
			k = rand() % (j + 1);
			temp = pref[i][j];
			pref[i][j] = pref[i][k];
			pref[i][k] = temp;
		}
	}
}

void StableMatch(int malePref[][SIZE], int femalePref[][SIZE], int n)
{
	int freeMale[SIZE];
	int matched[SIZE];
	memset(freeMale, 0, SIZE * sizeof(int));
	memset(matched, -1, SIZE * sizeof(int));
	int i;
	int curMale, curFemale, matchedMale, prefMale;
	while ((curMale = getFreeMale(freeMale, n)) != -1)
	{
		curFemale = getFemale(malePref[curMale], n);
		matchedMale = matched[curFemale];
		if (matchedMale == -1)
		{
			matched[curFemale] = curMale;
			freeMale[curMale] = -1;
		}
		else
		{
			prefMale = findPref(femalePref[curFemale], curMale, matchedMale, n);
			if (prefMale == curMale)
			{
				matched[curFemale] = curMale;
				freeMale[curMale] = -1;
				freeMale[matchedMale] = 0;
				deleteFemale(malePref[matchedMale], curFemale, n);
			}
			else
			{
				deleteFemale(malePref[curMale], curFemale, n);
			}
		}
	}
}

int findPref(int *pref, int male1, int male2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (pref[i] == male1)
			return male1;
		else if (pref[i] == male2)
			return male2;
	}
	return -1;
}

int getFemale(int *malePref, int n)
{
	int i, femaleIndex;
	for (i = 0; i < n; i++)
	{
		femaleIndex = malePref[i];
		if (femaleIndex != -1)
			return femaleIndex;
	}
	return -1;
}

void deleteFemale(int *pref, int female, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (pref[i] == female)
		{
			pref[i] = -1;
			return;
		}
	}
}

int getFreeMale(int *freeMale, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (freeMale[i] != -1)
			return i;
	}
	return -1;
}

void printMatch(int *match, int n)
{
	int i;
	printf("Female\tmale\n");
	for (i = 0; i<n; i++)
	{
		printf("%d\t%d\n", i, match[i]);
	}
}

/*print the preference table*/
void printPref(int pref[][SIZE], int n)
{
	int i,j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%d ", pref[i][j]);
		}
		printf("\n");
	}
}
