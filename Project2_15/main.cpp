#include<list>
#include<iostream>
#include<ctime>
#include<functional>
#include<vector>
#include<algorithm>
#include<queue>
#include<iterator>
#include<string>
#include<set>
#include<unordered_set>

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
				[it](int i) {return i != 0 && *it > i; });
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
	set<vector<int>> used;
	Board* b = new Board();
	int k = 0;
	used.insert(b->board);

	while(true)
	{
		if (cnt_steps == k)
			break;

		int r = rand() % 4;
		for (int i = 0; i < 4; ++i)
		{
			Board* b2 = new Board(b->board);
			if (b2->moveByInt((r + i) % 4))
			{
				auto it = used.find(b2->board);
				if (it == used.end())
				{
					delete b;
					b = b2;
					used.insert(b->board);
					++k;
					break;
				}
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
	set<vector<int>> used;
	ln.push_back(b);
	used.insert(b->board);

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

			auto it = used.find(b2->board);
			if (it == used.end())
			{
				ln.push_back(b2);
				used.insert(b2->board);
			}
		}
	}
}

void aStarSolution(Board* b, function<bool(Board*, Board*)> compBoards)
{
	b->steps = 0;

	priority_queue<Board*, vector<Board*>, std::function<bool(Board*, Board*)>> pq(compBoards);
	pq.push(b);
	set<vector<int>> used;
	used.insert(b->board);

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

			auto it = used.find(b2->board);
			if (it == used.end())
			{
				pq.push(b2);
				used.insert(b2->board);
			}
		}
	}
	if (pq.empty() && !founded)
		cout << endl << "-----------	error	----------" << endl << endl;
}

void idaStarSolution(Board* b2, int max_bound, function<bool(Board*, Board*)> cmpBoards, function<int(Board*)> weight)
{
	b2->steps = 0;
	bool founded = false;

	//cout << "curr max depth = " << depth << endl;

	Board* b = new Board(b2->board);
	priority_queue<Board*, vector<Board*>, std::function<bool(Board*, Board*)>> pq(cmpBoards);
	pq.push(b);
	set<vector<int>> used;
	used.insert(b->board);

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

			auto it = used.find(b2->board);
			if (it == used.end() && weight(b2) < max_bound)
			{
				pq.push(b2);
				used.insert(b2->board);
			}
		}
	}
			//if (pq.empty() && !founded)
			//cout << endl << "	not founded on this step" << endl << endl;
	
}

void printSolution(bool print_board, bool print_info)
{
	list<Board*> ln;

	while (last_b != nullptr)
	{
		ln.push_front(last_b);
		last_b = last_b->prev;
	}

	for (Board* b : ln)
	{
		if (print_info)
			b->printInfo();
		if (print_board)
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

void simpleSolutionTime(Board* b, bool print_boards, bool print_info)
{
	unsigned int start_time = clock();
	simpleSolution(b);
	unsigned int end_time = clock();

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time for simple solving =	" << search_time << endl;

	//printLen();
	printSolution(print_boards, print_info);
}

void aStarSolutionTime(Board* b, function<bool(Board*, Board*)> cmpBoards, bool print_boards, bool print_info)
{
	unsigned int start_time = clock();
	aStarSolution(b, cmpBoards);
	unsigned int end_time = clock();

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time for aStar solving =	" << search_time << endl;
	cout << "Solution length = " << last_b->steps << endl;

	//printLen();

	printSolution(print_boards, print_info);
}

void idaStarSolutionTime(Board* b, int max_bound, function<bool(Board*, Board*)> cmpBoards, function<int(Board*)> weight, 
	bool print_boards, bool print_info)
{
	unsigned int start_time = clock();
	idaStarSolution(b, max_bound, cmpBoards, weight);
	unsigned int end_time = clock();

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time for idaStar solving =	" << search_time << endl;
	if (last_b == nullptr)
		cout << "No solution for bound = " << max_bound << endl;
	else
		cout << "Solution length = " << last_b->steps << endl;

	//printLen();

	printSolution(print_boards, print_info);
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

void runCmpBy(Board* b, function<bool(Board*, Board*)> cmpBoards, function<int(Board*)> weight, 
	int max_bound, bool print_boards, bool print_info)
{
	aStarSolutionTime(b, cmpBoards, print_boards, print_info);
	idaStarSolutionTime(b, max_bound, cmpBoards, weight, print_boards, print_info);
}

int main()
{
	int max_bound = 70;
	int complexity = 55;

	Board* b1 = createTaskTime(complexity);

	//vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
	//vector<int> v1{ 5, 1, 7, 4, 0, 13, 6, 12, 9, 3, 10, 2, 14, 8, 11, 15 };
	//vector<int> v1{ 6, 7, 3, 10, 5, 1, 8, 2, 9, 15, 12, 4, 13, 11, 14, 0 };	//	Manh 38 steps, 1.8 seconds
	vector<int> v1{ 13, 8, 9, 2, 0, 7, 1, 12, 4, 6, 5, 3, 14, 11, 15, 10 };		//	Manh 45 steps, 1 second

	Board* b2 = new Board(v1);

	Board* b = b2;
	
	if (b->have_solution())
	{
		//simpleSolutionTime(b, false, false);

		//cout << endl << "					-----	Cmp by H" << endl;
		//runCmpBy(b, compH, [](Board* b) {return b->weightH(); }, max_bound, false, false);
		
		cout << endl << "					-----	Cmp by Manh" << endl;
		runCmpBy(b, compManh, [](Board* b) {return b->weightManh(); }, max_bound, false, false);
		
		//cout << endl << "					-----	Cmp by ManhH" << endl;
		//runCmpBy(b, compManhH, [](Board* b) {return b->weightManhH(); }, max_bound, false, false);


		b->printBoard();
		b->printInfo();
	}
	else
		cout << "No solution\n";

	system("pause");
}


/*



*/