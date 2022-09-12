// Task1.cpp - Cesar Quitazol

#include "TaskUtils.h"

int main(int argc, char** argv) {
    
    // Read input list
    std::vector<std::string> wordList = readWordList(argv[1]);

    // Execute Task1Filter (removing non-letter characters/ duplicates/ words with lenght < 3 and > 15)
    wordList = Task1Filter(wordList);

    // Write output 
    outputCleanFile(argv[2], wordList);
}

