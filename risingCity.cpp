#include <iostream>
#include<fstream>
#include <string>
#include<sstream>
#include <algorithm>
#include <queue>
using namespace std;
enum COLOR { RED, BLACK };
int size;
int capacity = 50000;
int globalTimer = 0, savedDay = 0;


//Node structure of buildings in min-heap
struct Node
{
	int buildingNumber;
	int executedTime;
	int totalTime;
	int daysSpentOnBuilding; //to keep count of number of days a building is worked upon at a time(should be <=5)
};
Node heap[500000];//Min-Heap => Globally accessible array of building nodes created. 


//Class for node structure of buildings in RBT
class RBTNode {

public:
	int buildingNumber;
	int executedTime;
	int totalTime;
	COLOR color;

	RBTNode* left, * right, * parent;

	RBTNode() {
		parent = left = right = NULL;
		buildingNumber = 0;
		executedTime = 0;
		totalTime = 0;
		color = RED;
	}


	RBTNode* uncle() {
		if (parent == NULL or parent->parent == NULL)
			return NULL;

		if (parent->isOnLeft())
			return parent->parent->right;
		else 
			return parent->parent->left;
	}


	// check if node is left child of parent 
	bool isOnLeft() { return this == parent->left; }

	// returns pointer to sibling 
	RBTNode* sibling() { 
		if (parent == NULL)
			return NULL;
		if (isOnLeft())
			return parent->right;
		return parent->left;
	}

	
	//A given node is replaced in the node's place by moving it down 


	void moveDown(RBTNode* nParent) {
		if (parent != NULL) {
			if (isOnLeft()) {
				parent->left = nParent;
			}
			else {
				parent->right = nParent;
			}
		}
		nParent->parent = parent;
		parent = nParent;
	}

	bool hasRedChild() {
		return (left != NULL and left->color == RED) or
			(right != NULL and right->color == RED);
	}
};



void swapRBT(RBTNode* x, RBTNode* y)
{
	RBTNode temp = *x;
	*x = *y;
	*y = temp;
}

void swapint(int x, int y)
{
	int temp = x;
	x = y;
	y = temp;
}




class RBTree {
	RBTNode* root;

	// node -> left rotated 
	void leftRotate(RBTNode* x) {
		
		RBTNode* nParent = x->right;
		if (x == root)
			root = nParent;

		x->moveDown(nParent);
		x->right = nParent->left;
		if (nParent->left != NULL)
			nParent->left->parent = x;
		nParent->left = x;
	}

	//node->right rotated
	void rightRotate(RBTNode* x) {
		RBTNode* nParent = x->left;
		if (x == root)
			root = nParent;

		x->moveDown(nParent);
		x->left = nParent->right;
		if (nParent->right != NULL)
			nParent->right->parent = x;
		nParent->right = x;
	}

	void swapColors(RBTNode* x1, RBTNode* x2) {
		COLOR temp;
		temp = x1->color;
		x1->color = x2->color;
		x2->color = temp;
	}

	//swap's the three data fields of one building node with another
	void swapValues(RBTNode* u, RBTNode* v) {
		int tempbuildno, tempexectime, temptotaltime;
		tempbuildno = u->buildingNumber;
		tempexectime = u->executedTime;
		temptotaltime = u->totalTime;
		u->buildingNumber = v->buildingNumber;
		u->executedTime = v->executedTime;
		u->totalTime = v->totalTime;
		v->buildingNumber = tempbuildno;
		v->executedTime = tempexectime;
		v->totalTime = temptotaltime;
	}

	
	void fixRedRed(RBTNode* x) {
		if (x == root) {
			x->color = BLACK;
			return;
		}

		// Initialization
		RBTNode* parent = x->parent, * grandparent = parent->parent,
			* uncle = x->uncle();

		if (parent->color != BLACK) {
			if (uncle != NULL && uncle->color == RED) {
				parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				fixRedRed(grandparent);
			}
			else {
				if (parent->isOnLeft()) {
					if (x->isOnLeft()) { 
						swapColors(parent, grandparent);
					}
					else {
						leftRotate(parent);
						swapColors(x, grandparent);
					} 
					rightRotate(grandparent);
				}
				else {
					if (x->isOnLeft()) {
						rightRotate(parent);
						swapColors(x, grandparent);
					}
					else {
						swapColors(parent, grandparent);
					}
					leftRotate(grandparent);
				}
			}
		}
	}


	//In the subtree of the given node finds the node that 
	//does not have a left child
	RBTNode* successor(RBTNode* x) {
		RBTNode* temp = x;

		while (temp->left != NULL)
			temp = temp->left;

		return temp;
	}

	
	//retruns a replacement node for a deleted node in BST
	RBTNode* BSTreplace(RBTNode* x) {
		
		if (x->left != NULL and x->right != NULL)
			return successor(x->right);

		if (x->left == NULL and x->right == NULL)
			return NULL;
		if (x->left != NULL)
			return x->left;
		else
			return x->right;
	}

	//Deletes a node from the RBT
	void deleteNode(RBTNode* v) {
		RBTNode* u = BSTreplace(v);
		bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
		RBTNode* parent = v->parent;

		if (u == NULL) {
			if (v == root) {
				root = NULL;
			}
			else {
				if (uvBlack) {
					fixDoubleBlack(v);
				}
				else {
					if (v->sibling() != NULL)
						v->sibling()->color = RED;
				}

				if (v->isOnLeft()) {
					parent->left = NULL;
				}
				else {
					parent->right = NULL;
				}
			}
			delete v;
			return;
		}

		if (v->left == NULL or v->right == NULL) {
			if (v == root) {
				v->buildingNumber = u->buildingNumber;
				v->executedTime = u->executedTime;
				v->totalTime = u->totalTime;
				v->left = v->right = NULL;
				delete u;
			}
			else {
				if (v->isOnLeft()) {
					parent->left = u;
				}
				else {
					parent->right = u;
				}
				delete v;
				u->parent = parent;
				if (uvBlack) {
					fixDoubleBlack(u);
				}
				else {
					u->color = BLACK;
				}
			}
			return;
		}
		swapValues(u, v);
		deleteNode(u);
	}

	void fixDoubleBlack(RBTNode* x) {
		if (x == root)
			return;

		RBTNode* sibling = x->sibling(), * parent = x->parent;
		if (sibling == NULL) {
			fixDoubleBlack(parent);
		}
		else {
			if (sibling->color == RED) {

				parent->color = RED;
				sibling->color = BLACK;
				if (sibling->isOnLeft()) {
					rightRotate(parent);
				}
				else {
					leftRotate(parent);
				}
				fixDoubleBlack(x);
			}
			else {
				if (sibling->hasRedChild()) {
					if (sibling->left != NULL and sibling->left->color == RED) {
						if (sibling->isOnLeft()) {
							sibling->left->color = sibling->color;
							sibling->color = parent->color;
							rightRotate(parent);
						}
						else {
							sibling->left->color = parent->color;
							rightRotate(sibling);
							leftRotate(parent);
						}
					}
					else {
						if (sibling->isOnLeft()) {
							sibling->right->color = parent->color;
							leftRotate(sibling);
							rightRotate(parent);
						}
						else {
							sibling->right->color = sibling->color;
							sibling->color = parent->color;
							leftRotate(parent);
						}
					}
					parent->color = BLACK;
				}
				else {
					sibling->color = RED;
					if (parent->color == BLACK)
						fixDoubleBlack(parent);
					else
						parent->color = BLACK;
				}
			}
		}
	}

	// prints level order for given node 
	void levelOrder(RBTNode* x) {
		if (x == NULL)
			return;
		queue<RBTNode*> q;
		RBTNode* curr;
		q.push(x);

		while (!q.empty()) {
			curr = q.front();
			q.pop();
			cout << curr->buildingNumber << " ";
			if (curr->left != NULL)
				q.push(curr->left);
			if (curr->right != NULL)
				q.push(curr->right);
		}
	}

public:
	
	RBTree() { root = NULL; }

	RBTNode* getRoot() { return root; }

	RBTNode* search(int n) {
		RBTNode* temp = root;
		while (temp != NULL) {
			if (n < temp->buildingNumber) {
				if (temp->left == NULL)
					break;
				else
					temp = temp->left;
			}
			else if (n == temp->buildingNumber) {
				break;
			}
			else {
				if (temp->right == NULL)
					break;
				else
					temp = temp->right;
			}
		}
		
		return temp;
	}

	// inserts the given value to tree 
	void insert(int bn,int et,int tt) {
		RBTNode* newNode = new RBTNode;
		newNode->buildingNumber = bn;
		newNode->executedTime = et;
		newNode->totalTime = tt;


		if (root == NULL) {
			newNode->color = BLACK;
			root = newNode;
		}
		else {
			RBTNode* temp = search(bn);

			if (temp->buildingNumber == bn) {
				return;
			}

			newNode->parent = temp;

			if (bn < temp->buildingNumber)
				temp->left = newNode;
			else
				temp->right = newNode;

			// fix red red voilaton if exists 
			fixRedRed(newNode);
		}
	}

	
	void deleteByVal(int n) {
		if (root == NULL) 
			return;

		RBTNode* v = search(n), * u;

		if (v->buildingNumber != n) {
			cout << "No node found to delete with value:" << n << endl;
			return;
		}

		deleteNode(v);
	}

	// prints level order of the tree 
	void printLevelOrder() {
		cout << "Level order: " << endl;
		if (root == NULL)
			cout << "Tree is empty" << endl;
		else
			levelOrder(root);
		cout << endl;
	}


	void updatetree(int buildingNumber)
	{
		RBTNode* node = new RBTNode;
		node = search(buildingNumber);
		if (node->buildingNumber == buildingNumber)
		{
			node->executedTime = node->executedTime + 1;
		}

	}

	
	
};

RBTree tree;

void swap(RBTNode* x, RBTNode* y)
{
		RBTNode temp = *x;
		*x = *y;
		*y = temp;
}

void swap(Node* x, Node* y)
{
	Node temp = *x;
	*x = *y;
	*y = temp;
}


int parent(int index)
{
	return (index - 1) / 2;
}

int leftChild(int index)
{
	return (2 * index + 1);
}

int rightChild(int index)
{
	return (2 * index + 2);
}



void insertIntoHeap(Node node)
{
	if ( ::size == capacity)
	{
		cout << "\nOverflow: Could not insertKey\n";
		return;
	}

	
	::size++;
	int i = ::size - 1;
	::heap[i] = node;


	while (i != 0)
	{
		if (( ::heap[parent(i)].executedTime == ::heap[i].executedTime) && (::heap[parent(i)].buildingNumber > ::heap[i].buildingNumber))
		{
			swap(&::heap[i], &::heap[parent(i)]);
			i = parent(i);
		}

		else if (::heap[parent(i)].executedTime > ::heap[i].executedTime)
		{
			swap(&::heap[i], &::heap[parent(i)]);
			i = parent(i);
		}
		else
		{
			i = parent(i);
		}
	}
	
}
void Insert(int buildingNo, int timeTotal)
{
	Node newNode;
	newNode.buildingNumber = buildingNo;
	newNode.executedTime = 0;
	newNode.totalTime = timeTotal;
	newNode.daysSpentOnBuilding = 0;
	insertIntoHeap(newNode);
}
void MinHeapify(int i)
{
	int l = leftChild(i);
	int r = rightChild(i);
	int smallest = i;
	if (l < ::size && ::heap[l].executedTime < ::heap[i].executedTime)
		smallest = l;
	if (r < ::size && ::heap[r].executedTime < ::heap[smallest].executedTime)
		smallest = r;
	if (smallest != i)
	{
		swap(&::heap[i], &::heap[smallest]);
		MinHeapify(smallest);
	}
}
int extractMin()
{
	if (::size <= 0)
		return -1;
	int root = ::heap[0].buildingNumber;
	::heap[0] = ::heap[::size - 1];
	::size--;
	MinHeapify(0);
	return root;
}


//The building is worked on for the entire day
// Executed time of the building goes up by 1 and is updated in the tree data structure too

void work_on_building()
{
	
	::heap[0].executedTime = ::heap[0].executedTime + 1;
	::heap[0].daysSpentOnBuilding = ::heap[0].daysSpentOnBuilding + 1;
	tree.updatetree(heap[0].buildingNumber);
}




int main(int argc, char** argv)
{
	::size = 0;
	int min, parameter1, parameter2;
	size_t pos = 0;
	string line, clockTime, functionName,parameter, name;

	ofstream myfile;
	myfile.open("output_file.txt"); 

	ifstream reader(argv[1]);
	if (!reader.is_open())
	{
		cout << "Error opening input file  " << endl;
		exit(1);
	}


	//loop runs until end of input file is reached
	while (!getline(reader, clockTime, ':').eof())
	{
		getline(reader, functionName, '(');
		getline(reader, parameter, ')'); 
		// Splitting each line of the file into clocktime, function name and its parameters


		//loop untill the global counter reaches the clocktime of a particular incoming instruction from the input file
		while (stoi(clockTime) != ::globalTimer )
		{
			//Progressing to next day
			::globalTimer++;
			
			//while the min heap is not empty
			if (::size > 0)
			{
				int remainingWorkDays, daysOnBuilding;
				Node root = ::heap[0];
				remainingWorkDays = ::heap[0].totalTime - ::heap[0].executedTime;
				daysOnBuilding = ::heap[0].daysSpentOnBuilding;

				//Ensures the a paticular building is not worked on for more than  5 days at a time
				//Removes a node(building) and resinserts into the heap, if the building has been worked for 5 days 
				while (heap[0].daysSpentOnBuilding >= 5)
				{
					Node newNode;
					newNode.executedTime = heap[0].executedTime;
					newNode.buildingNumber = heap[0].buildingNumber;
					newNode.daysSpentOnBuilding = 0;
					newNode.totalTime = heap[0].totalTime;
					int res= extractMin();
					insertIntoHeap(newNode);
				}

				//If the building at the top of the heap has only one more day to be worked upon.
				//Work on the building for a day and remove from heap and the tree as building is completed
				if (heap[0].totalTime - heap[0].executedTime == 1)
				{
					int res = 0;
					work_on_building();
					myfile << "(" << heap[0].buildingNumber << " ," << ::globalTimer <<")"<< endl;
					if (::heap[0].totalTime == ::heap[0].executedTime)
					{
						tree.deleteByVal(heap[0].buildingNumber);
						res = extractMin();

					}
					
					
					if (res == -1)
					{
						::heap[2000] = {};
					}

				}  


				// Continues to work on the building on top of the heap if number of days left is more than  1
				else if (::heap[0].totalTime - ::heap[0].executedTime > 1)
				{
					work_on_building();
				}


				if (remainingWorkDays < 0)
				{
					cout << "Remaning days neg";
					break;
				}
			}

		}

		// The global timer matches the arrival time of the next buiding
		if (::globalTimer == stoi(clockTime) && ((functionName == " Insert") || (functionName == "Insert")))
		{

			pos = parameter.find(',');
			
			istringstream(parameter.substr(0, pos )) >> parameter1;
			istringstream(parameter.substr(pos+1, parameter.length() - 1)) >> parameter2;
	//		

			//Insert building into the heap
			Insert(parameter1,parameter2);

			//Insert building into the RBT
			tree.insert(parameter1, 0, parameter2);  

			if (::globalTimer > 0)

				//Start working on the newly inserted building as its executed time is the least in the heap
				work_on_building();
			::globalTimer++;
		}

		//The global timer matches the incoming Print command arrival time
		if (::globalTimer == stoi(clockTime) && ((functionName == " PrintBuilding") || (functionName == "PrintBuilding") || (functionName == "PrintBuilding "))) 
		{
			
			RBTNode* searchednode = new RBTNode;
			pos = parameter.find(',');
			int param;


			//Printing details of one building
			if (pos == std::string::npos)
			{
				istringstream(parameter) >> param;

				searchednode = tree.search(param);

				if (searchednode != NULL && searchednode->buildingNumber == param)
				{
					myfile << "(" << searchednode->buildingNumber << "," << searchednode->executedTime << "," << searchednode->totalTime << ")" << "\n";
				}
				else
				{
					myfile << "(0, 0, 0)"<<endl;
				}
			}

			//Printing details of a range of buildings
			else
			{
				RBTNode* nodesearched = new RBTNode;
				istringstream(parameter.substr(0, pos )) >> parameter1;
				istringstream(parameter.substr(pos + 1, parameter.length() - 1)) >> parameter2;
				int building_present_in_range = 0, is_first_building_in_range=1;

				
				for (int i = parameter1;i <= parameter2;i++)
				{
					nodesearched = tree.search(i);
					
					//If printing out the first building found in the range , do not insert comma before printing
					if (nodesearched != NULL && nodesearched->buildingNumber == i && is_first_building_in_range == 1)
					{

					myfile << "(" << nodesearched->buildingNumber << "," << nodesearched->executedTime << "," << nodesearched->totalTime << ")  ";
					building_present_in_range = 1;
					is_first_building_in_range = 0;
					}

					//Every building that follows, insert a comma before printing out the triplet of that particular building
					else if (nodesearched != NULL && nodesearched->buildingNumber == i && is_first_building_in_range==0 )
					{
						
						myfile << ",  (" << nodesearched->buildingNumber << "," << nodesearched->executedTime << "," << nodesearched->totalTime << ")";
						building_present_in_range = 1;
					}

					
				}
				myfile << endl;

				// Print out (0,0,0) if no building is found in the given range
				if (building_present_in_range == 0)
				{
					myfile << "(0,0,0)" << endl;
				}
				
			}
			

			//After Printing if the heap is not empty, continue working on the building which has the min exectued time
			if (::size > 0)
			{
				int remainingWorkDays, daysOnBuilding;
				Node root = ::heap[0];
				remainingWorkDays = ::heap[0].totalTime - ::heap[0].executedTime;
				daysOnBuilding = ::heap[0].daysSpentOnBuilding;


				while (heap[0].daysSpentOnBuilding >= 5)
				{
					Node newNode;
					newNode.executedTime = heap[0].executedTime;
					newNode.buildingNumber = heap[0].buildingNumber;
					newNode.daysSpentOnBuilding = 0;
					newNode.totalTime = heap[0].totalTime;
					int res= extractMin();
					insertIntoHeap(newNode);
				}


				if (heap[0].totalTime - heap[0].executedTime == 1)
				{
					int res = 0;
					work_on_building();
					myfile << "(" << heap[0].buildingNumber << "," << ::globalTimer <<")"<< endl;
					
					if (::heap[0].totalTime == ::heap[0].executedTime)
					{
						tree.deleteByVal(heap[0].buildingNumber);
						res = extractMin();

					}
					
					
					if (res == -1)
					{
						::heap[2000] = {};
					}

				}  



				else if (::heap[0].totalTime - ::heap[0].executedTime > 1)
				{
					
					work_on_building();
				}


				if (remainingWorkDays < 0)
				{
					cout << "Remaning days neg";
					break;
				}
			}
			::globalTimer++;
		}

	}


	// After all the input from the input file has been read, and the heap is not epmty 
	while (::size > 0)
	{
		//Progress to next day
		::globalTimer++;

		//Work on a building only for a maximum of 5 days at a time
		while (::heap[0].daysSpentOnBuilding >= 5)
		{
			Node newNode;
			
			if (heap[0].executedTime == heap[0].totalTime)
			{
				tree.deleteByVal(heap[0].buildingNumber);
				extractMin();
				
			}
			else
			{
				
				newNode.executedTime = ::heap[0].executedTime;
				newNode.buildingNumber = ::heap[0].buildingNumber;
				newNode.daysSpentOnBuilding = 0;
				newNode.totalTime = ::heap[0].totalTime;
				extractMin();
				insertIntoHeap(newNode);
			}
			
		}

		//If the remaining days to be worked on the building is 0, delete the building from both the data structures 
		while (::heap[0].daysSpentOnBuilding < 5 && ::heap[0].totalTime - ::heap[0].executedTime == 0)
		{
			tree.deleteByVal(heap[0].buildingNumber);
			extractMin();
		}


		//If the remaining days to be worked on the building is 1, work on the building for one day and delete the building from the data structures
		if (::heap[0].daysSpentOnBuilding < 5 && ::heap[0].totalTime - ::heap[0].executedTime == 1)
		{
			work_on_building();
			if (heap[0].totalTime == heap[0].executedTime)
			{
				tree.deleteByVal(heap[0].buildingNumber);
				extractMin();
			}
		}

		//Continue to work on the building if 5 days have not expired and the remainng days is more than 1
		else if (::heap[0].daysSpentOnBuilding < 5 && ::heap[0].totalTime - ::heap[0].executedTime > 1)
		{
			work_on_building();
		}

		
	}


		
		myfile << "Total time expired = " << ::globalTimer << endl;

		return 0;
		myfile.close();
}

