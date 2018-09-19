#include<list>
#include<iostream>
#include<ctime>
#include<functional>
#include<vector>
#include<algorithm>
#include<queue>
#include<iterator>
#include<string>

using namespace std;


class Board
{
public:
	int zeroX, zeroY;

	vector<int> board;
	Board* prev;

	//Сколько чисел стоят не на своих позициях
	int h = 0;
	//Сколько шагов было сделано для получения текущей позиции
	int steps = 0;
	//Manhattan distance
	int manh = 0;

	Board()
	{
		h = 0;
		board.resize(16);
		zeroX = 3;
		zeroY = 3;

		for (int i = 0; i < board.size()-1; ++i)
			board[i] = i + 1;
		board[board.size() - 1] = 0;
	}

	Board(vector<int> board)
	{
		this->board.resize(16);
		h = 0;
		for (int i = 0; i < board.size(); ++i)
		{
			this->board[i] = board[i];
			if (board[i] == 0)
			{
				zeroX = i % 4;
				zeroY = i / 4;
			}
			else
			{
				if (board[i] != i + 1)
				{
					++h;
					manh += manh_dist(i, board[i] - 1);
				}
			}
		}
	}

	bool have_solution()
	{
		int sum = 0;

		for (auto it = board.begin(); it != std::prev(board.end(), 1); ++it)
		{
			sum += count_if(next(it, 1), board.end(),
				[it](int i) {return i !=0 && *it > i; });
		}

		return (sum % 2 == 0);
	}

	int weightH()
	{
		return steps + h;
	}

	int weightManh()
	{
		return steps + manh;
	}

	int weightManhH()
	{
		return steps + manh + h;
	}

	void printBoard()
	{
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
				cout << board[y * 4 + x] << "	";
			cout << endl;
		}
	}

	void printZeroPos()
	{
		cout << "x = " << zeroX << " | y = " << zeroY << endl;
	}

	void printH()
	{
		cout << "h = " << h << endl;
	}

	void printSteps()
	{
		cout << "steps = " << steps << endl;
	}

	void printManh()
	{
		cout << "manh = " << manh << endl;
	}

	void printWeightH()
	{
		cout << "weight = " << weightH() << endl;
	}

	void printWeightManh()
	{
		cout << "weightManh = " << weightManh() << endl;
	}

	void printWeightManhH()
	{
		cout << "weightManhH = " << weightManhH() << endl;
	}

	

	void printInfo()
	{
		printZeroPos();
		printH();
		printSteps();
		printManh();
		printWeightH();
		printWeightManh();
		printWeightManhH();
		cout << endl;
	}

	int zeroPos()
	{
		return zeroY * 4 + zeroX;
	}

	int manh_dist(int ind1, int ind2)
	{
		int m = abs(ind1 / 4 - ind2 / 4);
		m += abs(ind1 % 4 - ind2 % 4);
		return m;
	}

	bool zeroUp()
	{
		if (zeroY != 0)
		{
			int ind = zeroPos() - 4;
			manh -= manh_dist(ind, board[ind] - 1);

			swap(board[zeroPos()], board[ind]);			
			if (board[zeroPos()] == (zeroPos() + 1))
				--h;
			else
				++h;
			zeroY -= 1;

			int ind2 = zeroPos() + 4;
			manh += manh_dist(ind2, board[ind2] - 1);

			return true;
		}
		return false;
	}

	bool zeroDown()
	{
		if (zeroY != 3)
		{
			int ind = zeroPos() + 4;
			manh -= manh_dist(ind, board[ind] - 1);

			swap(board[zeroPos()], board[ind]);
			if (board[zeroPos()] == (zeroPos() + 1))
				--h;
			else
				++h;
			zeroY += 1;

			int ind2 = zeroPos() - 4;
			manh += manh_dist(ind2, board[ind2] - 1);

			return true;
		}
		return false;
	}

	bool zeroLeft()
	{
		if (zeroX != 0)
		{
			int ind = zeroPos() - 1;
			manh -= manh_dist(ind, board[ind] - 1);

			swap(board[zeroPos()], board[ind]);
			if (board[zeroPos()] == (zeroPos() + 1))
				--h;
			else
				++h;
			zeroX -= 1;

			int ind2 = zeroPos() + 1;
			manh += manh_dist(ind2, board[ind2] - 1);

			return true;
		}
		return false;
	}

	bool zeroRight()
	{
		if (zeroX != 3)
		{
			int ind = zeroPos() + 1;
			manh -= manh_dist(ind, board[ind] - 1);

			swap(board[zeroPos()], board[ind]);
			if (board[zeroPos()] == (zeroPos() + 1))
				--h;
			else
				++h;
			zeroX += 1;

			int ind2 = zeroPos() - 1;
			manh += manh_dist(ind2, board[ind2] - 1);

			return true;
		}
		return false;
	}

	bool moveByInt(int i)
	{
		if (i == 0)
		{
			if (zeroUp())
				return true;
			return false;
		}
		if (i == 1)
		{
			if (zeroLeft())
				return true;
			return false;
		}
		if (i == 2)
		{
			if (zeroDown())
				return true;
			return false;
		}
		if (i == 3)
		{
			if (zeroRight())
				return true;
			return false;
		}
		return false;
	}
};

bool operator == (const Board& b1, const Board& b2)
{
	for (int i = 0; i < b1.board.size(); ++i)
		if (b1.board[i] != b2.board[i])
			return false;
	return true;
}

Board* createTask(int cnt_steps)
{
	Board* b = new Board();
	int k = 0;

	while(true)
	{
		if (cnt_steps == k)
			break;

		int r = rand() % 4;
		for (int i = 0; i < 4; ++i)
		{
			if (b->moveByInt((r + i) % 4))
			{
				++k;
				break;
			}
		}
	}
	return b;
}

Board* last_b;
size_t list_len;

void simpleSolution(Board* b)
{
	list<Board*> ln;
	ln.push_back(b);

	bool founded = false;

	while (true)
	{
		if (founded)
			break;

		Board* pb = ln.front();
		ln.pop_front();

		if (pb->h == 0)
		{
			last_b = pb;
			list_len = ln.size();
			break;
		}

		for (int i = 0; i < 4; ++i)
		{
			Board* b2 = new Board(pb->board);
			b2->moveByInt(i);
			b2->prev = pb;

			if (b2->h == 0)
			{
				last_b = b2;
				list_len = ln.size();
				founded = true;
				break;
			}

			ln.push_back(b2);
		}
	}
}

void aStarSolution(Board* b, function<bool(Board*, Board*)> compBoards)
{
	b->steps = 0;

	priority_queue<Board*, vector<Board*>, std::function<bool(Board*, Board*)>> pq(compBoards);
	pq.push(b);

	bool founded = false;

	while (!pq.empty())
	{
		if (founded)
			break;

		Board* pb = pq.top();
		pq.pop();

		if (pb->h == 0)
		{
			last_b = pb;
			list_len = pq.size();
			break;
		}

		for (int i = 0; i < 4; ++i)
		{
			Board* b2 = new Board(pb->board);
			b2->moveByInt(i);
			b2->prev = pb;
			b2->steps = pb->steps + 1;

			if (b2->h == 0)
			{
				last_b = b2;
				list_len = pq.size();
				founded = true;
				break;
			}

			pq.push(b2);
		}
	}
	if (pq.empty() && !founded)
		cout << endl << "-----------	error	----------" << endl << endl;
}

void idaStarSolution(Board* b2, int max_depth, function<bool(Board*, Board*)> cmpBoards)
{
	b2->steps = 0;
	bool founded = false;

	for (int depth = 2; depth < 50; ++depth)
	{
		if (founded)
			break;
		//cout << "curr max depth = " << depth << endl;

		Board* b = new Board(b2->board);
		priority_queue<Board*, vector<Board*>, std::function<bool(Board*, Board*)>> pq(cmpBoards);
		pq.push(b);

		

		while (!pq.empty())
		{
			if (founded)
				break;

			Board* pb = pq.top();
			pq.pop();

			if (pb->h == 0)
			{
				last_b = pb;
				list_len = pq.size();
				break;
			}

			for (int i = 0; i < 4; ++i)
			{
				Board* b2 = new Board(pb->board);
				b2->moveByInt(i);
				b2->prev = pb;
				b2->steps = pb->steps + 1;

				if (b2->h == 0)
				{
					last_b = b2;
					list_len = pq.size();
					founded = true;
					break;
				}

				if (b2->steps < depth)
					pq.push(b2);
			}
		}
		//if (pq.empty() && !founded)
			//cout << endl << "	not founded on this step" << endl << endl;
	}
}

void printSolution()
{
	list<Board*> ln;

	while (last_b != nullptr)
	{
		ln.push_front(last_b);
		last_b = last_b->prev;
	}

	for (Board* b : ln)
	{
		b->printInfo();
		b->printBoard();
	}
	cout << "			---------------------			" << endl;
}

void printLen()
{
	cout << "	list length = " << list_len << endl;
}

Board* createTaskTime(int complexity)
{
	srand(time(0));

	unsigned int start_time = clock();
	Board* b = createTask(complexity);
	unsigned int end_time = clock(); // конечное время

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time for create task =	" << search_time << endl << endl;

	return b;
}

void simpleSolutionTime(Board* b, bool print_solution)
{
	unsigned int start_time = clock();
	simpleSolution(b);
	unsigned int end_time = clock();

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time for simple solving =	" << search_time << endl;

	printLen();
	if (print_solution)
		printSolution();
}

void aStarSolutionTime(Board* b, function<bool(Board*, Board*)> cmpBoards, bool print_solution)
{
	unsigned int start_time = clock();
	aStarSolution(b, cmpBoards);
	unsigned int end_time = clock();

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time for aStar solving =	" << search_time << endl;
	cout << "Solution length = " << last_b->steps << endl;

	printLen();

	if (print_solution)
		printSolution();
}

void idaStarSolutionTime(Board* b, int max_depth, function<bool(Board*, Board*)> cmpBoards, bool print_solution)
{
	unsigned int start_time = clock();
	idaStarSolution(b, max_depth, cmpBoards);
	unsigned int end_time = clock();

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time for idaStar solving =	" << search_time << endl;
	cout << "Solution length = " << last_b->steps << endl;

	printLen();

	if (print_solution)
		printSolution();
}

bool compH(Board* b1, Board* b2)
{
	return b1->weightH() > b2->weightH();
}

bool compManh(Board* b1, Board* b2)
{
	return b1->weightManh() > b2->weightManh();
}

bool compManhH(Board* b1, Board* b2)
{
	return b1->weightManhH() > b2->weightManhH();
}

int main()
{
	
	Board* b = createTaskTime(20);

	vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
	//vector<int> v1{ 10, 6, 4, 13, 8, 11, 3, 9, 1, 5, 14, 12, 0, 2, 15, 7 };

	Board* b2 = new Board(v1);
	
	if (b2->have_solution())
	{
		//simpleSolutionTime(b, true);

		cout << endl << "					-----	Cmp by H" << endl;
		aStarSolutionTime(b, compH, true);
		idaStarSolutionTime(b, 20, compH, true);

		cout << endl << "					-----	Cmp by Manh" << endl;
		aStarSolutionTime(b, compManh, true);
		idaStarSolutionTime(b, 20, compManh, true);

		cout << endl << "					-----	Cmp by ManhH" << endl;
		aStarSolutionTime(b, compManhH, true);
		idaStarSolutionTime(b, 20, compManhH, true);

		b->printBoard();
		b->printInfo();
	}
	else
		cout << "No solution\n";

	system("pause");
}


/*
complexity = 17

time for create task =  0

time for simple solving =       1.591
		list length = 2034671
x = 2 | y = 3
h = 13
steps = 0
manh = 11
weight = 13
weightManh = 11
weightManhH = 24

5       1       2       3
9       6       7       4
13      10      11      8
14      15      0       12
x = 1 | y = 3
h = 10
steps = 0
manh = 10
weight = 10
weightManh = 10
weightManhH = 20

5       1       2       3
9       6       7       4
13      10      11      8
14      0       15      12
x = 0 | y = 3
h = 9
steps = 0
manh = 9
weight = 9
weightManh = 9
weightManhH = 18

5       1       2       3
9       6       7       4
13      10      11      8
0       14      15      12
x = 0 | y = 2
h = 8
steps = 0
manh = 8
weight = 8
weightManh = 8
weightManhH = 16

5       1       2       3
9       6       7       4
0       10      11      8
13      14      15      12
x = 0 | y = 1
h = 7
steps = 0
manh = 7
weight = 7
weightManh = 7
weightManhH = 14

5       1       2       3
0       6       7       4
9       10      11      8
13      14      15      12
x = 0 | y = 0
h = 6
steps = 0
manh = 6
weight = 6
weightManh = 6
weightManhH = 12

0       1       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 0
manh = 5
weight = 5
weightManh = 5
weightManhH = 10

1       0       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 0
manh = 4
weight = 4
weightManh = 4
weightManhH = 8

1       2       0       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 0
h = 3
steps = 0
manh = 3
weight = 3
weightManh = 3
weightManhH = 6

1       2       3       0
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 0
manh = 2
weight = 2
weightManh = 2
weightManhH = 4

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 0
manh = 1
weight = 1
weightManh = 1
weightManhH = 2

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 0
manh = 0
weight = 0
weightManh = 0
weightManhH = 0

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------

				-----   Cmp by H
time for aStar solving =        0.001
Solution length = 11
		list length = 32
x = 2 | y = 3
h = 13
steps = 0
manh = 11
weight = 13
weightManh = 11
weightManhH = 24

5       1       2       3
9       6       7       4
13      10      11      8
14      15      0       12
x = 1 | y = 3
h = 10
steps = 1
manh = 10
weight = 11
weightManh = 11
weightManhH = 21

5       1       2       3
9       6       7       4
13      10      11      8
14      0       15      12
x = 0 | y = 3
h = 9
steps = 2
manh = 9
weight = 11
weightManh = 11
weightManhH = 20

5       1       2       3
9       6       7       4
13      10      11      8
0       14      15      12
x = 0 | y = 2
h = 8
steps = 3
manh = 8
weight = 11
weightManh = 11
weightManhH = 19

5       1       2       3
9       6       7       4
0       10      11      8
13      14      15      12
x = 0 | y = 1
h = 7
steps = 4
manh = 7
weight = 11
weightManh = 11
weightManhH = 18

5       1       2       3
0       6       7       4
9       10      11      8
13      14      15      12
x = 0 | y = 0
h = 6
steps = 5
manh = 6
weight = 11
weightManh = 11
weightManhH = 17

0       1       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 6
manh = 5
weight = 11
weightManh = 11
weightManhH = 16

1       0       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 7
manh = 4
weight = 11
weightManh = 11
weightManhH = 15

1       2       0       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 0
h = 3
steps = 8
manh = 3
weight = 11
weightManh = 11
weightManhH = 14

1       2       3       0
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 9
manh = 2
weight = 11
weightManh = 11
weightManhH = 13

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 10
manh = 1
weight = 11
weightManh = 11
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 11
manh = 0
weight = 11
weightManh = 11
weightManhH = 11

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------
time for idaStar solving =      1.07
Solution length = 11
		list length = 30
x = 2 | y = 3
h = 11
steps = 0
manh = 11
weight = 11
weightManh = 11
weightManhH = 22

5       1       2       3
9       6       7       4
13      10      11      8
14      15      0       12
x = 1 | y = 3
h = 10
steps = 1
manh = 10
weight = 11
weightManh = 11
weightManhH = 21

5       1       2       3
9       6       7       4
13      10      11      8
14      0       15      12
x = 0 | y = 3
h = 9
steps = 2
manh = 9
weight = 11
weightManh = 11
weightManhH = 20

5       1       2       3
9       6       7       4
13      10      11      8
0       14      15      12
x = 0 | y = 2
h = 8
steps = 3
manh = 8
weight = 11
weightManh = 11
weightManhH = 19

5       1       2       3
9       6       7       4
0       10      11      8
13      14      15      12
x = 0 | y = 1
h = 7
steps = 4
manh = 7
weight = 11
weightManh = 11
weightManhH = 18

5       1       2       3
0       6       7       4
9       10      11      8
13      14      15      12
x = 0 | y = 0
h = 6
steps = 5
manh = 6
weight = 11
weightManh = 11
weightManhH = 17

0       1       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 6
manh = 5
weight = 11
weightManh = 11
weightManhH = 16

1       0       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 7
manh = 4
weight = 11
weightManh = 11
weightManhH = 15

1       2       0       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 0
h = 3
steps = 8
manh = 3
weight = 11
weightManh = 11
weightManhH = 14

1       2       3       0
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 9
manh = 2
weight = 11
weightManh = 11
weightManhH = 13

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 10
manh = 1
weight = 11
weightManh = 11
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 11
manh = 0
weight = 11
weightManh = 11
weightManhH = 11

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------

				-----   Cmp by Manh
time for aStar solving =        0
Solution length = 11
		list length = 32
x = 2 | y = 3
h = 13
steps = 0
manh = 11
weight = 13
weightManh = 11
weightManhH = 24

5       1       2       3
9       6       7       4
13      10      11      8
14      15      0       12
x = 1 | y = 3
h = 10
steps = 1
manh = 10
weight = 11
weightManh = 11
weightManhH = 21

5       1       2       3
9       6       7       4
13      10      11      8
14      0       15      12
x = 0 | y = 3
h = 9
steps = 2
manh = 9
weight = 11
weightManh = 11
weightManhH = 20

5       1       2       3
9       6       7       4
13      10      11      8
0       14      15      12
x = 0 | y = 2
h = 8
steps = 3
manh = 8
weight = 11
weightManh = 11
weightManhH = 19

5       1       2       3
9       6       7       4
0       10      11      8
13      14      15      12
x = 0 | y = 1
h = 7
steps = 4
manh = 7
weight = 11
weightManh = 11
weightManhH = 18

5       1       2       3
0       6       7       4
9       10      11      8
13      14      15      12
x = 0 | y = 0
h = 6
steps = 5
manh = 6
weight = 11
weightManh = 11
weightManhH = 17

0       1       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 6
manh = 5
weight = 11
weightManh = 11
weightManhH = 16

1       0       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 7
manh = 4
weight = 11
weightManh = 11
weightManhH = 15

1       2       0       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 0
h = 3
steps = 8
manh = 3
weight = 11
weightManh = 11
weightManhH = 14

1       2       3       0
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 9
manh = 2
weight = 11
weightManh = 11
weightManhH = 13

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 10
manh = 1
weight = 11
weightManh = 11
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 11
manh = 0
weight = 11
weightManh = 11
weightManhH = 11

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------
time for idaStar solving =      1.084
Solution length = 11
		list length = 30
x = 2 | y = 3
h = 11
steps = 0
manh = 11
weight = 11
weightManh = 11
weightManhH = 22

5       1       2       3
9       6       7       4
13      10      11      8
14      15      0       12
x = 1 | y = 3
h = 10
steps = 1
manh = 10
weight = 11
weightManh = 11
weightManhH = 21

5       1       2       3
9       6       7       4
13      10      11      8
14      0       15      12
x = 0 | y = 3
h = 9
steps = 2
manh = 9
weight = 11
weightManh = 11
weightManhH = 20

5       1       2       3
9       6       7       4
13      10      11      8
0       14      15      12
x = 0 | y = 2
h = 8
steps = 3
manh = 8
weight = 11
weightManh = 11
weightManhH = 19

5       1       2       3
9       6       7       4
0       10      11      8
13      14      15      12
x = 0 | y = 1
h = 7
steps = 4
manh = 7
weight = 11
weightManh = 11
weightManhH = 18

5       1       2       3
0       6       7       4
9       10      11      8
13      14      15      12
x = 0 | y = 0
h = 6
steps = 5
manh = 6
weight = 11
weightManh = 11
weightManhH = 17

0       1       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 6
manh = 5
weight = 11
weightManh = 11
weightManhH = 16

1       0       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 7
manh = 4
weight = 11
weightManh = 11
weightManhH = 15

1       2       0       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 0
h = 3
steps = 8
manh = 3
weight = 11
weightManh = 11
weightManhH = 14

1       2       3       0
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 9
manh = 2
weight = 11
weightManh = 11
weightManhH = 13

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 10
manh = 1
weight = 11
weightManh = 11
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 11
manh = 0
weight = 11
weightManh = 11
weightManhH = 11

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------

				-----   Cmp by ManhH
time for aStar solving =        0
Solution length = 11
		list length = 32
x = 2 | y = 3
h = 13
steps = 0
manh = 11
weight = 13
weightManh = 11
weightManhH = 24

5       1       2       3
9       6       7       4
13      10      11      8
14      15      0       12
x = 1 | y = 3
h = 10
steps = 1
manh = 10
weight = 11
weightManh = 11
weightManhH = 21

5       1       2       3
9       6       7       4
13      10      11      8
14      0       15      12
x = 0 | y = 3
h = 9
steps = 2
manh = 9
weight = 11
weightManh = 11
weightManhH = 20

5       1       2       3
9       6       7       4
13      10      11      8
0       14      15      12
x = 0 | y = 2
h = 8
steps = 3
manh = 8
weight = 11
weightManh = 11
weightManhH = 19

5       1       2       3
9       6       7       4
0       10      11      8
13      14      15      12
x = 0 | y = 1
h = 7
steps = 4
manh = 7
weight = 11
weightManh = 11
weightManhH = 18

5       1       2       3
0       6       7       4
9       10      11      8
13      14      15      12
x = 0 | y = 0
h = 6
steps = 5
manh = 6
weight = 11
weightManh = 11
weightManhH = 17

0       1       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 6
manh = 5
weight = 11
weightManh = 11
weightManhH = 16

1       0       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 7
manh = 4
weight = 11
weightManh = 11
weightManhH = 15

1       2       0       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 0
h = 3
steps = 8
manh = 3
weight = 11
weightManh = 11
weightManhH = 14

1       2       3       0
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 9
manh = 2
weight = 11
weightManh = 11
weightManhH = 13

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 10
manh = 1
weight = 11
weightManh = 11
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 11
manh = 0
weight = 11
weightManh = 11
weightManhH = 11

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------
time for idaStar solving =      1.128
Solution length = 11
		list length = 30
x = 2 | y = 3
h = 11
steps = 0
manh = 11
weight = 11
weightManh = 11
weightManhH = 22

5       1       2       3
9       6       7       4
13      10      11      8
14      15      0       12
x = 1 | y = 3
h = 10
steps = 1
manh = 10
weight = 11
weightManh = 11
weightManhH = 21

5       1       2       3
9       6       7       4
13      10      11      8
14      0       15      12
x = 0 | y = 3
h = 9
steps = 2
manh = 9
weight = 11
weightManh = 11
weightManhH = 20

5       1       2       3
9       6       7       4
13      10      11      8
0       14      15      12
x = 0 | y = 2
h = 8
steps = 3
manh = 8
weight = 11
weightManh = 11
weightManhH = 19

5       1       2       3
9       6       7       4
0       10      11      8
13      14      15      12
x = 0 | y = 1
h = 7
steps = 4
manh = 7
weight = 11
weightManh = 11
weightManhH = 18

5       1       2       3
0       6       7       4
9       10      11      8
13      14      15      12
x = 0 | y = 0
h = 6
steps = 5
manh = 6
weight = 11
weightManh = 11
weightManhH = 17

0       1       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 6
manh = 5
weight = 11
weightManh = 11
weightManhH = 16

1       0       2       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 7
manh = 4
weight = 11
weightManh = 11
weightManhH = 15

1       2       0       3
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 0
h = 3
steps = 8
manh = 3
weight = 11
weightManh = 11
weightManhH = 14

1       2       3       0
5       6       7       4
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 9
manh = 2
weight = 11
weightManh = 11
weightManhH = 13

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 10
manh = 1
weight = 11
weightManh = 11
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 11
manh = 0
weight = 11
weightManh = 11
weightManhH = 11

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------
5       1       2       3
9       6       7       4
13      10      11      8
14      15      0       12
x = 2 | y = 3
h = 13
steps = 0
manh = 11
weight = 13
weightManh = 11
weightManhH = 24

*/


//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------


/*
time for create task =  0


										-----   Cmp by H
time for aStar solving =        0.004
Solution length = 12
		list length = 3656
x = 2 | y = 0
h = 12
steps = 0
manh = 12
weight = 12
weightManh = 12
weightManhH = 24

1       6       0       2
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 0
h = 9
steps = 1
manh = 11
weight = 10
weightManh = 12
weightManhH = 21

1       6       2       0
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 1
h = 7
steps = 2
manh = 10
weight = 9
weightManh = 12
weightManhH = 19

1       6       2       4
5       3       11      0
9       10      8       7
13      14      15      12
x = 3 | y = 2
h = 8
steps = 3
manh = 9
weight = 11
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      8       0
13      14      15      12
x = 2 | y = 2
h = 8
steps = 4
manh = 8
weight = 12
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      0       8
13      14      15      12
x = 2 | y = 1
h = 6
steps = 5
manh = 7
weight = 11
weightManh = 12
weightManhH = 18

1       6       2       4
5       3       0       7
9       10      11      8
13      14      15      12
x = 1 | y = 1
h = 7
steps = 6
manh = 6
weight = 13
weightManh = 12
weightManhH = 19

1       6       2       4
5       0       3       7
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 7
manh = 5
weight = 12
weightManh = 12
weightManhH = 17

1       0       2       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 8
manh = 4
weight = 12
weightManh = 12
weightManhH = 16

1       2       0       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 1
h = 3
steps = 9
manh = 3
weight = 12
weightManh = 12
weightManhH = 15

1       2       3       4
5       6       0       7
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 10
manh = 2
weight = 12
weightManh = 12
weightManhH = 14

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 11
manh = 1
weight = 12
weightManh = 12
weightManhH = 13

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 12
manh = 0
weight = 12
weightManh = 12
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------
time for idaStar solving =      3.302
Solution length = 12
		list length = 3654
x = 2 | y = 0
h = 8
steps = 0
manh = 12
weight = 8
weightManh = 12
weightManhH = 20

1       6       0       2
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 0
h = 9
steps = 1
manh = 11
weight = 10
weightManh = 12
weightManhH = 21

1       6       2       0
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 1
h = 7
steps = 2
manh = 10
weight = 9
weightManh = 12
weightManhH = 19

1       6       2       4
5       3       11      0
9       10      8       7
13      14      15      12
x = 3 | y = 2
h = 8
steps = 3
manh = 9
weight = 11
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      8       0
13      14      15      12
x = 2 | y = 2
h = 8
steps = 4
manh = 8
weight = 12
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      0       8
13      14      15      12
x = 2 | y = 1
h = 6
steps = 5
manh = 7
weight = 11
weightManh = 12
weightManhH = 18

1       6       2       4
5       3       0       7
9       10      11      8
13      14      15      12
x = 1 | y = 1
h = 7
steps = 6
manh = 6
weight = 13
weightManh = 12
weightManhH = 19

1       6       2       4
5       0       3       7
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 7
manh = 5
weight = 12
weightManh = 12
weightManhH = 17

1       0       2       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 8
manh = 4
weight = 12
weightManh = 12
weightManhH = 16

1       2       0       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 1
h = 3
steps = 9
manh = 3
weight = 12
weightManh = 12
weightManhH = 15

1       2       3       4
5       6       0       7
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 10
manh = 2
weight = 12
weightManh = 12
weightManhH = 14

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 11
manh = 1
weight = 12
weightManh = 12
weightManhH = 13

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 12
manh = 0
weight = 12
weightManh = 12
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------

										-----   Cmp by Manh
time for aStar solving =        0
Solution length = 12
		list length = 47
x = 2 | y = 0
h = 12
steps = 0
manh = 12
weight = 12
weightManh = 12
weightManhH = 24

1       6       0       2
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 0
h = 9
steps = 1
manh = 11
weight = 10
weightManh = 12
weightManhH = 21

1       6       2       0
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 1
h = 7
steps = 2
manh = 10
weight = 9
weightManh = 12
weightManhH = 19

1       6       2       4
5       3       11      0
9       10      8       7
13      14      15      12
x = 3 | y = 2
h = 8
steps = 3
manh = 9
weight = 11
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      8       0
13      14      15      12
x = 2 | y = 2
h = 8
steps = 4
manh = 8
weight = 12
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      0       8
13      14      15      12
x = 2 | y = 1
h = 6
steps = 5
manh = 7
weight = 11
weightManh = 12
weightManhH = 18

1       6       2       4
5       3       0       7
9       10      11      8
13      14      15      12
x = 1 | y = 1
h = 7
steps = 6
manh = 6
weight = 13
weightManh = 12
weightManhH = 19

1       6       2       4
5       0       3       7
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 7
manh = 5
weight = 12
weightManh = 12
weightManhH = 17

1       0       2       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 8
manh = 4
weight = 12
weightManh = 12
weightManhH = 16

1       2       0       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 1
h = 3
steps = 9
manh = 3
weight = 12
weightManh = 12
weightManhH = 15

1       2       3       4
5       6       0       7
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 10
manh = 2
weight = 12
weightManh = 12
weightManhH = 14

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 11
manh = 1
weight = 12
weightManh = 12
weightManhH = 13

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 12
manh = 0
weight = 12
weightManh = 12
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------
time for idaStar solving =      3.34
Solution length = 12
		list length = 45
x = 2 | y = 0
h = 8
steps = 0
manh = 12
weight = 8
weightManh = 12
weightManhH = 20

1       6       0       2
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 0
h = 9
steps = 1
manh = 11
weight = 10
weightManh = 12
weightManhH = 21

1       6       2       0
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 1
h = 7
steps = 2
manh = 10
weight = 9
weightManh = 12
weightManhH = 19

1       6       2       4
5       3       11      0
9       10      8       7
13      14      15      12
x = 3 | y = 2
h = 8
steps = 3
manh = 9
weight = 11
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      8       0
13      14      15      12
x = 2 | y = 2
h = 8
steps = 4
manh = 8
weight = 12
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      0       8
13      14      15      12
x = 2 | y = 1
h = 6
steps = 5
manh = 7
weight = 11
weightManh = 12
weightManhH = 18

1       6       2       4
5       3       0       7
9       10      11      8
13      14      15      12
x = 1 | y = 1
h = 7
steps = 6
manh = 6
weight = 13
weightManh = 12
weightManhH = 19

1       6       2       4
5       0       3       7
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 7
manh = 5
weight = 12
weightManh = 12
weightManhH = 17

1       0       2       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 8
manh = 4
weight = 12
weightManh = 12
weightManhH = 16

1       2       0       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 1
h = 3
steps = 9
manh = 3
weight = 12
weightManh = 12
weightManhH = 15

1       2       3       4
5       6       0       7
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 10
manh = 2
weight = 12
weightManh = 12
weightManhH = 14

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 11
manh = 1
weight = 12
weightManh = 12
weightManhH = 13

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 12
manh = 0
weight = 12
weightManh = 12
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------

										-----   Cmp by ManhH
time for aStar solving =        0.001
Solution length = 12
		list length = 416
x = 2 | y = 0
h = 12
steps = 0
manh = 12
weight = 12
weightManh = 12
weightManhH = 24

1       6       0       2
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 0
h = 9
steps = 1
manh = 11
weight = 10
weightManh = 12
weightManhH = 21

1       6       2       0
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 1
h = 7
steps = 2
manh = 10
weight = 9
weightManh = 12
weightManhH = 19

1       6       2       4
5       3       11      0
9       10      8       7
13      14      15      12
x = 3 | y = 2
h = 8
steps = 3
manh = 9
weight = 11
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      8       0
13      14      15      12
x = 2 | y = 2
h = 8
steps = 4
manh = 8
weight = 12
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      0       8
13      14      15      12
x = 2 | y = 1
h = 6
steps = 5
manh = 7
weight = 11
weightManh = 12
weightManhH = 18

1       6       2       4
5       3       0       7
9       10      11      8
13      14      15      12
x = 1 | y = 1
h = 7
steps = 6
manh = 6
weight = 13
weightManh = 12
weightManhH = 19

1       6       2       4
5       0       3       7
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 7
manh = 5
weight = 12
weightManh = 12
weightManhH = 17

1       0       2       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 8
manh = 4
weight = 12
weightManh = 12
weightManhH = 16

1       2       0       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 1
h = 3
steps = 9
manh = 3
weight = 12
weightManh = 12
weightManhH = 15

1       2       3       4
5       6       0       7
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 10
manh = 2
weight = 12
weightManh = 12
weightManhH = 14

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 11
manh = 1
weight = 12
weightManh = 12
weightManhH = 13

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 12
manh = 0
weight = 12
weightManh = 12
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------
time for idaStar solving =      3.798
Solution length = 12
		list length = 130
x = 2 | y = 0
h = 8
steps = 0
manh = 12
weight = 8
weightManh = 12
weightManhH = 20

1       6       0       2
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 0
h = 9
steps = 1
manh = 11
weight = 10
weightManh = 12
weightManhH = 21

1       6       2       0
5       3       11      4
9       10      8       7
13      14      15      12
x = 3 | y = 1
h = 7
steps = 2
manh = 10
weight = 9
weightManh = 12
weightManhH = 19

1       6       2       4
5       3       11      0
9       10      8       7
13      14      15      12
x = 3 | y = 2
h = 8
steps = 3
manh = 9
weight = 11
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      8       0
13      14      15      12
x = 2 | y = 2
h = 8
steps = 4
manh = 8
weight = 12
weightManh = 12
weightManhH = 20

1       6       2       4
5       3       11      7
9       10      0       8
13      14      15      12
x = 2 | y = 1
h = 6
steps = 5
manh = 7
weight = 11
weightManh = 12
weightManhH = 18

1       6       2       4
5       3       0       7
9       10      11      8
13      14      15      12
x = 1 | y = 1
h = 7
steps = 6
manh = 6
weight = 13
weightManh = 12
weightManhH = 19

1       6       2       4
5       0       3       7
9       10      11      8
13      14      15      12
x = 1 | y = 0
h = 5
steps = 7
manh = 5
weight = 12
weightManh = 12
weightManhH = 17

1       0       2       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 0
h = 4
steps = 8
manh = 4
weight = 12
weightManh = 12
weightManhH = 16

1       2       0       4
5       6       3       7
9       10      11      8
13      14      15      12
x = 2 | y = 1
h = 3
steps = 9
manh = 3
weight = 12
weightManh = 12
weightManhH = 15

1       2       3       4
5       6       0       7
9       10      11      8
13      14      15      12
x = 3 | y = 1
h = 2
steps = 10
manh = 2
weight = 12
weightManh = 12
weightManhH = 14

1       2       3       4
5       6       7       0
9       10      11      8
13      14      15      12
x = 3 | y = 2
h = 1
steps = 11
manh = 1
weight = 12
weightManh = 12
weightManhH = 13

1       2       3       4
5       6       7       8
9       10      11      0
13      14      15      12
x = 3 | y = 3
h = 0
steps = 12
manh = 0
weight = 12
weightManh = 12
weightManhH = 12

1       2       3       4
5       6       7       8
9       10      11      12
13      14      15      0
						---------------------
1       6       0       2
5       3       11      4
9       10      8       7
13      14      15      12
x = 2 | y = 0
h = 12
steps = 0
manh = 12
weight = 12
weightManh = 12
weightManhH = 24

Для продолжения нажмите любую клавишу . . .
*/