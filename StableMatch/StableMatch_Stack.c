#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node
{
	int index;
	struct Node *next;
} Node;
typedef Node *Stack, *OnePref;
typedef Stack *MalePref, *FemalePref, *StackArr;

/*Stack operation*/
void push(Stack stack, int ind);
Node* pop(Node * prevNode);
/*free memory*/
void freeStackArr(StackArr stackArr, int n);
void freeStack(Stack stack);
/*make comparison beween two male*/
int findPref(OnePref onePref, int male1, int male2);
/*get Node pointer of free male*/
Node *getMale(Stack freeMale);
/*algorithm*/
int *Stable_Match(MalePref malePref, FemalePref femalePref, int n);
/*create random input*/
StackArr randStackArr(int n);
int *randSerial(int n);
/*make output*/
void printMatch(int *match, int n);
void printPref(StackArr stackArr, int n);


int main()
{
	int i, n;
	clock_t begin, end, timeCost;
	StackArr malePref, femalePref;
	int *match;

	FILE *fp = NULL;
	if (!(fp = fopen("./out.txt", "w")))
	{
		printf("Can't open file\n");
		exit(0);
	}

	srand(time(NULL));
	for (n = 2; n <= 1000; n++)
	{
		timeCost = 0;
		for (i = 0; i<20; i++)
		{
			malePref = randStackArr(n);
			femalePref = randStackArr(n);
			//printPref(malePref, n);
			//printPref(femalePref, n);
			begin = clock();
			match = Stable_Match(malePref, femalePref, n);
			//printMatch(match, n);
			end = clock();
			free(match);
			freeStackArr(malePref, n);
			freeStackArr(femalePref, n);

			timeCost += end - begin;
		}

		fprintf(fp, "%d %ld\n", n, timeCost);
		printf("%d %ld\n", n, timeCost);
	}
	fclose(fp);
	return 0;
}

int findPref(OnePref onePref, int male1, int male2)
{
	for (onePref = onePref->next; onePref != NULL; onePref = onePref->next)
	{
		if (onePref->index == male1)
			return male1;
		if (onePref->index == male2)
			return male2;
	}
	return -1;
}

int *Stable_Match(MalePref malePref, FemalePref femalePref, int n)
{
	Stack freeMale = (Stack)calloc(1, sizeof(Node));
	Stack tempStack = freeMale;
	int i;
	for (i = 0; i<n; i++)
	{
		tempStack->next = (Node *)calloc(1, sizeof(Node));
		tempStack = tempStack->next;
		tempStack->index = i;
	}

	int *match = (int *)calloc(n, sizeof(int));
	memset(match, -1, n * sizeof(int));

	int maleIndex, femaleIndex, tempIndex, matchedMaleIndex;
	Node *curMale, *curFemale;
	while ((curMale = getMale(freeMale)) != NULL)
	{
		maleIndex = curMale->index;
		curFemale = malePref[maleIndex]->next;
		femaleIndex = curFemale->index;
		if (match[femaleIndex] != -1)
		{
			tempIndex = findPref(femalePref[femaleIndex], match[femaleIndex], maleIndex);
			if (tempIndex == maleIndex)
			{
				matchedMaleIndex = match[femaleIndex];
				pop(freeMale);
				push(freeMale, maleIndex);
				pop(malePref[matchedMaleIndex]);
				match[femaleIndex] = maleIndex;
			}
			else
			{
				pop(malePref[maleIndex]);
			}
		}
		else
		{
			match[femaleIndex] = maleIndex;
			pop(freeMale);
		}
	}
	freeStack(freeMale);
	return match;
}

Node* getMale(Stack freeMale)
{
	if (freeMale->next)
		return freeMale->next;
	else
		return NULL;
}

Node* pop(Node* prevNode)
{
	Node *curNode = prevNode->next;
	if (curNode == NULL)
		return NULL;
	if (curNode->next == NULL)
	{
		free(curNode);
		prevNode->next = NULL;
	}
	else
	{
		prevNode->next = curNode->next;
		free(curNode);
	}
	return curNode;
}

void push(Stack stack, int ind)
{
	Node * node = stack->next;
	stack->next = (Node *)calloc(1, sizeof(Node));
	stack->next->index = ind;
	stack->next->next = node;
}

void freeStack(Stack stack)
{
	if (!stack)
		return;
	while(stack->next != NULL)
	{
		pop(stack);
	}
	free(stack);
}

void freeStackArr(StackArr stackArr, int n)
{
	if (stackArr == NULL)
		return;
	int i;
	for (i = 0; i<n; i++)
	{
		if (stackArr[i] != NULL)
		{
			freeStack(stackArr[i]);
		}
	}
	free(stackArr);
}

/*create preference table of random*/
StackArr randStackArr(int n)
{
	StackArr stackArr = NULL;
	Stack tempStack = NULL;
	int i, j, *serial;

	/*initialize StackArr*/
	stackArr = (StackArr)calloc(n, sizeof(Stack));
	for (i = 0; i<n; i++)
		stackArr[i] = (Stack)calloc(1, sizeof(Node));

	/*create the StackArr*/
	for (i = 0; i<n; i++)
	{
		tempStack = stackArr[i];
		serial = randSerial(n);
		for (j = 0; j<n; j++)
		{
			tempStack->next = (Node *)calloc(1, sizeof(Node));
			tempStack = tempStack->next;
			tempStack->index = serial[j];
		}
		free(serial);
	}
	return stackArr;
}

/*create serial number of random without repeat*/
int *randSerial(int n)
{
	int i, j, temp, *serial;
	/*rand serial*/
	serial = (int *)malloc(n * sizeof(int));
	for (i = 0; i<n; i++)
		serial[i] = i;
	for (i = n - 1; i>0; i--)
	{
		j = rand() % (i + 1);
		temp = serial[i];
		serial[i] = serial[j];
		serial[j] = temp;
	}
	return serial;
}

/*print result of match*/
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
void printPref(StackArr stackArr, int n)
{
	int i;
	Stack stack;
	for (i = 0; i<n; i++)
	{
		for (stack = stackArr[i]->next; stack != NULL; stack = stack->next)
		{
			printf("%d ", stack->index);
		}
		printf("\n");
	}
}
