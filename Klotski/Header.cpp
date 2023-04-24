#include"Header.h"

#include<iostream>

int* findBlock(string field, char b, int size)
{
	int* pos = new int[size];
	int i = 0;
	int count = 0;
	while ((i < field.length()) && (count < size)) {
		if (field[i] == b) {
			pos[count] = i;
			count++;
		}
		i++;
	}
	return pos;
}

bool isDeltaConst(int* c1, int* c2, int size)
{
	int d = c1[0] - c2[0];
	for (int i = 1; i < size; i++) {
		if ((c1[i] - c2[i]) != d) {
			return false;
		}
	}
	return true;
}

map <char, char> getEq(string field, int w, map <char, int> info, int block_count, char exclude[3])
{
	map <char, int> ::iterator it = info.begin();
	map <char, char> eq_list;
	for (; it != info.end(); it++) {
		eq_list[it->first] = it->first;
	}
	for (int i = 0; i < 3; i++) {
		eq_list.erase(exclude[i]);
	}
	map<char, char> ::iterator it1 = eq_list.begin();
	map<char, char> ::iterator it2 = eq_list.begin();
	int* c1; int* c2;
	for (; it1 != eq_list.end(); it1++) {
		if (it1->first == it1->second) {
			c1 = findBlock(field, it1->first, info[it1->first]);
			it2 = it1;
			it2++;
			for (; it2 != eq_list.end(); it2++) {
				if ((it2->first == it2->second) && (info[it2->second] == info[it1->second])) {
					c2 = findBlock(field, it2->first, info[it2->first]);
					if (isDeltaConst(c1, c2, info[it1->first])) {
						it2->second = it1->first;
					}
					delete c2; // C6279
				}
			}
			delete c1;
		}
	}
	it = info.begin();
	for (; it != info.end(); it++) {
		if (!eq_list.count(it->first)) {
			eq_list[it->first] = it->first;
		}
	}
	return eq_list;
}

map <char, int> getFieldInfo(string field)
{
	map <char, int> info;
	for (int i = 0; i < field.length(); i++) {
		info[field[i]]++;
	}
	return info;
}

bool isOutOfBounds(int w, int h, int x, int y)
{
	return !((-1 < x) && (x < w) && (-1 < y) && (y < h));
}

string moveBlockByXY(string field, char block, int dx, int dy, map <char, int> info, int w, int h, char empty)
{
	string field_copy = field;
	int* old_pos = findBlock(field, block, info[block]);
	int* new_pos = new int[info[block]];
	int x = 0;
	int y = 0;
	bool OUB = 0;
	for (int i = 0; (i < info[block]) && !OUB; i++) {
		x = old_pos[i] % w + dx;
		y = old_pos[i] / w + dy;
		new_pos[i] = x + w * y;
		OUB = isOutOfBounds(w, h, x, y);
	}
	if (!OUB) {
		for (int i = 0; i < info[block]; i++) {
			field[old_pos[i]] = empty;
		}
		bool moveOK = 1;
		for (int i = 0; (i < info[block]) && moveOK; i++) {
			moveOK = (field[new_pos[i]] == empty);
			field[new_pos[i]] = block;
		}
		if (moveOK) {
			return field;
		}
	}
	return field_copy;
}

set <string> moveBlockOnce(string field, char block, map<char, int> info, int w, int h, char empty)
{
	set <string> result;
	int dir[8] = { 1, 0, -1, 0, 0, 1, 0, -1 };
	for (int i = 0; i < 4; i++) {
		result.insert(moveBlockByXY(field, block, -dir[2 * i], -dir[2 * i + 1], info, w, h, empty));
	}
	return result;
}

set <string> moveBlockAround(string field, char block, map<char, int> info, int w, int h, char empty)
{
	map <string, bool> map_result;
	map_result[field] = 0;
	map <string, bool> ::iterator map_it = map_result.begin();
	set <string> add_to_map;
	set <string> ::iterator set_it = add_to_map.begin();
	while (map_it != map_result.end()) {
		add_to_map = moveBlockOnce(map_it->first, block, info, w, h, empty);
		for (set_it = add_to_map.begin(); set_it != add_to_map.end(); set_it++) {
			if (map_result.count(*set_it) == 0) {
				map_result[*set_it] = 0;
			}
		}
		map_result[map_it->first] = 1;

		for (map_it = map_result.begin(); (map_it != map_result.end()) && (map_it->second != 0); map_it++) {}
		if (map_it == map_result.end()) {
			for (map_it = map_result.begin(); (map_it != map_result.end()) && (map_it->second != 0); map_it++) {}
		}
	}
	map_result.erase(field);
	set<string> set_result;
	for (map_it = map_result.begin(); map_it != map_result.end(); map_it++) {
		set_result.insert(map_it->first);
	}
	return set_result;
}

set <string> moveAllBlocks(string field, char empty, char immov, map <char, int> info, int w, int h)
{
	set <string> result;
	set <char> block_list;
	int dir[8] = { 1, 0, -1, 0, 0, 1, 0, -1 };
	int* empty_pos = findBlock(field, empty, info[empty]);
	int x = 0; int y = 0;

	for (int i = 0; i < info[empty]; i++) {
		for (int j = 0; j < 4; j++) {
			x = dir[2 * j] + (empty_pos[i] % w);
			y = dir[2 * j + 1] + (empty_pos[i] / w);
			if (!isOutOfBounds(w, h, x, y)) {
				if ((field[x + (w * y)] != empty) && (field[x + (w * y)] != immov)) {
					block_list.insert(field[x + (w * y)]);
				}
			}
		}
	}
	set <char> ::iterator it1;
	set <string> new_fields;
	set <string> ::iterator it2;
	for (it1 = block_list.begin(); it1 != block_list.end(); it1++) {
		new_fields = moveBlockAround(field, *it1, info, w, h, empty);
		for (it2 = new_fields.begin(); it2 != new_fields.end(); it2++) {
			result.insert(*it2);
		}
	}
	return result;
}

bool isSolved(string field, char win_block, int size, int* win_pos)
{
	for (int i = 0; i < size; i++) {
		if (field[win_pos[i]] != win_block) {
			return false;
		}
	}
	return true;
}

string fieldToType(string field, map<char, char> eq_list)
{
	for (int i = 0; i < field.length(); i++) {
		field[i] = eq_list[field[i]];
	}
	return field;
}

map<string, int> makeSearchTree(string field, int w, int h, char empty, char immov, map<char, int> info, char win_block, int* win_pos, map<char, char> eq_list)
{
	map<string, int> tree;
	int depth = 1;
	tree[field] = depth;
	map<string, int> ::iterator map_it = tree.begin();

	unordered_set <string> fieldtype_list;
	fieldtype_list.insert(fieldToType(field, eq_list));

	bool win = isSolved(field, win_block, info[win_block], win_pos);
	if (map_it != tree.end()) {
		printf("\rГлубина поиска: %d", depth);
		if (!win) {
			printf("\rГлубина поиска: %d", depth + 1);
		}
	}

	set <string> new_fields;
	set<string>::iterator set_it = new_fields.begin();
	string fieldtype = "";
	while (!win && (map_it != tree.end())) {
		new_fields = moveAllBlocks(map_it->first, empty, immov, info, w, h);
		for (set_it = new_fields.begin(); set_it != new_fields.end(); set_it++) {
			fieldtype = fieldToType(*set_it, eq_list);
			if (fieldtype_list.count(fieldtype) == 0) {
				fieldtype_list.insert(fieldtype);
				tree[*set_it] = depth + 1;
				win = (win || isSolved(*set_it, win_block, info[win_block], win_pos));
			}
		}
		tree[map_it->first] *= -1;
		for (map_it = tree.begin(); (map_it != tree.end()) && (map_it->second != depth); map_it++) {}
		if (map_it == tree.end()) {
			depth++;

			printf("\rГлубина поиска: %d", depth + 1);

			for (map_it = tree.begin(); (map_it != tree.end()) && (map_it->second != depth); map_it++) {}
		}
	}

	if (!win) {
		tree.erase(tree.begin(), tree.end());
	}
	else {
		for (map_it = tree.begin(); map_it != tree.end(); map_it++) {
			if (map_it->second < 0) {
				map_it->second *= -1;
			}
		}
	}
	return tree;
}

map <string, int> clearTreeDepth(map <string, int> tree, string s)
{
	map <string, int> ::iterator it = tree.begin();
	while (it != tree.end()) {
		if ((it->first != s) && (it->second == tree[s])) {
			tree.erase(it);
			it = tree.begin();
		}
		else {
			it++;
		}
	}
	return tree;
}

vector <string> solutionFromTree(map <string, int> tree, string finish, string start, map<char, int> info, map<char, char>eq_list, char empty, char immov, int w, int h)
{
	map<string, int> ::iterator map_it = tree.begin();
	vector <string> result;

	string field = finish;
	int depth = tree[finish];

	set <string> buf;
	set <string> ::iterator set_it = buf.begin();

	cout << '\n';

	while (result.size() < depth) {
		result.insert(result.begin(), field);
		if (field != start) {

			printf("\rПодготовка решения - ход %d/%d", depth - tree[field] + 1, depth - 1);

			buf = moveAllBlocks(field, empty, immov, info, w, h);
			for (set_it = buf.begin(); (tree.count(*set_it) == 0) || (tree[*set_it] >= tree[field]); set_it++) {}
			field = *set_it;
		}
	}
	return result;
}

vector <string> getSolution(string start, char win_block, int* win_pos, char empty, char immov, int w, int h, map <char, int> info, map<char, char> eq_list)
{
	map<string, int> tree = makeSearchTree(start, w, h, empty, immov, info, win_block, win_pos, eq_list);
	map<string, int> ::iterator map_it = tree.begin();
	vector <string> result;

	if (map_it != tree.end()) {
		while (!isSolved(map_it->first, win_block, info[win_block], win_pos)) {
			map_it++;
		}

		cout << '\n';
		cout << "Найдено кратчайшее решение.";

		result = solutionFromTree(tree, map_it->first, start, info, eq_list, empty, immov, w, h);
	}
	return result;
}

void printSolution(vector <string> s, int w, int h)
{
	if (s.size() == 0) {
		cout << '\n';
		cout << "Решений этой головоломки не существует.";
	}
	else {
		cout << '\n';
		vector <string> ::iterator it = s.begin();
		int k = 0;
		for (; it != s.end(); it++) {
			if (k != 0) {
				printf("%d ход\n", k);
			}
			for (int i = 0; i < h; i++) {
				cout << (*it).substr(i * w, w) << '\n';
			}
			k++;
			cout << '\n';
		}
		cout << "Конец решения.";
		cout << '\n';
	}
}
