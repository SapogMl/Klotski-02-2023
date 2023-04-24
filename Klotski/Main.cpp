#include<iostream>
#include"Header.h"

void solveKlotskiPuzzle()
{
	char empty = '.'; char immov = '-';

	int w, h;
	cout << "������ �������� ���� = ";
	cin >> w;
	cout << "������ �������� ���� = ";
	cin >> h;

	string start;
	char c;
	cout << "������� ����. ������ ����� ����������� ������� ���������. ����� (.) ���������� ������ ����� �� ����, ����� (-) � ����������� ����. ";
	cout << "��� ����������� ������, ���������� ����� ����� ������, ���������� �� ���� �������������� ������ �� ���� ���� � ��� �� ������." << '\n';
	for (int i = 0; i < w * h; i++) {
		cin >> c;
		start += c;
	}
	map <char, int> info = getFieldInfo(start);

	char win_block;
	cout << "���� ����. ����, ������� ����� ������� �� �����������." << '\n';
	cin >> win_block;

	int* win_pos = new int[info[win_block]];
	int buf = 0;
	cout << "�����, � ������� ������ ������� ����, ����� ��� ����� ���� ������� (���� ����� x, y - ���������� ��������������� ����� ����). ";
	cout << "����� ������� ���� �������� ���� � �����(0, 0)." << '\n';
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