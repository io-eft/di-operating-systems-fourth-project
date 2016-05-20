#include "mytree.hpp"

MyTree::Node::Node(MyINode* inode): left(NULL), right(NULL), otherNode(-1)
{
    content = inode->getINodeNumber();
    names.insertEntry(inode->getName());
}

MyTree::Node::~Node(){}

MyTree::MyTree(): root(NULL), nodes(0) {}

MyTree::~MyTree()
{
    if(!isEmpty())
        dissolveTree(root);
}

bool MyTree::isEmpty()
{
    if(root == NULL)
        return true;
    else
        return false;
}

void MyTree::insertEntry(MyINode* inode)
{
    if(isEmpty())
    {
        nodes++;
        Node* n = new Node(inode);
        root = n;
    }
    else
    {
        insertEntry(inode, root);
    }
}

void MyTree::insertEntry(MyINode* inode, Node* n)
{
    if(inode->getINodeNumber() == n->content)
    {
        n->names.insertEntry(inode->getName());
        return;
    }
    if(inode->getINodeNumber() < n->content)
    {
        if(n->left == NULL)
        {
            Node* newNode = new Node(inode);
            n->left = newNode;
            nodes++;
            return;
        }
        else
        {
            insertEntry(inode, n->left);
            return;
        }
    }
    if(inode->getINodeNumber() > n->content)
    {
        if(n->right == NULL)
        {
            Node* newNode = new Node(inode);
            n->right = newNode;
            nodes++;
            return;
        }
        else
        {
            insertEntry(inode, n->right);
            return;
        }
    }
}

void MyTree::dissolveTree(Node* n)
{
    if(n->left != NULL)
        dissolveTree(n->left);
    if(n->right != NULL)
        dissolveTree(n->right);
    delete n;
}

bool MyTree::exists(long inode)
{
    if(root != NULL)
        return exists(inode, root);
    return false;
}

bool MyTree::exists(long inode, Node* n)
{
    if(inode == n->content)
        return true;
    else
    {
        if(inode < n->content)
        {
            if(n->left == NULL)
                return false;
            else
                return exists(inode, n->left);
        }
        if(inode > n->content)
        {
            if(n->right == NULL)
                return false;
            else
                return exists(inode, n->right);
        }
    }
}

std::string MyTree::getPath(long inode)
{
    if(root != NULL)
        return getPath(inode, root);
    return ".";
}

std::string MyTree::getPath(long inode, Node* n)
{
    if(inode == n->content)
        return n->names.getTop();
    else
    {
        if(inode < n->content)
        {
            if(n->left == NULL)
                return ".";
            else
                return getPath(inode, n->left);
        }
        if(inode > n->content)
        {
            if(n->right == NULL)
                return ".";
            else
                return getPath(inode, n->right);
        }
    }
}

void MyTree::print()
{
    if(!isEmpty())
    {
        print(root);
    }
    else
        std::cout << "Tree is empty!" << std::endl;
}

void MyTree::print(Node* n)
{
    std::cout << n->content << std::endl;
    n->names.print();
    std::cout << std::endl;
    if(n->left != NULL)
        print(n->left);
    if(n->right != NULL)
        print(n->right);
}

MyList* MyTree::getAllPaths(long inode)
{
    if(!isEmpty())
        return getAllPaths(inode, root);
    MyList* a = new MyList;
        return a;
}

MyList* MyTree::getAllPaths(long inode, Node* n)
{
    MyList* a = new MyList;
    if(inode == n->content)
    {
        MyList* ml = new MyList;
        for(int i = 0; i < n->names.length(); i++)
        {
            ml->insertEntry((n->names)[i]);
        }
        delete a;
        return ml;
    }
    else
    {
        if(inode < n->content)
        {
            if(n->left == NULL)
                return a;
            else
            {
                delete a;
                return getAllPaths(inode, n->left);
            }
        }
        if(inode > n->content)
        {
            if(n->right == NULL)
                return a;
            else
            {
                delete a;
                return getAllPaths(inode, n->right);
            }
        }
    }
}

void MyTree::setOtherNode(MyINode* inode)
{
    if(!isEmpty())
        setOtherNode(inode, root);
}

void MyTree::setOtherNode(MyINode* inode, Node* n)
{
    if(n->names.exists(inode->getName()))
    {
        if(n->otherNode == -1)
            n->otherNode = inode->getINodeNumber();
        return;
    }
    if(n->left != NULL)
    {
        setOtherNode(inode, n->left);
    }
    if(n->right != NULL)
    {
        setOtherNode(inode, n->right);
    }
}

long MyTree::getOtherNode(std::string name)
{
    if(!isEmpty())
        return getOtherNode(name, root);
    return -1;
}

long MyTree::getOtherNode(std::string name, Node* n)
{
    long i = -1;
    if(n->names.exists(name))
        return n->otherNode;
    if(n->left != NULL)
    {
         i = getOtherNode(name, n->left);
         if(i != -1)
            return i;
    }
    if(n->right != NULL)
    {
        i = getOtherNode(name, n->right);
        if(i != -1)
            return i;
    }
    return i;
}
