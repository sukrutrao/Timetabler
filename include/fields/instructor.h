#include <string>

class Instructor {
private:
    std::string name;
public:
	Instructor(std::string);
	bool operator==(const Instructor &other);
};
