
class Segment {
private:
    int startSegment;
    int endSegment;
public:
	Segment(int, int);
	bool operator==(const Segment &other);
	int length();
	bool isIntersecting(const Segment &other);
};
