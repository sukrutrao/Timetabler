class Variable {
private:
    std::unique_ptr<Course> course;
public:
    Variable(std::unique_ptr<Course>);
};

class ClassroomFitVar : public Variable {
private:
    std::unique_ptr<Classroom> classroom;
public:
    ClassroomFitVar(std::unique_ptr<Course>, std::unique_ptr<Classroom>);
};

class SegmentVar : public Variable {
private:
    std::unique_ptr<Segment> segment;
public:
    SegmentVar(std::unique_ptr<Course>, std::unique_ptr<Segment>);
};

class InstructorVar : public Variable {
private:
    
}

class ClassroomVar : public Variable {
private:
    std::unique_ptr<Classroom> classroom;
public:
    ClassroomVar(std::unique_ptr<Course>, std::unique_ptr<Classroom>);
};



class 
