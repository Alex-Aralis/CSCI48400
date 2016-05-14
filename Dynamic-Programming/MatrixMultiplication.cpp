#include <iostream>
#include <vector>
#include <algorithm>

#define BUF_SIZE 1024

using namespace std;

//used to print my 2d array format
void print_matrix(vector<vector<long>> &M) {
	for (unsigned long i = 0; i < M.size(); i++) {
		cout << "[ ";
		for (unsigned long j = 0; j < M[i].size(); j++) {
			cout << M[i][j] << "\t";
		}
		cout << "]\n";
	}
}

//turns an entry in my matrixes array into a string for printing
char* get_dims(vector<long> &m, char* buf) {
	sprintf_s(buf, BUF_SIZE, "%ldx%ld", m[0], m[1]);
	return buf;
}

//takes a dynamically allocated buf and places parenthesis around it
char* parenthize(char* buf) {
	char tmp[BUF_SIZE];

	strcpy_s(tmp, buf);
	sprintf_s(buf, BUF_SIZE, "(%s)", tmp);

	return buf;
}

//the recursive portion of oreder_ops, used to produced the output string
char* recursive_out_string(vector<vector<long>> &matrixes, vector<vector<long>> &k_mins, long i, long j) {
	long next_k = k_mins[i][j];
	char* buf = (char*)malloc(BUF_SIZE);
	char* trash;
	
	if (next_k == -1) {
		get_dims(matrixes[i], buf);
		return buf;
	}
	else {
		strcpy_s(buf, BUF_SIZE, parenthize(trash = recursive_out_string(matrixes, k_mins, i, next_k)));
		free(trash);
		strcat_s(buf, BUF_SIZE, parenthize(trash = recursive_out_string(matrixes, k_mins, next_k + 1, j)));
		free(trash);
		return buf;
	}
}

//sets things up for the recursion and prints the out string and cleans up the last dynamic memory
void order_ops(vector<vector<long>> &matrixes, vector<vector<long>> &k_mins) {
	long n = k_mins.size();

	char* out_str = recursive_out_string(matrixes, k_mins, 0, n-1);

	cout << out_str;
	free(out_str);
}

int main(int argc, char** argv) {
	char buf[BUF_SIZE];
	long in_m;
	long in_n;
	vector<vector<long>> matrixes;

	/*
	parse input file with format
	MxN
	MxN
	...
	MxN
	EOF
	*/

	FILE* fd;
	fopen_s(&fd, "C:\\Users\\LXRLS\\Documents\\visual studio 2015\\Projects\\Project1\\Debug\\matrixes.txt", "r");

	while (fgets(buf, BUF_SIZE, fd)) {
		sscanf_s(buf, "%ldx%ld", &in_m, &in_n);
		long newarray[2] = { in_m, in_n };
		matrixes.emplace_back(newarray, newarray+sizeof(newarray)/sizeof(long));
	}

	cout << "\ninput matrix dimensions: \n";
	print_matrix(matrixes);

	//initialize the dynamic work space with -1's
	long n = matrixes.size();
	vector<vector<long>> dynamic_array(n, vector<long>(n, -1));
	vector<vector<long>> k_storage(n, vector<long>(n, -1));

	//initialize some variable for the loop
	long k_min = -1;
	long possible_cost = 0;
	long min_cost = -1;

	//run the main loop that traverses the cost array along its diagonal from 0,0 to n-1,n-1
	for (long i = 0; i < n; i++) {
		for (long j = i; j < n; j++) {
			if (i == 0) {
				//if element is along the main diagonal there is no cost
				dynamic_array[j - i][j] = 0;
			}
			else {
				//determine the min cost based on the k possible solutions of the subproblem
				min_cost = -1;
				for (long k = j - i; k < j; k++) {
					possible_cost = dynamic_array[j - i][k] + dynamic_array[k + 1][j] + matrixes[j - i][0] * matrixes[k][1] * matrixes[j][1];

					if (min_cost > possible_cost || min_cost == -1) {
						min_cost = possible_cost;
						k_min = k;
					}
				}

				//save the min cost of the subproblem and the k used to achieve the min cost
				dynamic_array[j - i][j] = min_cost;
				k_storage[j - i][j] = k_min;
			}
		}
	}

	//print out some interesting stuff
	cout << "cost matrix: \n";
	print_matrix(dynamic_array);

	cout << "\nk storage: \n";
	print_matrix(k_storage);

	//recurse back through the k values to find an optimal solution
	cout << "\nminimum cost: " << dynamic_array[0][n-1] << "\n";

	cout << "\nparenthesized multiplication: ";
	order_ops(matrixes, k_storage);
	cout << "\n";
}