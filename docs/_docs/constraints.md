---
layout: default
title: Default constraints
---

# Default Constraints Implemented in the Timetabler

This page describes the default constraints implemented in the Timetabler. Broadly, there are three types of constraints:

1. User Assignment Constraints
1. Field Value Assignment Constraints
1. Scheduling Constraints

## User Assignment Constraints
These constraints specify the extent to which the assignment of field values by the user must be respected. For example, if the user specifies that course A must have instructor B, then the weight of this constraint specifies whether the solver can change the instructor if needed, and if so, at what priority, or if must not change the field even if it means that creating a timetable is no longer possible. Weights are specified per field type.

The rules for specifying the weights are the same - a zero weight implies that the solver can freely change the field value, a negative weight implies that the solver must not change the value under any circumstance, and a positive weight provides how much importance the solver must give to this constraint relative to other constraints. These weights can be modified as usual in the fields file.

By default, the weights for these are 1, except for the fields IsMinor, Program, and Instructor, for which the weights are -1. This is under the assumption that the fact whether a course is minor, the programs for which a course is applicable, and the instructor of the course may not be flexible.

If any field in the input is left empty for a given course, the effective weight for that field pertaining to that course becomes zero.

## Field Value Assignment Constraints
These constraints specify the extent to which the solver must try to obtain a field value for a given field type, which is consistent with other constraints provided. For example, for the field type Instructor, this specifies the importance the solver must give to ensure that every course is assigned an instructor. Weights are specified per field type. This applied to all field types except Program, because assigning a program to a course is not an essential aspect, whereas every course generally needs an instructor, classroom, etc.

Once again, a a zero weight implies that the solver need not assign a field value to the particular field type, a negative weight implies that the solver must assign a value under any circumstance, and a positive weight provides how much importance the solver must give to this constraint relative to other constraints. These weights can be modified in the fields file. By default, the weights for these are 1.

## Scheduling Constraints
These constraints specify the rules to follow to ensure feasible scheduling. They include constraints such as the constraint that an instructor or a classroom cannot have two courses scheduled at the same time. They also include soft clauses, such as to try to place a core course in a morning slot.

The weights can be set using the same rules as already specified. By default, the weights are -1, as violating most of these constraints would not make practical sense. The exception is for soft clauses, which are - a core course should be in morning time, an elective course should be in non-morning time, the weights for which are 1. Any slot for which each time slot element begins before 1pm is considered to be a morning slot.

The following are the list of scheduling constraints implemented:

1. **instructorSingleCourseAtATime** \\
An Instructor can have only one course in a given segment and slot.
1. **classroomSingleCourseAtATime** \\
A Classroom can have only one course in a given segment and slot.
1. **programSingleCoreCourseAtATime** \\
No program can have two core courses with an intersecting schedule.
1. **minorInMinorTime** \\
A minor course must be given a minor slot and vice-versa. A slot can be specified to be a minor slot in the field input.
1. **exactlyOneSlotPerCourse** \\
A course must be given exactly one Slot. A slot is a set of time units in the week given a name, and is specified in the field input.
1. **exactlyOneInstructorPerCourse** \\
A course must be given exactly one Instructor.
1. **exactlyOneSegmentPerCourse** \\
A course must be given exactly one Segment. A "Segment" refers to a Segment object, which consists of a start and an end segment. For example, a course can be in the 14 segment, but it cannot be in both the 14 and 36 segments (in this case, it would actually then be in the 16 segment).
1. **exactlyOneIsMinorPerCourse** \\
A course must be given exactly one IsMinor value. This means that a course can either be a minor course or not, but not both.
1. **exactlyOneClassroomPerCourse** \\
A course must be given exactly one Classroom.
1. **coreInMorningTime** \\
A core course must be scheduled in a morning slot. This is by default a soft clause with weight 1. A morning slot is a slot in which all time units in the week have the start time before 1pm.
1. **electiveInNonMorningTime** \\
An elective course must be scheduled in a non morning slot. This is by default a soft clause with weight 1. A non-morning slot is any slot which is not a morning slot. In particular, it may have some time units in the week that are in the morning.
1. **programAtMostOneOfCoreOrElective** \\
A course can be applicable to a program as a core or elective, not both.