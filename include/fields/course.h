#include <string>
#include <vector>
#include <utility>

class Course {
private:
    std::string name;
    unsigned classSize;
    Instructor *instructor;
    std::vector<Program*> programs;
    Segment *segment;
    IsMinor isMinor;
public:
    Course(std::string, unsigned, Instructor*, Segment*, IsMinor);
    Course(std::string, unsigned, Instructor*, Segment*, IsMinor, std::vector<Program*>);
    void setPrograms(std::vector<Program*>);
    void addProgram(Program*);
    bool operator==(const Course &other);
};
