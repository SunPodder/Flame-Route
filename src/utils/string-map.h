#pragma once
typedef struct entry_t {
    char *key;
    char *value;
    struct entry_t *next;
} entry_t;

#pragma once
typedef struct {
    entry_t **entries;
} StringMap;


StringMap* str_map_create(unsigned int TABLE_SIZE);
char* str_map_get(StringMap* map, char* key);
char** str_map_get_keys(StringMap* map);
void str_map_set(StringMap* map, char* key, char* value);
void str_map_dump(StringMap* map);

