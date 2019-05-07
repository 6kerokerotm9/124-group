#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct chloe
{
	int number; //command order number
	string name; //name of command
	int prereq[]; //for now a list of the numbers for the prerequisites that will be used to determine
	int prereqcounter;
	double time; //time value
	
	chloe* prereqpointer; //points to prerequisite 
	chloe* next; //points to next order to create a linked list

	//function that takes the prereq int from the file and sets the prereq variable for linked list purposes
	//takes argument head that is the head pointer for the linked list
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

	void print() {
		cout << "Order number: " << number << endl;
		cout << "Name of command: " << name << endl;
		cout << "Prereqs: ";
		for(int i=0; i<prereqcounter; i++) {
			cout << prereq[i] << " ";
		}
		cout << endl;
		cout << "Time taken: " << time << endl;
	}
};

void parse(chloe * &tail) {
	ifstream file;
	file.open("chloe.txt");
	string line;
	string temp;  	
	int counter = 1; //loop counter
	//int othercounter = 0; //0 if on the prereq portion and 1 if it is looking for time
	while(getline(file, line)) {
		istringstream ss(line);
		counter = 0;
		while(getline(ss, temp, ':')) {
			istringstream ss2(temp);
			switch(counter) {
				case 0:
					ss2 >> tail->number;
					break;
				case 1:
					getline(ss2, tail->name);
					break;
				case 2:
					for(int i=0; i<temp.size(); i++) {
						if(temp[i] == ',') {
							istringstream ss3(temp.substr(0,1));
							ss3 >> tail->prereq[0];
							istringstream ss4(temp.substr(2));
							ss2 >> tail->prereq[1];	
							tail->prereqcounter = 2;
						}
						else if(i+1 == temp.size()) {	
							ss2 >> tail->prereq[0];
							tail->prereqcounter = 1;
						}
					}
					break;
				case 3:
					ss2 >> tail->time;
					break;
			}
			counter++;
		}
		tail = tail->next;
		tail->next = new chloe;
	}
	file.close();
}

int main()
{
	chloe * head = new chloe;
	chloe * tail = head;
	tail->next = new chloe;  
	parse(tail);
	chloe * temp = head;
	while(temp != tail) {
		temp->print();
		temp = temp->next;
	}
}
