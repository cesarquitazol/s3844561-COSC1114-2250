//TaskUtils.h - Cesar Quitazol

#include "TaskUtils.h"

// Return a filename of word list with int length length

std::string getListFilename(int length, std::string pre) { 
    
    std::stringstream path("");

    if (pre != "") {
        path << "." << pre;
    }

    path << ".words" << length << ".txt";
    return path.str();
}


// Checks if char c is letter. If no, output is false. Used to remove punctuation when cleaning data and in allLetters.
bool isLetter(char c) {

    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')  // // Checks for Upper case then Checks for Lower case
    );
}

// Checks if input string contains any non-letter characters (![a-z],[A-Z]). If no, output is false.
bool allLetters(std::string word) {

    for (int i = 0; i < word.length(); i++) {
        if (!isLetter(word[i])) {
            return true;
        }
    }
    return false;
}

// Returns a vector from file.
std::vector<std::string> readWordList(std::string dirtyFile) {

    // Read input file
    std::ifstream dirtyStream(dirtyFile);


    // Create vector to store each line
    std::vector<std::string> lines;

    // Read line and store in vector
    std::string currentWord;
    std::getline(dirtyStream, currentWord);

    while (dirtyStream.good()) {
        lines.push_back(currentWord);
        std::getline(dirtyStream, currentWord);
    }

    return lines;
}

// Removes duplicates from dirty file and writes a sorted vector into clean file
void outputCleanFile(std::string cleanFile, std::vector<std::string> lines) {

    // Open output file
    std::ofstream cleanStream(cleanFile);

    // Write output file
    
    if (lines.size() > 0) {
        cleanStream << lines[0];
    }

    for (int i = 1; i < lines.size(); i++) {
        cleanStream << "\n" << lines[i];
    }

    cleanStream << "\n";
}

//Task1Filter removes all duplicates, punctuation/ non-letter characters, words with length < 3 and > 15 and sorts.
std::vector<std::string> Task1Filter(std::vector<std::string> lines) {
    std::deque<std::string> cleanLine;

    for (int i = 0; i < lines.size(); i++) {
        std::string currentWord = lines[i];

        //Do not include words of length < 3 or >15
        //Also remove all punctuation/ non-letter characters
        if (currentWord.length() < 3 || currentWord.length() > 15 ||
            allLetters(currentWord)) {
            continue;
        }

        cleanLine.push_back(currentWord);
    }

    // Sort
    std::sort(cleanLine.begin(), cleanLine.end());

    //Because we have sorted we can easily identify and remove duplicates, as they will be next to each other.
    std::vector<std::string> cleanNoDup;

    if (!cleanLine.empty()) {
        std::string prev = cleanLine[0];
        std::string next = cleanLine[0];

        // Add first
        cleanNoDup.push_back(next);
        cleanLine.pop_front();

        // Add next element, checking if it equals the prev element before doing so.
        while (!cleanLine.empty()) {
            prev = next;
            next = cleanLine[0];

            //If there are 2 elements that are the same, skip over the duplicate.
            if (prev != next) {
                cleanNoDup.push_back(next);
            }

            cleanLine.pop_front();
        }
    }

    return cleanNoDup;
}


/* TASK 2 SPECIFIC UTILS/ FUNCTIONS*/

// Separate word lists by length using fork() and sort from 3rd letter
std::vector<pid_t> Map2(std::vector<std::string> list) {

    // Open one file for each word length (one file for word length 3, another for 4, etc...)
    std::vector<std::ofstream> tempFiles;

    for (int i = 0; i < 13; i++) {
        tempFiles.push_back(std::ofstream(getListFilename(i + 3, "temp.task2")));
        assert(tempFiles[i].good());
    }
    
    // Start writing words of each length to the file it belongs to (words with length 3 go to the 3temp.task2, 4 to 4temp.task2, etc...)
    for (std::string word : list) {
        tempFiles[word.length() - 3] << word << "\n";
    }

    // Close tempFiles
    for (int i = 0; i < 13; i++) {
        tempFiles[i].close();
    }

    // Use fork() to begin sorting each file starting at the 3rd letter of each word, as per the assignment specs instructions.
    std::vector<pid_t> childPID;
    int sortLength = 3;
    for (; sortLength < 16; sortLength++) {
        pid_t pid = fork();

        if (pid == 0) {
            childPID.clear();
            break;
        } else {
            childPID.push_back(pid);
        }
    }

    if (childPID.size() == 0) {
   
        // Open corresponding tempFile
        auto xList = readWordList(getListFilename(sortLength, "tmp.task2"));

        // Create function for sorting from 3rd letter
        auto sortFromThirdLetter = [](const std::string &a, const std::string &b) {
            std::string aSub = a.substr(2, std::string::npos);
            std::string bSub = b.substr(2, std::string::npos);

            return aSub < bSub;
        };

    
        // Sort words from third letter
        std::sort(xList.begin(), xList.end(), sortFromThirdLetter);

        // Output results to file
        outputCleanFile(getListFilename(sortLength, "task2"), xList);

    }

    // Return child PID
    return childPID;
}

void Reduce2(std::string cleanFile) {

    // Open all lists created from Map2()
    std::vector<std::deque<std::string>> wordLists;

    for (int i = 3; i < 16; i++) {
    
        std::deque<std::string> currentList;
        std::vector<std::string> currentListVec = readWordList(getListFilename(i, "task2"));
        std::move(begin(currentListVec), end(currentListVec), back_inserter(currentList));

        wordLists.push_back(currentList);
    }

    auto compareFirstWords = [](const std::deque<std::string> &a, const std::deque<std::string> &b) {

        if (a.size() == 0) {
            return false;

        } else if (b.size() == 0) {
            return true;
        }

        std::string aSub = a[0].substr(2, std::string::npos);
        std::string bSub = b[0].substr(2, std::string::npos);

        return aSub < bSub;
    };

    // Open output file
    std::ofstream outputStream(cleanFile);
    assert(outputStream.good());

    while (true) {
        // Sort list by the first word
        std::sort(wordLists.begin(), wordLists.end(), compareFirstWords);

        if (wordLists[0].size() == 0) {
            for (auto dq : wordLists) {
                assert(dq.size() == 0);
            }

            break;
        }

        outputStream << wordLists[0][0] << "\n";
        wordLists[0].pop_front();
    }
}


/* TASK 3 SPECIFIC UTILS/ FUNCTIONS*/

void Map3(int count, pthread_t* threads, MapThreadArgs* threadArgs) {

    for (int i = 3; i < 16; i++) {
        MapThreadArgs* args = &threadArgs[i - 3];
        args->wordLength = i;
        args->wordCount = 0;

        // Count words with length i
        for (int j = 0; j < count; j++) {
            if (Global[j].length() == args->wordLength) {
                args->wordCount++;
            }
        }

        args->indices = new int[args->wordCount];
        int curIndexPos = 0;

        for (int j = 0; j < count; j++) {
            if (Global[j].length() == args->wordLength) {
                args->indices[curIndexPos] = j;
                curIndexPos++;
            }
        }
        assert(mkfifo(getListFilename(args->wordLength, "task3").c_str(),
                      S_IRWXU /* Read/write/execute */) == 0);

        pthread_create(&threads[i - 3], NULL, Map3Thread, args);
    }
}

int Map3ThreadCompare(const void* a, const void* b) {
    // Read indices from array
    int aIdx = *(int*)a;
    int bIdx = *(int*)b;

    std::string aSub = Global[aIdx].substr(2, std::string::npos);
    std::string bSub = Global[bIdx].substr(2, std::string::npos);

    if (aSub < bSub) {
        return -1;

    } else {
        return 1;
    }
}

void* Map3Thread(void* argPtr) {

    MapThreadArgs* args = (MapThreadArgs*)argPtr;
    qsort(args->indices, args->wordCount, sizeof(unsigned int), Map3ThreadCompare);


    int outFile = open(getListFilename(args->wordLength, "task3").c_str(), O_WRONLY /* Write */);

    for (unsigned int i = 0; i < args->wordCount; i++) {
        int nWritten = write(outFile, Global[args->indices[i]].c_str(), args->wordLength);

        if (nWritten != (int)args->wordLength) {
            assert(false);
        }
    }

    close(outFile);

    return 0;
}

std::string readNextWord(char* buf, int pipe, int wordLength) {
    int currentLength = 0;

    while (currentLength != wordLength) {
        ssize_t ret = read(pipe, buf + currentLength, wordLength - currentLength);

        if (ret < 0) {
            
            printf("Read error reading pipe for len=%i\n", wordLength);
        }

        currentLength += ret;
    }

    buf[currentLength] = '\x00';

    return buf;
}

struct Reduce3WordList {

    int index;

    int wordCount;

    char nextWord[16];
};

int Reduce3Compare(const void* a, const void* b) {

    Reduce3WordList* List1 = (Reduce3WordList*)a;
    Reduce3WordList* List2 = (Reduce3WordList*)b;

    if (List1->wordCount == 0) {
        return 1;

    } else if (List2->wordCount == 0) {
        return -1;
    }

    std::string Substr1 = std::string(List1->nextWord).substr(2, std::string::npos);
    std::string Substr2 = std::string(List2->nextWord).substr(2, std::string::npos);

    if (Substr1 < Substr2) {
        return -1;
    } else {
        return 1;
    }
}

void Reduce3(std::string cleanFile, pthread_t* threads, MapThreadArgs* threadArgs) {

    int pipes[13];

    for (int i = 0; i < 13; i++) {
        pipes[i] = open(getListFilename(i + 3, "task3").c_str(), O_RDONLY /* Read */);
    }


    std::ofstream outStream(cleanFile);
    assert(outStream.good());


    Reduce3WordList currentWord[13];

    for (int i = 0; i < 13; i++) {

        currentWord[i].wordCount = threadArgs[i].wordCount;
        currentWord[i].index = i;
        readNextWord(&currentWord[i].nextWord[0], pipes[i], i + 3);
    }

    qsort(currentWord, 13, sizeof(Reduce3WordList), Reduce3Compare);

    while (currentWord[0].wordCount > 0) {

        outStream << currentWord[0].nextWord;
        outStream << "\n";

        if (--currentWord[0].wordCount > 0) {
            readNextWord(&currentWord[0].nextWord[0], pipes[currentWord[0].index], currentWord[0].index + 3);
        }

        qsort(currentWord, 13, sizeof(Reduce3WordList), Reduce3Compare);
    }

    for (int i = 0; i < 13; i++) {
        void* ret;
        assert(pthread_join(threads[i], &ret) == 0);
        assert(ret == 0);
    }
    for (int i = 0; i < 13; i++) {
        delete[] threadArgs[i].indices;
    }

    for (int i = 0; i < 13; i++) {
        close(pipes[i]);
    }

}
