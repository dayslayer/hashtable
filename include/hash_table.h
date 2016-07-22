#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED


typedef struct hash_table_s hash_table_t;


hash_table_t *ht_create (int size);
void ht_destroy (hash_table_t *ht, void (*free_data)(void*));

int ht_size (hash_table_t *ht);
int ht_count (hash_table_t *ht);
float ht_load_factor (hash_table_t *ht);

void *ht_find_key (hash_table_t *ht, char *key);
int ht_insert (hash_table_t *ht, char *key, void *value, int replace, void (*free_data)(void*));
int ht_remove_key (hash_table_t *ht, char *key, void **value, void (*free_data)(void*));

void ht_dbg_distribution (hash_table_t *ht, int group);
void ht_dbg_allocation (hash_table_t *ht);


#endif // HASH_TABLE_H_INCLUDED
