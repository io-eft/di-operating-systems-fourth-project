#include "myinode.hpp"
#include "myfunctions.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <string.h>
#include "mylist.hpp"
#include "mytree.hpp"
#include <sys/times.h>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 4 && argc != 5)
        cout << "Please run the program with the proper command.1" << endl;
    else
    {
        bool verbose = false;
        int i = 0;
        if(argc == 4)
            i = 1;
        else
        {
            verbose = true;
            i = 2;
            if(strcmp(argv[1], "-v") != 0)
            {
                cout << "Please run the program with the proper command.2" << endl;
                return 0;
            }
        }
        if(strcmp(argv[i], "-l") != 0)
        {
            cout << "Please run the program with the proper command.3" << endl;
            return 0;
        }
        else
        {

            double t1, t2;
            struct tms tb1, tb2;
            double ticspersec;

            ticspersec = (double) sysconf(_SC_CLK_TCK);
            t1 = (double) times(&tb1);

            MyTree tree;
            MyStats stats;
            string sourcePrefix = getPrefix(argv[i+1]);
            string targetPrefix = argv[i+2];

            if(targetPrefix[targetPrefix.length() - 1] != '/')
                targetPrefix += "/";
            string path = getDirectoryName(argv[i+1]);

            if(targetPrefix.find(sourcePrefix + path) != string::npos)
            {
                cout << "Target Directory is in source directory, exiting." << endl;
                return 0;
            }

            struct stat s;
            if(lstat(argv[i+1], &s) == -1)
            {
                cout << "lstat failed on given directory." << endl;
                return 0;
            }
            else
            {
                if(!S_ISDIR(s.st_mode))
                {
                    cout << "Input not a directory." << endl;
                    return 0;
                }
            }

            countItems(path, sourcePrefix, &stats, &tree);

            if(!prepareTarget(path, targetPrefix, s.st_mode, &stats, &tree, verbose))
            {
                cout << "Failed preparing the target." << endl;
                return 0;
            }
            copyDirectory(path, sourcePrefix, targetPrefix, &stats, &tree, verbose);

            t2 = (double) times(&tb2);
            stats.printStats(t2-t1);
        }
    }
}
