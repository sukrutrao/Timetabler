#!/bin/bash
Timetabler_VERSION=$(git describe --always --tags)
if [ -z "$Timetabler_VERSION" ]
then
	# Update the below line after every release
	Timetabler_VERSION="v0.3.0"
fi
VERSION_INCLUDE_STR="#ifndef __TIMETABLER_VERSION__\n#define __TIMETABLER_VERSION__ \"${Timetabler_VERSION}\"\n#endif"
if [ "$2" == "1" ]
then
	echo ${Timetabler_VERSION:1}
else
	echo -e $VERSION_INCLUDE_STR > $1/include/version.h
	echo "set (Timetabler_VERSION \"${Timetabler_VERSION:1}\")" > $1/TimetablerVersion.cmake
fi
