#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define T(x) (x == 2 ? 'O' : (x == 1 ? 'X' : ' '))
// 2&O 玩家 1&X ai
using namespace std;

int _pow_(int x, int y) {
	int ans = 1;
	while (y) {
		if(y & 1) ans *= x;
		x = x * x;
		y >>= 1;
	}
	return ans;
}

/*
0 1 2
3 4 5
6 7 8
*/

int a[20000], b[20000]; // a是节点可行性判断 b是指向该去的节点
// a=0 时说明该节点有输的可能


void print(int j) {
	for (int i = 0, x, y, z; i < 3; ++i) {
		printf("-------\n");
		x = j % 3; j /= 3;
		y = j % 3; j /= 3;
		z = j % 3; j /= 3;
		printf("|%c|%c|%c|\n", T(x), T(y), T(z));
	}
	printf("-------\n");
}

int check(int x) {
	int m[3][3];
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			m[i][j] = x % 3;
			x /= 3;
		}
	int flag;
	for (int t = 1; t <= 2; ++t) {
		for (int i = 0; i < 3; ++i) {
			flag = 1;
			for (int j = 0; j < 3; ++j)
				flag &= (t == m[i][j]);
			if (flag) return t;
		}
		for (int j = 0; j < 3; ++j) {
			flag = 1;
			for (int i = 0; i < 3; ++i)
				flag &= (t == m[i][j]);
			if (flag) return t;
		}

		flag = 1;
		for (int i = 0; i < 3; ++i)
			flag &= (t == m[i][i]);
		if (flag) return t;

		flag = 1;
		for (int i = 0; i < 3; ++i)
			flag &= (t == m[i][2 - i]);
		if (flag) return t;
	}
	return 0;
}

int trans(int now, int r, int c, int side) {
	return now + side * _pow_(3, 3 * r + c);
}

int dfs2(int now, int dph);
int dfs1(int now, int dph) {
	//ai下 now是一个人下的节点 下一层是ai下完人下，只要下一层有一个点必赢/平就是好节点
	if (a[now] != -1) return a[now];
	int tmp = check(now);
	if (tmp == 0) a[now] = 1;
	if (tmp == 1) a[now] = 2;
	if (tmp == 2) a[now] = 0;
	if (dph == 9) return a[now];
	else if (a[now] != 1) return a[now];
	a[now] = 0;
	for (int i = 0, k = now, to, tmp; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (k % 3 == 0) {
				to = trans(now, i, j, 1);
				tmp = dfs2(to, dph + 1);
				if (a[now] < tmp) {
					a[now] = tmp;
					b[now] = to;
				}
			}
			k /= 3;
		}
	}
	return a[now];
}

int dfs2(int now, int dph) {
	// 替人下
	// 如果有“不可挽回的局面”，那么这一个点就是坏的
	if (a[now] != -1) return a[now];
	int tmp = check(now);
	if (tmp == 0) a[now] = 1;
	if (tmp == 1) a[now] = 2;
	if (tmp == 2) a[now] = 0;
	if (dph == 9) return a[now];
	else if (a[now] != 1) return a[now];
	a[now] = 1; int flag = 1;
	for (int i = 0, k = now, to, tmp; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (k % 3 == 0) {
				to = trans(now, i, j, 2);
				tmp = dfs1(to, dph + 1);
				if (a[now] < tmp) {
					a[now] = tmp;
					b[now] = to;
				}
				if (tmp == 0) {
					flag = 0;
				}
			}
			k /= 3;
		}
	}
	if (flag == 0) a[now] = 0;
	return a[now];
}

void play(int now, int dph) {
	//真下
	print(now);
	if (dph >= 9) {
		if (a[now] == 2) {
			puts("Computer Wins");
		}
		if (a[now] == 1) {
			puts("Tie");
		}
		if (a[now] == 0) {
			puts("You Win");
		}
		return ;
	}
	dfs1(now, dph);
	if (a[now] == 0) {
		puts("You Win");
		return ;
	}
	now = b[now];
	puts("Computer's turn...");
	print(now);
	if (a[now] == 0) {
		puts("You Win");
		return ;
	}
	if (a[now] == 2 && check(now) == 1) {
		puts("You Lose");
		return ;
	}
	int r, c;
	puts("Your turn");
	scanf("%d%d", &r, &c);
	play(trans(now, r - 1, c - 1, 2), dph + 2);
}

int main() {
	memset(a, 0xff, sizeof(a));
	puts("-----Game Start-----");
	print(0);
	puts("Input 1 to go first...");
	int r, c; cin >> r;
	if (r == 1) {
		puts("Please enter your choice by row and column: ");
		scanf("%d%d", &r, &c);
		play(trans(0, r - 1, c - 1, 2), 1);
	}
	else {
		int tmp = trans(0, 1, 1, 1);
		print(tmp);
		puts("Your turn");
		scanf("%d%d", &r, &c);
		play(trans(tmp, r - 1, c - 1, 2), 2);
	}
	return 0;
}