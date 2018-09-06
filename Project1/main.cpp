#include<list>
#include<iostream>
#include<ctime>
#include<functional>

using namespace std;


int a = 2;
int b = 10000;

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
		cout << *it << " ";
	}
	cout << endl << nums.size() << endl;
}

void task1()
{
	cout << "				----- task1----- " << endl;

	unsigned int start_time = clock(); 

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
			print_path(curr_n);
			break;
		}
		node* nplus3 = new node(curr_n->num + 3, pred);
		node* nprod2 = new node(curr_n->num * 2, pred);
		ln.push_back(nplus3);
		ln.push_back(nprod2);
	}

	unsigned int end_time = clock(); // конечное время
	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC; 

	cout << endl << search_time << endl;

	system("pause");
}



void task2()
{
	cout << "				----- task2----- " << endl;

	unsigned int start_time = clock();

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
			print_path(curr_n);
			break;
		}
		node* nplus3 = new node(curr_n->num + 3, pred);
		node* nprod2 = new node(curr_n->num * 2, pred);
		node* nminus2 = new node(curr_n->num - 2, pred);
		ln.push_back(nplus3);
		ln.push_back(nprod2);
		ln.push_back(nminus2);
	}

	unsigned int end_time = clock(); // конечное время
	double search_time = (end_time - start_time) / (double)CLOCKS_PER_SEC;

	cout << endl << search_time << endl;

	system("pause");
}

int main()
{
	task1();
	task2();
}