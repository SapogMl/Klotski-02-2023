#pragma once

#include<map>
#include<set>
#include<string>
#include<unordered_set>
#include <vector>

using namespace std;

// обработка входных данных

int* findBlock(string field, char b, int size);

bool isDeltaConst(int* c1, int* c2, int size);

map <char, char> getEq(string field, int w, map <char, int> info, int block_count, char exclude[3]);

map <char, int> getFieldInfo(string field);

bool isOutOfBounds(int w, int h, int x, int y);

// ходы

string moveBlockByXY(string field, char block, int dx, int dy, map <char, int> info, int w, int h, char empty);

set <string> moveBlockOnce(string field, char block, map<char, int> info, int w, int h, char empty);

set <string> moveBlockAround(string field, char block, map<char, int> info, int w, int h, char empty);

set <string> moveAllBlocks(string field, char empty, char immov, map <char, int> info, int w, int h);

// поиск в ширину

bool isSolved(string field, char win_block, int size, int* win_pos);

string fieldToType(string field, map<char, char> eq_list);

map<string, int> makeSearchTree(string field, int w, int h, char empty, char immov, map<char, int> info, char win_block, int* win_pos, map<char, char> eq_list);

// выделение решения из поискового дерева

map <string, int> clearTreeDepth(map <string, int> tree, string s);

vector <string> solutionFromTree(map <string, int> tree, string finish, string start, map<char, int> info, map<char, char> eq_list, char empty, char immov, int w, int h);

vector <string> getSolution(string start, char win_block, int* win_pos, char empty, char immov, int w, int h, map <char, int> info, map<char, char> eq_list);

// вывод решения

void printSolution(vector <string> s, int w, int h);