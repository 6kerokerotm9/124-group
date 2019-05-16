#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct chloe
{
	int number; //command order number
	string name; //name of command
	int prereq[2]; //for now a list of the numbers for the prerequisites that will be used to determine
	int prereqcounter; //used to keep track of how many prereqs each process has 
	double time; //time value
	
	chloe* prereqpointer; //points to prerequisite 
	chloe* next; //points to next order to create a linked list

	//print function 
	void print() {
		cout << "Order number: " << number << endl;
		cout << "Name of command: " << name <<endl;
		cout << "Prereqs: ";
		for(int i=0; i<prereqcounter; i++) { //prints out the appropriate amount of prereqs
			cout << prereq[i] << " ";
		}
		cout << endl;
		cout << "Time taken: " << time << endl;
		cout << endl;
	}
};

struct path {
	vector<chloe*> criticalpath;
	double time;
};

void parse(chloe * &tail, string name) {
	ifstream file;
	file.open(name);
	string line; //temp variables: this one is used with the first getline and holds the entire line of a file
	string temp; //holds the string from the line variable all the way up to the colon 
	int counter; //loop counter which is needed for the second loop to fill in the different struct members
	//int othercounter = 0; //0 if on the prereq portion and 1 if it is looking for time
	while(getline(file, line)) { //keeps going until the file ends
		istringstream ss(line); //runs the lines through string stream so it can be used in the next getline function
		counter = 0; 
		while(getline(ss, temp, ':')) {
			istringstream ss2(temp); //puts the string taken from the line into a string stream to use for converting
			switch(counter) {
				case 0: //we know the order of values in each line of the file so we use a counter and switch statements to fill the members respectively
					ss2 >> tail->number;
					break;
				case 1:
					getline(ss2, tail->name); //takes the entire string including whitespaces for the operation name
					break;
				case 2:
					for(int i=0; i<temp.size(); i++) { //for loop will check to see if the current line has a comma, meaning it has more than one prereq
						if(temp[i] == ',') { //if there is a comma 
							istringstream ss3(temp.substr(0,1)); //substring is from start of string up to the comma 
							ss3 >> tail->prereq[0]; //take the first value before the comma and store it in the first prereq place
							istringstream ss4(temp.substr(2));
							ss4 >> tail->prereq[1];	
							tail->prereqcounter = 2; //set the prereqcounter to 2 for printing purposes
							break; //break out if comma is found so the loop ends reliably without having to check for another comma 
						}
						else if(i+1 == temp.size()) { //if there is no comma, meaning there is only one prereq	
							ss2 >> tail->prereq[0];
							tail->prereqcounter = 1;
							break;
						}
					}
					break;
				case 3:
					ss2 >> tail->time;
					break;
			}
			counter++; 
		}
		tail = tail->next; //moves on to the next place in the linked list
		tail->next = new chloe; //initialize the next member in the struct
	}
	file.close();
}

void critR(vector<vector<chloe*>>& critpaths, vector<chloe*>& temppath, chloe * head, int counter, int endloop) {
	vector<chloe*> currentpath;
	vector<chloe*> branching;
	chloe * temp = head;
	chloe * temp2 = head;
	
	while(temp2->number != endloop) {
		while(temp->name != "") {
			if(temp->prereqcounter == 2) {
				if((temp->prereq[0]) == counter || (temp->prereq[1]) == counter) {
					currentpath.push_back(temp);
				}
			}
			else if(temp->prereq[0] == counter) {
				currentpath.push_back(temp);
			}
			temp = temp->next;
		}
		temp = head;
		if(currentpath.size() > 1) {
			branching = temppath;
			for(int j=0; j<currentpath.size(); j++) {
				temppath.push_back(currentpath[j]);
				critR(critpaths, temppath, head, currentpath[j]->number, endloop);
				critpaths.push_back(temppath);
				temppath = branching;
			}
			break;
		}
		else {
			temppath.push_back(currentpath[0]);
			temp2 = currentpath[0];
			counter = temp2->number;
			currentpath = {};
		}
	}
}

void findcrit(vector<vector<chloe*>> critpaths, path& critical) {
	double largesttime = 0;
	double totals = 0;
	int largestindex;
	
	for(int i=0; i<critpaths.size(); i++) {
		for(int j=0; j<critpaths[i].size(); j++) {
			totals += critpaths[i][j]->time;
		}
		if(totals > largesttime) {
			largesttime = totals;
			largestindex = i;
		}
		totals = 0;
	}
	for(int i=0; i<critpaths[largestindex].size(); i++) {
		critical.criticalpath.push_back(critpaths[largestindex][i]);
	}
	critical.time = largesttime;
}

void crit(chloe * head, chloe * tail, path& critical) {
	chloe * temp = head;
	chloe * temp2 = head;
	vector<int> times; //holds the times of each path
	int counter = 0;
	int endloop;

	vector<vector<chloe*>> critpaths;
	vector<chloe*> currentpath;
	vector<chloe*> temppath;
	vector<chloe*> branching;
	
	//find the final numbered step
	while(temp->next->name != "") {
		temp = temp->next;
	}
	endloop = temp->number;
	temp = head;
	
	while(temp2->number != endloop) {
		while(temp->name != "") {
			if(temp->prereqcounter == 2) {
				if((temp->prereq[0]) == counter || (temp->prereq[1]) == counter) {
					currentpath.push_back(temp);
				}
			}
			else if(temp->prereq[0] == counter) {
				currentpath.push_back(temp);
			}
			temp = temp->next;
		}
		temp = head;
		if(currentpath.size() > 1) {
			branching = temppath;
			for(int j=0; j<currentpath.size(); j++) {
				temppath.push_back(currentpath[j]);
				critR(critpaths, temppath, head, currentpath[j]->number, endloop);
				critpaths.push_back(temppath);
				temppath = branching;
			}
			break;
		}
		else {
			temppath.push_back(currentpath[0]);
			temp2 = currentpath[0];
			counter = temp2->number;
			currentpath = {};
			if(temp2->number == endloop) {
				critpaths.push_back(temppath);
			}
		}
	}
	findcrit(critpaths, critical);
}
	
int main () {
	string files[] = {"chloe.txt", "HouseTasks.txt", "noodlepasta.txt"};
	string filename = files[1]; //holds the file that is chosen by the user from the html page
	
	chloe * head = new chloe;
	chloe * tail = head;
	tail->next = new chloe; 
	path critical;
	
	parse(tail, filename);
	crit(head, tail, critical);
	
	for(int i=0; i<critical.criticalpath.size(); i++) {
		critical.criticalpath[i]->print();
	}
	cout << "Total time: " << critical.time << endl;
}

/*
for(int i=0; i<critpaths.size(); i++) {
		for(int j=0; j<critpaths[i].size(); j++) {
			critpaths[i][j]->print();
		}
		cout << "******************" << endl;
	}

while(temp != tail) {
		temp->print();
		temp = temp->next;
	}

while(counter < 15) {
		while(temp->name != "") {
			if(temp->prereqcounter == 2) {
				if((temp->prereq[0]) == counter || (temp->prereq[1]) == counter) {
					currentpath.push_back(temp);
				}
			}
			else if(temp->prereq[0] == counter) {
				currentpath.push_back(temp);
			}
			temp = temp->next;
		}
		if(currentpath.empty()) {
			break;
		}
		critpaths.push_back(currentpath);
		counter++;	
		currentpath = {};
		temp = head;
	}
	cout << " q" << endl;
}

//function that takes the prereq int from the file and sets the prereq variable for linked list purposes
	//takes argument head that is the head pointer for the linked list
	//not used for now because we need to see how we are gonna draw our tree first
	void bestgirl(chloe* head) {
		chloe* current = head; //temp variable that holds place in linked list
		while(head != NULL) {
			if(current->number == number) {
				prereqpointer = current;
				break;
			}
			head++;
		}
	}
*/
