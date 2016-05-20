#ifndef __MYLIST__HPP
#define __MYLIST__HPP
#include <iostream>
#include <string>	

class MyList{
    struct Node{
        std::string content;
        Node* next;

        Node(const char* c);
	Node(std::string s);
        Node();
	~Node();
    };
    Node* head;
    Node* tail;
    unsigned int items;

    public:
        MyList();
        ~MyList();
        bool isEmpty();
	bool exists(std::string name);
        int length();

        std::string operator[](unsigned int i);

        void insertEntry(const char* c);
	void insertEntry(std::string s);
        std::string getTop();
        void removeTopEntry();
	std::string itemAt(int i);

        void print();
};

#endif

