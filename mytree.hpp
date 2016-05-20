#ifndef __MYTREE__HPP
#define __MYTREE__HPP
#include <iostream>
#include "mylist.hpp"
#include "myinode.hpp"

class MyTree{
    struct Node{
        long content;
	long otherNode;
	MyList names;

        Node* left;
        Node* right;

        Node(MyINode* inode);
        ~Node();
    };
    Node* root;
    int nodes;

        //bool isEmpty();

        void insertEntry(MyINode* inode, Node* n);
	void setOtherNode(MyINode* inode, Node* n);
	bool exists(long inode, Node* n);

	std::string getPath(long inode, Node* n);
	MyList* getAllPaths(long inode, Node* n);
	long getOtherNode(std::string name, Node* n);	
	void print(Node* n);
	void dissolveTree(Node* n);

    public:
        MyTree();
        ~MyTree();
	bool isEmpty();
	void insertEntry(MyINode* inode);
        bool exists(long inode);
	
	std::string getPath(long inode);
	MyList* getAllPaths(long inode);

	void setOtherNode(MyINode* inode);
	long getOtherNode(std::string name);

  
	void print();
};

#endif
