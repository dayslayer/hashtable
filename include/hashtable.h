#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED


typedef struct hashtable_s hashtable;


/*
 * Collision resolution methods.
 */
#define HT_COLLISION_LINEAR


hashtable *ht_create (int size);
void ht_destroy (hashtable *ht, void (*free_data)(void*));

int ht_size (hashtable *ht);
int ht_count (hashtable *ht);
float ht_load_factor (hashtable *ht);

void *ht_find_key (hashtable *ht, char *key);
int ht_insert (hashtable *ht, char *key, void *value, int replace, void (*free_data)(void*));
void *ht_remove_key (hashtable *ht, char *key);

void ht_dbg_distribution (hashtable *ht, int group);
void ht_dbg_allocation (hashtable *ht);


#endif // HASHTABLE_H_INCLUDED
