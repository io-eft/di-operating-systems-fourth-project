all: rapcp

rapcp: rapcp.o myinode.o myfunctions.o mylist.o mytree.o mystats.o myinodelist.o
	g++ -g -Wall rapcp.o myinode.o myfunctions.o mylist.o mytree.o mystats.o myinodelist.o -o rapcp

rapcp.o: rapcp.cpp
	g++ -c -g3 rapcp.cpp

myinode.o: myinode.cpp
	g++ -c -g3 myinode.cpp

myfunctions.o: myfunctions.cpp
	g++ -c -g3 myfunctions.cpp

mylist.o: mylist.cpp
	g++ -c -g3 mylist.cpp

mytree.o: mytree.cpp
	g++ -c -g3 mytree.cpp

mystats.o: mystats.cpp
	g++ -c -g3 mystats.cpp

myinodelist.o: myinodelist.cpp
	g++ -c -g3 myinodelist.cpp

clean:
	rm -rf *o test
