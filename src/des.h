#ifndef _DES_H_
#define _DES_H_

#define ENCRYPTION_MODE 1
#define DECRYPTION_MODE 0

typedef struct {
	uint64_t key64;
    uint64_t key56;
    uint32_t key_split[2];
	uint64_t subkey[16];
} key_set_t;

#endif
