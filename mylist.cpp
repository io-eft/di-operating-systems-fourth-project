#include "mylist.hpp"

MyList::Node::Node(): content(""), next(NULL) {}

MyList::Node::Node(const char* c): content(c), next(NULL){}

MyList::Node::Node(std::string s): content(s), next(NULL){}

MyList::Node::~Node()
{

}

MyList::MyList():head(NULL), tail(NULL), items(0) {}

MyList::~MyList()
{
    Node* temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        delete temp;
    }
}

bool MyList::isEmpty()
{
    if(head == NULL)
        return true;
    else
        return false;
}

void MyList::insertEntry(const char* c)
{
    Node* n = new Node(c);
    if(isEmpty())
    {
        head = n;
        tail = n;
    }
    else
    {
        tail->next = n;
        tail = n;
    }
    items++;
}

void MyList::insertEntry(std::string s)
{
    Node* n = new Node(s);
    if(isEmpty())
    {
        head = n;
        tail = n;
    }
    else
    {
        tail->next = n;
        tail = n;
    }
    items++;
}

void MyList::print()
{
    Node* current = head;
    while(current != NULL)
    {
        std::cout << current->content << std::endl;
        current=current->next;
    }
}

std::string MyList::getTop()
{
    return head->content;
}

void MyList::removeTopEntry()
{
    Node* temp = head;
    head = head->next;
    items--;
    delete temp;
}

int MyList::length()
{
    return items;
}

std::string MyList::operator[](unsigned int i)
{
    if(i <= items)
    {
        Node* n = head;
        for(int j = 0; j < i; j++)
            n = n->next;
        return n->content;
    }
    else
    {
        std::string s = "empty string";
        return s;
    }
}

bool MyList::exists(std::string name)
{
    Node* temp = head;
    while(temp != NULL)
    {
        if(temp->content == name)
            return true;
        temp = temp->next;
    }
    return false;
}

std::string MyList::itemAt(int i)
{
    if(i <= items)
    {
        Node* n = head;
        for(int j = 0; j < i; j++)
            n = n->next;
        return n->content;
    }
    else
    {
        std::string s = "empty string";
        return s;
    }
}
