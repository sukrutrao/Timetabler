enum class MinorType {
	isMinor,
	isNotMinor
}


class IsMinor : public Field {
private:
	MinorType minorType;
public:
    IsMinor(MinorType); 
    FieldType getType();
};
