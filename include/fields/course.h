#include <string>
#include <vector>
#include <utility>

class Course {
private:
    std::string name;
    unsigned classSize;
    std::unique_ptr<Instructor> instructor;
    std::vector<std::pair<std::unique_ptr<Program>, std::unique_ptr<CourseType>>> programs;
    std::unique_ptr<Segment> segment;
    std::unique_ptr<IsMinor> isMinor;
public:
    Course(std::string, std::unique_ptr<Instructor>, std::unique_ptr<Segment>, std::unique_ptr<IsMinor>);
    void setPrograms(std::vector<std::pair<std::unique_ptr<Program>, std::unique_ptr<CourseType>>>);
    void addProgram(std::unique_ptr<Program>, std::unique_ptr<CourseType>);
};
