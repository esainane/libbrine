
#ifndef SHA1_H
#define SHA1_H

#include <glib.h>

#define SHA1_HASH_SIZE 20
typedef GChecksum *sha1_state_t;
void sha1_init(sha1_state_t *);
void sha1_append(sha1_state_t *, const guint8 *, unsigned int);
char *sha1_random_uuid(sha1_state_t *);
void random_bytes(unsigned char *buf, int count);

#endif /* SHA1_H */
