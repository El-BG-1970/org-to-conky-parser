# Org to conky parser
This is a small C project meant to parse and organize the contents of my EMACS org todolist, in order to display it on my desktop with Conky.

## Getting started
To get a copy of the project, clone the repository. 
To compile it, a simple `make` should do just fine.

## Prerequisites
This project is set to compile using `clang`, if you want to change that, go into the `Makefile` and edit the `CC` line.

## Use with Conky
- Compile the project
- Add the following line to your conky config: `${exec /path/to/otc /path/to/todolist.org}`
