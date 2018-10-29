---
layout: default
title: Input files
---

# Input files required by the program

Four file paths are given to the program as command line arguments.

1. fields.yml
2. input.csv
3. custom.txt
4. output.csv

## YAML fields file

The information related to fields is given in a YAML file.
An example of the YAML file is given below.

```yaml
# To specify weights for the clauses.
# The first element of the array is the weight for the clause specifying assignment of the field value to a particular course.
# The second element of the array is the weight for the clause specifying that there should be exactly one assignment for the current field value to a particular course.
# For program field, a course can be offered to many courses, hence only one weight for the clause specifying assignment is given.
# The weight is an integer. Negative weight indicates hard clause and positive weight indicates soft clause with the corresponding weight.
weights: # Assignment, high level
  instructor: [-1, -1]
  segment: [-1, -1]
  is_minor: [-1, -1]
  program: -1
  classroom: [-1, -1]
  slot: [-1, -1]

# To specify weights for the predefined clauses
# No  Clause                             Info
# 0   instructorSingleCourseAtATime      Instructor can have only one course in a given segment and slot.
# 1   classroomSingleCourseAtATime       Classroom can have only one course in a given segment and slot.
# 2   programSingleCoreCourseAtATime     No program can have two core courses with an intersecting schedule.
# 3   minorInMinorTime                   A minor course must be given a minor slot and vice-versa.
# 4   exactlyOneSlotPerCourse            A course must be given exactly one Slot.
# 5   exactlyOneInstructorPerCourse      A course must be given exactly one Instructor.
# 6   exactlyOneSegmentPerCourse         A course must be given exactly one Segment.
# 7   exactlyOneIsMinorPerCourse         A course must be given exactly one IsMinor value.
# 8   exactlyOneClassroomPerCourse       A course must be given exactly one Classroom.
# 9   coreInMorningTime                  A core course must be scheduled in a morning slot.
# 10  electiveInNonMorningTime           An elective course must be scheduled in a non morning slot.
# 11  programAtMostOneOfCoreOrElective   A course can be applicable to a program as a core or elective, not both.

# Note - a morning slot is any slot where every time period has a start time before 13:00.

# Here specify the predefined clause number and their corresponding weight
# The default value, if not specified is -1 (Hard clause)
predefined_weights:
  - clause: 0
    weight: -1
  - clause: 8
    weight: 10

# Specify the instructors
instructors:
  - A
  - B

# Specify the classrooms with their capacity
classrooms:
  - number: CL1
    size: 50
  - number: CL2
    size: 80

# Specify the start and end segment numbers
segments:
  start: 1
  end: 6

# Specify the possible slots in the timetable
slots:
  - name: A
    is_minor: false
    time_periods:
      - day: Monday
        start: 16:00
        end: 17:30
      - day: Wednesday
        start: 14:30
        end: 16:00
  - name: B
    is_minor: true
    time_periods:
      - day: Tuesday
        start: 10:00
        end: 11:00

# Specify all the programs
programs:
  - B.Tech.1
  - B.Tech.2
  - B.Tech.3
  - B.Tech.4
  - M.Tech.
```

The desciption of default constraints implemented in the timetabler can be found [here](constraints).

## Input CSV file

The course information is given is a CSV file.
In the file, the preassigned classroom and slot can be specified.
```csv
name,class_size,instructor,segment,is_minor,B.Tech.1,B.Tech.2,B.Tech.3,B.Tech.4,M.Tech.,classroom,slot
CS101,50,A,14,No,Core,Elective,No,No,No,,
CS102,30,B,16,Yes,No,No,Elective,No,Core,CL1,A
```

## Custom constraints

The custom constraints can be specified in another file.
The grammar for custom constraint specification is given [here](custom).

## Output CSV file

The program prints the output in the CSV file in the same format as that of input.