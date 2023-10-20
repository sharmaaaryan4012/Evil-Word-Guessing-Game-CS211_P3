// Name: Aaryan Sharma
// CS 211 (Fall 2023) - Prof Scott Reckinger
// Project - 3 (A Dynamically Evil Word Guessing Game)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Pattern_struct {
    char* pat;  //string pattern - exs: "hello", "-ell-", "-----"
    int count;  //frequency of pattern in the current word list
    int changes;//number of occurences of new letter in this pattern
} Pattern;


void printSettings(int wordSize, int numGuesses, bool statOption, bool wordListOption,
                   bool letterListOption, bool patternListOption) {                                                     // This is a helper function, which prints all of the chosen preferences.

    printf("Game Settings:\n");
    printf("  Word Size = %d\n", wordSize);
    printf("  Number of Guesses = %d\n", numGuesses);
    statOption == 0 ? printf("  View Stats Mode = OFF\n") : printf("  View Stats Mode = ON\n");
    wordListOption == 0 ? printf("  View Word List Mode = OFF\n") : printf("  View Word List Mode = ON\n");
    letterListOption == 0 ? printf("  View Letter List Mode = OFF\n") : printf("  View Letter List Mode = ON\n");
    patternListOption == 0 ? printf("  View Pattern List Mode = OFF\n") : printf("  View Pattern List Mode = ON\n");
    fflush(stdin);
}


void printWordPattern(int wordSize, char* prevPattern) {                                                                // This is a helper function, which print the word pattern, based on the wordSize parameter.
    printf("The word pattern is: ");
    for (int i=0; i<wordSize; i++) {
        printf("-");
        strcat(prevPattern, "-");
    }
    printf("\n");

}


int strNumMods(char* str1, char* str2) {                                                                                // This function returns count of character differences between two strings(str1 and str2).
    int count = 0;
    while ((*str1) != '\0' && (*str2) != '\0') {                                                                        // While loop traverses the two strings(str1 and str2).
        if ((*str1) != (*str2)) {                                                                                       // Dereferences and compares each character present inside str1 and str2.
            count++;
        }
        str1++;                                                                                                         // Increments the pointer to the next character.
        str2++;
    }
    count += abs((int)(strlen(str1) - strlen(str2)));                                                                   // Add the difference of string lengths to "count".

    return count;
}


int strDiffInd(char* str1, char* str2) {                                                                                // This function  returns the index where the first difference occurs.
    int idx = 0;
    while ((*str1) != '\0' && (*str2) != '\0') {                                                                        // While loop traverses the two strings(str1 and str2).
        if ((*str1) != (*str2)) {                                                                                       // Dereferences and compares each character present inside str1 and str2.
            break;
        }
        idx++;
        str1++;                                                                                                         // Increments the pointer to the next character.
        str2++;
    }

    return idx;
}

void patternMerge(Pattern** pArrPtr, int* pCountPtr) {                                                                  // This is a helper function, which merges the duplicates present inside "pArr".
    int pCount = *pCountPtr;
    int merged;

    do {
        merged = 0;
        for (int i = 0; i < pCount - 1; i++) {
            for (int j = i + 1; j < pCount; j++) {

                if (strcmp((*pArrPtr)[i].pat, (*pArrPtr)[j].pat) == 0) {                                                // If the patterns are the same, we merge the duplicates and update the pCount.
                    merged = 1;

                    if ((*pArrPtr)[i].changes >= (*pArrPtr)[j].changes) {
                        (*pArrPtr)[i].count += (*pArrPtr)[j].count;                                                     // Adding up counts while merging.
                        free((*pArrPtr)[j].pat);                                                                        // Freeing pattern string.

                        for (int k = j; k < pCount - 1; k++) {
                            (*pArrPtr)[k] = (*pArrPtr)[k + 1];
                        }

                        pCount--;
                        j--;                                                                                            // Decrementing j according to the new "pArr"
                    }


                    else {
                        (*pArrPtr)[j].count += (*pArrPtr)[i].count;                                                     // Adding up counts while merging.
                        free((*pArrPtr)[i].pat);

                        for (int k = i; k < pCount - 1; k++) {
                            (*pArrPtr)[k] = (*pArrPtr)[k + 1];
                        }

                        pCount--;
                        (*pArrPtr) = realloc((*pArrPtr), sizeof(Pattern) * pCount);                            // Reallocating the array to match the new pattern count

                        i--;
                        break;
                    }
                }
            }
        }
    } while (merged);

    *pCountPtr = pCount;                                                                                                // Updating "pCount" according to the new size of "pArr".
}




bool tBreaker1(Pattern** pArrPtr, int* pCountPtr) {                                                                     // This is a helper function, which filters the patterns present in the "pArr", based on the maximum "count".
    int max = (*pArrPtr)[0].count;

    for (int i = 1; i < *pCountPtr; i++) {                                                                              // Finding the maximum "count" value.
        if ((*pArrPtr)[i].count > max) {
            max = (*pArrPtr)[i].count;
        }
    }


    int index = 0;
    for (int i = 0; i < *pCountPtr; i++) {                                                                              // Filter patterns with counts less than "max".
        if ((*pArrPtr)[i].count == max) {
            (*pArrPtr)[index] = (*pArrPtr)[i];
            index++;
        }
    }
    *pCountPtr = index;

    return (*pCountPtr == 1);
}


bool tBreaker2(Pattern** pArrPtr, int* pCountPtr) {                                                                     // This is a helper function, which filters the patterns present in the "pArr", based on the lowest "changes".
    int min = (*pArrPtr)[0].changes;

    for (int i = 0; i < *pCountPtr; i++) {                                                                              // Finding the minimum "changes" value.
        if ((*pArrPtr)[i].changes < min) {
            min = (*pArrPtr)[i].changes;
        }
    }

    int index = 0;                                                                                                      // Filter patterns with changes greater than minChanges
    for (int i = 0; i < *pCountPtr; i++) {
        if ((*pArrPtr)[i].changes == min) {
            (*pArrPtr)[index] = (*pArrPtr)[i];
            index++;
        }
    }
    *pCountPtr = index;

    return (*pCountPtr == 1);
}


void tBreaker3(Pattern** pArrPtr, int* pCountPtr, int wordSize, char choice) {                                          // This is a helper function, which filters the patterns present in the "pArr", based on the first occurrence of "choice".

    int occ = wordSize;
    int selected = -1;

    for (int i = 0; i < (*pCountPtr); i++) {                                                                            // Looping through each pattern to determine the first occurrence of "choice".
        for (int j = 0; j < wordSize; j++) {
            if (choice == (*pArrPtr)[i].pat[j]) {
                if (j < occ) {                                                                                          // If this occurrence is earlier than previous occurrences, update "occ" and "selected".
                    occ = j;
                    selected = i;
                }
                break;
            }
        }
    }


    if (selected != -1) {                                                                                               // If the pattern contains "choice", we move forward with the calculation.
        Pattern* newPtr = (Pattern*)malloc(sizeof(Pattern));

        if (newPtr) {
            *newPtr = (*pArrPtr)[selected];                                                                             // Allocating memory for a single "Pattern".

            free(*pArrPtr);                                                                                             // Free the old memory and point pArrPtr to the new memory
            *pArrPtr = newPtr;

            (*pCountPtr) = 1;                                                                                           // Updating the values of
        }
    }
}


void wordChange(char choice, int* wordCount, int wordSize, char*** wordList, Pattern** pArrPtr, int* pCountPtr) {       // This is a helper function, which modifies the "wordList" based on the new "pArr".
    bool idx[(*wordCount)];                                                                                             // Boolean array, which indicates whether a word is carried-forward during the restructuring.

    for (int i = 0; i < (*wordCount); i++) {
        bool match = true;

        for (int j = 0; j < wordSize; j++) {

            if(!(((*wordList)[i][j] == (*pArrPtr)[0].pat[j]) || ((*pArrPtr)[0].pat[j] == '-' && (*wordList)[i][j] != choice))) {
                match = false;
                break;
            }
        }

        idx[i] = match;
    }

    char** temp = (char**)malloc(sizeof(char*) * (*wordCount));                                                    // Allocating memory to "temp" array, which holds all of the words temporarily.

    int matchCount = 0;

    for (int i = 0; i < (*wordCount); i++) {
        if (idx[i]) {
            temp[matchCount++] = (*wordList)[i];                                                                        // In case the word matches, we add it to the "temp" array.
        }
    }

    *wordList = (char**)realloc(*wordList, sizeof(char*) * matchCount);                                                 // Resizing the "wordList", such that it fits only the selected words.

    for (int i = 0; i < matchCount; i++) {
        (*wordList)[i] = temp[i];
    }

    *wordCount = matchCount;                                                                                            // Updating the values of wordCount and freeing the "temp" array.

    free(temp);
}


char* generatePattern(char choice, int wordSize) {
    static char pattern[100];
    for (int i = 0; i < wordSize; i++) {
        pattern[i] = choice;
    }
    pattern[wordSize] = '\0';
    return pattern;
}


bool patternChange(char choice, int* wordCount, int wordSize, char*** wordList, Pattern**
pArrPtr, int* pCountPtr, bool patternListOption, bool* history, char prevPattern[wordSize]) {                           // This is a helper function, which modifies the pattern present inside "pArr".
    bool exist = 0;

    for (int i = 0; i < (*wordCount); i++) {
        for (int j = 0; j < wordSize; j++) {
            if (history[(*wordList)[i][j] - 97]) {
                (*pArrPtr)[i].pat[j] = (*wordList)[i][j];                                                               // Updating the patterns using their corresponding word from "wordList".
            }
        }
        (*pArrPtr)[i].changes = strNumMods((*pArrPtr)[i].pat, prevPattern);
        (*pArrPtr)[i].count++;
    }

    patternMerge(pArrPtr, pCountPtr);                                                                                   // Merging all duplicates present inside "pArr".

    if (patternListOption) {                                                                                            // If the pattern list option is turned on, the code prints all possible patterns.
        printf("All patterns for letter %c: \n", choice);
        for (int i=0; i< (*pCountPtr); i++) {
            printf("  %s \tcount = %d \tchanges = %d\n", (*pArrPtr)[i].pat, (*pArrPtr)[i].count, (*pArrPtr)[i].changes);
        }
    }

    if (*pCountPtr != 1) {                                                                                              // If there are multiple patterns present in the "pArr", we execute the tie-breaker functions.
        if (!tBreaker1(pArrPtr, pCountPtr)) {
            if (!tBreaker2(pArrPtr, pCountPtr)) {
                tBreaker3(pArrPtr, pCountPtr, wordSize, choice);
            }
        }
    }

    wordChange(choice, wordCount, wordSize, wordList,  pArrPtr, pCountPtr);                                             // Function call to modify the "wordList" based on the new "pArr".

    for (int i=0; i<wordSize; i++) {
        if (choice == (*pArrPtr)[0].pat[i]) {
            exist = 1;
        }
    }
    fflush(stdin);
    return exist;

}

void setPatternArray(int wordSize, int wordCount, int* pCountPtr, int* pCapacityPtr, Pattern** pArrPtr) {               // This is a helper function, which resets the "pArr" for each new iteration of the main while loop.
    (*pCountPtr) = wordCount;

    char patStr[wordSize+1];
    for (int i=0; i<wordSize; i++) {                                                                                    // Creating a pattern string(with '-' character).
        patStr[i] = '-';
    }
    patStr[wordSize] = '\0';                                                                                            // Placing '\0' at the end for termination.

    while ((*pCountPtr) > (*pCapacityPtr)) {                                                                            // While loop increases the size of pArr to accommodate all of the patterns.
        (*pCapacityPtr) *= 2;
        Pattern* pPtr = (Pattern*) malloc (sizeof(Pattern)*(*pCapacityPtr));
        free(*pArrPtr);
        (*pArrPtr) = pPtr;
    }


    for (int i = 0; i < wordCount; i++) {
        (*pArrPtr)[i].pat = (char*) malloc(sizeof(char) * (wordSize+1));
        (*pArrPtr)[i].count = 0;                                                                                        // Initializing "count" and "changes" at 0.
        (*pArrPtr)[i].changes = 0;
        strcpy((*pArrPtr)[i].pat, patStr);                                                                              // Copying over patStr to each index of pArr.
    }
}


void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {                                              // This function adds "newWord" to a dynamically allocated array of strings.
    if (((*numWords)+1) > *maxWords) {
        *maxWords *= 2;
        char** temp = malloc(sizeof(char*) * (*maxWords));                                                         // In case the "numWords" exceeds "maxWord", we double the size allocated to "words" array.
        for (int i=0; i<(*numWords); i++) {
            temp[i] = (*words)[i];
        }
        free(*words);
        *words = temp;
    }

    char* wPtr = malloc((strlen(newWord) + 1) * sizeof(char));                                                  // Allocating heap space for the "newWord."
    strcpy(wPtr, newWord);

    (*words)[*numWords] = wPtr;
    (*numWords)++;
}


void finalCheck(int wordCount, Pattern* pArr, char** wordList, bool* solved) {                                          // This is a helper function, which checks whether the pattern matches any of the words present in the "wordList".
    for (int i=0; i<wordCount; i++) {
        if (!strcmp(pArr[0].pat, wordList[i])) {
            *solved = 1;
            printf("You solved the word!\nThe word is: %s\nGame over.\n", wordList[i]);
        }
    }
}


int main(int argc, char* argv[]) {

    printf("Welcome to the (Evil) Word Guessing Game!\n\n");
    bool solved = false;
    fflush(stdin);

    int wordSize = 5;
    int numGuesses = 26;
    bool statOption = 0;
    bool wordListOption = 0;
    bool letterListOption = 0;
    bool patternListOption = 0;
    bool verboseOption = 0;

    for (int i = 1; i < argc; i++) {                                                                                    // Looping through the array "argv", in order to read different command-line prompts.

        if (strcmp(argv[i], "-v") == 0) {                                                                               // In case "-v" commnad is passed, verboseOption is updated to 1.
            verboseOption = 1;
        }

        else if (strcmp(argv[i], "-n") == 0 && (i + 1)<argc) {                                                          // On passing the "-n" command, the program searches the next index for a valid wordSize.
            if (atoi(argv[i + 1]) <= 1 || !(isdigit(argv[i+1][0]))) {
                printf("Invalid word size.\nTerminating program...");
                return 0;
            }

            wordSize = atoi(argv[i + 1]);
            i++;
        }

        else if (strcmp(argv[i], "-g") == 0 && (i + 1)<argc) {                                                          // On passing the "-g" command, the program searches the next index for a valid numGuesses.
            if (atoi(argv[i + 1]) <= 0) {
                printf("Invalid number of guesses.\nTerminating program...");
                return 0;
            }

            numGuesses = atoi(argv[i + 1]);
            i++;
        }

        else if (strcmp(argv[i], "-s") == 0) {                                                                          // In case "-s" commnad is passed, statOption is updated to 1.
            statOption = 1;
        }

        else if (strcmp(argv[i], "-w") == 0) {                                                                          // In case "-w" commnad is passed, wordListOption is updated to 1.
            wordListOption = 1;
        }

        else if (strcmp(argv[i], "-l") == 0) {                                                                          // In case "-l" commnad is passed, letterListOption is updated to 1.
            letterListOption = 1;
        }

        else if (strcmp(argv[i], "-p") == 0) {                                                                          // In case "-p" commnad is passed, patternListOption is updated to 1.
            patternListOption = 1;
        }

        else {                                                                                                          // In case the command line input is invalid, the program terminates.
            printf("Invalid command-line argument.\nTerminating program...");
            return 0;
        }

    }
    fflush(stdin);

    if (verboseOption == 1) {                                                                                           // If the verboseOption is true, all of the ListOptions are turn "ON".
        statOption = 1;
        wordListOption = 1;
        letterListOption = 1;
        patternListOption = 1;
    }

    printSettings(wordSize, numGuesses, statOption, wordListOption, letterListOption, patternListOption);               // Function call to print setting preferences.

    int capacity = 4;
    int totalCount = 0;
    int wordCount = 0;
    int max = 0;

    char** wordList = (char**)malloc(capacity*sizeof(char*));
    char longest[100];

    FILE* fPtr;
    fPtr = fopen("dictionary.txt", "r");                                                                 // Opening the "dictionary.txt" file.

    if (fPtr != NULL) {
        char line[100];
        while (fgets(line, 100, fPtr) != NULL) {                                                                        // Reading the file to 1.Adding words to "wordList", 2.Count # of words and 3.Find the longest word.
            line[strcspn(line, "\n")] = '\0';                                                                // Remove the '\n' character from the end of the string and replace it by '\0' for termination.
            if (strlen(line) > max) {                                                                                // If the string is longer than the previous max, we update "max" and "longest".
                max = strlen(line);
                strcpy(longest, line);
            }

            if (strlen(line) == wordSize) {                                                                          // If the size of the "line" matches "wordSize", we add "line" to the "wordList" dynamic array.
                addWord(&wordList, &wordCount, &capacity, line);
            }

            totalCount++;                                                                                               // Counts the total number of words in a file.
        }
    }
    fclose(fPtr);                                                                                                       // Closing the file "dictionary.txt".


    if (statOption) {
        printf("The dictionary contains %d words total.\n", totalCount);
        printf("The longest word %s has %d chars.\n", longest, max);
        printf("The dictionary contains %d words of length %d.\n", wordCount, wordSize);
        printf("Max size of the dynamic words array is %d.\n", capacity);
    }


    if (wordListOption) {
        printf("Words of length %d:\n", wordSize);

        if (wordCount == 0) {
            printf("Dictionary has no words of length %d.\nTerminating program...", wordSize);
            return 0;
        }

        for(int i = 0; i < wordCount; i++) {
            printf("  %s\n", wordList[i]);
        }
    }

    char prevPattern[wordSize];
    memset(prevPattern, '\0', wordSize);
    printWordPattern(wordSize, prevPattern);                                                                            // Function call to print the word pattern.
    printf("\n");
    fflush(stdin);

    char choice = '\0';
    char choiceStr[wordSize];

    bool history[26];                                                                                                   // Boolean array which keeps track of user inputs.
    for (int i=0; i<26; i++) {
        history[i] = 0;
    }

    bool displayGuesses = 1;
    bool askPrompt = 1;

    int pCapacity = 4;
    int pCount = wordCount;
    Pattern* pArr = (Pattern*) malloc(sizeof(Pattern)*(pCapacity));

    while(choice != '#' && !solved && numGuesses > 0) {                                                                 // The main while loop, which runs the game.
        if (displayGuesses) {
            printf("Number of guesses remaining: %d\n", numGuesses);
        }

        if (letterListOption) {                                                                                         // If the letter list option is turned on, we print out the
            printf("Previously guessed letters: ");
            for (int i = 0; i < 26; i++) {
                if (history[i]) {
                    printf("%c ", i + 97);
                }
            }
            printf("\n");
        }

        if (choice != '\n' && askPrompt) {
            printf("Guess a letter (# to end game): \n");
        }

        fflush(stdin);
        scanf("%s", choiceStr);                                                                                         // Getting user's input.
        choice = choiceStr[0];


        if (choice == '#' || choice == '\n') {
            displayGuesses = 0;
            continue;
        }

        if (choice == ' ') {
            displayGuesses = 0;
            askPrompt = 0;
            continue;
        }

        if (isupper(choice) || !isalpha(choice)) {                                                                // The user is prompted to re-enter their choice, in case the letter has already been entered or is upper case.
            printf("Invalid letter...\n");
            displayGuesses = 0;
            continue;
        }

        if (history[choice - 97] == 1) {
            printf("Letter previously guessed...\n");
            displayGuesses = 0;
            continue;
        }

        history[choice-97] = 1;                                                                                         // Changing the status of the letter to true.

        setPatternArray(wordSize, wordCount, &pCount, &pCapacity,&pArr);                   // Function call to set the "pArr" for the new iteration.

        bool exist = patternChange(choice, &wordCount, wordSize, &wordList, &pArr, &pCount, patternListOption,history, prevPattern);          // Function call to modify the patterns.

        if (exist) {
            printf("Good guess! The word has at least one %c.\n", choice);
            displayGuesses = 1;
        }

        else {
            printf("Oops, there are no %c's. You used a guess.\n", choice);
            displayGuesses = 1;
            numGuesses--;
        }

        if (statOption) {
            int numPossible = 0;
            for (int i = 0; i < wordCount; i++) {
                numPossible++;
            }
            printf("Number of possible words remaining: %d\n", numPossible);
        }

        if (wordListOption) {                                                                                           // Printing out the possible words for the next iteration.
            printf("Possible words are now:\n");
            for (int i = 0; i < wordCount; i++) {
                printf("  %s\n", wordList[i]);
            }
        }

        printf("The word pattern is: %s\n\n", pArr[0].pat);
        strncpy(prevPattern, pArr[0].pat, wordSize);

        finalCheck(wordCount, pArr, wordList, &solved);                                                                 // Checks if the user has guessed the right pattern.
        capacity = wordCount;
        askPrompt = 1;
    }

    if (choice == '#') {
        printf("Terminating game...\n");
        return 0;
    }

    if (numGuesses <= 0) {                                                                                              // Freeing each word present inside the "wordList".
        printf("You ran out of guesses and did not solve the word.\n");
        printf("The word is: %s\n", wordList[0]);
        printf("Game over.");
    }

    for (int i=0; i<capacity; i++) {
        free(wordList[i]);
    }

    free(wordList);

    return 0;
}