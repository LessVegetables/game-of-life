#include "loopDetection.h"

HashNode* hashTable[TABLE_SIZE] = {0};

int containsHash(uint32_t hash)
{
    int index = hash % TABLE_SIZE;
    HashNode* node = hashTable[index];
    while (node != NULL)
    {
        if (node->hash == hash)
        {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void addHash(uint32_t hash)
{
    int index = hash % TABLE_SIZE;
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->hash = hash;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

void freeHashTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        HashNode* node = hashTable[i];
        while (node != NULL)
        {
            HashNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
}

int loopDetected()
{
    // BEGIN: loop detection
    uint32_t hash = SuperFastHash((const char*)cellArray, sizeof(cellArray));
    // Check if this state has been seen before
    if (containsHash(hash))
    {
        addHash(hash);
        return 1;
    }
    // Add the hash of the current state to the hash table

    // Image LoadImageFromTexture(Texture2D texture)

    addHash(hash);
    // END: loop detection
    return 0;
}