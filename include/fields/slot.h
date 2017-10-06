class Time {
private:
    unsigned hours;
    unsigned minutes;
public:
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
};
