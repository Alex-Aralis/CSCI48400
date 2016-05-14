#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <algorithm>

#define BUFFER_SIZE 1024

using namespace std;

//a test to determine the location of the peak
vector<vector<long>*>* test(vector<long>* p, vector<long> &A) {
	//these are the indexes in A of the endpoints of the intervals test is examining
	long prea = (*p)[0];
	long prec = ((*p)[1] - (*p)[0]) / 2 + (*p)[0];
	long preb = prec - 1;
	long pred = (*p)[1] - 1;

	//these are the values store therewithin
	long vala = A[prea];
	long valb = A[preb];
	long valc = A[prec];
	long vald = A[pred];

	//if this is the final iteration then...
	if (prea == preb && prec == pred) {
		//if vala is greater then return prea
		if (vala > valc) {
			return new vector<vector<long>*>({ new vector<long>({ prea, prea + 1 }) });
		}
		//else return prec
		else {
			return new vector<vector<long>*>({ new vector<long>({ prec, prec + 1 }) });
		}
	}

	//the case that peak must be in interval [prea,preb+1]
	if (vala >= valb && valc > vald) { 
		return new vector<vector<long>*>({ new vector<long>({ prea,preb+1 }) });
	}
	//the case that peak must be in interval [prec,pred+1]
	else if (vala < valb && valc <= vald) {
		return new vector<vector<long>*>({ new vector<long>({ prec,pred+1 }) });
	}
	//the case that case could be in either of the intervals.
	else {
		return new vector<vector<long>*>({ new vector<long>({ prea,preb+1 }), new vector<long>({ prec,pred+1 }) });
	}
}

//reads in peak.txt and prerforms the algorithm
int main() {
	FILE* fd;

	//open the file
	fopen_s(&fd, "peak.txt", "r");

	char buf[BUFFER_SIZE];
	vector<long> A;

	//read in the file and create the array
	while (fgets(buf, BUFFER_SIZE, fd)) {
		A.push_back(stol(string(buf)));
	}

	//close the file
	fclose(fd);


	//initialize the loop
	long i = 0;

	//set both alpha and beta to the same thing so they preform duplicate operations
	//until there is a divergence do to an indeterminate test
	vector<long>* alpha = new vector<long>({ 0, (long) A.size() });
	vector<long>* beta = new vector<long>({ 0, (long) A.size() });

	//variables to store the returns from test
	vector<vector<long>*> *a, *b;

	while (log2(A.size()) > i) {
		i++; //increment i, only used to terminate the loop

		//test the intervals alpha and beta
		a = test(alpha, A);
		b = test(beta, A);

		//delete the old alpha and beta
		delete alpha, beta;

		//retrive the new alpha and beta from the results of test
		alpha = (*a)[0];
		beta = (*b)[0];

		//in the case of an indeterminate test result 
		//which is indicated by a size == 2 
		//stop the simulated recusion on either alpha or beta
		//and start down the new uncertain path instead
		if (a->size() == 2) {
			//delete old beta
			delete beta;

			//replace beta ending that recursion path
			beta = (*a)[1];
			
			//delete second return from test on beta in the case that 
			if (b->size() == 2) {
				delete (*b)[1];
			}
		}
		else if (b->size() == 2) {
			//delete old alpha
			delete alpha;

			//replace old alpha ending that recursion path
			alpha = (*b)[1];
		}

		//throw away the returns from the test
		delete a, b;
	}

	//print out the index in alpha or beta with the greatest value
	cout << "The peak of peak.txt is: " << ( A[alpha->front()] > A[beta->front()] ? alpha->front() : beta->front() ) << "\n";

	//delete final alpha and beta where the results were stored
	delete alpha, beta;
}
