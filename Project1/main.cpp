#include<list>
#include<iostream>
#include<ctime>
#include<functional>
#include<vector>
#include<algorithm>

using namespace std;

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

list<int> nums;

void fill_nums_by_leaf(node* leaf)
{
	nums.clear();
	node* n = leaf;
	nums.push_front(n->num);

	while (n->pred != nullptr)
	{
		n = n->pred;
		nums.push_front(n->num);
	}
}

void print_path(node* leaf)
{
	fill_nums_by_leaf(leaf);

	for (auto it = nums.begin(); it != nums.end(); ++it)
	{
		std::cout << *it << " ";
	}
	cout << endl << "path length =	" << nums.size() << endl;
}

void print_path_bidirectional(node* leaf_up, node* leaf_down)
{
	fill_nums_by_leaf(leaf_up);
	for (auto it = nums.begin(); it != nums.end(); ++it)
	{
		std::cout << *it << " ";
	}
	int cnt_nums_first = nums.size();

	fill_nums_by_leaf(leaf_down);
	nums.pop_back();
	for (auto it = nums.rbegin(); it != nums.rend(); ++it)
	{
		std::cout << *it << " ";
	}
	cout << endl << "path length =	" << nums.size() + cnt_nums_first << endl;
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
	cout << endl << "len_queue =		" << len_queue << endl;

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time =	" << search_time << endl;
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
			
			auto it = find_if(ln.begin(), ln.end(), 
				[nf](node* n) {return n->num == nf->num; });
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
	cout << endl << "len_queue =		" << len_queue << endl;

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time =	" << search_time << endl;
}

node *lup, *ldown;
size_t len_queue_up, len_queue_down;

void min_path_bidirectional(int a, int b, vector<function<int(int)>> functions_up, vector<function<int(int)>> functions_down)
{
	list<node*> lnup;
	node* n1 = new node(a);
	lnup.push_back(n1);

	list<node*> lndown;
	node* n2 = new node(b);
	lndown.push_back(n2);

	bool founded = false;
	while (true)
	{
		if (founded)
			break;
		node* curr_n_up = lnup.front();
		lnup.pop_front();
		node* pred_up = curr_n_up;
		for (auto fup : functions_up)
		{
			node* nf = new node(fup(curr_n_up->num), pred_up);
			lnup.push_back(nf);
		}

		node* curr_n_down = lndown.front();
		lndown.pop_front();
		node* pred_down = curr_n_down;
		for (auto fdown : functions_down)
		{
			node* nf = new node(fdown(curr_n_down->num), pred_down);
			lndown.push_back(nf);
		}

		for (auto itup = lnup.begin(); itup != lnup.end(); ++itup)
		{
			node* nup = *itup;
			auto itdown = find_if(lndown.begin(), lndown.end(),
				[nup](node* n) {return n->num == nup->num; });
			if (itdown != lndown.end())
			{
				lup = nup;
				ldown = *itdown;
				len_queue_up = lnup.size();
				len_queue_down = lndown.size();
				founded = true;
				break;
			}
		}
	}
}

void min_path_bidirectional_time(int a, int b, vector<function<int(int)>> functions_up, vector<function<int(int)>> functions_down)
{
	unsigned int start_time = clock();

	min_path_bidirectional(a, b, functions_up, functions_down);

	unsigned int end_time = clock(); // конечное время

	print_path_bidirectional(lup, ldown);
	cout << "len queue_up =		" << len_queue_up << endl;
	cout << "len queue_down =	" << len_queue_down << endl;
	cout << "lens queues sum =	" << len_queue_up + len_queue_down << endl;

	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	cout << "time =	" << search_time << endl;
}

int main()
{
	int a = 2;
	int b = 1000;

	vector<function<int(int)>> v1;
	v1.push_back([](int x) {return x * 2; });
	v1.push_back([](int x) {return x + 3; });

	cout << "				----- basic operations search up ----- " << endl;
	min_path_time(a, b, v1);
	
	cout << "				----- basic operations search up unique ----- " << endl;
	min_path_unique_time(a, b, v1);



	v1.push_back([](int x) {return x - 2; });
	cout << "				----- basic operations + 'x->x-2' search up ----- " << endl;
	min_path_time(a, b, v1);

	cout << "				----- basic operations + 'x->x-2' search up unique ----- " << endl;
	min_path_unique_time(a, b, v1);
	v1.pop_back();



	vector<function<int(int)>> v2;
	v2.push_back([](int x) {return x % 2 == 0? x / 2: x - 3; });
	v2.push_back([](int x) {return x - 3; });

	cout << "				----- basic operations search down ----- " << endl;
	min_path_time(b, a, v2);

	cout << "				----- basic operations search down unique ----- " << endl;
	min_path_unique_time(b, a, v2);


	cout << "				----- bidirectional search ----- " << endl;
	min_path_bidirectional_time(a, b, v1, v2);

	system("pause");
}