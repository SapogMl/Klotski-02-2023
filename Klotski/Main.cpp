#include<iostream>
#include"Header.h"

void solveKlotskiPuzzle()
{
	char empty = '.'; char immov = '-';

	int w, h;
	cout << "Ширина игрового поля = ";
	cin >> w;
	cout << "Высота игрового поля = ";
	cin >> h;

	string start;
	char c;
	cout << "Игровое поле. Разные блоки обозначайте разными символами. Точка (.) обозначает пустое место на поле, дефис (-) — неподвижный блок. ";
	cout << "Для обозначения блоков, занимающих более одной клетки, проставьте во всех соотвествующих местах на поле один и тот же символ." << '\n';
	for (int i = 0; i < w * h; i++) {
		cin >> c;
		start += c;
	}
	map <char, int> info = getFieldInfo(start);

	char win_block;
	cout << "Цель игры. Блок, который нужно достать из головоломки." << '\n';
	cin >> win_block;

	int* win_pos = new int[info[win_block]];
	int buf = 0;
	cout << "Место, в которое должен попасть блок, чтобы его можно было достать (пары чисел x, y - координаты соответствующих точек поля). ";
	cout << "Левый верхний угол игрового поля — точка(0, 0)." << '\n';
	for (int i = 0; i < info[win_block]; i++) {
		cin >> buf;
		win_pos[i] = buf;
		cin >> buf;
		win_pos[i] = win_pos[i] + buf * w;
	}

	cout << '\n';

	char sp_blocks[3]{ empty, immov, win_block };
	map <char, char> eq_list = getEq(start, w, info, info.size(), sp_blocks);

	vector <string> Solution = getSolution(start, win_block, win_pos, empty, immov, w, h, info, eq_list);
	printSolution(Solution, w, h);
}



int main()
{
	setlocale(LC_ALL, "Russian");

	solveKlotskiPuzzle();

	char stop;
	cin >> stop;

	return EXIT_SUCCESS;
}