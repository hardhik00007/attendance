
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;

const string DATA_FILE = "attendance.txt";
const char DELIM = '|';

struct Student {
    int id;
    string name;
    int totalClasses;
    int attendedClasses;

    double getAttendancePercentage() const {
        if (totalClasses == 0) return 0;
        return (double)attendedClasses / totalClasses * 100;
    }
};

// Utility to get today's date
string today_date() {
    time_t t = time(nullptr);
    tm *lt = localtime(&t);
    char buf[11];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
    return string(buf);
}

// Read data from file
vector<Student> loadData() {
    vector<Student> students;
    ifstream file(DATA_FILE);
    if (!file.is_open()) return students;

    Student s;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        getline(ss, temp, DELIM);
        s.id = stoi(temp);
        getline(ss, s.name, DELIM);
        getline(ss, temp, DELIM);
        s.totalClasses = stoi(temp);
        getline(ss, temp, DELIM);
        s.attendedClasses = stoi(temp);
        students.push_back(s);
    }
    file.close();
    return students;
}

// Write data to file
void saveData(const vector<Student>& students) {
    ofstream file(DATA_FILE, ios::trunc);
    for (const auto &s : students) {
        file << s.id << DELIM << s.name << DELIM 
             << s.totalClasses << DELIM << s.attendedClasses << "\n";
    }
    file.close();
}

// Add new student
void addStudent(vector<Student> &students) {
    Student s;
    cout << "Enter Student ID: ";
    cin >> s.id;
    cout << "Enter Student Name: ";
    cin.ignore();
    getline(cin, s.name);
    s.totalClasses = 0;
    s.attendedClasses = 0;
    students.push_back(s);
    saveData(students);
    cout << "Student added successfully!\n";
}

// Take attendance for a specific class
void takeAttendance(vector<Student> &students) {
    cout << "\nTaking attendance for date: " << today_date() << "\n";
    for (auto &s : students) {
        char present;
        cout << "Is " << s.name << " present? (y/n): ";
        cin >> present;
        s.totalClasses++;
        if (present == 'y' || present == 'Y')
            s.attendedClasses++;
    }
    saveData(students);
    cout << "\nAttendance updated for " << today_date() << "!\n";
}

// Add attendance manually for a specific date/class
void addPastAttendance(vector<Student> &students) {
    string date;
    cout << "Enter past date (YYYY-MM-DD): ";
    cin >> date;
    cout << "\nAdding attendance for " << date << "\n";
    for (auto &s : students) {
        char present;
        cout << "Was " << s.name << " present on " << date << "? (y/n): ";
        cin >> present;
        s.totalClasses++;
        if (present == 'y' || present == 'Y')
            s.attendedClasses++;
    }
    saveData(students);
    cout << "Past attendance added successfully!\n";
}

// Delete a student record
void deleteStudent(vector<Student> &students) {
    int id;
    cout << "Enter student ID to delete: ";
    cin >> id;
    bool found = false;
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->id == id) {
            found = true;
            students.erase(it);
            break;
        }
    }
    if (found) {
        saveData(students);
        cout << "Student deleted successfully!\n";
    } else {
        cout << "Student not found!\n";
    }
}

// View attendance report
void viewAttendance(const vector<Student> &students) {
    cout << "\n------------------ Attendance Report ------------------\n";
    cout << left << setw(10) << "ID"
         << setw(20) << "Name"
         << setw(20) << "Attendance (%)"
         << setw(15) << "Status\n";
    cout << "-------------------------------------------------------\n";
    for (const auto &s : students) {
        double percent = s.getAttendancePercentage();
        cout << left << setw(10) << s.id
             << setw(20) << s.name
             << setw(20) << fixed << setprecision(2) << percent
             << setw(15) << (percent >= 75 ? "Regular" : "Warning") << "\n";
    }
}

// Main menu
void menu() {
    vector<Student> students = loadData();
    int choice;
    do {
        cout << "\n========== Attendance Management System ==========\n";
        cout << "1. Add New Student\n";
        cout << "2. Take Attendance (Today)\n";
        cout << "3. Add Attendance from Past\n";
        cout << "4. View Attendance Report\n";
        cout << "5. Delete Student Record\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addStudent(students); break;
            case 2: takeAttendance(students); break;
            case 3: addPastAttendance(students); break;
            case 4: viewAttendance(students); break;
            case 5: deleteStudent(students); break;
            case 6: cout << "Exiting system...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);
}

int main() {
    menu();
    return 0;
}
