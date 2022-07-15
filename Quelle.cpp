#include <iostream>
#include <math.h>
#include <list>
#include <vector>
#include <string>
using namespace std;


//INPUT:
//

float factor = 7.0 / 5.0;
int periodarray[] = { 3, 7, 7 };
//int src[] = { 1,0,0,0,0,2,0,0,0, 1,0,0,0,0,3,0,0,0 };  
string schedulestring = "1020 1030"; 

//
/////////////////////////

//Confirms the validity of a schedule by counting the distances inbetween occurences of the elements and comparing to the periods-
void testschedule(vector<int> schedulevector, vector<int> periodvector){
	vector<int> dists;
	for (int j = 0; j < periodvector.size(); j++) {
		dists.push_back(0);
		float f = factor*periodvector[j];
		periodvector[j] = floorf(f);
	}
	int counter = 0;
	for (int i = 0; i < 2 * schedulevector.size(); i++) {   //We go through the schedule twice and measure the distance inbetween occurences of each period
		for (int j = 0; j < periodvector.size(); j++) {
			dists[j]++;                               //Whenever we move increase the distance travelled but whenever we reach a period set it to 0
			if (schedulevector[counter] == j+1) {
				dists[j] = 0;
			}
			if (dists[j] >= periodvector[j]) {             //should the distance be larger than the period the schedule is invalid.
				cout << "Invalid schedule!" << j << " ";        
				cin.get();
			}
		}
		counter = counter+1;
		counter = counter % schedulevector.size();
	}
}

//Converts the schedule from a string to a vector
void convert(vector<int> * emptyvector) {
	for (int i = 0; i < schedulestring.length(); i++) {
		if (schedulestring[i] != ' ') {
			emptyvector->push_back(stoi(schedulestring.substr(i, 1)));
		}
	}
}


//determines the minimum number of holes in p consecutive positions in the schedule, i.e determines h for p
int countholes(vector<int> schedulevector, int p) {
	//Determine the overall number of holes in the schedule
	int overallholes = 0;                               
	for (int i = 0; i < schedulevector.size(); i++) {
		if (schedulevector[i] == 0) {
			overallholes++;
		}
	}
	int c = p / schedulevector.size();                       
	int d = p % schedulevector.size();
	//p=c*schedulesize+d
	//Determine the minimum number of holes in d consecutive positions
	int start = 0;
	int min = schedulevector.size();
	while (start < schedulevector.size()) {                 
		int holesinsection = 0;
		int counter = start;
		for (int i = 0; i < d; i++) {
			counter = counter+1;
			counter = counter % schedulevector.size();
			if (schedulevector[counter] == 0) {
				holesinsection ++;
			}
		}
		if (holesinsection < min) {
			min = holesinsection;
		}
		start++;
	}
	return c*overallholes+min;
}


int main() {
	//Converting the input into vectors
	vector<int> schedulevector; 
	convert(&schedulevector);
	vector<int> periodvector(std::begin(periodarray), std::end(periodarray));
	
	//Is this schedule valid?
	testschedule(schedulevector, periodvector);

	//Calculate D_A
	float DA = 1.0;
	for (int i = 0; i < periodvector.size(); i++) {
		DA = DA - 1.0 / periodvector[i];
	}
	cout << "DA " << DA << " ";
	cin.get();

	//Determine a suitable amount of iterations and check for h/p \geq D_A for these periods
	int iterations = schedulevector.size() * 5 + 100;
	int pstar = 0;
	for (int i = 1; i < iterations; i++) {
		float f = factor*i;
		int p = floorf(f);
		int h = countholes(schedulevector, p);
		float temp = h;
		temp = temp / p;
		temp = temp - DA;
		if (temp < 0) {
			pstar = i;
		}
		cout << "Iteration: " << i << endl;
	}
	cout << "Result: p* >= " << pstar+1;
	cin.get();
}