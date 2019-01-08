/******************************************************************************

Online C++ Compiler.
Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
//includes and libraries for execute 
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <algorithm>

//FOR Threads
#include <condition_variable>
#include <mutex>
#include <thread>

using namespace std;
//--------------------------------------------------------------------------------------------------
// Name: Producer / Consumer 
// Created By: ERIC SANCHEZ
// Created On: January ,2019
//---------------------------------------------------------------------------------------------------

void presentaMenuGeneral();
//Declare vectors
vector <string> vValues;
std::mutex mutex_;
std::condition_variable condVar;
string contenido;
string contenido2;

//Functions for Split Vectors
std::vector<std::string> split_istringstream(std::string str) {
	std::vector<std::string> resultado;
	std::istringstream isstream(str);
	std::string palabra;
	while (isstream >> palabra) {
		resultado.push_back(palabra);
	}
	return resultado;
}

std::vector<std::string> split_getline(std::string str, char delim) {
	std::vector<std::string> resultado;
	std::istringstream isstream(str);
	std::string palabra;
	while (std::getline(isstream, palabra, delim)) {
		resultado.push_back(palabra);
	}
	return resultado;
}

//   Functions for THREADS 

void doTheWork() {
	ifstream fs(contenido2.c_str(), ios::in);
	char linea[128];
	string values;
	long contador = 0L;
	if (fs.fail())
		cerr << "The File Not Exist" << endl;
	else
		while (!fs.eof()) {
			fs.getline(linea, sizeof(linea));

			std::string str(linea);
			std::vector<std::string> v;
			std::clock_t    start;

			start = std::clock();
			//cicle for separate str
			for (int i = 0; i < 1000000; i++) {
				v = split_getline(str, ' ');
			}

			//print 
			for (int i = 0; i < 1000000; i++) {
				v = split_istringstream(str);
			}
			//Print values vector to sort
			for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
				std::cout << *it << std::endl;
			}


			std::sort(v.begin(), v.end());
			std::cout << "myvector sorting:";
			for (std::vector<string>::iterator it = v.begin(); it != v.end(); ++it) {
				std::cout << ' ' << *it;
				std::cout << '\n';
			}

			//vValues.push_back(linea);

			cout << linea << endl;


			if ((++contador % 24) == 0) {
				cout << "continue...";
				cin.get();
			}
		}
	fs.close();
	std::cout << "Processing shared data." << std::endl;
}

//function for waiting to work in the trteads
void waitingForWork() {
	std::cout << "Worker: Waiting for work." << std::endl;

	std::unique_lock<std::mutex> lck(mutex_);
	condVar.wait(lck);
	doTheWork();
	std::cout << "Work done." << std::endl;
}
//funtion for set data for order vector 
//This function read file and put data into vector and sorted this
void setDataReady() {
	ifstream fs(contenido2.c_str(), ios::in);
	char linea[128];
	string values;
	long contador = 0L;
	if (fs.fail())
		cerr << "The File Not Exist" << endl;
	else
		while (!fs.eof()) {
			fs.getline(linea, sizeof(linea));

			std::string str(linea);
			std::vector<std::string> v;
			std::clock_t    start;

			start = std::clock();
			for (int i = 0; i < 1000000; i++) {
				v = split_getline(str, ' ');
			}

			//print 
			for (int i = 0; i < 1000000; i++) {
				v = split_istringstream(str);
			}

			//print order of vector
			std::sort(v.begin(), v.end());
			std::cout << "myvector sorting:";
			for (std::vector<string>::iterator it = v.begin(); it != v.end(); ++it) {
				std::cout << ' ' << *it;
				std::cout << '\n';
			}

			//Print line of file
			cout << linea << endl;


			if ((++contador % 24) == 0) {
				cout << "continuar...";
				cin.get();
			}
		}
	fs.close();


	std::cout << "Sender: Data is ready." << std::endl;
	condVar.notify_one();
}

bool myfunction(int i, int j) { return (i<j); }
struct myclass {
	bool operator() (int i, int j) { return (i<j); }
} myobject;

//Source code
//int main(int argc, char *argv[]) value main with argc and argv parameters with the name of file
int main() {
	//when intro program show the menu for TXT operations
	// in the opcion 1 create a file and option 2 read this file line for line until eOF file
	presentaMenuGeneral();
	std::cout << std::endl;
	std::thread t1(waitingForWork); //execute first thread
	std::thread t2(setDataReady);  //execute second thread
	t1.join();
	t2.join();

	std::cout << std::endl;
}

void presentaMenuGeneral() {

	int opcion;
	do {
		system("cls");
		cout << "Principal Menu" << endl;
		cout << "========================" << endl;
		cout << "[1]Create File TXT" << endl;
		cout << "[2]Open File TXT" << endl;
		cout << "[3]Edit File TXT" << endl;
		cout << "[0]Exit of program" << endl;
		cout << "========================" << endl;
		cout << "Please (number) Option: ";
		cin >> opcion;
		switch (opcion) {
		case 0:
			break;
		case 1:
		{
			cout << "Enter the name of the file to create (without the .txt) ";
			cin.ignore();
			getline(cin, contenido2);
			cout << "Enter Content of File ";
			getline(cin, contenido);
			contenido2 += ".txt";
			ofstream fs(contenido2.c_str());
			fs << contenido << endl;
			fs.close();
			cout << "File Created Succesfully" << endl;
			//system("pause");
			break;
		}
		case 2:
		{
			cout << "Enter the name of the file to read (without the .txt) ";
			cin.ignore();
			getline(cin, contenido2);
			contenido2 += ".txt";
			//ifstream fs(contenido2.c_str(), ios:: in );
			//system("pause");
			break;
		}
		case 3:
		{
			cout << "Enter the name of the file to updated (without the .txt): ";
			cin.ignore();
			getline(cin, contenido2);
			contenido2 += ".txt";
			cout << "Enter NEW Content of File: ";
			getline(cin, contenido);
			ofstream fs(contenido2.c_str(), ios::out);
			fs << contenido << endl;
			fs.close();
			cout << "File Modified Succesfully" << endl;
			//system("pause");
			break;
		}
		default:
			cout << "\nEl numero de opcion escrito no es valido\n" << endl;
			//system("pause");
		}
	} while (opcion != 0);
}
