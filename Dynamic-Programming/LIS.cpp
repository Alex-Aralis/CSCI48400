#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

#define BUF_SIZE 1024

using namespace std;

void print_array(vector<long> A) {
	long N = A.size();

	cout << "[\t";
	for (long i = 0; i < N; i++) {
		cout << A[i] << "\t";
	}
	cout << "]\n";
}


void print_stack(stack<long> s) {
	cout << "[\t";
	while (!s.empty()) {
		cout << s.top() << "\t";
		s.pop();
	}
	cout << "]\n";
}

int main() {
	vector<long> X;
	vector<long> L;

	char buf[BUF_SIZE];

	//open file
	FILE* fd;
	fopen_s(&fd, "incseq.txt", "r");

	//discard sequence length
	fgets(buf, BUF_SIZE, fd);

	//initialize X and L
	while (fgets(buf, BUF_SIZE, fd)) {
		X.push_back(atol(buf));
		L.push_back(-1);
	}

	//determine size of X
	long N = X.size();
	long m;

	//fill out L with max predecesors
	for (long i = 0; i < N; i++) {

		m = 0;
		for (long j = 0; j < i; j++) {
			if (X[i] > X[j]) {
				if (L[j] > m) {
					m = L[j];
				}
			}
		}

		L[i] = m + 1;
	}
	
	//find max sequence
	m = 0;
	long m_index;
	for (long i = 0; i < N; i++) {
		if (m < L[i]) {
			m = L[i];
			m_index = i;
		}
	}
	cout << "X: ";
	print_array(X);
	cout << "L: ";
	print_array(L);
	cout << "\n";

	cout << "Max increasing subsequence length: "<< m << "\n\n";

	long i = m_index;
	long index = -1;
	stack<long> seq;
	
	//find a maximum increasing subsequence and store it in seq
	while (L[i] > 1) {
		seq.push(X[i]);
		for (long j = 0; j < i; j++) {
			if (X[j] < X[i]) {
				if (L[j] == L[i] - 1) {
					m = X[j];
					index = j;
				}
			}
		}

		i = index;
	}

	seq.push(X[i]);

	//print out a maximal increasing subsequence
	cout << "A maximum increasing subseequence:\n";
	print_stack(seq);
}