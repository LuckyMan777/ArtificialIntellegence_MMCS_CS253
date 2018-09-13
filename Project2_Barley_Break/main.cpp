#include<list>
#include<iostream>
#include<ctime>
#include<functional>
#include<vector>
#include<algorithm>
#include<queue>

using namespace std;


class Board
{
public:
	int zeroX, zeroY;

	vector<int> board;
	Board* prev;

	//Сколько чисел стоят не на своих позициях
	int h;
	//Сколько шагов было сделано для получения текущей позиции
	int steps;

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
					++h;
			}
		}
	}

	int weight()
	{
		return steps + h;
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

	void printWeight()
	{
		cout << "weight = " << weight() << endl;
	}

	void printAll()
	{
		printBoard();
		printZeroPos();
		printH();
		printWeight();
		cout << endl;
	}

	int zeroPos()
	{
		return zeroY * 4 + zeroX;
	}

	bool zeroUp()
	{
		if (zeroY != 0)
		{
			swap(board[zeroPos()], board[zeroPos()-4]);
			
			if (board[zeroPos()] == (zeroPos() + 1))
				--h;
			else
				++h;

			zeroY -= 1;
			return true;
		}
		return false;
	}

	bool zeroDown()
	{
		if (zeroY != 3)
		{
			swap(board[zeroPos()], board[zeroPos() + 4]);

			if (board[zeroPos()] == (zeroPos() + 1))
				--h;
			else
				++h;

			zeroY += 1;
			return true;
		}
		return false;
	}

	bool zeroLeft()
	{
		if (zeroX != 0)
		{
			swap(board[zeroPos()], board[zeroPos() - 1]);

			if (board[zeroPos()] == (zeroPos() + 1))
				--h;
			else
				++h;

			zeroX -= 1;
			return true;
		}
		return false;
	}

	bool zeroRight()
	{
		if (zeroX != 3)
		{
			swap(board[zeroPos()], board[zeroPos() + 1]);

			if (board[zeroPos()] == (zeroPos() + 1))
				--h;
			else
				++h;

			zeroX += 1;
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

bool comp(Board* b1, Board* b2)
{
	return b1->weight() > b2->weight();
}


void aStarSolution(Board* b)
{
	b->steps = 0;

	priority_queue<Board*, vector<Board*>, std::function<bool(Board*, Board*)>> pq(comp);
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

void printSolution()
{
	list<Board*> ln;

	while (last_b != nullptr)
	{
		ln.push_front(last_b);
		last_b = last_b->prev;
	}

	cout << "			weight = " << ln.front()->weight() << endl << endl;
	for (Board* b : ln)
		b->printAll();
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

void aStarSolutionTime(Board* b, bool print_solution)
{
	unsigned int start_time = clock();
	aStarSolution(b);
	unsigned int end_time = clock();

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time for aStar solving =	" << search_time << endl;

	printLen();

	if (print_solution)
		printSolution();
}

int main()
{
	
	Board* b = createTaskTime(10);

	vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
	Board* b2 = new Board(v1);

	simpleSolutionTime(b, true);

	aStarSolutionTime(b, true);

	system("pause");
}