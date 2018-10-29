---
layout: default
title: Custom constraints
---

# Grammar for custom constraints

```
integer              : [-]?[0-9]+
fieldtype            : INSTRUCTOR | SEGMENT | ISMINOR | PROGRAM
value                : [a-zA-Z0-9\.-@ ]+
allvalues            : [*]
listvalues           : [{] value (, value)* [}]
values               : allvalues | SAME | NOTSAME | listvalues
classroomdecl        : CLASSROOM values
slotdecl             : SLOT values
coursenoexceptdecl   : COURSE values
courseexceptdecl     : COURSE EXCEPT values
coursedecl           : coursenoexceptdecl | courseexceptdecl
decl                 : classroomdecl | slotdecl
decls                : decl (AND decl)*
fielddecl            : fieldtype values
fielddecls           : (fielddecl (AND fielddecl)*)?
constraint_expr      : coursedecl fielddecls (NOT)? IN decls
constraint_braced    : [(] constraint_or [)]
constraint_not       : NOT constraint_braced
constraint_val       : constraint_expr | constraint_not | constraint_braced
constraint_and       : constraint_val (AND constraint_val)*
constraint_or        : constraint_and (OR constraint_or)*
constraint_unbundle  : coursedecl UNBUNDLE fielddecls (NOT)? IN decl WEIGHT integer
wconstraint          : constraint_or WEIGHT integer
grammar              : (wconstraint | constraint_unbundle)*
```

Weighted constraints are specified in the file. Weights can be either positive or negative integer. Positive weight specifies that the clause is soft whereas negative weight specifies that the clause is hard. Constraints are written according to the above-given grammar.

## Examples

```
COURSE {CS101,CS102} SEGMENT {12,16,14} IN CLASSROOM {CL1, CL2} WEIGHT 5
```
The above constraint specifies that the courses CS101, CS102 which are scheduled in segments in either of 12,16,14 must be scheduled in the classroom CL1 or CL2.

```
COURSE * INSTRUCTOR {A} IN SLOT {A,B,C} AND CLASSROOM {CL1,CL2} WEIGHT -1
```
The above constraint specifies that all the courses which are taught by instructor A should be scheduled in slot A or B or C and classroom CL1 or CL2.

```
COURSE {CS101, CS102} IN CLASSROOM SAME WEIGHT 5
```
The above constraint specifies that the courses CS101 and CS102 should be scheduled in the same classroom. Similarly `NOTSAME` can be used to specify that they should not be scheduled in the same classroom.

```
(COURSE * INSTRUCTOR {A,B} IN SLOT {A,B}) AND (COURSE {CS101,CS102} NOT IN CLASSROOM {CL2}) WEIGHT 10
```
More complex constraints can be written by disjuntion and conjunction of other clauses.