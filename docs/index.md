---
index: true
---

This project generates a timetable given a set of inputs and constraints. It encodes constraints of the timetabling problem as a SAT formula in the Conjunctive Normal Form, and then calls a MaxSAT solver to solve the problem.

The code documentation can be found [here](https://timetabler.readthedocs.io).

## Installation

**For Ubuntu/Debian:** Use the deb package from [here](https://github.com/GoodDeeds/Timetabler/releases).

Timetabler can also be installed as a snap package from [here](https://snapcraft.io/timetabler) or by using the following command.
```bash
$ sudo snap install timetabler
```

Note: If you are using Ubuntu version prior to 16.04 or a distribution other than Ubuntu, then snapd has to be installed by following the instructions given [here](https://docs.snapcraft.io/installing-snapd/6735).

To build the timetabler from source, follow the instructions given in the [README.md](https://github.com/GoodDeeds/Timetabler/blob/master/README.md) of the project.

## Running the Timetabler

To execute the program, use
```bash
$ timetabler fields.yml input.csv custom.txt output.csv
```
where
* `fields.yml` is the path to the file containing the list of values a field can take and the weights for the constraints. This includes list of instructors, available classrooms, weights of clauses, etc.
* `input.csv` is the path to the file containing the input data. This file contains the course data input given to the solver as a CSV file.
* `custom.txt` is the path to the file containing the list of custom constraints. This file contains the custom constraints that can be provided to the solver using the grammar provided. For the full grammar, please refer [here](docs/custom).
* `output.csv` is the path to the file to which the output must be written to.

A detailed explanation on each file can be found [here](docs/files).

{% if page.index %}
## Documentation
{% for doc in site.docs %}
1. [{{doc.title}}]({{doc.url|prepend:site.baseurl}})
{% endfor %}
{% endif %}

## Video tutorial

**TODO**

## License

This software is provided under the [MIT License](https://github.com/GoodDeeds/Timetabler/blob/master/LICENSE).
