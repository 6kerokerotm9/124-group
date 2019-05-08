/*
Joshua Sjah
Justin Sanchez 
Jayush Aurora
Minh Ngo

This program (for now) is the data structure that will be used as well as the parsing function to read values from the file and put the values into a data structure
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

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

	//print function 
	void print() {
		cout << "Order number: " << number << endl;
		cout << "Name of command: " << name << endl;
		cout << "Prereqs: ";
		for(int i=0; i<prereqcounter; i++) { //prints out the appropriate amount of prereqs
			cout << prereq[i] << " ";
		}
		cout << endl;
		cout << "Time taken: " << time << endl;
		cout << endl;
	}
};

void parse(chloe * &tail) {
	ifstream file;
	file.open("chloe.txt");
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
