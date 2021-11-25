#include <cmath>
#include <cstdio>
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

struct cb{
	int v; // 记录这个局面下子节点有几个能赢的
	int t; // 记录应该去哪个
	int d; // 树深 根节点为0
	cb() {};
	cb(int _v, int _t, int _d) {
		v = _v; t = _t; d = _d;
	}
} cx[20000] = {{0, 0, 0}};



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

int dfs(int deep, int r, int c, int f) { // 上一层深度 r c 父节点状态
// dfs 用来建树和记忆化
	int x = f + (2 - deep % 2) * _pow_(3, 3 * r + c);
	//printf("dfs: %d r:%d c:%d\n", deep, r, c);
	//print(x);
	if (cx[x].d) return cx[x].v;
	cx[x].d = ++deep;
	cx[x].v = 0; cx[x].t = 0;
	if (deep == 9) {
		puts("LEAVES!!!");
		print(x);
		cx[x].v = (check(x) == 1);
		printf("%d\n", cx[x].v);
		return cx[x].v;
	}
	int to;
	if (deep % 2) {
		// ai下
		for (int i = 0, z = x; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (!(z % 3)) {
					to = x + _pow_(3, 3 * i + j);
					if (dfs(deep, i, j, x)) {
						cx[x].v++;
						if (cx[to].v > cx[cx[x].t].v)
							cx[x].t = to;
					}
				}
				z /= 3;
			}
		}
	}
	else {
		for (int i = 0, z = x; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (!(z % 3)) {
					to = x + 2 * _pow_(3, 3 * i + j);
					if (dfs(deep, i, j, x)) {
						cx[x].v++;
						if (cx[to].v > cx[cx[x].t].v)
							cx[x].t = to;
					}
				}
				z /= 3;
			}
		}
	}
	return cx[x].v;
}

void play(int deep, int r, int c, int x) { // 上一层深度 r c 父节点状态 只走玩家输入的奇数层
	printf("playing: %d %d\n", r, c);
	if (deep == 10) {
		puts("ERROR!!!!");
		exit(1);
	}
	int now = x + 2 * _pow_(3, 3 * r + c);
	print(now);
	puts("Computer tern: ");
	if (cx[now].d) {
		if (!cx[now].v) {
			puts("电脑死了");
			return ;
		}
		print(cx[now].t);

		scanf("%d%d", &r, &c); r--; c--;
		play(deep + 2, r, c, cx[now].t);
		return ;
	}
	for (int i = 0, _x = now, to; i < 3; ++i) 
		for (int j = 0; j < 3; ++j) {
			if (!(_x % 3)) {
				//print(now);
				
				to = now + _pow_(3, 3 * i + j);
				if (dfs(deep + 1, i, j, now)) {
					cx[now].v++;
					if (cx[to].v > cx[cx[now].t].v)
						cx[now].t = to;
				}
			}
			_x /= 3;
		}
	puts("seek finish");

	if (!cx[now].v) {
		puts("电脑死了");
		return ;
	}
	else {
		print(cx[now].t);
		puts("Please enter your choice by row and column: "); // 玩家先手 父节点deep为奇数时ai下 玩家用O ai用X
		scanf("%d%d", &r, &c); r--; c--;
		play(deep + 2, r, c, cx[now].t);
	}
}

int main() {
	cx[0] = {0, 0, 0};
	print(0);
	puts("Please enter your choice by row and column: "); // 玩家先手 父节点deep为奇数时ai下 玩家用O ai用X
	int r, c;
	scanf("%d%d", &r, &c); r--; c--;
	play(0, r, c, 0);
	return 0;
}