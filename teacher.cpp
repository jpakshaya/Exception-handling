#include "users.h"
#include <stdexcept>

class StudentException : public std::runtime_error {
public:
    explicit StudentException(const string& message) : std::runtime_error(message) {}
};

Teacher :: Teacher(const string &userId = "", const string &userName = "", Course* c = nullptr) : User(userId, userName), courses(c)
{
    if (courses == nullptr) 
    {
        courses = new Course[1];  // Initialize with default values
        courses[0].Name = -1;
    }
}

Teacher :: ~Teacher()
{
    delete[] courses;
}

void Teacher :: viewProfile() const 
{
    try {
        cout << "\n========== Teacher Profile ==========\n";
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;

        // Read teachers data
        ifstream teachersFile("teachers.json");
        if (!teachersFile.is_open()) {
            throw runtime_error("Unable to open teachers.json");
        }
        json teachersData;
        teachersFile >> teachersData;
        teachersFile.close();

        // Validate teachers data structure
        if (!teachersData.contains("teachers")) {
            throw runtime_error("Invalid teachers data format");
        }

        // Check if teacher has any assigned courses
        auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                                 [this](const json& teacher) { return teacher["id"] == this->id; });

        if (teacherIt == teachersData["teachers"].end()) {
            throw StudentException("Teacher not found in the system");
        }

        if (!teacherIt->contains("courses") || teacherIt->at("courses").empty()) {
            cout << "Courses: Not assigned to any courses.\n";
            return;
        }

        cout << "\nAssigned Courses:\n";
        cout << "----------------------------------------------------\n";
        cout << "Course Code\tCourse Name\n";
        cout << "----------------------------------------------------\n";

        // Read courses data
        ifstream coursesFile("courses.json");
        if (!coursesFile.is_open()) {
            throw runtime_error("Unable to open courses.json");
        }
        json coursesData;
        coursesFile >> coursesData;
        coursesFile.close();

        // Validate courses data structure
        if (!coursesData.contains("courses")) {
            throw runtime_error("Invalid courses data format");
        }

        for (const auto& course : teacherIt->at("courses")) {
            string courseCode = course["Code"];

            auto courseIt = find_if(coursesData["courses"].begin(), coursesData["courses"].end(),
                                    [&courseCode](const json& c) { return c["code"] == courseCode; });

            string courseName = (courseIt != coursesData["courses"].end()) ? courseIt->at("name") : "Unknown";

            cout << courseCode << "\t\t" << courseName << "\n";
        }

        cout << "----------------------------------------------------\n";
        cout << "Total Courses: " << teacherIt->at("courses").size() << endl;
        cout << "\n======================================\n";
    }
    catch (const StudentException& e) {
        cout << "Profile Error: " << e.what() << endl;
    }
    catch (const runtime_error& e) {
        cout << "System Error: " << e.what() << endl;
    }
    catch (const json::exception& e) {
        cout << "Data Error: " << e.what() << endl;
    }
    catch (...) {
        cout << "An unexpected error occurred while viewing profile." << endl;
    }
}

void Teacher :: updateAttendance(const string& studentId, const string& courseCode) {
    try {
        // Read teachers data
        ifstream teachersFile("teachers.json");
        if (!teachersFile.is_open()) {
            throw runtime_error("Unable to open teachers.json");
        }
        json teachersData;
        teachersFile >> teachersData;
        teachersFile.close();

        // Validate teachers data
        if (!teachersData.contains("teachers")) {
            throw runtime_error("Invalid teachers data format");
        }

        // Check if the teacher is handling any courses
        auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                                 [this](const json& teacher) { return teacher["id"] == this->id; });

        // Validate student ID format (assuming it should not be empty)
        if (studentId.empty()) {
            throw StudentException("Invalid student ID. ID cannot be empty.");
        }

        // Read courses data to verify course existence
        ifstream coursesFile("courses.json");
        if (!coursesFile.is_open()) {
            throw runtime_error("Unable to open courses.json");
        }
        json coursesData;
        coursesFile >> coursesData;
        coursesFile.close();

        // Validate course code
        auto courseIt = find_if(coursesData["courses"].begin(), coursesData["courses"].end(),
                                [&courseCode](const json& course) { return course["code"] == courseCode; });
        if (courseIt == coursesData["courses"].end()) {
            throw StudentException("Invalid course code. Course does not exist.");
        }

        if (teacherIt == teachersData["teachers"].end() || !teacherIt->contains("courses") || teacherIt->at("courses").empty()) {
            throw StudentException("You are not assigned to any courses.");
        }

        // Verify if the teacher is handling the entered course
        auto teacherCourseIt = find_if(teacherIt->at("courses").begin(), teacherIt->at("courses").end(),
                                [&courseCode](const json& course) { return course["Code"] == courseCode; });

        if (teacherCourseIt == teacherIt->at("courses").end()) {
            throw StudentException("You are not authorized to update attendance for this course.");
        }

        // Read enrollments data
        ifstream enrollmentsFile("enrollments.json");
        if (!enrollmentsFile.is_open()) {
            throw runtime_error("Unable to open enrollments.json");
        }
        json enrollmentsData;
        enrollmentsFile >> enrollmentsData;
        enrollmentsFile.close();

        // Validate enrollments data
        if (!enrollmentsData.contains("enrollments")) {
            throw runtime_error("Invalid enrollments data format");
        }

        // Check if the student exists in enrollments
        if (!enrollmentsData["enrollments"].contains(studentId)) {
            throw StudentException("Invalid student ID. Student not found in system.");
        }

        // Check if the student is enrolled in the course
        auto enrollmentIt = find_if(enrollmentsData["enrollments"][studentId].begin(),
                                    enrollmentsData["enrollments"][studentId].end(),
                                    [&courseCode](const json& enrollment) {
                                        return enrollment["courseCode"] == courseCode;
                                    });

        if (enrollmentIt == enrollmentsData["enrollments"][studentId].end()) {
            throw StudentException("Student is not enrolled in this course.");
        }

        int totalClasses, absents;

        // Get total classes and absents from teacher
        cout << "Enter total number of classes: ";
        cin >> totalClasses;
        cout << "Enter number of absents: ";
        cin >> absents;

        // Validate input
        if (totalClasses <= 0) {
            throw invalid_argument("Total classes must be greater than 0.");
        }
        if (absents < 0 || absents > totalClasses) {
            throw invalid_argument("Invalid number of absents.");
        }

        // Calculate attendance percentage
        float attendancePercentage = ((float)(totalClasses - absents) / totalClasses) * 100;

        // Round to two decimal places
        attendancePercentage = round(attendancePercentage * 100.0) / 100.0;

        // Update the attendance
        for (auto& enrollment : enrollmentsData["enrollments"][studentId]) {
            if (enrollment["courseCode"] == courseCode) {
                enrollment["attendance"] = attendancePercentage;
                break;
            }
        }

        // Write updated data back to file
        ofstream outFile("enrollments.json");
        if (!outFile.is_open()) {
            throw runtime_error("Unable to open enrollments.json for writing");
        }
        outFile << setw(4) << enrollmentsData << endl;
        outFile.close();

        cout << "Attendance updated successfully. New attendance: " << attendancePercentage << "%" << endl;
    }
    catch (const StudentException& e) {
        cout << "Student Error: " << e.what() << endl;
    }
    catch (const runtime_error& e) {
        cout << "System Error: " << e.what() << endl;
    }
    catch (const invalid_argument& e) {
        cout << "Input Error: " << e.what() << endl;
    }
    catch (const json::exception& e) {
        cout << "Data Error: " << e.what() << endl;
    }
    catch (...) {
        cout << "An unexpected error occurred while updating attendance." << endl;
    }
}

void Teacher :: updateGrade(const string& studentId, const string& courseCode) {
    try {
        json teachersData, enrollmentsData, coursesData;

        // Read teachers data
        ifstream teachersFile("teachers.json");
        if (!teachersFile.is_open()) {
            throw runtime_error("Unable to open teachers.json for reading!");
        }
        teachersFile >> teachersData;
        teachersFile.close();

        // Validate teachers data
        if (!teachersData.contains("teachers")) {
            throw runtime_error("Invalid teachers data format");
        }

        // Read courses data
        ifstream coursesFile("courses.json");
        if (!coursesFile.is_open()) {
            throw runtime_error("Unable to open courses.json for reading!");
        }
        coursesFile >> coursesData;
        coursesFile.close();

        // Validate course existence
        auto courseIt = find_if(coursesData["courses"].begin(), coursesData["courses"].end(),
                                [&courseCode](const json& course) { return course["code"] == courseCode; });
        if (courseIt == coursesData["courses"].end()) {
            throw StudentException("Invalid course code. Course does not exist.");
        }

        // Read enrollments data
        ifstream enrollmentsFile("enrollments.json");
        if (!enrollmentsFile.is_open()) {
            throw runtime_error("Unable to open enrollments.json for reading!");
        }
        enrollmentsFile >> enrollmentsData;
        enrollmentsFile.close();

        // Validate enrollments data
        if (!enrollmentsData.contains("enrollments")) {
            throw runtime_error("Invalid enrollments data format");
        }

        // Validate student ID format
        if (studentId.empty()) {
            throw StudentException("Invalid student ID. ID cannot be empty.");
        }

        // Verify teacher's authorization
        auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                                 [this](const json& teacher) { return teacher["id"] == this->id; });

        if (teacherIt == teachersData["teachers"].end() || !teacherIt->contains("courses")) {
            throw StudentException("You are not authorized to update grades.");
        }

        // Check if the teacher is assigned to the course
        auto teacherCourseIt = find_if(teacherIt->at("courses").begin(), teacherIt->at("courses").end(),
                                [&courseCode](const json& course) { return course["Code"] == courseCode; });

        if (teacherCourseIt == teacherIt->at("courses").end()) {
            throw StudentException("You are not authorized to update grades for this course.");
        }

        // Check if the student exists in enrollments
        if (!enrollmentsData["enrollments"].contains(studentId)) {
            throw StudentException("Invalid student ID. Student not found in system.");
        }

        auto enrollmentIt = find_if(enrollmentsData["enrollments"][studentId].begin(),
                                    enrollmentsData["enrollments"][studentId].end(),
                                    [&courseCode](const json& enrollment) {
                                        return enrollment["courseCode"] == courseCode;
                                    });

        if (enrollmentIt == enrollmentsData["enrollments"][studentId].end()) {
            throw StudentException("Student is not enrolled in this course.");
        }

        // Get grades from teacher
        float grade;
        cout << "\nEnter the new grade for the student (0-100): ";
        cin >> grade;

        // Validate input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Invalid input. Please enter a numeric value.");
        }

        if (grade < 0 || grade > 100) {
            throw invalid_argument("Invalid grade. Must be between 0 and 100.");
        }

        // Update the grade
        (*enrollmentIt)["grade"] = grade;

        // Write updated data back to file
        ofstream outFile("enrollments.json");
        if (!outFile.is_open()) {
            throw runtime_error("Unable to open enrollments.json for writing!");
        }
        outFile << enrollmentsData.dump(4);
        outFile.close();

        cout << "\nGrade updated successfully!" << endl;
        cout << "New Grade: " << grade << endl;
    }
    catch (const StudentException& e) {
        cout << "Student Error: " << e.what() << endl;
    }
    catch (const runtime_error& e) {
        cout << "System Error: " << e.what() << endl;
    }
    catch (const invalid_argument& e) {
        cout << "Input Error: " << e.what() << endl;
    }
    catch (const json::exception& e) {
        cout << "Data Error: " << e.what() << endl;
    }
    catch (...) {
        cout << "An unexpected error occurred while updating grade." << endl;
    }
}