#include "users.h"

void Student :: viewAttendances() const {
    try {
        // Read courses data
        ifstream coursesFile("courses.json");
        if (!coursesFile.is_open()) {
            throw runtime_error("Error: Unable to open courses.json");
        }
        json coursesData;
        coursesFile >> coursesData;
        coursesFile.close();

        // Read enrollments data
        ifstream enrollmentsFile("enrollments.json");
        if (!enrollmentsFile.is_open()) {
            throw runtime_error("Error: Unable to open enrollments.json");
        }
        json enrollmentsData;
        enrollmentsFile >> enrollmentsData;
        enrollmentsFile.close();

        // Check if student has any enrollments
        if (!enrollmentsData["enrollments"].contains(id)) {
            throw runtime_error("You are not enrolled in any courses.");
        }

        // Check if enrollment data is valid
        if (enrollmentsData["enrollments"][id].empty()) {
            throw runtime_error("No enrollment data found for this student.");
        }

        cout << "\nAttendance Report for " << name << " (ID: " << id << ")" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << "Course Code\tCourse Name\t\tAttendance" << endl;
        cout << "----------------------------------------------------" << endl;

        float totalAttendance = 0.0f;
        int courseCount = 0;

        for (const auto& enrollment : enrollmentsData["enrollments"][id]) {
            // Validate course code
            if (!enrollment.contains("courseCode")) {
                throw runtime_error("Missing course code in enrollment data");
            }

            string courseCode = enrollment["courseCode"];

            // Validate attendance
            if (!enrollment.contains("attendance")) {
                throw runtime_error("Missing attendance for course: " + courseCode);
            }
            float attendance = enrollment["attendance"];

            // Find course name
            string courseName = "Unknown Course";
            bool courseFound = false;
            for (const auto& course : coursesData["courses"]) {
                if (course["code"] == courseCode) {
                    courseName = course["name"];
                    courseFound = true;
                    break;
                }
            }

            if (!courseFound) {
                throw runtime_error("Course details not found for course code: " + courseCode);
            }

            // Validate attendance range
            if (attendance < 0 || attendance > 100) {
                throw runtime_error("Invalid attendance for course " + courseCode + ": " + to_string(attendance));
            }

            // Format output
            cout << courseCode << "\t\t";
            if (courseName.length() < 8) {
                cout << courseName << "\t\t\t";
            } else if (courseName.length() < 16) {
                cout << courseName << "\t\t";
            } else {
                cout << courseName << "\t";
            }
            cout << attendance << "%" << endl;

            totalAttendance += attendance;
            courseCount++;
        }

        // Validate course count
        if (courseCount == 0) {
            throw runtime_error("No valid courses found for the student");
        }

        float overallAttendance = (totalAttendance / courseCount);
        cout << "----------------------------------------------------" << endl;
        cout << "Overall Attendance: " << overallAttendance << "%" << endl << endl;
    }
    catch (const runtime_error& e) {
        // Log or display error messages
        cerr << "Error in viewing attendance: " << e.what() << endl;
    }
    catch (const json::exception& e) {
        // Catch JSON parsing errors
        cerr << "JSON parsing error: " << e.what() << endl;
    }
    catch (const exception& e) {
        // Catch any other standard exceptions
        cerr << "Unexpected error: " << e.what() << endl;
    }
    catch (...) {
        // Catch-all for any unexpected errors
        cerr << "An unknown error occurred while retrieving attendance." << endl;
    }
}

void Student :: viewProfile() const {
    try {
        cout << "\n========== Student Profile ==========\n";
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;

        // Read enrollments data
        ifstream enrollmentsFile("enrollments.json");
        if (!enrollmentsFile.is_open()) {
            throw runtime_error("Error: Unable to open enrollments.json");
        }
        json enrollmentsData;
        enrollmentsFile >> enrollmentsData;
        enrollmentsFile.close();

        // Check if student is enrolled in any courses
        if (!enrollmentsData["enrollments"].contains(id)) {
            throw runtime_error("Not enrolled in any courses.");
        }

        // Validate enrollment data
        if (enrollmentsData["enrollments"][id].empty()) {
            throw runtime_error("No valid enrollment data found.");
        }

        cout << "\nEnrolled Courses:\n";
        cout << "----------------------------------------------------\n";
        cout << "Course Code\tGrade\t\tAttendance\n";
        cout << "----------------------------------------------------\n";

        float totalGrade = 0.0f;
        float totalAttendance = 0.0f;
        int courseCount = 0;

        for (const auto& enrollment : enrollmentsData["enrollments"][id]) {
            // Validate course code
            if (!enrollment.contains("courseCode")) {
                throw runtime_error("Missing course code in enrollment data");
            }

            string courseCode = enrollment["courseCode"];

            // Validate grade and attendance
            if (!enrollment.contains("grade") || !enrollment.contains("attendance")) {
                throw runtime_error("Missing grade or attendance for course: " + courseCode);
            }

            float grade = enrollment["grade"];
            float attendance = enrollment["attendance"];

            // Validate grade and attendance ranges
            if (grade < 0 || grade > 100 || attendance < 0 || attendance > 100) {
                throw runtime_error("Invalid grade or attendance for course: " + courseCode);
            }

            cout << courseCode << "\t\t" << grade << "\t\t" << attendance << "%\n";

            totalGrade += grade;
            totalAttendance += attendance;
            courseCount++;
        }

        // Validate course count
        if (courseCount == 0) {
            throw runtime_error("No valid courses found for the student");
        }

        cout << "----------------------------------------------------\n";
        float avgGrade = totalGrade / courseCount;
        float avgAttendance = totalAttendance / courseCount;
        
        // Round to two decimal places
        avgGrade = round(avgGrade * 100.0) / 100.0;
        avgAttendance = round(avgAttendance * 100.0) / 100.0;

        cout << "Average Grade: " << avgGrade << endl;
        cout << "Average Attendance: " << avgAttendance << "%\n";

        cout << "\n======================================\n";
    }
    catch (const runtime_error& e) {
        // Log or display error messages
        cerr << "Error in viewing profile: " << e.what() << endl;
    }
    catch (const json::exception& e) {
        // Catch JSON parsing errors
        cerr << "JSON parsing error: " << e.what() << endl;
    }
    catch (const exception& e) {
        // Catch any other standard exceptions
        cerr << "Unexpected error: " << e.what() << endl;
    }
    catch (...) {
        // Catch-all for any unexpected errors
        cerr << "An unknown error occurred while retrieving student profile." << endl;
    }
}

// The viewGrades method remains the same as in the previous implementation
void Student :: viewGrades() const {
    try {
        // Read courses data
        ifstream coursesFile("courses.json");
        if (!coursesFile.is_open()) {
            throw runtime_error("Unable to open courses.json");
        }
        json coursesData;
        coursesFile >> coursesData;
        coursesFile.close();

        // Read enrollments data
        ifstream enrollmentsFile("enrollments.json");
        if (!enrollmentsFile.is_open()) {
            throw runtime_error("Unable to open enrollments.json");
        }
        json enrollmentsData;
        enrollmentsFile >> enrollmentsData;
        enrollmentsFile.close();

        // Check if student ID exists in enrollments
        if (!enrollmentsData["enrollments"].contains(id)) {
            throw runtime_error("Student is not enrolled in any courses");
        }

        // Check if enrollment data is empty
        if (enrollmentsData["enrollments"][id].empty()) {
            throw runtime_error("No course enrollments found for this student");
        }

        cout << "\nGrade Report for " << name << " (ID: " << id << ")" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << "Course Code\tCourse Name\t\tGrade" << endl;
        cout << "----------------------------------------------------" << endl;

        float totalGrade = 0.0f;
        int courseCount = 0;

        for (const auto& enrollment : enrollmentsData["enrollments"][id]) {
            // Validate course code
            if (!enrollment.contains("courseCode")) {
                throw runtime_error("Missing course code in enrollment data");
            }

            string courseCode = enrollment["courseCode"];

            // Validate grade
            if (!enrollment.contains("grade")) {
                throw runtime_error("Missing grade for course: " + courseCode);
            }
            float grade = enrollment["grade"];

            // Find course name
            string courseName = "Unknown Course";
            bool courseFound = false;
            for (const auto& course : coursesData["courses"]) {
                if (course["code"] == courseCode) {
                    courseName = course["name"];
                    courseFound = true;
                    break;
                }
            }

            if (!courseFound) {
                throw runtime_error("Course details not found for course code: " + courseCode);
            }

            // Validate grade range
            if (grade < 0 || grade > 100) {
                throw runtime_error("Invalid grade for course " + courseCode + ": " + to_string(grade));
            }

            // Format output
            cout << courseCode << "\t\t";
            if (courseName.length() < 8) {
                cout << courseName << "\t\t\t";
            } else if (courseName.length() < 16) {
                cout << courseName << "\t\t";
            } else {
                cout << courseName << "\t";
            }
            cout << grade << endl;

            totalGrade += grade;
            courseCount++;
        }

        // Validate course count
        if (courseCount == 0) {
            throw runtime_error("No valid courses found for the student");
        }

        float gpa = (courseCount > 0) ? (totalGrade / courseCount) : 0.0f;
        cout << "----------------------------------------------------" << endl;

        gpa = round(gpa) / 100.0;
        cout << "GPA: " << gpa << endl << endl;
    }
    catch (const runtime_error& e) {
        // Log or display error messages
        cerr << "Error in viewing grades: " << e.what() << endl;
    }
    catch (const json::exception& e) {
        // Catch JSON parsing errors
        cerr << "JSON parsing error: " << e.what() << endl;
    }
    catch (const exception& e) {
        // Catch any other standard exceptions
        cerr << "Unexpected error: " << e.what() << endl;
    }
    catch (...) {
        // Catch-all for any unexpected errors
        cerr << "An unknown error occurred while retrieving grades." << endl;
    }
}

Student :: Student(const string &userId = "", const string& userName = "", StudentStats* stat = nullptr) : User(userId, userName), stats(stat) 
{
    if (stats == nullptr) 
    {
        stats = new StudentStats[1];  // Initialize with default values
        stats[0] = {"", 0, 0.0f};  // Default course code, grade, and attendance
    }
}
   
Student :: ~Student() 
{
    delete[] stats;
}