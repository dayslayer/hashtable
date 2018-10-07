#include "hashtable.h"

#include <stdlib.h>
#include <string.h>


struct ht_entry_s
{
    char *key;
    void *value;
    struct ht_entry_s *next;
};

struct hashtable_s
{
    struct ht_entry_s **buckets;

    /* number of buckets (first-level entries) */
    int n_buckets;
    /* actual number of elements stored in the table */
    int n_entries;
};


static unsigned ht_hash (char *key, int mod);


hashtable *ht_create (int size)
{
    int i;
    hashtable *ht = malloc(sizeof(hashtable));

    if (ht == NULL)
        return NULL;

    ht->buckets = malloc(size * sizeof(struct ht_entry_s*));

    if (ht->buckets == NULL) {
        free(ht);
        return NULL;
    }

    for (i = 0; i < size; i++)
        ht->buckets[i] = NULL;

    ht->n_buckets = size;
    ht->n_entries = 0;

    return ht;
}

void ht_destroy (hashtable *ht, void (*free_data)(void*))
{
    int i;
    struct ht_entry_s *p;

    for (i = 0; i < ht->n_buckets; i++) {
        while (ht->buckets[i] != NULL) {
            p = ht->buckets[i];
            ht->buckets[i] = p->next;
            if (free_data != NULL)
                free_data(p->value);
            free(p->key);
            free(p);
        }
    }

    free(ht->buckets);
    free(ht);
}

int ht_size (hashtable *ht)
{
    return ht->n_buckets;
}

int ht_count (hashtable *ht)
{
    return ht->n_entries;
}

float ht_load_factor (hashtable *ht)
{
    return (float)ht->n_entries / ht->n_buckets;
}

void *ht_find_key (hashtable *ht, char *key)
{
    struct ht_entry_s *entry;
    unsigned hash = ht_hash(key, ht->n_buckets);

    for (entry = ht->buckets[hash]; entry != NULL; entry = entry->next)
        if (strcmp(key, entry->key) == 0)
            return entry->value;

    return NULL;
}

int ht_insert (hashtable *ht, char *key, void *value, int replace, void (*free_data)(void*))
{
    struct ht_entry_s *entry;

    unsigned hash = ht_hash(key, ht->n_buckets);
    int found = 0;

    /* check if key already exists */
    for (entry = ht->buckets[hash]; entry != NULL; entry = entry->next) {
        if (strcmp(key, entry->key) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {

        entry = malloc(sizeof(struct ht_entry_s));

        if (entry == NULL)
            return 0;

        entry->key = strdup(key);

        if (entry->key == NULL) {
            free(entry);
            return 0;
        }

        entry->value = value;

        /* insert at head */
        entry->next = ht->buckets[hash];
        ht->buckets[hash] = entry;

        ht->n_entries++;
    }
    else {
        /* found */
        if (!replace)
            return 0;
        /* replace */
        if (free_data != NULL)
            free_data(entry->value);
        entry->value = value;
    }

    return 1;
}

void *ht_remove_key (hashtable *ht, char *key)
{
    struct ht_entry_s *entry, *prev;
    void *value;

    unsigned hash = ht_hash(key, ht->n_buckets);
    int found = 0;

    for (entry = ht->buckets[hash]; entry != NULL; entry = entry->next) {
        if (strcmp(key, entry->key) == 0) {
            found = 1;
            break;
        }
        prev = entry;
    }

    if (!found)
        return NULL;

    value = entry->value;

    if (entry == ht->buckets[hash])
        ht->buckets[hash] = entry->next;
    else
        prev->next = entry->next;

    ht->n_entries--;

    free(entry->key);
    free(entry);

    return value;
}

static unsigned ht_hash (char *key, int mod)
{
    /* http://stackoverflow.com/questions/7666509/hash-function-for-string */
    unsigned state = 5381;

    while (*key != 0) {
        state = (state << 5) - state + *key;
        key++;
    }

    return state % mod;
}

#include <stdio.h>

void ht_dbg_distribution (hashtable *ht, int group)
{
    int i, t;
    struct ht_entry_s *p;

    printf("\n");

    for (i = 0; i < ht->n_buckets; ) {

        t = ((i+group) < ht->n_buckets) ? (i+group) : ht->n_buckets;

        printf("buckets %4d - %4d: ", i, t);

        for ( ; i < t; i++)
            for (p = ht->buckets[i]; p != NULL; p = p->next)
                printf("*");

        printf("\n");
    }

    printf("\n");
}

void ht_dbg_allocation (hashtable *ht)
{
    int i, j, count;
    struct ht_entry_s *p;

    printf("\n");

    for (i = 0; i < ht->n_buckets; i++) {
        count = 0;
        for (p = ht->buckets[i]; p != NULL; p = p->next)
            count++;
        if (count > 0) {
            printf("bucket %d: %d (", i, count);
            for (p = ht->buckets[i], j=0; p != NULL; p = p->next, j++) {
                printf("%s", p->key);
                if (j < count-1)
                    printf(", ");
            }
            printf(")\n");
        }
    }

    printf("\n");
}
