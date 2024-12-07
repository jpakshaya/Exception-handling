#include "users.h"

// Student Functions

void Principal :: createStudent(const string &id, const string &name) 
{
    json studentsData;
    
    // 1. Read existing students data
    ifstream inFile("students.json");
    if (inFile.is_open()) {
        inFile >> studentsData;
        inFile.close();
    }
    
    // 2. Check if student with same ID exists
    for (const auto& student : studentsData["students"]) {
        if (student["id"] == id) {
            cout << "Error: Student with ID " << id << " already exists!" << endl;
            return;
        }
    }
    
    // 3. Add new student
    json newStudent;
    newStudent["id"] = id;
    newStudent["name"] = name;
    studentsData["students"].push_back(newStudent);
    
    // 4. Write updated data back to file
    ofstream outFile("students.json");
    if (outFile.is_open()) {
        outFile << studentsData.dump(4);  // The '4' argument adds indentation for readability
        outFile.close();
        cout << "Student " << name << " with ID " << id << " created successfully!" << endl;
    } else {
        cout << "Error: Unable to open students.json for writing!" << endl;
    }
}

void Principal :: retrieveStudent(const string &id) 
{
    json studentsData, enrollmentsData;
    
    // Read students data
    ifstream studentsFile("students.json");
    if (!studentsFile.is_open()) {
        cout << "Error: Unable to open students.json for reading!" << endl;
        return;
    }
    studentsFile >> studentsData;
    studentsFile.close();

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for reading!" << endl;
        return;
    }
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    // Find student
    auto studentIt = find_if(studentsData["students"].begin(), studentsData["students"].end(),
                             [&id](const json& student) { return student["id"] == id; });

    if (studentIt == studentsData["students"].end()) {
        cout << "Error: No student found with ID " << id << endl;
        return;
    }

    string studentName = (*studentIt)["name"];

    // Present options to user
    cout << "\nStudent found! What information would you like to see?" << endl;
    cout << "1. Basic Info (ID and Name)" << endl;
    cout << "2. Course Information" << endl;
    cout << "3. All Information" << endl;
    cout << "Enter your choice (1-3): ";

    int choice;
    cin >> choice;

    switch(choice) 
    {
        case 1: 
        {   // Basic Info
            cout << "\nStudent Basic Information:" << endl;
            cout << "ID: " << id << endl;
            cout << "Name: " << studentName << endl;
            break;
        }

        case 2: 
        {   // Course Info
            cout << "\nStudent Course Information:" << endl;
            if (enrollmentsData["enrollments"].contains(id)) {
                for (const auto& course : enrollmentsData["enrollments"][id]) {
                    cout << "Course: " << course["courseCode"] 
                         << ", Grade: " << course["grade"] 
                         << ", Attendance: " << course["attendance"] << "%" << endl;
                }
            } else {
                cout << "No courses registered for this student." << endl;
            }
            break;
        }

        case 3: 
        {   // All Info
            cout << "\nAll Student Information:" << endl;
            cout << "ID: " << id << endl;
            cout << "Name: " << studentName << endl;
            
            cout << "\nCourse Information:" << endl;
            if (enrollmentsData["enrollments"].contains(id)) {
                for (const auto& course : enrollmentsData["enrollments"][id]) {
                    cout << "Course: " << course["courseCode"] 
                         << ", Grade: " << course["grade"] 
                         << ", Attendance: " << course["attendance"] << "%" << endl;
                }
            } else {
                cout << "No courses registered for this student." << endl;
            }
            break;
        }

        default:
        {
            cout << "Invalid choice!" << endl;
            break;
        }
    }
}

void Principal :: updateStudent(const string &id) 
{
    json studentsData, enrollmentsData, coursesData;
    
    // 1. Read existing students data
    ifstream studentsFile("students.json");
    if (!studentsFile.is_open()) {
        cout << "Error: Unable to open students.json for reading!" << endl;
        return;
    }
    studentsFile >> studentsData;
    studentsFile.close();

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for reading!" << endl;
        return;
    }
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    // Read courses data
    ifstream coursesFile("courses.json");
    if (!coursesFile.is_open()) {
        cout << "Error: Unable to open courses.json for reading!" << endl;
        return;
    }
    coursesFile >> coursesData;
    coursesFile.close();

    // Find student
    auto studentIt = find_if(studentsData["students"].begin(), studentsData["students"].end(),
                           [&id](const json& student) { return student["id"] == id; });

    if (studentIt == studentsData["students"].end()) {
        cout << "Error: No student found with ID " << id << endl;
        return;
    }

    // Present update options
    cout << "\nWhat would you like to update?" << endl;
    cout << "1. Edit Name" << endl;
    cout << "2. Add Courses" << endl;
    cout << "3. Delete Courses" << endl;
    cout << "Enter your choice (1-3): ";

    int choice;
    cin >> choice;
    cin.ignore(); // Clear newline from input buffer

    switch(choice) 
    {
        case 1: 
        {   // Edit Name
            cout << "Current Name: " << (*studentIt)["name"] << endl;
            cout << "Enter new name: ";
            string newName;
            getline(cin, newName);

            (*studentIt)["name"] = newName;

            // Write updated student data
            ofstream outFile("students.json");
            if (outFile.is_open()) {
                outFile << studentsData.dump(4);
                outFile.close();
                cout << "Student name updated successfully!" << endl;
            } else {
                cout << "Error: Unable to save updated information!" << endl;
            }
            break;
        }

        case 2: 
        {   // Add Courses
            cout << "Enter course details:" << endl;
            cout << "Course Code: ";
            string courseCode;
            getline(cin, courseCode);

            // Check if course exists
            bool courseExists = false;
            for (const auto& course : coursesData["courses"]) {
                if (course["code"] == courseCode) {
                    courseExists = true;
                    break;
                }
            }

            if (!courseExists) {
                cout << "Error: Course with code " << courseCode << " does not exist!" << endl;
                return;
            }

            cout << "Initial Grade: ";
            int grade;
            cin >> grade;

            cout << "Initial Attendance (%): ";
            int attendance;
            cin >> attendance;

            if (attendance < 0 || attendance > 100) {
                cout << "Invalid attendance percentage!" << endl;
                return;
            }

            // Check if student is already enrolled in this course
            bool alreadyEnrolled = false;
            if (enrollmentsData["enrollments"].contains(id)) {
                for (const auto& course : enrollmentsData["enrollments"][id]) {
                    if (course["courseCode"] == courseCode) {
                        alreadyEnrolled = true;
                        break;
                    }
                }
            }

            if (alreadyEnrolled) {
                cout << "Error: Student is already enrolled in this course!" << endl;
                return;
            }

            // Create new course object
            json newCourse;
            newCourse["courseCode"] = courseCode;
            newCourse["grade"] = grade;
            newCourse["attendance"] = attendance;

            // Add course to student's enrollments
            if (!enrollmentsData["enrollments"].contains(id)) {
                enrollmentsData["enrollments"][id] = json::array();
            }
            enrollmentsData["enrollments"][id].push_back(newCourse);

            // Write updated enrollments data
            ofstream outFile("enrollments.json");
            if (outFile.is_open()) {
                outFile << enrollmentsData.dump(4);
                outFile.close();
                cout << "Course added successfully!" << endl;
            } else {
                cout << "Error: Unable to save updated information!" << endl;
            }
            break;
        }

        case 3: 
        {   // Delete Courses
            if (!enrollmentsData["enrollments"].contains(id) || 
                enrollmentsData["enrollments"][id].empty()) {
                cout << "No courses found for this student." << endl;
                return;
            }

            cout << "\nCurrent Courses:" << endl;
            int courseIndex = 0;
            for (const auto& course : enrollmentsData["enrollments"][id]) {
                cout << courseIndex + 1 << ". Course: " << course["courseCode"] 
                     << ", Grade: " << course["grade"] 
                     << ", Attendance: " << course["attendance"] << "%" << endl;
                courseIndex++;
            }

            cout << "\nEnter course number to delete (1-" << courseIndex << "): ";
            int courseChoice;
            cin >> courseChoice;
            courseChoice--; // Convert to 0-based index

            if (courseChoice < 0 || courseChoice >= courseIndex) {
                cout << "Invalid course number!" << endl;
                return;
            }

            // Get the course code to be deleted
            string deletedCourseCode = enrollmentsData["enrollments"][id][courseChoice]["courseCode"];

            // Remove the selected course
            enrollmentsData["enrollments"][id].erase(
                enrollmentsData["enrollments"][id].begin() + courseChoice
            );

            // Write updated enrollments data
            ofstream outFile("enrollments.json");
            if (outFile.is_open()) {
                outFile << enrollmentsData.dump(4);
                outFile.close();
                cout << "Course " << deletedCourseCode << " deleted successfully!" << endl;
            } else {
                cout << "Error: Unable to save updated information!" << endl;
            }
            break;
        }

        default:
            cout << "Invalid choice!" << endl;
            break;
    }
}

void Principal :: deleteStudent(const string &id) 
{
    json studentsData, enrollmentsData;

    // Read students data
    ifstream studentsFile("students.json");
    if (!studentsFile.is_open()) {
        cout << "Error: Unable to open students.json for reading!" << endl;
        return;
    }
    studentsFile >> studentsData;
    studentsFile.close();

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for reading!" << endl;
        return;
    }
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    // Check if the student exists
    auto studentIt = find_if(studentsData["students"].begin(), studentsData["students"].end(),
                              [&id](const json& student) { return student["id"] == id; });

    if (studentIt == studentsData["students"].end()) {
        cout << "Error: No student found with ID " << id << endl;
        return;
    }

    // Store student name for confirmation message
    string studentName = (*studentIt)["name"];

    // Ask for confirmation
    cout << "Are you sure you want to delete student " << studentName << " (ID: " << id << ")? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Deletion cancelled." << endl;
        return;
    }

    // Remove the student from the students array
    studentsData["students"].erase(studentIt);

    // Remove the student's enrollments (if any exist)
    if (enrollmentsData["enrollments"].contains(id)) {
        enrollmentsData["enrollments"].erase(id);
    }

    // Write updated students data back to file
    ofstream outStudentsFile("students.json");
    if (!outStudentsFile.is_open()) {
        cout << "Error: Unable to open students.json for writing!" << endl;
        return;
    }
    outStudentsFile << studentsData.dump(4);
    outStudentsFile.close();

    // Write updated enrollments data back to file
    ofstream outEnrollmentsFile("enrollments.json");
    if (!outEnrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for writing!" << endl;
        return;
    }
    outEnrollmentsFile << enrollmentsData.dump(4);
    outEnrollmentsFile.close();

    cout << "Student " << studentName << " (ID: " << id << ") has been successfully deleted." << endl;
}

void Principal :: viewAllStudents() const
{
    json studentsData;

    // Read students data
    ifstream studentsFile("students.json");
    if (!studentsFile.is_open()) {
        cout << "Error: Unable to open students.json for reading!" << endl;
        return;
    }
    studentsFile >> studentsData;
    studentsFile.close();

    // Check if there are any students
    if (studentsData["students"].empty()) {
        cout << "No students found." << endl;
        return;
    }

    // Display all students
    cout << "\nList of Students:" << endl;
    for (const auto& student : studentsData["students"]) {
        cout << "ID: " << student["id"] << ", Name: " << student["name"] << endl;
    }
}

// Teacher Functions

void Principal :: createTeacher(const string &id, const string &name) 
{
    json teachersData;

    // Read existing teachers data
    ifstream teachersFile("teachers.json");
    if (teachersFile.is_open()) {
        teachersFile >> teachersData;
        teachersFile.close();
    } else {
        cout << "Error: Unable to open teachers.json for reading!" << endl;
        return;
    }

    // Check if the teacher ID already exists
    auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                              [&id](const json& teacher) { return teacher["id"] == id; });

    if (teacherIt != teachersData["teachers"].end()) {
        cout << "Error: A teacher with ID " << id << " already exists." << endl;
        return;
    }

    // Create a new teacher entry
    json newTeacher;
    newTeacher["id"] = id;
    newTeacher["name"] = name;

    // Add the new teacher to the teachers array
    teachersData["teachers"].push_back(newTeacher);

    // Write updated teachers data back to file
    ofstream outTeachersFile("teachers.json");
    if (!outTeachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for writing!" << endl;
        return;
    }
    outTeachersFile << teachersData.dump(4);
    outTeachersFile.close();

    cout << "Teacher " << name << " with ID " << id << " has been successfully created." << endl;
}

void Principal :: retrieveTeacher(const string &id) 
{
    json teachersData;

    // Read teachers data
    ifstream teachersFile("teachers.json");
    if (!teachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for reading!" << endl;
        return;
    }
    teachersFile >> teachersData;
    teachersFile.close();

    // Find the teacher by ID
    auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                              [&id](const json& teacher) { return teacher["id"] == id; });

    if (teacherIt == teachersData["teachers"].end()) {
        cout << "Error: No teacher found with ID " << id << endl;
        return;
    }

    // Present options to user
    cout << "\nTeacher found! What information would you like to see?" << endl;
    cout << "1. Basic Info (ID and Name)" << endl;
    cout << "2. Overall Info (ID, Name, and Courses)" << endl;
    cout << "Enter your choice (1-2): ";

    int choice;
    cin >> choice;

    switch(choice) 
    {
        case 1: 
        {   // Basic Info
            cout << "\nTeacher Basic Information:" << endl;
            cout << "ID: " << (*teacherIt)["id"] << endl;
            cout << "Name: " << (*teacherIt)["name"] << endl;
            break;
        }

        case 2: 
        {   // Overall Info
            cout << "\nOverall Teacher Information:" << endl;
            cout << "ID: " << (*teacherIt)["id"] << endl;
            cout << "Name: " << (*teacherIt)["name"] << endl;

            // Assuming each teacher has an array of courses they handle
            if ((*teacherIt).contains("courses")) {
                cout << "Courses handled:" << endl;
                for (const auto& course : (*teacherIt)["courses"]) {
                    cout << "- Code: " << course["Code"] << ", Name: " << course["Name"] << endl;
                }
            } else {
                cout << "No courses assigned to this teacher." << endl;
            }
            break;
        }

        default:
        {
            cout << "Invalid choice!" << endl;
            break;
        }
    }
}

void Principal :: updateTeacher(const string &id) 
{
    json teachersData, coursesData;

    // Read existing teachers data
    ifstream teachersFile("teachers.json");
    if (!teachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for reading!" << endl;
        return;
    }
    teachersFile >> teachersData;
    teachersFile.close();

    // Read existing courses data
    ifstream coursesFile("courses.json");
    if (!coursesFile.is_open()) {
        cout << "Error: Unable to open courses.json for reading!" << endl;
        return;
    }
    coursesFile >> coursesData;
    coursesFile.close();

    // Check if teacher exists
    auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                              [&id](const json& teacher) { return teacher["id"] == id; });

    if (teacherIt == teachersData["teachers"].end()) {
        cout << "Error: No teacher found with ID " << id << endl;
        return;
    }

    string teacherName = (*teacherIt)["name"];

    // Present update options
    cout << "\nUpdate options for teacher " << teacherName << " (ID: " << id << "):" << endl;
    cout << "1. Update Name" << endl;
    cout << "2. Add Course" << endl;
    cout << "3. Delete Course" << endl;
    cout << "Enter your choice (1-3): ";

    int choice;
    cin >> choice;
    cin.ignore(); // Clear newline from input buffer

    switch(choice) {
        case 1: {
            // Update Name
            cout << "Enter new name: ";
            string newName;
            getline(cin, newName);

            // Update name in the JSON
            (*teacherIt)["name"] = newName;

            // Save updated data
            ofstream outFile("teachers.json");
            outFile << teachersData.dump(4);
            outFile.close();

            cout << "Name updated successfully!" << endl;
            break;
        }

        case 2: {
            // Add Course
            cout << "Enter course code to add: ";
            string courseCode, courseName;
            getline(cin, courseCode);

            // Verify if course exists
            bool courseExists = false;
            for (const auto& course : coursesData["courses"]) {
                if (course["code"] == courseCode) {
                    courseExists = true;
                    break;
                }
            }

            if (!courseExists) {
                cout << "Error: Course does not exist!" << endl;
                break;
            }

            // Check if teacher is already assigned to this course
            bool alreadyAssigned = false;
            if ((*teacherIt).contains("courses")) {
                for (const auto& course : (*teacherIt)["courses"]) {
                    if (course["Code"] == courseCode) {
                        alreadyAssigned = true;
                        break;
                    }
                }
            }

            if (alreadyAssigned) {
                cout << "Teacher is already assigned to this course!" << endl;
                break;
            }

            for (const auto& course : coursesData["courses"]) {
                if (course["code"] == courseCode) {
                    courseName = course["name"];
                    break;
                }
            }

            // Add new course to teacher's courses
            json newCourse;
            newCourse["Code"] = courseCode;
            newCourse["Name"] = courseName;

            if (!(*teacherIt).contains("courses")) {
                (*teacherIt)["courses"] = json::array();
            }
            (*teacherIt)["courses"].push_back(newCourse);

            // Save updated teachers data
            ofstream outFile("teachers.json");
            outFile << teachersData.dump(4);
            outFile.close();

            cout << "Course added successfully!" << endl;
            break;
        }

        case 3: {
            // Delete Course
            cout << "Enter course code to delete: ";
            string courseCode;
            getline(cin, courseCode);

            bool courseFound = false;
            if ((*teacherIt).contains("courses")) {
                auto& teacherCourses = (*teacherIt)["courses"];
                for (size_t i = 0; i < teacherCourses.size(); i++) {
                    if (teacherCourses[i]["Code"] == courseCode) {
                        teacherCourses.erase(teacherCourses.begin() + i);
                        courseFound = true;
                        break;
                    }
                }
            }

            if (!courseFound) {
                cout << "Teacher is not assigned to this course!" << endl;
            } else {
                // Save updated teachers data
                ofstream outFile("teachers.json");
                outFile << teachersData.dump(4);
                outFile.close();
                cout << "Course deleted successfully!" << endl;
            }
            break;
        }

        default:
            cout << "Invalid choice!" << endl;
            break;
    }
}

void Principal :: deleteTeacher(const string &id) 
{
    json teachersData, coursesData;

    // Read teachers data
    ifstream teachersFile("teachers.json");
    if (!teachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for reading!" << endl;
        return;
    }
    teachersFile >> teachersData;
    teachersFile.close();

    // Check if the teacher exists
    auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                           [&id](const json& teacher) { return teacher["id"] == id; });

    if (teacherIt == teachersData["teachers"].end()) {
        cout << "Error: No teacher found with ID " << id << endl;
        return;
    }

    // Store teacher name for confirmation message
    string teacherName = (*teacherIt)["name"];

    // Ask for confirmation
    cout << "Are you sure you want to delete teacher " << teacherName << " (ID: " << id << ")? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Deletion cancelled." << endl;
        return;
    }

    // Check if teacher has assigned courses
    if ((*teacherIt).contains("courses") && !(*teacherIt)["courses"].empty()) {
        cout << "\nWarning: This teacher has assigned courses:" << endl;
        for (const auto& course : (*teacherIt)["courses"]) {
            cout << "- Course Code: " << course["Code"] << endl;
        }
        
        cout << "\nDo you still want to proceed with deletion? (y/n): ";
        cin >> confirm;
        
        if (confirm != 'y' && confirm != 'Y') {
            cout << "Deletion cancelled." << endl;
            return;
        }
    }

    // Remove the teacher from the teachers array
    teachersData["teachers"].erase(teacherIt);

    // Write updated teachers data back to file
    ofstream outTeachersFile("teachers.json");
    if (!outTeachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for writing!" << endl;
        return;
    }
    outTeachersFile << teachersData.dump(4);
    outTeachersFile.close();

    cout << "Teacher " << teacherName << " (ID: " << id << ") has been successfully deleted." << endl;
}

void Principal :: viewAllTeachers() const
{
    json teachersData;

    // Read teachers data
    ifstream teachersFile("teachers.json");
    if (!teachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for reading!" << endl;
        return;
    }
    teachersFile >> teachersData;
    teachersFile.close();

    // Check if there are any teachers
    if (teachersData["teachers"].empty()) {
        cout << "No teachers found." << endl;
        return;
    }

    // Display all teachers
    cout << "\nList of Teachers:" << endl;
    for (const auto& teacher : teachersData["teachers"]) {
        cout << "ID: " << teacher["id"] << ", Name: " << teacher["name"] << endl;
    }
}

// Courses Functions

void Principal :: createCourse(const string &code, const string &name) 
{
    json coursesData;
    
    // Read existing courses data
    ifstream coursesFile("courses.json");
    if (coursesFile.is_open()) {
        coursesFile >> coursesData;
        coursesFile.close();
    } else {
        // If file doesn't exist, initialize with an empty courses array
        coursesData["courses"] = json::array();
    }
    
    // Check if course with same code exists
    for (const auto& course : coursesData["courses"]) {
        if (course["code"] == code) {
            cout << "Error: Course with code " << code << " already exists!" << endl;
            return;
        }
    }
    
    // Create new course
    json newCourse;
    newCourse["code"] = code;
    newCourse["name"] = name;
    
    // Add new course to courses array
    coursesData["courses"].push_back(newCourse);
    
    // Write updated data back to file
    ofstream outFile("courses.json");
    if (outFile.is_open()) {
        outFile << coursesData.dump(4);  // The '4' argument adds indentation for readability
        outFile.close();
        cout << "Course " << name << " with code " << code << " created successfully!" << endl;
    } else {
        cout << "Error: Unable to open courses.json for writing!" << endl;
    }
}

void Principal :: retrieveCourse(const string &code) {
    json coursesData, teachersData, enrollmentsData;

    // Read courses data
    ifstream coursesFile("courses.json");
    if (!coursesFile.is_open()) {
        cout << "Error: Unable to open courses.json for reading!" << endl;
        return;
    }
    coursesFile >> coursesData;
    coursesFile.close();

    // Read teachers data
    ifstream teachersFile("teachers.json");
    if (!teachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for reading!" << endl;
        return;
    }
    teachersFile >> teachersData;
    teachersFile.close();

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for reading!" << endl;
        return;
    }
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    // Find the course
    auto courseIt = find_if(coursesData["courses"].begin(), coursesData["courses"].end(),
                          [&code](const json& course) { return course["code"] == code; });

    if (courseIt == coursesData["courses"].end()) {
        cout << "Error: No course found with code " << code << endl;
        return;
    }

    // Present options to user
    cout << "\nCourse found! What information would you like to see?" << endl;
    cout << "1. Basic Info (Code and Name)" << endl;
    cout << "2. Assigned Teachers" << endl;
    cout << "3. Enrolled Students" << endl;
    cout << "4. All Information" << endl;
    cout << "Enter your choice (1-4): ";

    int choice;
    cin >> choice;

    switch(choice) {
        case 1: {
            // Basic Info
            cout << "\nCourse Basic Information:" << endl;
            cout << "Code: " << (*courseIt)["code"] << endl;
            cout << "Name: " << (*courseIt)["name"] << endl;
            break;
        }

        case 2: {
            // Assigned Teachers
            cout << "\nAssigned Teachers:" << endl;
            bool teachersFound = false;
            
            for (const auto& teacher : teachersData["teachers"]) {
                if (teacher.contains("courses")) {
                    for (const auto& course : teacher["courses"]) {
                        if (course["Code"] == code) {
                            cout << "Teacher ID: " << teacher["id"] 
                                 << ", Name: " << teacher["name"] << endl;
                            teachersFound = true;
                        }
                    }
                }
            }

            if (!teachersFound) {
                cout << "No teachers assigned to this course." << endl;
            }
            break;
        }

        case 3: {
            // Enrolled Students
            cout << "\nEnrolled Students:" << endl;
            bool studentsFound = false;
            
            for (const auto& enrollment : enrollmentsData["enrollments"].items()) {
                string studentId = enrollment.key();
                for (const auto& course : enrollment.value()) {
                    if (course["courseCode"] == code) {
                        // Find student name from students.json
                        ifstream studentsFile("students.json");
                        if (studentsFile.is_open()) {
                            json studentsData;
                            studentsFile >> studentsData;
                            for (const auto& student : studentsData["students"]) {
                                if (student["id"] == studentId) {
                                    cout << "Student ID: " << studentId 
                                         << ", Name: " << student["name"]
                                         << ", Grade: " << course["grade"]
                                         << ", Attendance: " << course["attendance"] << "%" << endl;
                                    studentsFound = true;
                                    break;
                                }
                            }
                        }
                        studentsFile.close();
                    }
                }
            }

            if (!studentsFound) {
                cout << "No students enrolled in this course." << endl;
            }
            break;
        }

        case 4: {
            // All Information
            cout << "\nComplete Course Information:" << endl;
            cout << "Code: " << (*courseIt)["code"] << endl;
            cout << "Name: " << (*courseIt)["name"] << endl;

            // Assigned Teachers
            cout << "\nAssigned Teachers:" << endl;
            bool teachersFound = false;
            for (const auto& teacher : teachersData["teachers"]) {
                if (teacher.contains("courses")) {
                    for (const auto& course : teacher["courses"]) {
                        if (course["Code"] == code) {
                            cout << "Teacher ID: " << teacher["id"] 
                                 << ", Name: " << teacher["name"] << endl;
                            teachersFound = true;
                        }
                    }
                }
            }
            if (!teachersFound) {
                cout << "No teachers assigned to this course." << endl;
            }

            // Enrolled Students
            cout << "\nEnrolled Students:" << endl;
            bool studentsFound = false;
            for (const auto& enrollment : enrollmentsData["enrollments"].items()) {
                string studentId = enrollment.key();
                for (const auto& course : enrollment.value()) {
                    if (course["courseCode"] == code) {
                        // Find student name from students.json
                        ifstream studentsFile("students.json");
                        if (studentsFile.is_open()) {
                            json studentsData;
                            studentsFile >> studentsData;
                            for (const auto& student : studentsData["students"]) {
                                if (student["id"] == studentId) {
                                    cout << "Student ID: " << studentId 
                                         << ", Name: " << student["name"]
                                         << ", Grade: " << course["grade"]
                                         << ", Attendance: " << course["attendance"] << "%" << endl;
                                    studentsFound = true;
                                    break;
                                }
                            }
                        }
                        studentsFile.close();
                    }
                }
            }
            if (!studentsFound) {
                cout << "No students enrolled in this course." << endl;
            }
            break;
        }

        default:
            cout << "Invalid choice!" << endl;
            break;
    }
}

void Principal :: updateCourse(const string &id) 
{
    json coursesData;

    // Read existing courses data
    ifstream coursesFile("courses.json");
    if (!coursesFile.is_open()) {
        cout << "Error: Unable to open courses.json for reading!" << endl;
        return;
    }
    coursesFile >> coursesData;
    coursesFile.close();

    // Check if course exists
    auto courseIt = find_if(coursesData["courses"].begin(), coursesData["courses"].end(),
                          [&id](const json& course) { return course["code"] == id; });

    if (courseIt == coursesData["courses"].end()) {
        cout << "Error: No course found with code " << id << endl;
        return;
    }

    // Get current course information
    string currentName = (*courseIt)["name"];

    // Display current information
    cout << "\nCurrent course information:" << endl;
    cout << "Code: " << id << endl;
    cout << "Name: " << currentName << endl;

    // Get new name
    cout << "\nEnter new course name (or press Enter to keep current): ";
    string newName;
    getline(cin, newName);

    // Update only if a new name was entered
    if (!newName.empty()) {
        (*courseIt)["name"] = newName;

        // Save updated data
        ofstream outFile("courses.json");
        if (!outFile.is_open()) {
            cout << "Error: Unable to open courses.json for writing!" << endl;
            return;
        }
        outFile << coursesData.dump(4);
        outFile.close();

        cout << "Course name updated successfully!" << endl;
    } else {
        cout << "No changes made to course name." << endl;
    }
}

void Principal :: deleteCourse(const string &code) {
    json coursesData, teachersData, enrollmentsData;

    // Read courses data
    ifstream coursesFile("courses.json");
    if (!coursesFile.is_open()) {
        cout << "Error: Unable to open courses.json for reading!" << endl;
        return;
    }
    coursesFile >> coursesData;
    coursesFile.close();

    // Read teachers data
    ifstream teachersFile("teachers.json");
    if (!teachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for reading!" << endl;
        return;
    }
    teachersFile >> teachersData;
    teachersFile.close();

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for reading!" << endl;
        return;
    }
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    // Find the course
    auto courseIt = find_if(coursesData["courses"].begin(), coursesData["courses"].end(),
                          [&code](const json& course) { return course["code"] == code; });

    if (courseIt == coursesData["courses"].end()) {
        cout << "Error: No course found with code " << code << endl;
        return;
    }

    string courseName = (*courseIt)["name"];

    // Check if course has any enrollments or assigned teachers
    bool hasEnrollments = false;
    bool hasTeachers = false;

    for (json::iterator it = enrollmentsData["enrollments"].begin(); it != enrollmentsData["enrollments"].end(); ++it) {
        json& courses = it.value();
        if (any_of(courses.begin(), courses.end(), 
                   [&code](const json& course) { return course["courseCode"] == code; })) {
            hasEnrollments = true;
            break;
        }
    }

    for (auto& teacher : teachersData["teachers"]) {
        if (teacher.contains("courses") && 
            any_of(teacher["courses"].begin(), teacher["courses"].end(),
                   [&code](const json& course) { return course["Code"] == code; })) {
            hasTeachers = true;
            break;
        }
    }

    // If course has enrollments or teachers, ask for confirmation
    if (hasEnrollments || hasTeachers) {
        cout << "\nWarning: This course has ";
        if (hasEnrollments && hasTeachers) {
            cout << "enrolled students and assigned teachers";
        } else if (hasEnrollments) {
            cout << "enrolled students";
        } else {
            cout << "assigned teachers";
        }
        cout << ".\nDeleting this course will remove all associated enrollments and assignments." << endl;

        cout << "\nAre you sure you want to proceed? (y/n): ";
        char confirm;
        cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') {
            cout << "Deletion cancelled." << endl;
            return;
        }
    }

    // Remove course from courses.json
    coursesData["courses"].erase(courseIt);

    // Remove course from all student enrollments
    for (json::iterator it = enrollmentsData["enrollments"].begin(); it != enrollmentsData["enrollments"].end(); ++it) {
        json& courses = it.value();
        courses.erase(
            remove_if(courses.begin(), courses.end(),
                     [&code](const json& course) { return course["courseCode"] == code; }),
            courses.end()
        );
    }

    // Remove course from all teacher assignments
    for (auto& teacher : teachersData["teachers"]) {
        if (teacher.contains("courses")) {
            auto& teacherCourses = teacher["courses"];
            teacherCourses.erase(
                remove_if(teacherCourses.begin(), teacherCourses.end(),
                         [&code](const json& course) { return course["Code"] == code; }),
                teacherCourses.end()
            );
        }
    }

    // Save updated courses data
    ofstream outCoursesFile("courses.json");
    if (!outCoursesFile.is_open()) {
        cout << "Error: Unable to open courses.json for writing!" << endl;
        return;
    }
    outCoursesFile << coursesData.dump(4);
    outCoursesFile.close();

    // Save updated enrollments data
    ofstream outEnrollmentsFile("enrollments.json");
    if (!outEnrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for writing!" << endl;
        return;
    }
    outEnrollmentsFile << enrollmentsData.dump(4);
    outEnrollmentsFile.close();

    // Save updated teachers data
    ofstream outTeachersFile("teachers.json");
    if (!outTeachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for writing!" << endl;
        return;
    }
    outTeachersFile << teachersData.dump(4);
    outTeachersFile.close();

    cout << "Course " << courseName << " (Code: " << code << ") has been successfully deleted." << endl;
}

void Principal :: viewAllCourses() const
{
    json coursesData;

    // Read courses data
    ifstream coursesFile("courses.json");
    if (!coursesFile.is_open()) {
        cout << "Error: Unable to open courses.json for reading!" << endl;
        return;
    }
    coursesFile >> coursesData;
    coursesFile.close();

    // Check if there are any courses
    if (coursesData["courses"].empty()) {
        cout << "No courses found." << endl;
        return;
    }

    // Display all courses
    cout << "\nList of Courses:" << endl;
    for (const auto& course : coursesData["courses"]) {
        cout << "Code: " << course["code"] << ", Name: " << course["name"] << endl;
    }
}

// Overriden function to display Principal details

void Principal :: viewProfile() const 
{
    cout << endl;
    cout << "Principal Profile:\n";
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
}


