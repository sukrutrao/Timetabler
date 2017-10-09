class Variable {
private:
    Course *course;
    Field *field;
    FieldType fieldType;
public:
    Variable(Course*, Field*);
    bool hasSameCourse(const Variable &other);
    bool hasSameType(const Variable &other);
    FieldType getType();
};
