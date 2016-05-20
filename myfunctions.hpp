#ifndef __MYFUNCTIONS__HPP
#define __MYFUNCTIONS__HPP
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "myinode.hpp"
#include "myinodelist.hpp"
#include "mystats.hpp"
#include "mytree.hpp"

//void compare(MyINode m1, MyINode  m2);

bool prepareTarget(std::string path, std::string prefix, mode_t mode, MyStats* stats, MyTree* tree, bool verbose);

int fileCopy(std::string path, std::string sourcePrefix, std::string targetPrefix, MyINode* mind);

//void listDirectory(const char* dirName);

void countItems(std::string path, std::string pathPrefix, MyStats* ms, MyTree* tree);

void getOtherNodes(std::string path, std::string pathPrefix, MyTree* tree);

void removeDirectory(std::string path, std::string pathPrefix, MyStats* stats, bool verbose);

bool isTargetUpdated(MyINode* source, MyINode* target);

void copySymLink(std::string oldpath, std::string sourcePrefix, std::string targetPrefix, int pathSize, bool verbose);

std::string getPrefix(const char* pathName);

std::string getDirectoryName(const char* pathName);

void copyEntireDirectory(std::string path, std::string sourcePrefix, std::string targetPrefix, MyStats* stats, MyTree* tree, bool verbose);

void copyDirectory(std::string path, std::string sourcePrefix, std::string targetPrefix, MyStats* stats, MyTree* tree, bool verbose); //some verbose missing

void getDirectoryItems(DIR* dirptr, std::string path,std::string pathPrefix, MyINodeList* myFiles, MyINodeList* mySoftlinks, MyINodeList* myDirectories);

void removeExtraItems(MyINodeList* ml, std::string prefix, MyStats* stats, int mode, bool verbose);

void createNewFileOrHardLink(MyINode* mi, std::string sourcePrefix, std::string targetPrefix, MyTree* tree, MyStats* stats, bool verbose);

#endif

