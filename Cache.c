/*
 * Cache.c
 *
 *  Created on: Apr 19, 2018
 *      Author: student
 */

#include "Cache.h"

struct CacheLine {
	unsigned long tag; // 8 bytes?
	uint8_t is_valid;   // determines if it's been set
	//uint8_t *data;     // we actually probably don't need this
};

struct CacheSet {
	struct CacheLine *lines; // must malloc()
};

struct Cache {
	struct CacheSet *sets; // must malloc()
	unsigned int E; // associativity
	unsigned int b; // # of bits in offset
	unsigned int s; // # of bits for set (index)
};

/** Returns the pointer to the freshly initialized cache
 * @param E The associativity, determines how many lines per set
 * @param b The block offset
 * @param s The index bits, determines how many sets there are
 */
struct Cache* initCache(unsigned int E, unsigned int b, unsigned int s) {
	struct Cache* cache = malloc(sizeof(struct Cache));
	cache->E = E;
	cache->b = b;
	cache->s = s;
	unsigned int numSets = pw2(s);
	cache->sets = malloc(numSets * sizeof(struct CacheSet));
	for (int i = 0; i < numSets; i++) {
		// init each set
		cache->sets[i] = initSet(E);
	}
	return cache;
}

/**
 *
 */
struct CacheSet initSet(unsigned int E) {
	struct CacheSet set;
	unsigned int numLines = pw2(E);
	set.lines = malloc(numLines * sizeof(struct CacheLine));
	for (int i = 0; i < numLines; i++) {
		set.lines[i] = initLine();
	}
	return set;
}

/**
 *
 */
struct CacheLine initLine() {
	struct CacheLine line;
	line.is_valid = 0;
	// tag is E bits long
	line.tag = 0;
	return line;
}

/** Returns 2^p where p is the passed in value
 * @param p The number to get the 2^power of
 * @return 2^p
 */
unsigned int pw2(unsigned int p) {
	unsigned int pow = 1;
	for (; p > 0; p--) {
		pow *= 2;
	}
	return pow;
}
