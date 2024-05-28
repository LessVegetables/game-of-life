#ifndef LOOPDETECTION_H
#define LOOPDETECTION_H

#include "superFastHash.h"
#include "game.h"   // for cellArray

#define TABLE_SIZE 10007  // A prime number for the hash table size

typedef struct HashNode
{
    uint32_t hash;
    struct HashNode* next;
} HashNode;

HashNode* hashTable[TABLE_SIZE];

int containsHash(uint32_t hash);
void addHash(uint32_t hash);
void freeHashTable();
int loopDetected();

#endif
