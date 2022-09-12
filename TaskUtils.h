
#ifndef __UTILS_H__
#define __UTILS_H__


#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <deque>
#include <algorithm>


#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iterator>
#include <assert.h>
#include <pthread.h>
#include <fcntl.h>


// Return a filename of word list with int length length
std::string getListFilename(int length, std::string pre);

// Checks if char c is letter. If no, output is false. Used to remove punctuation when cleaning data and in allLetters.
bool isLetter(char c);

// Checks if input string contains any non-letter characters (![a-z],[A-Z]). If no, output is false.
bool allLetters(std::string input);

// Removes all punctuation from input string and returns it.
std::string cleanString(std::string inputStr);

// Returns a vector from file.
std::vector<std::string> readWordList(std::string dirtyFile);

// Removes duplicates from dirty file and writes a sorted vector into clean file
void outputCleanFile(std::string cleanFile, std::vector<std::string> lines);

//Task1Filter removes all duplicates, punctuation/ non-letter characters, words with length < 3 and > 15 and sorts.
std::vector<std::string> Task1Filter(std::vector<std::string> lines);

// Split a vector of words by length and return a map of length to a vector of words of that length.
std::map<int, std::vector<std::string>> splitByLength(std::vector<std::string> lines);


/* TASK 2 SPECIFIC UTILS/ FUNCTIONS */

// Separate word lists by length using fork()
std::vector<pid_t> Map2(std::vector<std::string> list);


//Reduce2 function. Merge from Map2
void Reduce2(std::string cleanFile2);


/* TASK 3 SPECIFIC UTILS/ FUNCTIONS*/

static std::vector<std::string> Global;

struct MapThreadArgs {
    int wordLength;
    int wordCount;
    int* indices;
};

void Map3(int count, pthread_t* threads, MapThreadArgs* threadArgs);
void Reduce3(std::string outPath, pthread_t* threads, MapThreadArgs* threadArgs);
void* Map3Thread(void* argPtr);

#endif /* __UTILS_H__ */