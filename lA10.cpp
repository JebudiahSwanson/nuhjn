#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Initialize a trie node
struct TrieNode* createNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (newNode) {
        newNode->isEndOfWord = 0;
        newNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Inserts a word into the trie
void insert(struct TrieNode* root, char* word) {
    struct TrieNode* temp = root;
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        int index = word[i] - 'a';
        if (!temp->children[index]) {
            temp->children[index] = createNode();
        }
        temp = temp->children[index];
    }
    temp->isEndOfWord = 1;
    temp->count++;
}

// Returns the number of occurrences of a word in the trie
int numberOfOccurrences(struct TrieNode* root, char* word) {
    struct TrieNode* temp = root;
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        int index = word[i] - 'a';
        if (!temp->children[index]) {
            return 0;
        }
        temp = temp->children[index];
    }
    return (temp != NULL && temp->isEndOfWord) ? temp->count : 0;
}

// Deallocate memory allocated for the trie
void deallocateTrie(struct TrieNode* root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            deallocateTrie(root->children[i]);
        }
    }
    free(root);
}

// Reads words from a file and returns the number of words
int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File not found!\n");
        exit(1);
    }
    int numWords = 0;
    char word[256];
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = (char*)malloc(strlen(word) + 1);
        strcpy(pInWords[numWords], word);
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void) {
    char* inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }
    struct TrieNode* root = createNode();
    for (int i = 0; i < numWords; i++) {
        insert(root, inWords[i]);
    }
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }
    deallocateTrie(root);
    return 0;
}
