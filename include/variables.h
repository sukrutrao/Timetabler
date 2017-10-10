class Variable {
private:
    Course *course;
    Field *field;
public:
    Variable(Course*, Field*);
    bool hasSameCourse(const Variable &other);
    bool hasSameType(const Variable &other);
    bool hasSameFieldValue(const Variable &other);
    FieldType getType();
};
