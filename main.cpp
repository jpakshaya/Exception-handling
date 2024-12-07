#include "users.h"

int main() {
    string userId, userName;
    char userType;
    bool running = true;

    while(running) {
        // Login Menu
        cout << "\n=== School Management System ===\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Choice: ";
        
        char choice;
        cin >> choice;

        if(choice == '2') {
            cout << "Thank you for using the system!\n";
            break;
        }

        // Login Process
        cout << "\nEnter User Type (P for Principal, T for Teacher, S for Student): ";
        cin >> userType;
        cout << "Enter User ID: ";
        cin >> userId;

        // Verify user exists and get their data
        json userData;
        bool userFound = false;
        
        string fileName;
        switch(toupper(userType)) {
            case 'P': fileName = "principals.json"; break;
            case 'T': fileName = "teachers.json"; break;
            case 'S': fileName = "students.json"; break;
            default: 
                cout << "Invalid user type!\n";
                continue;
        }

        ifstream file(fileName);
        if(!file.is_open()) {
            cout << "Error: Unable to open " << fileName << endl;
            continue;
        }

        file >> userData;
        file.close();


        // Find user and get their name
        if (toupper(userType) == 'P') {
            for (const auto& principal : userData["principals"]) {
                if (principal["id"] == userId) {
                    userName = principal["name"];
                    userFound = true;
                    break;
                }
            }
        } 
        else {
            string userCategory = (toupper(userType) == 'T') ? "teachers" : "students";
            for(const auto& user : userData[userCategory]) {
                if(user["id"] == userId) {
                    userName = user["name"];
                    userFound = true;
                    break;
                }
            }
        }

        if(!userFound) {
            cout << "User not found!\n";
            continue;
        }

        cout << "\nWelcome, " << userName << "!\n";

        // Role-specific menus
        bool loggedIn = true;
        while(loggedIn) {
            switch(toupper(userType)) {
                case 'P': {
                    Principal principal(userId, userName);
                    cout << "\n=== Principal Menu ===\n";
                    cout << "1. View Profile\n";
                    cout << "2. Manage Students\n";
                    cout << "3. Manage Teachers\n";
                    cout << "4. Manage Courses\n";
                    cout << "5. Logout\n";
                    cout << "Choice: ";
                    
                    char principalChoice;
                    cin >> principalChoice;

                    switch(principalChoice) {
                        case '1':
                            principal.viewProfile();
                            break;
                        case '2': {
                            cout << "\n=== Student Management ===\n";
                            cout << "1. Create Student\n";
                            cout << "2. View All Students\n";
                            cout << "3. Update Student\n";
                            cout << "4. Delete Student\n";
                            cout << "5. Back\n";
                            cout << "Choice: ";
                            
                            char studentMgmtChoice;
                            cin >> studentMgmtChoice;
                            string studentId, studentName;

                            switch(studentMgmtChoice) {
                                case '1':
                                    cout << "Enter Student ID: ";
                                    cin >> studentId;
                                    cout << "Enter Student Name: ";
                                    cin.ignore();
                                    getline(cin, studentName);
                                    principal.createStudent(studentId, studentName);
                                    break;
                                case '2':
                                    principal.viewAllStudents();
                                    break;
                                case '3':
                                    cout << "Enter Student ID to update: ";
                                    cin >> studentId;
                                    principal.updateStudent(studentId);
                                    break;
                                case '4':
                                    cout << "Enter Student ID to delete: ";
                                    cin >> studentId;
                                    principal.deleteStudent(studentId);
                                    break;
                            }
                            break;
                        }
                        case '3': {
                            cout << "\n=== Teacher Management ===\n";
                            cout << "1. Create Teacher\n";
                            cout << "2. View All Teachers\n";
                            cout << "3. Update Teacher\n";
                            cout << "4. Delete Teacher\n";
                            cout << "5. Back\n";
                            cout << "Choice: ";
                            
                            char teacherMgmtChoice;
                            cin >> teacherMgmtChoice;
                            string teacherId, teacherName;

                            switch(teacherMgmtChoice) {
                                case '1':
                                    cout << "Enter Teacher ID: ";
                                    cin >> teacherId;
                                    cout << "Enter Teacher Name: ";
                                    cin.ignore();
                                    getline(cin, teacherName);
                                    principal.createTeacher(teacherId, teacherName);
                                    break;
                                case '2':
                                    principal.viewAllTeachers();
                                    break;
                                case '3':
                                    cout << "Enter Teacher ID to update: ";
                                    cin >> teacherId;
                                    principal.updateTeacher(teacherId);
                                    break;
                                case '4':
                                    cout << "Enter Teacher ID to delete: ";
                                    cin >> teacherId;
                                    principal.deleteTeacher(teacherId);
                                    break;
                            }
                            break;
                        }
                        case '4': {
                            cout << "\n=== Course Management ===\n";
                            cout << "1. Create Course\n";
                            cout << "2. View All Courses\n";
                            cout << "3. Update Course\n";
                            cout << "4. Delete Course\n";
                            cout << "5. Back\n";
                            cout << "Choice: ";
                            
                            char courseMgmtChoice;
                            cin >> courseMgmtChoice;
                            string courseId, courseName;

                            switch(courseMgmtChoice) {
                                case '1':
                                    cout << "Enter Course ID: ";
                                    cin >> courseId;
                                    cout << "Enter Course Name: ";
                                    cin.ignore();
                                    getline(cin, courseName);
                                    principal.createCourse(courseId, courseName);
                                    break;
                                case '2':
                                    principal.viewAllCourses();
                                    break;
                                case '3':
                                    cout << "Enter Course ID to update: ";
                                    cin >> courseId;
                                    principal.updateCourse(courseId);
                                    break;
                                case '4':
                                    cout << "Enter Course ID to delete: ";
                                    cin >> courseId;
                                    principal.deleteCourse(courseId);
                                    break;
                            }
                            break;
                        }
                        case '5':
                            loggedIn = false;
                            break;
                    }
                    break;
                }
                
                case 'T': {
                    // Get teacher's courses from JSON file
                    Course* teacherCourses = nullptr; // You'll need to implement this
                    Teacher teacher(userId, userName, teacherCourses);
                    
                    cout << "\n=== Teacher Menu ===\n";
                    cout << "1. View Profile\n";
                    cout << "2. Update Student Grade\n";
                    cout << "3. Update Student Attendance\n";
                    cout << "4. Logout\n";
                    cout << "Choice: ";
                    
                    char teacherChoice;
                    cin >> teacherChoice;

                    switch(teacherChoice) {
                        case '1':
                            teacher.viewProfile();
                            break;
                        case '2': {
                            string studentId, courseCode;
                            cout << "Enter Student ID: ";
                            cin >> studentId;
                            cout << "Enter Course Code: ";
                            cin >> courseCode;
                            teacher.updateGrade(studentId, courseCode);
                            break;
                        }
                        case '3': {
                            string studentId, courseCode;
                            cout << "Enter Student ID: ";
                            cin >> studentId;
                            cout << "Enter Course Code: ";
                            cin >> courseCode;
                            teacher.updateAttendance(studentId, courseCode);
                            break;
                        }
                        case '4':
                            loggedIn = false;
                            break;
                    }
                    break;
                }
                
                case 'S': {
                    StudentStats* studentStats = nullptr; // You'll need to implement this
                    Student student(userId, userName, studentStats);
                    
                    cout << "\n=== Student Menu ===\n";
                    cout << "1. View Profile\n";
                    cout << "2. View Grades\n";
                    cout << "3. View Attendance\n";
                    cout << "4. Logout\n";
                    cout << "Choice: ";
                    
                    char studentChoice;
                    cin >> studentChoice;

                    switch(studentChoice) {
                        case '1':
                            student.viewProfile();
                            break;
                        case '2':
                            student.viewGrades();
                            break;
                        case '3':
                            student.viewAttendances();
                            break;
                        case '4':
                            loggedIn = false;
                            break;
                    }
                    break;
                }
            }
        }
    }
    return 0;
}