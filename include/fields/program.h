
class Program {
private:
    std::string name; 
    CourseType courseType;
public:
	Program(std::string, CourseType);
	bool operator==(const Program &other); 
};
