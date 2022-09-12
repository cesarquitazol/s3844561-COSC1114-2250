//Task3.cpp - Cesar Quitazol

#include "TaskUtils.h"

int main(int argc, char** argv) {

    // Read input list
    std::vector<std::string> wordList = readWordList(argv[1]);

    // Execute Task1Filter (removing non-letter characters/ duplicates/ words with lenght < 3 and > 15)
    wordList = Task1Filter(wordList);

    // Initialise global array of words
    Global = wordList;  // new std::string[lines.size()];

    // Create threads for each word count
    pthread_t threads[13];
    MapThreadArgs threadArgs[13];
    Map3(wordList.size(), threads, threadArgs);

    // Perform reduce/merge step
    Reduce3(argv[2], threads, threadArgs);
}