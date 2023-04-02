#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSTACK 5000


typedef enum {false,true} bool;

enum _state {parallel,vertical};

typedef struct {
	int column;
	int row;
	enum _state state;
	int count;
}position;

typedef struct {
	position list[MAXSTACK];
	int top;
}stack;

void InitStack(stack* pstack);
bool IsEmpty(stack* pstack);
bool IsFull(stack* pstack);
position Peek(stack* pstack);
void Push(stack* pstack, position w);
void Pop(stack* pstack);
bool Isfinish(position dest, position now);


int main() {

	int a=0;
	int n;
	scanf("%d", &n);
	char** matrix = malloc(sizeof(char*) * n);
	for (int i = 0; i < n; i++) {
		matrix[i] = malloc(sizeof(char) * (n+1));
	}

	for (int i = 0; i < n; i++) {
		scanf("%s", matrix[i]);
	}//input save


	position p, h;
	p.row = 0, p.column = 0;
	p.state = vertical;
	h.row = 0, h.column = 0;
	h.state = vertical;

	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (matrix[i][j] == 'H') {
				if (matrix[i][j + 1] == 'H') {
					h.state = parallel;
				}
				h.column += j;
				h.row += i;
				matrix[i][j] = '0';
			}
			else if (matrix[i][j] == 'P') {
				if (matrix[i][j + 1] == 'P') {
					p.state = parallel;
				}
				p.column += j;
				p.row += i;
				matrix[i][j] = '0';
			}
		}
	}
	h.row = h.row / 3;
	h.column = h.column / 3;
	p.row = p.row / 3;
	p.column = p.column / 3;
	p.count = 0;

	if (Isfinish(h,p)) {
		printf("0");
		return 0;
	}

	int** vervisit = malloc(sizeof(int*) * n);
	int** parvisit = malloc(sizeof(int*) * n);

	for (int i = 0; i < n; i++) {
		vervisit[i] = malloc(sizeof(int) * n);
		parvisit[i] = malloc(sizeof(int) * n);
		memset(vervisit[i], 0, sizeof(int) * n);
		memset(parvisit[i], 0, sizeof(int) * n);
	}

	if (p.state == vertical) {
		vervisit[p.row][p.column] = 1;
	}
	else {
		parvisit[p.row][p.column] = 1;
	}

	stack* dsf =malloc(sizeof(stack));
	InitStack(dsf);
	Push(dsf, p);
	while (!IsEmpty(dsf)) {
		position move = Peek(dsf);
		Pop(dsf);
		if (move.state == vertical) {
			if ((move.column > 0) && ((matrix[move.row][move.column - 1] == '0') && (matrix[move.row + 1][move.column - 1] == '0') && (matrix[move.row - 1][move.column - 1] == '0'))) {
				position item = move;
				item.column = move.column - 1;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((vervisit[item.row][item.column] == 0)) {
					Push(dsf, item);
					vervisit[item.row][item.column] = 1;
				}
			}//move left at vertical
			if ((move.column < (n - 1)) && (matrix[move.row][move.column + 1] == '0') && (matrix[move.row + 1][move.column + 1] == '0') && (matrix[move.row - 1][move.column + 1] == '0')) {
				position item = move;
				item.column = move.column + 1;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((vervisit[item.row][item.column] == 0)) {
					Push(dsf, item);
					vervisit[item.row][item.column] = 1;
				}
			}//move right at vertical
			if ((move.row > 1 && (matrix[move.row - 2][move.column] == '0'))) {
				position item = move;
				item.row = move.row - 1;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((vervisit[item.row][item.column] == 0)) {
					Push(dsf, item);
					vervisit[item.row][item.column] = 1;
				}
			}//move up at vertical
			if ((move.row < n - 2) && (matrix[move.row + 2][move.column] == '0')) {
				position item = move;
				item.row = move.row + 1;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((vervisit[item.row][item.column] == 0)) {
					Push(dsf, item);
					vervisit[item.row][item.column] = 1;
				}
			}//move down at vertical
			if ((matrix[move.row][move.column - 1] == '0') && (matrix[move.row][move.column] == '0') && (matrix[move.row][move.column + 1] == '0') && (matrix[move.row - 1][move.column - 1] == '0') && (matrix[move.row - 1][move.column] == '0') && (matrix[move.row - 1][move.column + 1] == '0') && (matrix[move.row + 1][move.column - 1] == '0') && (matrix[move.row + 1][move.column] == '0') && (matrix[move.row + 1][move.column + 1] == '0')) {
				position item = move;
				item.state = parallel;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((parvisit[item.row][item.column] != 1)) {
					Push(dsf, item);
					parvisit[item.row][item.column] = 1;
				}
			}// rotate
		}
		else if (move.state == parallel) {
			if ((move.column > 1) && (matrix[move.row][move.column - 2] == '0')) {
				position item = move;
				item.column = move.column - 1;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((parvisit[item.row][item.column] != 1)) {
					Push(dsf, item);
					parvisit[item.row][item.column] = 1;
				}
			}
			if ((move.column < n - 2) && (matrix[move.row][move.column + 2] == '0')) {
				position item = move;
				item.column = move.column + 1;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((parvisit[item.row][item.column] != 1)) {
					Push(dsf, item);
					parvisit[item.row][item.column] = 1;
				}
			}
			if ((move.row > 0) && (matrix[move.row - 1][move.column - 1] == '0') && (matrix[move.row - 1][move.column] == '0') && (matrix[move.row - 1][move.column + 1] == '0')) {
				position item = move;
				item.row = move.row - 1;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((parvisit[item.row][item.column] != 1)) {
					Push(dsf, item);
					parvisit[item.row][item.column] = 1;
				}
			}
			if ((move.row < n - 1) && (matrix[move.row + 1][move.column - 1] == '0') && (matrix[move.row + 1][move.column] == '0') && (matrix[move.row + 1][move.column + 1] == '0')) {
				position item = move;
				item.row = move.row + 1;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((parvisit[item.row][item.column] != 1)) {
					Push(dsf, item);
					parvisit[item.row][item.column] = 1;
				}
			}
			if ((move.row>0&&move.row<n-1)&&(move.column>0&&move.column<n-1)&&(matrix[move.row][move.column - 1] == '0') && (matrix[move.row][move.column] == '0') && (matrix[move.row][move.column + 1] == '0') && (matrix[move.row - 1][move.column - 1] == '0') && (matrix[move.row - 1][move.column] == '0') && (matrix[move.row - 1][move.column + 1] == '0') && (matrix[move.row + 1][move.column - 1] == '0') && (matrix[move.row + 1][move.column] == '0') && (matrix[move.row + 1][move.column + 1] == '0')) {
				position item = move;
				item.state = vertical;
				item.count++;
				if (Isfinish(h, item)) {
					p.count = item.count;
					break;
				}
				if ((vervisit[item.row][item.column] != 1)) {
					Push(dsf, item);
					vervisit[item.row][item.column] = 1;
				}
			}			
		}
	}
	printf("%d", p.count);


	free(dsf);
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
		free(vervisit[i]);
		free(parvisit[i]);
	}
	free(matrix);
	free(vervisit);
	free(parvisit);

}

void InitStack(stack* pstack) {
	pstack->top = -1;
}

bool IsEmpty(stack* pstack) {
	return pstack->top == -1;
}

bool IsFull(stack* pstack) {
	return pstack->top == MAXSTACK-1;
}

position Peek(stack* pstack) {
	if (IsEmpty(pstack)) {
		exit(1);
	}
	return pstack->list[pstack->top];
}

void Push(stack* pstack, position w) {
	if (IsFull(pstack)) {
		exit(1);
	}
	pstack->list[++(pstack->top)] = w;
}

void Pop(stack* pstack) {
	if (IsEmpty(pstack)){
		exit(1);
	}
	pstack->top--;
}

bool Isfinish(position dest,position now) {
	if (dest.row == now.row) {
		if (dest.column == now.column) {
			if (dest.state == now.state) {
				return true;
			}
		}
	}
	return false;
}