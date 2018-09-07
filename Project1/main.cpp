#include<list>
#include<iostream>
#include<ctime>
#include<functional>
#include<vector>
#include<algorithm>

using namespace std;


int a = 2;
int b = 500;

class node
{
public:
	int num;
	node* pred;

	node(int num)
	{
		this->num = num;
		pred = nullptr;
	}

	node(int num, node* pred)
	{
		this->num = num;
		this->pred = pred;
	}
};

bool operator == (const node& n1, const node& n2)
{
	return n1.num == n2.num;
}

bool operator < (const node& n1, const node& n2)
{
	return n1.num < n2.num;
}

void print_path(node* leaf)
{
	list<int> nums;
	node* n = leaf;
	nums.push_front(n->num);
	
	while (n->pred != nullptr)
	{
		n = n->pred;
		nums.push_front(n->num);
	}

	for (auto it = nums.begin(); it != nums.end(); ++it)
	{
		std::cout << *it << " ";
	}
	cout << endl << nums.size() << endl;
}

node* last_leaf = nullptr;
size_t len_queue;

void min_path(int a, int b, vector<function<int (int)>> functions)
{
	list<node*> ln;
	node* n1 = new node(a);
	ln.push_back(n1);

	while (true)
	{
		node* curr_n = ln.front();
		ln.pop_front();
		node* pred = curr_n;
		if (curr_n->num == b)
		{
			last_leaf = curr_n;
			len_queue = ln.size();
			break;
		}
		for (auto f : functions)
		{
			node* nf = new node(f(curr_n->num), pred);
			ln.push_back(nf);
		}
	}
}

void min_path_time(int a, int b, vector<function<int(int)>> functions)
{
	unsigned int start_time = clock();

	min_path(a, b, functions);

	unsigned int end_time = clock(); // конечное время

	print_path(last_leaf);
	cout << endl << "len_queue = " << len_queue << endl;

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time = " << search_time << endl;
}


void min_path_unique(int a, int b, vector<function<int(int)>> functions)
{
	list<node*> ln;
	node* n1 = new node(a);
	ln.push_back(n1);

	while (true)
	{
		node* curr_n = ln.front();
		ln.pop_front();
		node* pred = curr_n;
		if (curr_n->num == b)
		{
			last_leaf = curr_n;
			len_queue = ln.size();
			break;
		}
		for (auto f : functions)
		{
			node* nf = new node(f(curr_n->num), pred);
			
			auto it = adjacent_find(ln.begin(), ln.end(), 
				[nf](node* n1, node* n2) {return n1->num == nf->num || n2->num == nf->num; });
			if (it == ln.end())
			{
				ln.push_back(nf);
			}
		}
	}
}

void min_path_unique_time(int a, int b, vector<function<int(int)>> functions)
{
	unsigned int start_time = clock();

	min_path_unique(a, b, functions);

	unsigned int end_time = clock(); // конечное время

	print_path(last_leaf);
	cout << endl << "len_queue = " << len_queue << endl;

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time = " << search_time << endl;
}

int main()
{
	vector<function<int(int)>> v1;
	v1.push_back([](int x) {return x * 2; });
	v1.push_back([](int x) {return x + 3; });

	cout << "				----- task1 ----- " << endl;
	min_path_time(a, b, v1);
	
	cout << "				----- task1 unique ----- " << endl;
	min_path_unique_time(a, b, v1);

	v1.push_back([](int x) {return x - 2; });
	cout << "				----- task2 ----- " << endl;
	min_path_time(a, b, v1);

	cout << "				----- task2 unique ----- " << endl;
	min_path_unique_time(a, b, v1);

	vector<function<int(int)>> v2;
	v2.push_back([](int x) {return x / 2; });
	v2.push_back([](int x) {return x - 3; });

	cout << "				----- task3 ----- " << endl;
	min_path_time(b, a, v2);

	cout << "				----- task3 unique ----- " << endl;
	min_path_unique_time(b, a, v2);

	system("pause");
}