#ifndef __MYSTATS__HPP
#define __MYSTATS__HPP
#include <iostream>

class MyStats
{
   int totalFiles;
   int changedFiles;
   int removedFiles;
   
   int totalDirectories;
   int changedDirectories;
   int removedDirectories;

   int totalSoftLinks;
   int changedSoftLinks;
   int removedSoftLinks;

   int totalHardLinks;
   int changedHardLinks;

   int bytes;

   public:
	MyStats();

	void increaseTotalFiles();
	void increaseTotalFiles(int a);
	void increaseChangedFiles();
	void increaseChangedFiles(int a);
	void increaseRemovedFiles();
	
	void increaseTotalDirectories();
	void increaseTotalDirectories(int a);
	void increaseChangedDirectories();
	void increaseChangedDirectories(int a);
	void increaseRemovedDirectories();
	
	void increaseTotalSoftLinks();
	void increaseTotalSoftLinks(int a);
	void increaseChangedSoftLinks();
	void increaseChangedSoftLinks(int a);
	void increaseRemovedSoftLinks();

	void increaseTotalHardLinks();
	void increaseTotalHardLinks(int a);
	void increaseChangedHardLinks();
	void increaseChangedHardLinks(int a);

	void increaseBytes(int a);

	/*int getTotalFiles();
	int getChangedFiles();
	int getRemovedFiles();

	int getTotalDirectories();
	int getChangedDirectories();
	int getRemovedDirectories();

	int getTotalSoftLinks();
	int getChangedSoftLinks();
	int getRemovedSoftLinks();
	
	int getTotalHardLinks();
	int getChangedHardLinks();
	int getRemovedHardLinks();*/	

	int getBytes();

	void printStats(double time);
};

#endif
