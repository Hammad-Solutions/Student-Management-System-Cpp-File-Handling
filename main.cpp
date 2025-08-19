#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

// ====================== Utility Functions ======================
void saveAdmin() {
    ofstream fout("admin.txt");
    fout << "admin admin\n"; // default admin: username=admin, password=admin
    fout.close();
}

bool checkAdminLogin(string u, string p) {
    ifstream fin("admin.txt");
    string user, pass;
    while (fin >> user >> pass) {
        if (user == u && pass == p) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

bool checkTeacherLogin(string u, string p, string &tname) {
    ifstream fin("teachers.txt");
    string user, pass, name;
    while (fin >> user >> pass) {
        getline(fin, name); // read full name
        if (user == u && pass == p) {
            tname = name;
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

bool checkStudentLogin(string sid, string sname) {
    ifstream fin("students.txt");
    string id, name;
    while (fin >> id) {
        getline(fin, name);
        if (id == sid && name == " " + sname) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

// ====================== Admin Functions ======================
void addTeacher() {
    string u, p, n;
    cout << "Enter Teacher Username: ";
    cin >> u;
    cout << "Enter Password: ";
    cin >> p;
    cin.ignore();
    cout << "Enter Full Name: ";
    getline(cin, n);

    ofstream fout("teachers.txt", ios::app);
    fout << u << " " << p << " " << n << endl;
    fout.close();

    cout << "Teacher Added!\n";
}

void listTeachers() {
    ifstream fin("teachers.txt");
    string u, p, n;
    cout << "--- Teachers ---\n";
    while (fin >> u >> p) {
        getline(fin, n);
        cout << "Username: " << u << " | Name: " << n << endl;
    }
    fin.close();
}

// ====================== Teacher Functions ======================
void addStudent() {
    string id, name;
    cout << "Enter Student ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter Student Name: ";
    getline(cin, name);

    ofstream fout("students.txt", ios::app);
    fout << id << " " << name << endl;
    fout.close();
    cout << "Student Added!\n";
}

void listStudents() {
    ifstream fin("students.txt");
    string id, name;
    cout << "--- Students ---\n";
    while (fin >> id) {
        getline(fin, name);
        cout << "ID: " << id << " | Name:" << name << endl;
    }
    fin.close();
}

void removeStudent() {
    string id;
    cout << "Enter Student ID to remove: ";
    cin >> id;

    ifstream fin("students.txt");
    ofstream temp("temp.txt");

    string sid, name;
    bool found = false;
    while (fin >> sid) {
        getline(fin, name);
        if (sid != id) {
            temp << sid << name << endl;
        } else {
            found = true;
        }
    }
    fin.close();
    temp.close();
    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found) cout << "Student removed!\n";
    else cout << "Student not found.\n";
}

void markAttendance() {
    string date, id;
    int present;
    cout << "Enter Date (e.g. 2025-08-17): ";
    cin >> date;

    ifstream fin("students.txt");
    ofstream fout("attendance.txt", ios::app);

    string sid, name;
    while (fin >> sid) {
        getline(fin, name);
        cout << "Mark attendance for " << sid << name << " (1=Present,0=Absent): ";
        cin >> present;
        fout << date << " " << sid << " " << present << endl;
    }

    fin.close();
    fout.close();
    cout << "Attendance marked!\n";
}

void attendancePercentage(string sid) {
    ifstream fin("attendance.txt");
    string date, id;
    int mark;
    int total = 0, present = 0;

    while (fin >> date >> id >> mark) {
        if (id == sid) {
            total++;
            if (mark == 1) present++;
        }
    }
    fin.close();

    if (total == 0) cout << "No attendance record found.\n";
    else cout << "Attendance % = " << (present * 100.0 / total) << "%\n";
}

// ====================== Menus ======================
void adminMenu() {
    int ch;
    do {
        cout << "\n--- Admin Menu ---\n1. Add Teacher\n2. List Teachers\n0. Logout\nChoice: ";
        cin >> ch;
        if (ch == 1) addTeacher();
        else if (ch == 2) listTeachers();
    } while (ch != 0);
}

void teacherMenu(string tname) {
    int ch;
    do {
        cout << "\n--- Teacher Menu (" << tname << ") ---\n1. Add Student\n2. List Students\n3. Remove Student\n4. Mark Attendance\n5. Attendance % of Student\n0. Logout\nChoice: ";
        cin >> ch;
        if (ch == 1) addStudent();
        else if (ch == 2) listStudents();
        else if (ch == 3) removeStudent();
        else if (ch == 4) markAttendance();
        else if (ch == 5) {
            string sid;
            cout << "Enter Student ID: ";
            cin >> sid;
            attendancePercentage(sid);
        }
    } while (ch != 0);
}

void studentMenu(string sid, string sname) {
    int ch;
    do {
        cout << "\n--- Student Menu (" << sname << ") ---\n1. View Attendance %\n0. Logout\nChoice: ";
        cin >> ch;
        if (ch == 1) attendancePercentage(sid);
    } while (ch != 0);
}

// ====================== Main ======================
int main() {
    saveAdmin(); // ensure admin account exists
    int choice;

    do {
        cout << "\n--- Student Management System ---\n1. Admin Login\n2. Teacher Login\n3. Student Login\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string u, p;
            cout << "Enter Admin Username: "; cin >> u;
            cout << "Enter Password: "; cin >> p;
            if (checkAdminLogin(u, p)) adminMenu();
            else cout << "Invalid Admin Login!\n";
        }
        else if (choice == 2) {
            string u, p, tname;
            cout << "Enter Teacher Username: "; cin >> u;
            cout << "Enter Password: "; cin >> p;
            if (checkTeacherLogin(u, p, tname)) teacherMenu(tname);
            else cout << "Invalid Teacher Login!\n";
        }
        else if (choice == 3) {
            string sid, sname;
            cout << "Enter Student ID: "; cin >> sid;
            cin.ignore();
            cout << "Enter Student Name: "; getline(cin, sname);
            if (checkStudentLogin(sid, sname)) studentMenu(sid, sname);
            else cout << "Invalid Student Login!\n";
        }

    } while (choice != 0);

    return 0;
}
