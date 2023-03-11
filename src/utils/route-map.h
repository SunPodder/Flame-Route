typedef struct entry_t {
    char *key;
    char *value;
    struct entry_t *next;
} entry_t;


typedef struct {
    entry_t **entries;
} HashTable;


HashTable* ht_create(unsigned int TABLE_SIZE);
char* ht_get(HashTable* HashTable, char* key);
void ht_set(HashTable* HashTable, char* key, char* value);
void ht_dump(HashTable* HashTable);

