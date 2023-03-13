#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "route-map.h"


unsigned int hash(const char *key, unsigned int TABLE_SIZE) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    // do several rounds of multiplication
    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    // make sure value is 0 <= value < TABLE_SIZE
    value = value % TABLE_SIZE;

    return value;
}

RouteMap *route_map_create(unsigned int TABLE_SIZE) {
    // allocate table
    RouteMap *RouteMap = malloc(sizeof(RouteMap) * 1);

    // allocate table entries
    RouteMap->entries = malloc(sizeof(route_entry_t*) * TABLE_SIZE);

    // set each to null (needed for proper operation)
    int i = 0;
    for (; i < TABLE_SIZE; ++i) {
        RouteMap->entries[i] = NULL;
    }

    return RouteMap;
}

route_entry_t *route_map_pair(char *key, Route *value) {
    // allocate the entry
    route_entry_t *entry = malloc(sizeof(entry_t));
    entry->path = malloc(strlen(key) + 1);
    entry->route = value; //malloc(sizeof(&value) + 1);

    // copy the key and value in place
    strcpy(entry->path, key);

    // next starts out null but may be set later on
    entry->next = NULL;

    return entry;
}


void route_map_set(RouteMap *RouteMap, char *key, Route* route) {
    unsigned int slot = hash(key, sizeof(RouteMap->entries) / sizeof(route_entry_t*));

    // try to look up an entry set
    route_entry_t *entry = RouteMap->entries[slot];

    // no entry means slot empty, insert immediately
    if (entry == NULL) {
        RouteMap->entries[slot] = route_map_pair(key, route);
        return;
    }

    route_entry_t *prev;

    // walk through each entry until either the end is
    // reached or a matching key is found
    while (entry != NULL) {
        // check key
        if (strcmp(entry->path, key) == 0) {
            // match found, replace value
            free(entry->route);
            entry->route = route;
            return;
        }

        // walk to next
        prev = entry;
        entry = prev->next;
    }

    // end of chain reached without a match, add new
    prev->next = route_map_pair(key, route);
}


Route* route_map_get(RouteMap *routemap, char* path) {
    unsigned int slot = hash(path, sizeof(routemap->entries)/sizeof(route_entry_t*));

    // try to find a valid slot
    route_entry_t *entry = routemap->entries[slot];

    // no slot means no entry
    if (entry == NULL) {
        return NULL;
    }

    // walk through each entry in the slot, which could just be a single thing
    while (entry != NULL) {
        // return value if found
        if (strcmp(entry->path, path) == 0) {
            return entry->route;
        }

        // proceed to next key if available
        entry = entry->next;
    }

    // reaching here means there were >= 1 entries but no key match
    return NULL;
}

void route_map_del(RouteMap *RouteMap, char *path) {
    unsigned int bucket = hash(path, sizeof(RouteMap->entries)/sizeof(route_entry_t*));

    // try to find a valid bucket
    route_entry_t *entry = RouteMap->entries[bucket];

    // no bucket means no entry
    if (entry == NULL) {
        return;
    }

    route_entry_t *prev;
    int idx = 0;

    // walk through each entry until either the end is reached or a matching key is found
    while (entry != NULL) {
        // check key
        if (strcmp(entry->path, path) == 0) {
            // first item and no next entry
            if (entry->next == NULL && idx == 0) {
                RouteMap->entries[bucket] = NULL;
            }

            // first item with a next entry
            if (entry->next != NULL && idx == 0) {
                RouteMap->entries[bucket] = entry->next;
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
            free(entry->path);
            free(entry->route);
            free(entry);

            return;
        }

        // walk to next
        prev = entry;
        entry = prev->next;

        ++idx;
    }
}

void route_map_dump(RouteMap *RouteMap) {
    for (int i = 0; i < sizeof(RouteMap->entries)/sizeof(route_entry_t*); ++i) {
        route_entry_t *entry = RouteMap->entries[i];

        if (entry == NULL) {
            continue;
        }

        for(;;) {
            printf("%s -> %s\n", entry->path, entry->route->path);

            if (entry->next == NULL) {
                break;
            }

            entry = entry->next;
        }

        printf("\n");
    }
}

