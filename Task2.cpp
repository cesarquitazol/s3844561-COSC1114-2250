// Task2.cpp - Cesar Quitazol

#include "TaskUtils.h"

int main(int argc, char **argv) {

    // Read input list
    std::vector<std::string> wordList = readWordList(argv[1]);

    // Execute Task1Filter (removing non-letter characters/ duplicates/ words with lenght < 3 and > 15)
    wordList = Task1Filter(wordList);

    // Separate words by length using Map2
    std::vector<pid_t> childPID = Map2(wordList);

    if (childPID.size() > 0) {

        for (pid_t child : childPID) {
            waitpid(child, NULL, 0);
        }
        // Merge using Reduce2
        Reduce2(argv[2]);
    } 
    return 0;
}
