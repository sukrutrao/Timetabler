class Classroom : public Field {
private:
    std::string number;
    unsigned size;
public:
	Classroom(std::string, unsigned);
	bool operator==(const Classroom &other);
	bool sizeLessThan(const Classroom &other);
	FieldType getType();
};
