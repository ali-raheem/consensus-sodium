#include "consensus.h"

int consensus_init() {
  if(sodium_init() < 0)
    return CONSENSUS_INIT_ERROR;
  return CONSENSUS_INIT_OK;
}

int consensus_generate_secret(unsigned char secret[CONSENSUS_SECRET_LENGTH], unsigned char hash[CONSENSUS_HASH_LENGTH]){
    randombytes_buf(secret, CONSENSUS_SECRET_LENGTH);
    crypto_generichash(hash, CONSENSUS_HASH_LENGTH,
    		secret, CONSENSUS_SECRET_LENGTH,
		NULL, 0);
  return CONSENSUS_RNG_OK;
}

int consensus_check_hashes(size_t count, unsigned char hashes[count][CONSENSUS_HASH_LENGTH]) {
  int i, j;
  for(i = 0; i < count - 1; i++) {
    for(j = i + 1; j < count; j++) {
      if(sodium_memcmp(hashes[i],
		hashes[j],
		CONSENSUS_HASH_LENGTH) == 0)
	return CONSENSUS_CHECK_ERROR;
    }
  }
  return CONSENSUS_CHECK_OK;
}

int consensus_verify_secrets(size_t count,
			     unsigned char hashes[count][CONSENSUS_HASH_LENGTH],
			     unsigned char secrets[count][CONSENSUS_HASH_LENGTH])
{
  unsigned char hash[CONSENSUS_HASH_LENGTH];
  unsigned int i, j;
  for(i = 0; i < count; i++){
    crypto_generichash(hash, sizeof hash,
		       secrets[i], CONSENSUS_SECRET_LENGTH,
		       NULL, 0);

    int match = 0;
    for(j = 0; j < count; j++) {
      if (sodium_memcmp(hash,
		 hashes[j],
		 CONSENSUS_HASH_LENGTH) == 0) {
	match = 1;
	break;
      }
    }
    if (match == 0)
      return CONSENSUS_VERIFY_ERROR;
  }
  return CONSENSUS_VERIFY_OK;
}

int consensus_generate_shared(size_t count,
			      unsigned char secrets[count][CONSENSUS_SECRET_LENGTH],
			      unsigned char* buffer) {
  sodium_memzero(buffer, CONSENSUS_SECRET_LENGTH);
  int i, j;
  for(j = 0; j < count; j++) {
    for(i = 0; i < CONSENSUS_SECRET_LENGTH; i++) {
      buffer[i] = buffer[i] ^ secrets[j][i];
    }
  }
  return CONSENSUS_GENERATE_OK;
}
