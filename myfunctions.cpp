#include "myfunctions.hpp"

bool prepareTarget(std::string path, std::string prefix, mode_t mode, MyStats* stats, MyTree* tree, bool verbose)
{   //prepare target directory
    struct stat s;
    DIR* dirptr;
    std::string directoryName = prefix + path;          //Get New folder's location
    if((dirptr = opendir(prefix.c_str())) == NULL)      //Open target Directory
        std::cout << "Cannot access the directory " << prefix << std::endl;
    else
    {
        struct dirent* direntptr;
        while((direntptr = readdir(dirptr)) != NULL)    //while there are items on target directory
        {
            if(strcmp(direntptr->d_name, path.c_str()) == 0)    //in case one has the same name as our target
            {
                if(lstat(directoryName.c_str(), &s) == -1)      //get the stats of the item
                {
                    std::cout << "lstat failed on " << directoryName << std::endl;
                    closedir(dirptr);
                    return false;
                }
                else
                {
                    if(S_ISDIR(s.st_mode))  //if it's a directory. procceed
                    {
                        closedir(dirptr);
                        getOtherNodes(path, prefix, tree);
                        return true;
                    }
                    else    //else, delete it and create a directory
                    {
                        if(verbose)
                        {
                            std::cout << "Removing Entity at " << directoryName << " and creating a directory." << std::endl;
                        }
                        stats->increaseChangedDirectories();
                        if(S_ISREG(s.st_mode))
                            stats->increaseRemovedFiles();
                        if(S_ISLNK(s.st_mode))
                            stats->increaseRemovedSoftLinks();
                        remove(directoryName.c_str());
                        mkdir(directoryName.c_str(), mode);
                        //closedir(dirptr);
                        return true;
                    }
                }
            }
        }
        if(verbose) //if you don't find it, create it
        {
            std::cout << "Creating directory at " << directoryName << "." << std::endl;
        }
        stats->increaseChangedDirectories();
        mkdir(directoryName.c_str(), mode);
        closedir(dirptr);
        return true;
    }
    return false;
}

int fileCopy(std::string path, std::string sourcePrefix, std::string targetPrefix, MyINode* mind)
{
    int fileRead, fileWrite, bitsRead, bitsWritten;
    void* buffer;
    int fileSize = mind->getFileSize();
    std::string oldPath = sourcePrefix + path;  //get full path of source file
    std::string newPath = targetPrefix + path;  //get relative path of target file

    if((fileRead = open(oldPath.c_str(), O_RDONLY)) < 0)    //open the source
    {
        std::cout << "Failed to open (old) file to fileCopy." << std::endl;
        return -1;
    }

    buffer = malloc(fileSize);  //allocate enough memory to copy
    if(buffer == NULL)          //in case something went wrong with the allocation, free and exit
    {
        free(buffer);
        std::cout << "Malloc failed in fileCopy." << std::endl;
        return -2;
    }

    if((bitsRead = read(fileRead, buffer, fileSize)) == -1) //read the whole file
    {
        free(buffer);   //if something went wrong, free and exit
        std::cout << "Failed to read the (old) file in fileCopy." <<std::endl;
        return -3;
    }

    if((fileWrite = open(newPath.c_str(), O_CREAT | O_TRUNC | O_RDWR, mind->getMode())) < 0) //open file to write
    {
        free(buffer);   //if something went wrong, free and exit
        std::cout << "Failed to open (new) file to fileCopy. " << newPath << std::endl;
        return -4;
    }

    bitsWritten = write(fileWrite, buffer, fileSize);   //write on file

    if(bitsWritten != bitsRead)     //check what you wrote
    {
        free(buffer);           ////if something went wrong, free and exit
        std::cout << "Problem with write." << std::endl;
        return -5;
    }

    free(buffer);   //free memory

    if(close(fileRead) == -1)   //close files
    {
        std::cout << "Failed to close old file." << std::endl;
        return -6;
    }
    if(close(fileWrite) == -1)
    {
        std::cout << "Failed to close new file." << std::endl;
        return -7;
    }
    return bitsWritten;
}

void countItems(std::string path, std::string pathPrefix, MyStats* ms, MyTree* tree)
{    //count items and fill the tree
    ms->increaseTotalDirectories();
    DIR* dirptr;
    std::string directoryName = pathPrefix + path;
    if((dirptr = opendir(directoryName.c_str())) == NULL)
        std::cout << "Cannot access the directory " << directoryName << "." << std::endl;
    else
    {
        MyINodeList myFiles, MySoftLinks, MyDirectories;
        getDirectoryItems(dirptr, path, pathPrefix, &myFiles, &MySoftLinks, &MyDirectories);
        ms->increaseTotalSoftLinks(MySoftLinks.length());
        while(!myFiles.isEmpty())
        {
            MyINode* mi;
            mi = myFiles.getTop();
            if(tree->exists(mi->getINodeNumber()))
                ms->increaseTotalHardLinks();
            else
                ms->increaseTotalFiles();
            tree->insertEntry(mi);
            myFiles.removeTopEntry();
        }
        while(!MyDirectories.isEmpty())
        {
            MyINode* mi;
            mi = MyDirectories.getTop();
            countItems(mi->getName(), pathPrefix, ms, tree);
            MyDirectories.removeTopEntry();
        }
    }
}

void removeDirectory(std::string path, std::string pathPrefix, MyStats* stats, bool verbose)
{   //remove an entire directory, after removing it's contents
    DIR* dirptr;
    std::string dirName = pathPrefix + path;
    if((dirptr = opendir(dirName.c_str())) == NULL)
        std::cout << "Cannot access the directory " << dirName << "." << std::endl;
    else
    {
        MyINodeList myFiles, mySoftLinks, myDirectories;
        getDirectoryItems(dirptr, path, pathPrefix, &myFiles, &mySoftLinks, &myDirectories);
        while(!mySoftLinks.isEmpty())
        {
            MyINode* mi;
            mi = mySoftLinks.getTop();
            std::string nam = pathPrefix + mi->getName();
            if(verbose)
                std::cout << "Removing softlink: " << nam.c_str() << std::endl;
            remove(nam.c_str());
            mySoftLinks.removeTopEntry();
            stats->increaseRemovedSoftLinks();
        }
        while(!myFiles.isEmpty())
        {
            MyINode* mi;
            mi = myFiles.getTop();
            stats->increaseRemovedFiles();
            std::string nam = pathPrefix + mi->getName();
            if(verbose)
                std::cout << "Removing file: " << nam.c_str() << std::endl;
            remove(nam.c_str());
            myFiles.removeTopEntry();
        }
        while(!myDirectories.isEmpty())
        {
            MyINode* mi;
            mi = myDirectories.getTop();
            removeDirectory(mi->getName().c_str(), pathPrefix, stats, verbose);//do it recursively for other directories
            myDirectories.removeTopEntry();
        }
    }
    stats->increaseRemovedDirectories();
    std::string nam = pathPrefix + path;
    if(verbose)
        std::cout << "Removing directory: " << nam.c_str() << std::endl;
    remove(nam.c_str());
}

bool isTargetUpdated(MyINode* source, MyINode* target)
{
    if(source->getFileSize() == target->getFileSize() && source->getLastModification() < target->getLastModification())
        return true;
    else
        return false;
}

void copySymLink(std::string path, std::string sourcePrefix, std::string targetPrefix, int pathSize, bool verbose)
{
    char buff[PATH_MAX +1];
    memset(buff, '\0', PATH_MAX+1);


    ssize_t r = 0;
    std::string linklocation = sourcePrefix + path;
    r = readlink(linklocation.c_str(), buff, pathSize);
    //read the link
    if(r < 0)
        std::cout << "ReadLink failed." << std::endl;
    else
    {
        std::string linkPath = buff;
        std::string targetPath = targetPrefix + path;   //set the target location

        if(linkPath.substr(0, sourcePrefix.length()) == sourcePrefix)
        {   //check if it's full or relative link
            std::string temp = linkPath;
            linkPath = targetPrefix;
            linkPath += temp.substr(sourcePrefix.length()); //if it is full, change the prefix
        }
        if(verbose)
                std::cout << "Creating softlink to " << targetPath << ", linking to " << linkPath << "." << std::endl;
        if((symlink(linkPath.c_str(), targetPath.c_str())) == -1)
        {
            std::cout << "Symlink failed." << std::endl;
        }
    }
}

std::string getPrefix(const char* pathName)
{
    char buff[PATH_MAX +1];
    memset(buff, '\0', PATH_MAX+1);
    std::string prefix;
    realpath(pathName, buff);

    prefix = buff;
    int i;
    for(i = prefix.length(); i > 0; i--)
    {
        if(prefix[i-1] == '/')
            break;
    }
    prefix = prefix.substr(0, i);
    return prefix;
}

std::string getDirectoryName(const char* pathName)
{
    std::string name = pathName;
    for(int i = name.length(); i > 0; i--)
    {
        if(name[i - 1] == '/')
            return name.substr(i);
    }
    return name;
}

void copyEntireDirectory(std::string path, std::string sourcePrefix, std::string targetPrefix, MyStats* stats, MyTree* tree, bool verbose)
{   //copy an entire directory, there's no need to check for updates or name conflicts
    DIR* dirptr;
    std::string sourceDirectory = sourcePrefix + path;
    if(verbose)
        std::cout << "Copying Directory " << sourceDirectory << " to " << targetPrefix << path << "." << std::endl;

    if((dirptr = opendir(sourceDirectory.c_str())) == NULL)
        std::cout << "Cannot access the directory " << sourceDirectory << "." << std::endl;
    else
    {
        MyINodeList myFiles, mySoftlinks, myDirectories;
        getDirectoryItems(dirptr, path, sourcePrefix, &myFiles, &mySoftlinks, &myDirectories);

        while(!myFiles.isEmpty())
        {
            MyINode* mi = myFiles.getTop();
            createNewFileOrHardLink(mi, sourcePrefix, targetPrefix, tree, stats, verbose);
            myFiles.removeTopEntry();
        }
        while(!mySoftlinks.isEmpty())
        {
            MyINode* mi;
            mi = mySoftlinks.getTop();
            copySymLink(mi->getName(), sourcePrefix, targetPrefix, mi->getFileSize(), verbose);
            stats->increaseChangedSoftLinks();
            mySoftlinks.removeTopEntry();
        }
        while(!myDirectories.isEmpty())
        {
            MyINode* mi;
            mi = myDirectories.getTop();
            std::string newDirectory = targetPrefix + mi->getName();
            if(verbose)
                std::cout << "Creating directory: " << newDirectory << "." << std::endl;
            mkdir(newDirectory.c_str(), mi->getMode());
            copyEntireDirectory(mi->getName(), sourcePrefix, targetPrefix, stats, tree, verbose);   //do it recursively
            stats->increaseChangedDirectories();
            myDirectories.removeTopEntry();
        }
    }
}


void copyDirectory(std::string path, std::string sourcePrefix, std::string targetPrefix, MyStats* stats, MyTree* tree, bool verbose)
{   //copy the directory
    struct stat s;
    DIR* sourceDirPtr;
    DIR* targetDirPtr;

    std::string sourceDirectory = sourcePrefix + path;  //get the full  path of source
    std::string targetDirectory = targetPrefix + path;  //get the full path of target
    if(verbose)
        std::cout << "Copying Directory " << sourceDirectory << " to " << targetDirectory << "." << std::endl;

    if((sourceDirPtr = opendir(sourceDirectory.c_str())) == NULL)   //open source
        std::cout << "Cannot access the directory " << sourceDirectory << "." << std::endl;
    else
    {
        if((targetDirPtr = opendir(targetDirectory.c_str())) == NULL)   //open target
            std::cout << "Cannot access the directory " << targetDirectory << "." << std::endl;
        else
        {
            MyINodeList mySourceFiles, mySourceSoftlinks, mySourceDirectories;
            MyINodeList myTargetFiles, myTargetSoftlinks, myTargetDirectories;
            //get source and target items
            getDirectoryItems(sourceDirPtr, path, sourcePrefix, &mySourceFiles, &mySourceSoftlinks, &mySourceDirectories);
            getDirectoryItems(targetDirPtr, path, targetPrefix, &myTargetFiles, &myTargetSoftlinks, &myTargetDirectories);

            while(!mySourceFiles.isEmpty())
            {   //while there are files
                MyINode* sourceNode = mySourceFiles.getTop();//get the top entry
                int position = myTargetFiles.getPosition(sourceNode->getName());    //check if there's a file entry with the same name
                if(position == -1)  //if not
                {
                    position = myTargetSoftlinks.getPosition(sourceNode->getName());    //check
                    if(position == -1)
                    {
                        position = myTargetDirectories.getPosition(sourceNode->getName());
                        if(position != -1)
                        {
                            MyINode* targetNode = myTargetDirectories.itemAt(position);
                            removeDirectory(targetNode->getName().c_str(), targetPrefix, stats, verbose);
                            stats->increaseRemovedDirectories();
                            targetNode->setUpToDate();
                        }
                    }
                    else
                    {
                        MyINode* targetNode = myTargetSoftlinks.itemAt(position);
                        std::string nam = targetPrefix + targetNode->getName();
                        if(verbose)
                            std::cout << "Removing soft link: " << nam << std::endl;
                        remove(nam.c_str());
                        stats->increaseRemovedSoftLinks();
                        targetNode->setUpToDate();
                    }
                    createNewFileOrHardLink(sourceNode, sourcePrefix, targetPrefix, tree, stats, verbose);
                }
                else
                {
                    MyINode* targetNode = myTargetFiles.itemAt(position);
                    if(!isTargetUpdated(sourceNode, targetNode))
                    {
                        createNewFileOrHardLink(sourceNode, sourcePrefix, targetPrefix, tree, stats, verbose);
                        targetNode->setUpToDate();
                    }
                    else
                    {
                        MyList* names = tree->getAllPaths(sourceNode->getINodeNumber());
                      //  if(names->length() > 1)
                        //{
                            int otherNode = tree->getOtherNode(targetNode->getName());
                            if(otherNode != targetNode->getINodeNumber())
                            {
                                int index = 0;
                                for(index = 0; index < names->length(); index++)
                                {
                                    if(targetNode->getName() != names->itemAt(index))
                                        break;
                                }

                                std::string hardLinkOldPath = targetPrefix + names->itemAt(index);
                                std::string hardLinkNewPath = targetPrefix + targetNode->getName();
                                if(verbose)
                                    std::cout << "Creating a new hardlink from " << hardLinkOldPath << " to " << hardLinkNewPath << "." << std::endl;
                                std::string nam = targetPrefix + targetNode->getName();
                                remove(nam.c_str());
                                link(hardLinkOldPath.c_str(), hardLinkNewPath.c_str());
                                stats->increaseChangedHardLinks();
                            }
                        //}
                        /*else
                        {

                            std::string newPath = tree->getPath(sourceNode->getINodeNumber());
                            if(newPath != targetNode->getName())
                            {
                                std::string hardLinkOldPath = targetPrefix + newPath;
                                std::string hardLinkNewPath = targetPrefix + targetNode->getName();
                                if(verbose)
                                    std::cout << "Creating a new hardlink from " << hardLinkOldPath << " to " << hardLinkNewPath << "." << std::endl;
                                std::cout << "PATH GETTING REMOVED: ************************* " << hardLinkNewPath << std::endl;
                                remove(hardLinkNewPath.c_str());
                                link(hardLinkOldPath.c_str(), hardLinkNewPath.c_str());
                                stats->increaseChangedHardLinks();
                            }
                        }*/
                        delete names;
                    }
                    targetNode->setUpToDate();
                }
                mySourceFiles.removeTopEntry();
            }
            removeExtraItems(&myTargetFiles, targetPrefix, stats, 1, verbose);
            while(!mySourceSoftlinks.isEmpty())
            {
                MyINode* sourceNode = mySourceSoftlinks.getTop();
                int position = myTargetSoftlinks.getPosition(sourceNode->getName());
                if(position == -1)
                {
                    position = myTargetDirectories.getPosition(sourceNode->getName());
                    if(position == -1)
                        copySymLink(sourceNode->getName(), sourcePrefix, targetPrefix, sourceNode->getFileSize(), verbose);
                    else
                    {
                        MyINode* targetNode = myTargetDirectories.itemAt(position);
                        removeDirectory(targetNode->getName().c_str(), targetPrefix, stats, verbose);
                        stats->increaseRemovedDirectories();
                        targetNode->setUpToDate();
                        copySymLink(sourceNode->getName(), sourcePrefix, targetPrefix, sourceNode->getFileSize(), verbose);
                    }
                    stats->increaseChangedSoftLinks();
                }
                else
                {
                    MyINode* targetNode = myTargetSoftlinks.itemAt(position);
                    if(sourceNode->getLastModification() >= targetNode->getLastModification())
                    {
                        std::string nam = targetPrefix + targetNode->getName();
                        if(verbose)
                            std::cout << "Removing softlink: " << nam << ". ";
                        remove(nam.c_str());
                        copySymLink(sourceNode->getName(), sourcePrefix, targetPrefix, sourceNode->getFileSize(), verbose);
                        stats->increaseChangedSoftLinks();
                    }
                    targetNode->setUpToDate();
                }
                mySourceSoftlinks.removeTopEntry();
            }
            removeExtraItems(&myTargetSoftlinks, targetPrefix, stats, 2, verbose);
            while(!mySourceDirectories.isEmpty())
            {
                MyINode* sourceNode = mySourceDirectories.getTop();
                int position = myTargetDirectories.getPosition(sourceNode->getName());
                if(position == -1)
                {
                    std::string newDirectory = targetPrefix + sourceNode->getName();
                    if(verbose)
                        std::cout << "Creating new directory: " << newDirectory << std::endl;
                    mkdir(newDirectory.c_str(), sourceNode->getMode());
                    copyEntireDirectory(sourceNode->getName(), sourcePrefix, targetPrefix, stats, tree, verbose);
                    stats->increaseChangedDirectories();
                }
                else
                {
                    MyINode* targetNode = myTargetDirectories.itemAt(position);
                    copyDirectory(sourceNode->getName(), sourcePrefix, targetPrefix, stats, tree, verbose);
                    stats->increaseChangedDirectories();
                    targetNode->setUpToDate();
                }
                mySourceDirectories.removeTopEntry();
            }
            removeExtraItems(&myTargetDirectories, targetPrefix, stats, 3, verbose);
        }
    }
}

void getDirectoryItems(DIR* dirptr, std::string path,std::string pathPrefix, MyINodeList* myFiles, MyINodeList* mySoftlinks, MyINodeList* myDirectories)
{   //get items of a given directory
    struct stat s;
    struct dirent* direntptr;
    while((direntptr = readdir(dirptr)) != NULL)
    {
        if(strcmp(direntptr->d_name, ".") != 0 && strcmp(direntptr->d_name, ".."))
        {
            std::string relativePath = path + "/" + direntptr->d_name;
            std::string nam = pathPrefix + relativePath;
             if(lstat(nam.c_str(), &s) == -1)
                std::cout << "lstat failed (" << nam <<")." << std::endl;
            else
            {   //put it to the proper list
                MyINode* mind = new MyINode(s, relativePath.c_str());
                if(S_ISREG(s.st_mode))
                    myFiles->insertEntry(mind);
                if(S_ISDIR(s.st_mode))
                    myDirectories->insertEntry(mind);
                if(S_ISLNK(s.st_mode))
                    mySoftlinks->insertEntry(mind);
            }
        }
    }
    if(closedir(dirptr) == -1)  //directory closes here, be careful not to close it twice
        std::cout << "Failed to close directory." << std::endl;
}

void removeExtraItems(MyINodeList* ml, std::string prefix, MyStats* stats, int mode, bool verbose)
{   //remove extra items found on target
    if(mode == 1 || mode == 2)
    {
        for(int i = 0; i < ml->length(); i++)
        {
            MyINode* temp;
            temp = ml->itemAt(i);
            if(!temp->isUpdated())
            {
                std::string path = prefix + temp->getName();
                if(verbose)
                    std::cout << "Removing: " << path << std::endl;
                remove(path.c_str());
                if(mode == 1)
                    stats->increaseRemovedFiles();
                if(mode == 2)
                    stats->increaseRemovedSoftLinks();
            }
        }
    }
    if(mode == 3)
    {
        for(int i = 0; i < ml->length(); i++)
        {
            MyINode* temp;
            temp = ml->itemAt(i);
            if(!temp->isUpdated())
            {
                removeDirectory(temp->getName(), prefix, stats, verbose);
            }
        }
    }
}

void createNewFileOrHardLink(MyINode* mi, std::string sourcePrefix, std::string targetPrefix, MyTree* tree, MyStats* stats, bool verbose)
{   //check if it's a new file or a hardlink
    MyList* names = tree->getAllPaths(mi->getINodeNumber());

    int otherNode;
    if(names->length() > 1)
    {
        otherNode = tree->getOtherNode(mi->getName());
        if(otherNode == -1)
        {
            tree->setOtherNode(mi);
            if(verbose)
                std::cout << "Copying file: " << targetPrefix << mi->getName() << "." << std::endl;
            int bytes = fileCopy(mi->getName(), sourcePrefix, targetPrefix, mi);
            if(bytes >= 0)
                stats->increaseBytes(bytes);
            stats->increaseChangedFiles();
        }
        else
        {
            struct stat s;
            std::string nam = targetPrefix + mi->getName();
            if(lstat(nam.c_str(), &s) == -1)
            {
                std::cout << "lstat failed (" << nam <<")." << std::endl;
            }
            MyINode mind(s, mi->getName().c_str());

            if(mind.getINodeNumber() == otherNode && !isTargetUpdated(mi, &mind))
            {
                if(verbose)
                    std::cout << "Copying file: " << targetPrefix << mi->getName() << "." << std::endl;
                int bytes = fileCopy(mi->getName(), sourcePrefix, targetPrefix, mi);
                if(bytes >= 0)
                    stats->increaseBytes(bytes);
                stats->increaseChangedFiles();
            }
            else
            {
                int index = 0;
                for(index = 0; index < names->length(); index++)
                {
                    if(mi->getName() != names->itemAt(index))
                        break;
                }
                std::string hardLinkOldPath = targetPrefix + names->itemAt(index);
                std::string hardLinkNewPath = targetPrefix + mi->getName();




                if(verbose)
                    std::cout << "Creating a new hardlink from " << hardLinkOldPath << " to " << hardLinkNewPath << "." << std::endl;
                //remove(hardLinkNewPath.c_str());
                link(hardLinkOldPath.c_str(), hardLinkNewPath.c_str());
                stats->increaseChangedHardLinks();
            }
        }
    }
    else
    {
        tree->setOtherNode(mi);
        if(verbose)
            std::cout << "Copying file: " << targetPrefix << mi->getName() << "." << std::endl;
        int bytes = fileCopy(mi->getName(), sourcePrefix, targetPrefix, mi);
        if(bytes >= 0)
            stats->increaseBytes(bytes);
        stats->increaseChangedFiles();
    }
    delete names;
}

void getOtherNodes(std::string path, std::string pathPrefix, MyTree* tree)
{   //get corresponding nodes for source inodes, if the folder already exists
    DIR* dirptr;
    std::string directoryName = pathPrefix + path;
    if((dirptr = opendir(directoryName.c_str())) == NULL)
        std::cout << "Cannot access the directory " << directoryName << "." << std::endl;
    else
    {
        MyINodeList myFiles, MySoftLinks, MyDirectories;
        getDirectoryItems(dirptr, path, pathPrefix, &myFiles, &MySoftLinks, &MyDirectories);
        while(!myFiles.isEmpty())
        {
            MyINode* mi;
            mi = myFiles.getTop();
            tree->setOtherNode(mi);
            myFiles.removeTopEntry();
        }
        while(!MyDirectories.isEmpty())
        {
            MyINode* mi;
            mi = MyDirectories.getTop();
            getOtherNodes(mi->getName(), pathPrefix, tree);
            MyDirectories.removeTopEntry();
        }
    }
}
