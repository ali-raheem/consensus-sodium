# Consensus

Small library for multiple parties to estbalish a shared random number. Even if you are the only honest party.

**This version is not compatible with [consensus](https://github.com/ali-raheem/consensus) which uses libtomcrypt.**

## Algorithim

1. All parties pick a secret random number
2. Each party shares a hash of that random number
3. Each party reveals the random number
4. Each party ensures the hashes matches the revealed number
5. All the random numbers are bitwise XOR'd to produce a random number

## Dependencies

- [libsodium](https://github.com/jedisct1/libsodium)


## Build

```
gcc -o bin/consensus_test src/test.c src/consensus.c -static -ltomcrypt
```

## Usage

See `src/test.c`, each party only generates one secret, disseminating the hash, and then the secret is left up to the user.