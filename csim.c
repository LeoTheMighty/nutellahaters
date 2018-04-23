/*  usernames: lbelyi and ncarvajalerker
 *  team name: lbelyi-ncarvajalerker
 *  REAL team name : The Nutella Haters
 */

#define _GNU_SOURCE

#include "cachelab.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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


void clear_cache(cache *cache, long int numSets, int numLines, long  int blockSize){
	int setIndex;
	for (setIndex = 0; setIndex < numSets; setIndex ++){
		cacheSet set = cache.sets[setIndex];
		if (set.lines != NULL){	
			free(set.lines);
		}
	} 
	if (Cache.sets != NULL) {
		free(cache.sets);
	}
	free(cache);
}

void printUsage() {
	printf("./csim-ref: Missing required command line argument\n");
	printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
	printf("Options:\n");
	printf("  -h         Print this help message.");
	printf("  -v         Optional verbose flag.\n");
	printf("  -s <num>   Number of set index bits.\n");
	printf("  -E <num>   Number of lines per set.\n");
	printf("  -b <num>   Number of block offset bits.\n");
	printf("  -t <file>  Trace file.\n\n");
	printf("Examples:\n");
	printf("  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
	printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace");
}


int main(int argc, char* argv[])
{
	// ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>
	if (argc != 10 || !strcmp(argv[1], "-h")) {
		printUsage();
		return EXIT_FAILURE;
	}

	int index_bits = atoi(argv[3]);
	int lines_set = atoi(argv[5]);
	int block_offset = atoi(argv[7]);
	char *filename = argv[9];

	struct Cache *cache = initCache(lines_set, index_bits, block_offset);

	// TODO Some of this should change (for academic honesty reasons)
	// File reading variables
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	size_t read;
	//int size;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("Could not open file...");
		return EXIT_FAILURE;
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		// read is the length
		// line is the string line
		char operation;
		int hexStartIndex = 2;
		if (line[0] == ' ') {
			// THIS SHOULD ALWAYS BE 'M' 'L' or 'S'
			operation = line[1];
			hexStartIndex++;
		}
		else {
			// THIS SHOULD ALWAYS BE 'I'
			operation = line[0];
		}

		int i = hexStartIndex;
		char hex[10];
		char c;
		//strtok(line, " ,");
		for (; (c = line[i]) != ','; i++) {
			hex[i - hexStartIndex] = c;
		}
		hex[9] = '\0';

		int hexNum = strtol(hex, NULL, 16);

		//from i+1 until the end, that string represents the size
		i++;
		int dataSize = atoi(&line[i]);
		// Ends at the endline anyways, so will be the string

		// done reading from the line, now process it
	}

	fclose(fp);
	if (line)
		free(line);
    printSummary(0, 0, 0);
    //clear_cache();

    return EXIT_SUCCESS;
}

