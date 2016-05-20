#include "mystats.hpp"

MyStats::MyStats(): totalFiles(0), changedFiles(0), removedFiles(0), totalDirectories(0),
changedDirectories(0), removedDirectories(0), totalSoftLinks(0), changedSoftLinks(0), removedSoftLinks(0),
totalHardLinks(0), changedHardLinks(0), bytes(0) {}

void MyStats::increaseTotalFiles(){totalFiles++;}

void MyStats::increaseTotalFiles(int a){totalFiles+=a;}

void MyStats::increaseChangedFiles(){changedFiles++;}

void MyStats::increaseChangedFiles(int a){changedFiles+=a;}

void MyStats::increaseRemovedFiles(){removedFiles++;}

void MyStats::increaseTotalDirectories(){totalDirectories++;}

void MyStats::increaseTotalDirectories(int a){totalDirectories+=a;}

void MyStats::increaseChangedDirectories(){changedDirectories++;}

void MyStats::increaseChangedDirectories(int a){changedDirectories+=a;}

void MyStats::increaseRemovedDirectories(){removedDirectories++;}

void MyStats::increaseTotalSoftLinks(){totalSoftLinks++;}

void MyStats::increaseTotalSoftLinks(int a){totalSoftLinks+=a;}

void MyStats::increaseChangedSoftLinks(){changedSoftLinks++;}

void MyStats::increaseChangedSoftLinks(int a){changedSoftLinks+=a;}

void MyStats::increaseRemovedSoftLinks(){removedSoftLinks++;}

void MyStats::increaseTotalHardLinks(){totalHardLinks++;}

void MyStats::increaseTotalHardLinks(int a){totalHardLinks+=a;}

void MyStats::increaseChangedHardLinks(){changedHardLinks++;}

void MyStats::increaseChangedHardLinks(int a){changedHardLinks+=a;}

void MyStats::increaseBytes(int a){bytes+=a;}
/*
int MyStats::getTotalFiles(){return totalFiles;}

int MyStats::getChangedFiles(){return changedFiles;}

int MyStats::getRemovedFiles(){return removedFiles;}

int MyStats::getTotalDirectories(){return totalDirectories;}

int MyStats::getChangedDirectories(){return changedDirectories;}

int MyStats::getRemovedDirectories(){return removedDirectories;}

int MyStats::getTotalSoftLinks(){return totalSoftLinks;}

int MyStats::getChangedSoftLinks(){return changedSoftLinks;}

int MyStats::getRemovedSoftLinks(){return removedSoftLinks;}

int MyStats::getTotalHardLinks(){return totalHardLinks;}

int MyStats::getChangedHardLinks(){return changedHardLinks;}

int MyStats::getRemovedHardLinks(){return removedHardLinks;}
*/
int MyStats::getBytes(){return bytes;}

void MyStats::printStats(double time)
{
    double rate = (double)bytes / time;
    std::cout << std::endl;
    std::cout << "Copied " << changedFiles << " files out of " << totalFiles << " files in total." << std::endl;
    std::cout << "Copied " << changedHardLinks << " hard links out of " << totalHardLinks << " hard links in total." << std::endl;
    std::cout << "Changed " <<changedSoftLinks << " softlinks out of " << totalSoftLinks << " softlinks in total." << std::endl;
    std::cout << "There are " << changedDirectories << " directories with altered content out of " << totalDirectories << " directories in total." << std::endl;
    std::cout << "Removed " << removedFiles << " files, " << removedSoftLinks << " softlinks and " << removedDirectories << " directories." << std::endl;
    std::cout << "Copied " << bytes << " bytes in " << time << " seconds, with a rate of " << rate << " bytes/sec" << std::endl;
    std::cout << std::endl;
}
