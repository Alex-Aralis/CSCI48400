#include <iostream>
#include <queue>
#include <stdlib.h>

#define BUF_SIZE 1024

using namespace std;

//used to store ascii value huffman code pairs
class char_code {
	char* const code;
public:
	const char ascii;

	char_code(char c, char* given_code) : ascii(c) , code((char*)malloc(strlen(given_code) + 1)){
		
		strcpy_s(code, strlen(given_code) + 1, given_code);
	}

	char* get_code() {
		return code;
	}

	//convert char_code to codetable.txt entry
	char* to_string(char* buf) {
		char* sym;
		char ascii_string[] = { ascii, '\0' };

		if (ascii == '\n') {
			sym = "LF";
		}
		else {

			sym = ascii_string;
		}

		sprintf_s(buf, BUF_SIZE, "%s:%s", sym, code);
		return buf;
	}

	~char_code() {
		free(code);
	}
};

//used to compare char_code pointers
class char_code_comparator {
public:
	bool operator() (char_code* &lhs, char_code* &rhs) {
		return lhs->ascii < rhs->ascii;
	}
};

//class that holds the ascii values and their frequencies
//also can be used as a non-terminal node
class node{
public:
	const char ascii;
	const bool is_leaf;
	const long frequency;
private:
	node* const left;
	node* const right;

public:
	//constructor for leaf node
	node(node* left, node* right): left(left), right(right), frequency(left->frequency + right->frequency), is_leaf(false), ascii('\0') {}

	//constructor for non-terminal node
	node(long frequency, char c) : frequency(frequency), ascii(c), is_leaf(true) , left(nullptr), right(nullptr){}

	//recursively delete all children of node upon delete
	~node() {
		delete left;
		delete right;
	}

	//assumes this is the root and fills vec with char_codes recursively generated
	void get_codes(vector<char_code*>* vec) {
		char buf[BUF_SIZE];

		buf[0] = '0';
		rec_for_codes(buf, 1, left, vec);
		buf[0] = '1';
		rec_for_codes(buf, 1, right, vec);
	}

private:
	//recursive call for get_codes, no need to allocate memory for strings because
	//char_code dynamically allocates room for buf automatically
	void rec_for_codes(char* buf, long depth, node* cur, vector<char_code*>* vec) {
		if (cur->is_leaf) {
			buf[depth] = '\0';
			vec->push_back(new char_code(cur->ascii, buf));
		}
		else {
			buf[depth] = '0';
			rec_for_codes(buf, depth + 1, cur->left, vec);
			buf[depth] = '1';
			rec_for_codes(buf, depth + 1, cur->right, vec);
		}
	}
};

//class for comparing node pointers
class node_comparator {
public: 
	bool operator() (node* &lhs, node* &rhs) {
		return lhs->frequency > rhs->frequency;
	}
};

int main() {

	//open file freq.txt for reading
	FILE* fd;
	fopen_s(&fd, "freq.txt", "r");
	
	//create a min heap by frequency value of node
	priority_queue<node*, vector<node*>, node_comparator> heap;

	char c;
	char buf[BUF_SIZE];

	//read in the input and place leaf nodes into the heap
	while ((c = fgetc(fd)) != EOF) {
		//check for LF
		if (c == 'L') {
			if (fgetc(fd) == 'F') {
				c = '\n';
				
				//discarde space
				fgetc(fd);
			}
			else {
				c = 'L';
			}
		}else{
			//discard space
			fgetc(fd);
		}

		fgets(buf, BUF_SIZE, fd);

		//shove onto the heap
		heap.push(new node(atol(buf), c));
	}

	fclose(fd);

	//assemble the tree
	node* tmp1;
	node* tmp2;
	while (heap.size() > 1) {
		//pop smallest two values off heap
		tmp1 = heap.top();
		heap.pop();
		tmp2 = heap.top();
		heap.pop();

		//stick them together and put them back
		heap.push(new node(tmp1, tmp2));
	}

	//give the last topmost node a better name
	node* root = heap.top();

	//create the vector used for storing the outputs
	vector<char_code*> out_vec;

	//recurse through the tree and put all outputs into the out_vec
	root->get_codes(&out_vec);

	//recurively clean up all the allocated nodes
	delete root;

	//object used to compare char_codes based on ascii value
	char_code_comparator comparator;

	//sort all the character code pairs by ascii value
	sort(out_vec.begin(), out_vec.end(), comparator);
	

	//open up the codetable file
	fopen_s(&fd, "codetable.txt", "w+");


	//write to the code table file
	for (unsigned long i = 0; i < out_vec.size(); i++) {
		
		fprintf(fd, "%s\n", out_vec[i]->to_string(buf));
		delete out_vec[i];
	}

	//close codetable
	fclose(fd);
}