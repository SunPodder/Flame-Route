#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "string-map.h"


extern unsigned int hash(const char *str, unsigned int len);

StringMap *str_map_create(unsigned int TABLE_SIZE) {
    // allocate table
    StringMap *StringMap = malloc(sizeof(StringMap) * 1);

    // allocate table entries
    StringMap->entries = malloc(sizeof(entry_t*) * TABLE_SIZE);

    // set each to null (needed for proper operation)
    int i = 0;
    for (; i < TABLE_SIZE; ++i) {
        StringMap->entries[i] = NULL;
    }

    return StringMap;
}

entry_t *str_map_pair(const char *key, const char *value) {
    // allocate the entry
    entry_t *entry = malloc(sizeof(entry_t) * 1);
    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(value) + 1);

    // copy the key and value in place
    strcpy(entry->key, key);
    strcpy(entry->value, value);

    // next starts out null but may be set later on
    entry->next = NULL;

    return entry;
}


void str_map_set(StringMap *StringMap, char *key, char *value) {
    unsigned int slot = hash(key, sizeof(StringMap->entries) / sizeof(entry_t*));

    // try to look up an entry set
    entry_t *entry = StringMap->entries[slot];

    // no entry means slot empty, insert immediately
    if (entry == NULL) {
        StringMap->entries[slot] = str_map_pair(key, value);
        return;
    }

    entry_t *prev;

    // walk through each entry until either the end is
    // reached or a matching key is found
    while (entry != NULL) {
        // check key
        if (strcmp(entry->key, key) == 0) {
            // match found, replace value
            free(entry->value);
            entry->value = malloc(strlen(value) + 1);
            strcpy(entry->value, value);
            return;
        }

        // walk to next
        prev = entry;
        entry = prev->next;
    }

    // end of chain reached without a match, add new
    prev->next = str_map_pair(key, value);
}


char* str_map_get(StringMap *StringMap, char *key) {
    unsigned int slot = hash(key, sizeof(StringMap->entries)/sizeof(entry_t*));

    // try to find a valid slot
    entry_t *entry = StringMap->entries[slot];

    // no slot means no entry
    if (entry == NULL) {
        return NULL;
    }

    // walk through each entry in the slot, which could just be a single thing
    while (entry != NULL) {
        // return value if found
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }

        // proceed to next key if available
        entry = entry->next;
    }

    // reaching here means there were >= 1 entries but no key match
    return NULL;
}

void str_map_del(StringMap *StringMap, const char *key) {
    unsigned int bucket = hash(key, sizeof(StringMap->entries)/sizeof(entry_t*));

    // try to find a valid bucket
    entry_t *entry = StringMap->entries[bucket];

    // no bucket means no entry
    if (entry == NULL) {
        return;
    }

    entry_t *prev;
    int idx = 0;

    // walk through each entry until either the end is reached or a matching key is found
    while (entry != NULL) {
        // check key
        if (strcmp(entry->key, key) == 0) {
            // first item and no next entry
            if (entry->next == NULL && idx == 0) {
                StringMap->entries[bucket] = NULL;
            }

            // first item with a next entry
            if (entry->next != NULL && idx == 0) {
                StringMap->entries[bucket] = entry->next;
            }

            // last item
            if (entry->next == NULL && idx != 0) {
                prev->next = NULL;
            }

            // middle item
            if (entry->next != NULL && idx != 0) {
                prev->next = entry->next;
            }

            // free the deleted entry
            free(entry->key);
            free(entry->value);
            free(entry);

            return;
        }

        // walk to next
        prev = entry;
        entry = prev->next;

        ++idx;
    }
}

void str_map_dump(StringMap *StringMap) {
    for (int i = 0; i < sizeof(StringMap->entries)/sizeof(entry_t*); ++i) {
        entry_t *entry = StringMap->entries[i];

        if (entry == NULL) {
            continue;
        }

        for(;;) {
            printf("%s -> %s\n", entry->key, entry->value);

            if (entry->next == NULL) {
                break;
            }

            entry = entry->next;
        }

        printf("\n");
    }
}

