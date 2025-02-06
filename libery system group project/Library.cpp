#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <fstream>	// for file input/output
#include <sstream>
#include <string>
#include <map>
#include "windows.h"
#include <stdlib.h>


using namespace std;

class Book {
public:
	string id;
	string title;
	string author;
	string publisher;
	string year_published;
	bool available;
	Book(string i, string t, string a, string p, string y, bool status) {
		id = i;
		title = t;
		author = a;
		publisher = p;
		year_published = y;
		available = status;
	}
};

class Borrower {
public:
	string id;
	string firstName;
	string lastName;
	string contact_number;
	vector<Book> borrowedBooks;
	int number_of_books_borrowers;
	Borrower(string i, string fn, string ln, string c, int nb) {
		id = i;
		firstName = fn;
		lastName = ln;
		contact_number = c;
		number_of_books_borrowers = nb;
	}
};

//Global Variables
static vector<Book> books;
static vector<Borrower> borrowers;
static string BookListPath;
static string BorrowListPath;
static int borrowerCount;

//Function Prototype
//R0
vector<string> splitLine(const string& line, char delimiter);
void readCSV(string);
int extractFields(string, char[][101]);
void ImportBookList();
void DisplayMainMenu();

//R1
void DisplayManageBooksMenu();
void DisplayBooks(vector<Book>);
void SearchBooks(string);
void AddBook(string, string, string, string, string);
void RemoveBook(string);
//R5.2
void EditBook(string);

//R2
void DisplayManageborrowerMenu();
void Displayborrowers(vector<Borrower>);
void Searchborrowers(string);
void Addborrower(string, string, string);
void Removeborrower(string);
//R5.3
void EditBorrower(string);

//R3
void borrowbook();
//R4
void returnbook();
//R5.1
void votingSystem();
//R6
void memberList();
//R7
void confirmExit();
//R8
void cleardata();


int main() {
	ImportBookList();
	DisplayMainMenu();
	return 0;
}

//Extended Function
vector<string> splitLine(const string& line, char delimiter) {
	vector<string> fields;
	stringstream ss(line);
	string field;
	bool inQuotes = false;
	while (ss) {
		char c = ss.get();
		if (!ss) break;
		if (c == '"') {
			inQuotes = !inQuotes;
		}
		else if (c == delimiter && !inQuotes) {
			fields.push_back(field);
			field.clear();
		}
		else {
			field += c;
		}
	}
	fields.push_back(field);
	return fields;
}

int extractFields(string line, char fields[][101]) {
	int numFields = 0;
	bool insideQuote = false;
	int start = 0;
	for (int i = 0; i < line.length(); i++) {
		char c = line[i];
		if (c == '"') {
			insideQuote = !insideQuote;
		}
		else if (c == ',' && !insideQuote) {
			line.copy(fields[numFields], i - start, start);
			fields[numFields][i - start] = '\0';
			start = i + 1;
			numFields++;
		}
	}
	line.copy(fields[numFields], line.length() - start, start);
	fields[numFields][line.length() - start] = '\0';
	numFields++;
	return numFields;
}

void readCSV(string filename) {
	fstream inFile;				// for handling file
	string line;				// for storing 1 line in a file
	char fields[10][101] = {};	// for storing extracted fields (assume max. 10 fields per line, each field has max. 100 char)
	int numFields;				// for storing number of fields per line
	int countRecords = 0;		// for counting the number of records in csv file

	inFile.open(filename);
	if (!inFile.is_open()) {
		cout << "Cannot open file \"" << filename << "\"\n";
		return;
	}

	while (getline(inFile, line, '\n')) {				// read line by line until end of file
		numFields = extractFields(line, fields);		// call function to extract fields from the line

		for (int i = 0; i < numFields; i++)				// *** display the fields of this line
			cout << i << ": " << fields[i] << endl;		// *** you should modify this code for fields processing
		cout << "\n\n";									// *** e.g. assign fields to data members of an object

		countRecords++;
	}

	cout << countRecords << " Record(s) imported.\n";
	inFile.close();
}

void ImportBookList() {
	char in;
	fstream inFile1;				// for handling file
	fstream inFile2;				// for handling file

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Import book list from file ? [Y / N] : ";
	cin >> in;
	if (in == 'Y' || in == 'y') {
		do {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "Path of book list file : ";
			cin >> BookListPath;
			cout << "Importing book list . . . " << endl;
			inFile1.open(BookListPath);//input path to open file
			if (!inFile1.is_open()) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << "Cannot open file" << endl;
			}
			else {
				//add books
				string line;
				while (getline(inFile1, line)) {
					vector<string> fields = splitLine(line, ',');
					AddBook(fields[0], fields[1], fields[2], fields[3], fields[4]);
				}
				inFile1.close();
				break;
			}
		} while (true);
		cout << "Done" << endl;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "No book list is imported" << endl;
	}

	//Borrow List
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Import borrower list from file ? [Y / N] : ";
	cin >> in;
	if (in == 'Y' || in == 'y') {
		do {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "Path of borrower list file : ";
			cin >> BorrowListPath;
			cout << "Importing borrower list . . . ";
			inFile2.open(BorrowListPath);//input path to open file
			if (!inFile2.is_open()) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << "Cannot open file" << endl;
			}
			else {
				//add borrowers
				string line;
				while (getline(inFile2, line)) {
					vector<string> fields = splitLine(line, ',');
					Addborrower(fields[0], fields[1], fields[2]);
				}
				inFile2.close();
				break;
			}
		} while (true);
		cout << "Done" << endl;
		BorrowListPath = BorrowListPath;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "No borrower list is imported" << endl;
	}
}

void DisplayMainMenu() {
	cout << endl;
	char ans;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "*** Library Management System ***\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "[1] Manage books\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "[2] Manage borrowers\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
	cout << "[3] Borrow book(s)\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "[4] Return book(s)\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "[5] Voting System\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	cout << "[6] Member List\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "[7] Clear Screen\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "[8] Exit\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "*********************************\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Option (1 - 8): ";
	cin >> ans;
	switch (ans) {
	case '1':
		DisplayManageBooksMenu();
		break;
	case'2':
		DisplayManageborrowerMenu();
		break;
	case'3':
		borrowbook();
		break;
	case '4':
		returnbook();
		break;
	case '5':
		votingSystem();
		break;
	case '6':
		memberList();
		break;
	case '7':
		cleardata();
		break;
	case'8':
		confirmExit();
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Invalid option, please try again." << endl;
		DisplayMainMenu();
	}
}

//R1
void DisplayManageBooksMenu() {
	char ans;
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "********* Manage Books **********\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "      [1] Display books\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "      [2] Search book\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "      [3] Add book\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "      [4] Remove book\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
	cout << "      [5] Edit Book\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "      [6] Back\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "*********************************\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Option (1 - 6): ";

	cin >> ans;
	cin.ignore();
	string i;
	string t;
	string a;
	string p;
	string y;
	string k;
	switch (ans) {
	case '1':
		DisplayBooks(books);
		DisplayManageBooksMenu();
		break;
	case '2':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "Keywords: ";
		cin.ignore();
		getline(cin, k);
		SearchBooks(k);
		DisplayManageBooksMenu();
		break;
	case '3':
		cout << "Use _ to replace the space " << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "ID: ";
		cin >> i;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Title: ";
		cin >> t;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "Author: ";
		cin >> a;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		cout << "Publisher: ";
		cin >> p;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "Year: ";
		cin >> y;
		AddBook(i, y, a, p, y);
		DisplayManageBooksMenu();
		break;
	case '4':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "ID: ";
		cin >> i;
		RemoveBook(i);
		DisplayManageBooksMenu();
		break;
	case'5':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "ID: ";
		cin >> i;
		EditBook(i);
		DisplayManageBooksMenu();
		break;
	case '6':
		DisplayMainMenu();
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Invalid option, please try again." << endl;
		DisplayManageBooksMenu();
	}
}

void DisplayBooks(vector<Book> books) {
	sort(books.begin(), books.end(), [](const Book& a, const Book& b) {//sort by title from a to z
		return a.title < b.title;
	});

	cout << endl;
	for (int i = 0; i < books.size(); i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << left << setw(15) << "ID "
			<< setw(85) << left << "Book Details"
			<< setw(15) << left << "Availability?" << endl;

		cout << left << setw(15) << books[i].id
			<< setw(85) << left << books[i].title
			<< setw(15) << left << (books[i].available ? "Available" : "Not Available") << endl;

		cout << left << setw(15) << "   " << setw(85) << left << books[i].author << endl;
		cout << left << setw(15) << "   " << setw(85) << left << books[i].publisher + " (" + books[i].year_published + ")" << endl;
		cout << endl;
	}
}

void SearchBooks(string keyword) {
	vector<Book> result;
	bool exactMatch = false;
	if (keyword.front() == '"' && keyword.back() == '"') {
		exactMatch = true;
		keyword = keyword.substr(1, keyword.size() - 2);
	}
	for (const auto& book : books) {
		bool isMatch = false;
		vector<string> fields = { book.id, book.title, book.author, book.publisher, book.year_published };
		if (exactMatch) {
			for (const auto& field : fields) {
				if (field.find(keyword) != string::npos) {
					isMatch = true;
					break;
				}
			}
		}
		else {
			vector<string> keywords;
			istringstream iss(keyword);
			for (string s; iss >> s; ) keywords.push_back(s);
			for (const auto& field : fields) {
				for (const auto& k : keywords) {
					if (field.find(k) != string::npos) {
						isMatch = true;
						break;
					}
				}
				if (isMatch) break;
			}
		}
		if (isMatch) result.push_back(book);
	}
	DisplayBooks(result);
}

void AddBook(string id, string title, string author, string publisher, string yearPublished) {
	if (books.size() >= 1000) {
		return;
	}
	if (atoi(yearPublished.c_str()) < 0) {
		return;
	}
	for (auto it = books.begin(); it != books.end(); ++it) {
		if (it->id == id) {
			return;
		}
	}
	books.push_back(Book(id, title, author, publisher, yearPublished, true));
}

void RemoveBook(string id) {
	for (auto it = books.begin(); it != books.end(); ++it) {
		if (it->id == id) {
			books.erase(it); //use erase to Erase elements if the id is match
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Book with ID " << id << " has been removed from the system." << endl;
			return;
		}
		else if (it->id == id) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "Book with ID " << id << " is not available for removal." << endl;
			return;
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "Book with ID " << id << " does not exist in the system." << endl;
}

//R2
void DisplayManageborrowerMenu() {
	cout << endl;
	char ans;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "********* Manage borrowers **********\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "      [1] Display borrowers\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "      [2] Search borrowers\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "      [3] Add borrowers\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "      [4] Remove borrowers\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
	cout << "      [5] Edit borrowers\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "      [6] Back\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "*********************************\n";
	cout << "Option (1 - 6): ";

	cin >> ans;
	string i;
	string fn;
	string ln;
	string c;
	string nb;
	string k;
	switch (ans) {
	case '1':
		Displayborrowers(borrowers);
		DisplayManageborrowerMenu();
		break;
	case '2':
		cout << "Please input ID or Name or Contact Number: ";
		cin.ignore();
		getline(cin, k);
		Searchborrowers(k);
		DisplayManageborrowerMenu();
		break;
	case '3':
		cout << "Use _ to replace the space " << endl;
		cout << "Last Name: ";
		cin >> ln;
		cout << "First Name: ";
		cin >> fn;
		cout << "Contact_number: ";
		cin >> c;
		Addborrower(ln, fn, c);
		DisplayManageborrowerMenu();
		break;
	case '4':
		cout << "ID: ";
		cin >> i;
		Removeborrower(i);
		DisplayManageborrowerMenu();
		break;
	case'5':
		cout << "ID: ";
		cin >> i;
		EditBorrower(i);
		DisplayManageborrowerMenu();
		break;
	case '6':
		DisplayMainMenu();
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Invalid option, please try again." << endl;
		DisplayManageborrowerMenu();
	}
}

void Displayborrowers(vector<Borrower>borrowers) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
	cout << endl << "Borrowers Details" << endl;
	for (int i = 0; i < borrowers.size(); i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
		cout << setw(10) << left << "ID"
			<< setw(40) << left << "Name"
			<< setw(20) << left << "Contact number"
			<< setw(30) << left << "Number of books borrrows" << endl;

		cout << setw(10) << left << borrowers[i].id
			<< setw(40) << (borrowers[i].lastName + " " + borrowers[i].firstName)
			<< setw(20) << left << borrowers[i].contact_number
			<< setw(30) << left << to_string(borrowers[i].number_of_books_borrowers) << endl;

		cout << endl;
	}
}

void Searchborrowers(string keyword) {
	vector<Borrower> result;
	bool exactMatch = false;
	if (keyword.front() == '"' && keyword.back() == '"') {
		exactMatch = true;
		keyword = keyword.substr(1, keyword.size() - 2);
	}
	for (const auto& Borrower : borrowers) {
		bool isMatch = false;
		vector<string> fields = { Borrower.id,Borrower.firstName,Borrower.lastName,Borrower.contact_number };
		if (exactMatch) {
			for (const auto& field : fields) {
				if (field.find(keyword) != string::npos) {
					isMatch = true;
					break;
				}
			}
		}
		else {
			vector<string> keywords;
			istringstream iss(keyword);
			for (string s; iss >> s; ) keywords.push_back(s);
			for (const auto& field : fields) {
				for (const auto& k : keywords) {
					if (field.find(k) != string::npos) {
						isMatch = true;
						break;
					}
				}
				if (isMatch) break;
			}
		}
		if (isMatch) result.push_back(Borrower);
	}
	Displayborrowers(result);
}

void Addborrower(string LastName, string FirstName, string Contact_number) {
	// Check Borrower number length
	if (borrowerCount >= 500) {
		return;
	}
	// Check last name length
	if (LastName.length() > 10) {
		cout << "Error: Last name must be at most 10 characters long." << endl;
		return;
	}

	// Check first name length
	if (FirstName.length() > 30) {
		cout << "Error: First name must be at most 30 characters long." << endl;
		return;
	}
	// Check Contact number
	if (Contact_number.length() != 8 || (Contact_number[0] != '2' && Contact_number[0] != '3' && Contact_number[0] != '5' && Contact_number[0] != '6' && Contact_number[0] != '9')) {
		cout << "Error: Contact number must be an 8-digit number that begins with 2, 3, 5, 6, or 9." << endl;
		return;
	}

	string id = "";

	// Convert last name to upper case
	for (int i = 0; i < LastName.length(); i++)
		LastName[i] = toupper(LastName[i]);

	// Capitalize first name
	bool newWord = true;
	for (int i = 0; i < FirstName.length(); i++) {
		if (newWord)
			FirstName[i] = toupper(FirstName[i]);
		else
			FirstName[i] = tolower(FirstName[i]);
		newWord = (FirstName[i] == ' ');
	}
	// set id to be HKCCxxxx
	if (borrowerCount < 10) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		id = "HKCC000" + to_string(borrowerCount + 1);
	}
	else if (borrowerCount < 100) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		id = "HKCC00" + to_string(borrowerCount + 1);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		id = "HKCC0" + to_string(borrowerCount + 1);
	}
	borrowers.push_back(Borrower(id, FirstName, LastName, Contact_number, 0));
	++borrowerCount;
}

void Removeborrower(string id) {
	for (auto it = borrowers.begin(); it != borrowers.end(); ++it) {
		if (it->id == id) {
			borrowers.erase(it);//same with RemoveBook
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "borrower with ID " << id << " has been removed from the system." << endl;
			return;
		}
		else if (it->id == id) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "borrower with ID " << id << " is not available for removal." << endl;
			return;
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "borrower with ID " << id << " does not exist in the system." << endl;
}
//R3
void borrowbook() {
	cout << endl;
	string borrower_id, book_id;
	bool borrower_found = false, book_found = false;
	int books_to_borrow = 0;
	const int MAX_BORROWQUOTA = 5;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "Enter borrower ID: ";
	cin >> borrower_id;

	// Find the borrower
	for (int i = 0; i < borrowers.size(); i++) {
		if (i >= borrowers.size()) {
			break;
		}
		if (borrowers[i].id == borrower_id) {
			borrower_found = true;
			int remaining_quota = MAX_BORROWQUOTA - borrowers[i].borrowedBooks.size();// Check if the borrower has enough quota
			if (remaining_quota <= 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "This borrower has reached the maximum borrowing limit." << endl;
				return;
			}
			// Enter the books id
			while (books_to_borrow < remaining_quota) { //check if the book to borrow smaller than the max quota 5
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << "Enter book ID to borrow (enter 'D' to finish): ";
				cin >> book_id;
				if (book_id == "D" || book_id == "d") {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
					cout << "You have borrowed " << books_to_borrow << " books." << endl;
					break;
				}
				// Find the book
				book_found = false; // Reset book_found to false
				for (int j = 0; j < books.size(); j++) {
					if (books[j].id == book_id) {
						book_found = true;
						// Check if the book is available
						if (!books[j].available) {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
							cout << "The book is not available for borrowing." << endl;
							break;
						}
						// Borrow the book
						books[j].available = false;
						borrowers[i].borrowedBooks.push_back(books[j]);
						if (borrowers[i].number_of_books_borrowers < MAX_BORROWQUOTA) {//Check if borrower has reached the maximum borrowing limit
							borrowers[i].number_of_books_borrowers++;//Update the number of books borrowed
						}
						books_to_borrow++;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						cout << "Book borrowed successfully." << endl;
						break;
					}
				}
				if (!book_found) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
					cout << "Invalid book ID." << endl;

				}
			}

		}
	}
	if (!borrower_found) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Invalid borrower ID." << endl;
	}
	DisplayMainMenu();
}

//R4
void returnbook() {
	string borrower_id, book_id;
	bool borrower_found = false, book_found = false;

	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "Enter borrower ID: ";
	cin >> borrower_id;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout << "Enter book ID: ";
	cin >> book_id;

	// Find the borrower
	for (int i = 0; i < borrowers.size(); i++) {
		if (borrowers[i].id == borrower_id) {
			borrower_found = true;

			// Find the book in the borrower's list of borrowed books
			for (int j = 0; j < borrowers[i].borrowedBooks.size(); j++) {
				if (borrowers[i].borrowedBooks[j].id == book_id) {
					book_found = true;

					// Mark the book as available again
					borrowers[i].borrowedBooks[j].available = true;

					// Remove the book from the borrower's list of borrowed books
					borrowers[i].borrowedBooks.erase(borrowers[i].borrowedBooks.begin() + j);
					borrowers[i].number_of_books_borrowers--;//Update the number of books borrowed
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
					cout << "Book returned successfully." << endl;
					break;
				}
			}

			break;
		}
	}

	if (!borrower_found) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Invalid borrower ID." << endl;
	}
	else if (!book_found) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "The borrower did not borrow the book." << endl;
	}
	DisplayMainMenu();
}

//R5
void votingSystem() {
	map<string, int> candidates;
	candidates["1"] = 0;
	candidates["2"] = 0;
	candidates["3"] = 0;
	// to vote
	string choice;
	while (true) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
		string c1 = "The Old Man and the sea";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		string c2 = "The Twilight";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		string c3 = "Three Days To See";

		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "Favorite Book Poll" << endl;
		cout << "Please choose your favorite book from the following books." << endl;
		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
		cout << setw(30) << left << c1 << " - Press [1]" << endl;
		cout << setw(30) << left << c2 << " - Press [2]" << endl;
		cout << setw(30) << left << c3 << " - Press [3]" << endl;
		cout << setw(30) << "See result" << " - Press [-1]" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "**********************************************************" << endl;
		cout << "Please enter your choice: ";
		cin >> choice;
		if (choice == "-1") {
			break;
		}
		else if (candidates.find(choice) == candidates.end()) {
			cout << "Invalid option, please try again." << endl << endl;
			continue; // continue the loop to allow user to enter again
		}
		else {
			candidates[choice]++;
			cout << "You have successfully voted for option " << choice << "." << endl << endl;
		}
	}


	// show poll results
	cout << endl << "Voting results:" << endl;
	for (const auto& candidate : candidates) {
		cout << "Option " << candidate.first << " won " << candidate.second << " votes." << endl;
	}
	cout << endl;
	DisplayMainMenu();
}

//R5.2
void EditBook(string id) {
	// Find the book with the given ID
	Book* book = nullptr;
	for (auto& b : books) {
		if (b.id == id) {
			book = &b;
			break;
		}
	}
	if (book == nullptr) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Book not found\n";
		return;
	}

	// Display the edit menu
	char ans;
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "*** Edit Book ***\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout <<
		"[1] Edit title\n"
		<<
		"[2] Edit author\n"
		<<
		"[3] Edit publisher\n"
		<<
		"[4] Edit year published\n"
		<<
		"[5] Back\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "***************\n";
	cout << "Option (1 - 5): ";
	cin >> ans;
	cin.ignore();

	// Update the selected field
	string newValue;
	switch (ans) {
	case '1':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "New title: ";
		getline(cin, newValue);
		book->title = newValue;
		EditBook(id);
		break;
	case '2':
		cout << "New author: ";
		getline(cin, newValue);
		book->author = newValue;
		EditBook(id);
		break;
	case '3':
		cout << "New publisher: ";
		getline(cin, newValue);
		book->publisher = newValue;
		EditBook(id);
		break;
	case '4':
		cout << "New year published: ";
		getline(cin, newValue);
		book->year_published = newValue;
		EditBook(id);
		break;
	case '5':
		return;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Invalid option, please try again." << endl;
		EditBook(id);
		break;
	}
}

//R5.3
void EditBorrower(string id) {
	// Find the borrower with the given ID
	Borrower* borrower = nullptr;
	for (auto& b : borrowers) {
		if (b.id == id) {
			borrower = &b;
			break;
		}
	}
	if (borrower == nullptr) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Borrower not found\n";
		return;
	}

	// Display the edit menu
	char ans;
	cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout <<
		"*** Edit Borrower ***\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	cout <<
		"[1] Edit first name\n"
		<<
		"[2] Edit last name\n"
		<<
		"[3] Edit contact number\n"
		<<
		"[4] Back\n"
		<<
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "***************\n"
		<< "Option (1 - 4): ";
	cin >> ans;
	cin.ignore();

	// Update the selected field
	string newValue;
	switch (ans) {
	case '1':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "New first name: ";
		getline(cin, newValue);
		borrower->firstName = newValue;
		EditBorrower(id);
		break;
	case '2':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "New last name: ";
		getline(cin, newValue);
		borrower->lastName = newValue;
		EditBorrower(id);
		break;
	case '3':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << "New contact number: ";
		getline(cin, newValue);
		borrower->contact_number = newValue;
		EditBorrower(id);
		break;
	case '4':
		return;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Invalid option, please try again." << endl;
		EditBorrower(id);
		break;
	}
}

//R6 
void memberList() {
	cout << endl;
	cout << right;
	cout << setw(25) << "Stuent Name" << setw(15) << "Stuent ID" << setw(15) << "Class" << endl;
	cout << setw(25) << "-----------" << setw(15) << "---------" << setw(15) << "-----" << endl;
	cout << setw(25) << "Group Leader - Ma Kai Lun" << setw(15) << "22025202A" << setw(15) << "206C" << endl;
	DisplayMainMenu();
}

//R7
void cleardata() {
	cout << endl;
	char clear;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Confirm to clear output:" << endl;
	cout << "Confirm clear output, please input [Y / y] or [N / n] to keep output. " << endl;
	cin >> clear;
	switch (clear)
	{
	case'y':
		system("cls");
		DisplayMainMenu();
		break;
	case'Y':
		system("cls");
		DisplayMainMenu();
		break;
	case'n':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "You will keep the output in this system " << endl;
		DisplayMainMenu();
		break;
	case'N':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "You will keep the output in this system " << endl;
		DisplayMainMenu();
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Incorrect input please enter again  " << endl;
		cleardata();
		break;
	}
}

//R8
void confirmExit() {
	cout << endl;
	char exit;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Confirm to exit system:" << endl;
	cout << "Confirm exit system, please input [Y / y] or [N / n] to Stay in the system, " << endl;
	cin >> exit;
	switch (exit)
	{
	case'y':
		break;
	case'Y':
		break;
	case'n':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "You will keep stay in this system " << endl;
		DisplayMainMenu();
		break;
	case'N':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "You will keep stay in this system " << endl;
		DisplayMainMenu();
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "Incorrect input please enter again  " << endl;
		confirmExit();
		break;
	}
}

