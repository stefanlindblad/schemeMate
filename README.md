# schemeMate

schemeMate is a scheme command line interpreter for the [scheme](http://www.scheme.com/tspl4/intro.html#./intro:h1) language written in C. 

Installation:

Clone the repository and execute "make" to build the source code.  
If you have make and gcc installed and in your path environment,  
you can afterwards launch schemeMate by typing "schemeMate".  

====================================================================

The repository also contains config files to use Visual Studio Code  
in combination with GCC / GDB to develop with the interpreter.  
The json config files would need to be adopted to your compiler location.  

====================================================================

Types:  
* #t - Logical True
* #f - Logical False
* #void - Empty Type
* 42 - Numbers (integer)
* nil/() - Null Type
* "a" - Strings
* cons/car/cdr - List Access
* functions
* symbols

Implemented math functions:  
* "+" - Addition
* "-" - Substraction
* "*" - Multiplikation
* "/" - Division
* max - Maximum
* min - Minimum
* magnitude - Absolute Value
* modulo - Modulo Operation

Implemented logic functions:  
* eq? - Memory based comparison (eq? Auto Auto) => #t
* = - Number based comparison (= 5 4 3) => #f
* string= - String based comparsion (string= "abc" "abc") => #t

Most important Limitations:
* No garbage collection currently
* Only supports integers

// Stefan Seibert, in the Summer 2017
