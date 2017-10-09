class Time {
private:
    unsigned hours;
    unsigned minutes;
public:
    Time(unsigned, unsigned);
    Time& operator=(const Time&);
    bool operator==(const Time&);
    bool operator<(const Time&);
    bool operator<=(const Time&);  
    bool operator>=(const Time&);
    bool operator>(const Time&);
};

class Slot {
private:
    std::string name;
    Time startTime, endTime;
public:
    Slot(std::string, Time&, Time&);
    bool operator==(const Slot &other);
    bool isIntersecting(const Slot &other);
};
