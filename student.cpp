#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>  // for system()

using namespace std;

// Student Class

class student {
private:
    string student_name;
    int student_id;
    int age;
    string course;
    string grade;
    string address;
    string phone_number;
    int marks;

public:
    student() {}

    student(string name, int id, int age, string course, int marks, string address, string number) {
        student_name = name;
        student_id = id;
        this->age = age;
        this->course = course;
        this->marks = marks;
        this->grade = calculateGrade(marks);
        this->address = address;
        phone_number = number;
    }

    // Getter function to acess the private data members
    
    int getid() const { return student_id; }
    string getname() const { return student_name; }
    int getage() const { return age; }
    string getcourse() const { return course; }
    string getgrade() const { return grade; }
    string getaddress() const { return address; }
    string getnum() const { return phone_number; }
    int getmarks() const { return marks; }

    // Setter function to edit few data members if required

    void setname(string name) { student_name = name; }
    void setid(int id) { student_id = id; }
    void setage(int a) { age = a; }
    void setcourse(string c) { course = c; }
    void setmarks(int m) { 
        marks = m; 
        grade = calculateGrade(marks);
    }
    void setaddress(string a) { address = a; }
    void setnum(string n) { phone_number = n; }

    void display() const {
        cout << "----------------------------------------\n";
        cout << "ID           : " << student_id << endl;
        cout << "Name         : " << student_name << endl;
        cout << "Age          : " << age << endl;
        cout << "Course       : " << course << endl;
        cout << "Marks        : " << marks << endl;
        cout << "Grade        : " << grade << endl;
        cout << "Address      : " << address << endl;
        cout << "Phone Number : " << phone_number << endl;
    }

    string toLine() const {
        return student_name + "|" + to_string(student_id) + "|" + to_string(age) + "|" + course + "|" 
               + to_string(marks) + "|" + grade + "|" + address + "|" + phone_number;
    }

    static student fromLine(const string& line) {
        stringstream ss(line);
        string fields[8];
        int i = 0;
        while (getline(ss, fields[i], '|') && i < 8) i++;

        if (i != 8) {
            throw invalid_argument("Invalid student record: incorrect number of fields");
        }

        student s;
        try {
            s.student_name = fields[0];
            s.student_id = stoi(fields[1]);
            s.age = stoi(fields[2]);
            s.course = fields[3];
            s.marks = stoi(fields[4]);
            s.grade = fields[5];
            s.address = fields[6];
            s.phone_number = fields[7];
        } catch (...) {
            throw invalid_argument("Invalid student record: non-numeric fields where numeric expected");
        }

        return s;
    }

    static string calculateGrade(int marks) {
        if (marks >= 90) return "A+";
        else if (marks >= 80) return "A";
        else if (marks >= 70) return "B";
        else if (marks >= 60) return "C";
        else if (marks >= 50) return "D";
        else return "F";
    }
};

//clear screen function

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "Press Enter to continue...";
    cin.ignore(); //ignore input buffer
}

void saveAllStudents(const vector<student>& students, const string& filename) {
    ofstream file(filename);
    for (const student& s : students) {
        file << s.toLine() << endl;
    }
    file.close();
}

vector<student> loadAllStudents(const string& filename) {
    vector<student> students;
    ifstream file(filename);
    string line;

    int count = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;

        try {
            students.push_back(student::fromLine(line));
            count++;
        } catch (const exception& e) {
            cerr << "Warning: Skipping invalid line in file: " << line << endl;
        }
    }
    file.close();

    cout << "🔹 Loaded " << count << " student(s) from file.\n";
    return students;
}

//login info

bool login() {
    string password;
    cout << "Enter admin password: ";
    cin >> password;
    cin.ignore();
    return password == "admin123"; //password to acess
}

void displayAll(const vector<student>& students) {
    if (students.empty()) {
        cout << " No student data available.\n";
        return;
    }
    for (const student& s : students) {
        s.display();
    }
}

//function to add new student

void addStudent(vector<student>& students, const string& filename) {
    string name, course, address, phone;
    int id, age, marks;

    cout << "\nEnter student details:\n";
    cout << "ID: "; cin >> id; cin.ignore();
    cout << "Name: "; getline(cin, name);
    cout << "Age: "; cin >> age; cin.ignore();
    cout << "Course: "; getline(cin, course);
    cout << "Marks (0-100): "; cin >> marks; cin.ignore();
    cout << "Address: "; getline(cin, address);
    cout << "Phone: "; getline(cin, phone);

    student s(name, id, age, course, marks, address, phone);
    students.push_back(s);
    saveAllStudents(students, filename);

    cout << " Student added and saved.\n";
}

//function to edit the students either by name or id.

void editStudentById(vector<student>& students, const string& filename) {
    int id;
    cout << "\nEnter student ID to edit: ";
    cin >> id;
    cin.ignore();

    for (student& s : students) {
        if (s.getid() == id) {
            cout << "\nEditing student: " << s.getname() << endl;

            string name, course, address, phone;
            int age, marks;

            cout << "New name (" << s.getname() << "): "; getline(cin, name);
            cout << "New age (" << s.getage() << "): "; cin >> age; cin.ignore();
            cout << "New course (" << s.getcourse() << "): "; getline(cin, course);
            cout << "New marks (" << s.getmarks() << "): "; cin >> marks; cin.ignore();
            cout << "New address (" << s.getaddress() << "): "; getline(cin, address);
            cout << "New phone (" << s.getnum() << "): "; getline(cin, phone);

            s.setname(name);
            s.setage(age);
            s.setcourse(course);
            s.setmarks(marks);
            s.setaddress(address);
            s.setnum(phone);

            saveAllStudents(students, filename);
            cout << " Student updated and saved.\n";
            return;
        }
    }

    cout << "Student ID not found.\n";
}

//function to search student either by name or id

void searchStudent(const vector<student>& students) {
    int choice;
    cout << "\nSearch by:\n1. ID\n2. Name\nEnter choice: ";
    cin >> choice;
    cin.ignore();

    bool found = false;
    if (choice == 1) {
        int id;
        cout << "Enter ID: ";
        cin >> id;

        for (const student& s : students) {
            if (s.getid() == id) {
                cout << "\n Student found:\n";
                s.display();
                found = true;
                break;
            }
        }
    } else if (choice == 2) {
        string name;
        cout << "Enter name: ";
        getline(cin, name);

        for (const student& s : students) {
            if (s.getname() == name) {
                cout << "\n🔍 Student found:\n";
                s.display();
                found = true;
            }
        }
    } else {
        cout << " Invalid option.\n";
    }

    if (!found) {
        cout << " Student not found.\n";
    }
}

bool compareByName(const student& a, const student& b) {
    return a.getname() < b.getname();
}

bool compareByGrade(const student& a, const student& b) {
    return a.getgrade() > b.getgrade(); // descending
}

//fnction for sorting students either by name or by grade

void sortStudents(vector<student>& students) {
    int choice;
    cout << "\nSort by:\n1. Name (A-Z)\n2. Grade (High to Low)\nEnter choice: ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        sort(students.begin(), students.end(), compareByName);
    } else if (choice == 2) {
        sort(students.begin(), students.end(), compareByGrade);
    } else {
        cout << " Invalid choice.\n";
        return;
    }

    cout << "\n Sorted Students:\n";
    for (const student& s : students) s.display();
}

//function to export the text file to comma seperated value (csv)

void exportToCSV(const vector<student>& students, const string& filename) {
    ofstream file(filename);
    file << "ID,Name,Age,Course,Marks,Grade,Address,Phone\n";

    for (const student& s : students) {
        file << s.getid() << "," << s.getname() << "," << s.getage() << ","
             << s.getcourse() << "," << s.getmarks() << "," << s.getgrade() << ","
             << s.getaddress() << "," << s.getnum() << "\n";
    }

    file.close();
    cout << " Exported to " << filename << endl;
}

// Main Function

int main() {
    if (!login()) {
        cout << " Access denied.\n";
        return 0;
    }

    const string filename = "students.txt";
    vector<student> students = loadAllStudents(filename);

    int choice;
    do {
        cout << "\n Student Management System\n";
        cout << "1. View all students\n";
        cout << "2. Add new student\n";
        cout << "3. Edit student by ID\n";
        cout << "4. Search student\n";
        cout << "5. Sort students\n";
        cout << "6. Export to CSV\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                displayAll(students);
                pause();
                break;
            case 2:
                addStudent(students, filename);
                pause();
                break;
            case 3:
                editStudentById(students, filename);
                pause();
                break;
            case 4:
                searchStudent(students);
                pause();
                break;
            case 5:
                sortStudents(students);
                pause();
                break;
            case 6:
                exportToCSV(students, "students.csv");
                pause();
                break;
            case 0:
                cout << " Exiting...\n";
                break;
            default:
                cout << " Invalid choice.\n";
                pause();
        }

    } while (choice != 0);

    return 0;
}
