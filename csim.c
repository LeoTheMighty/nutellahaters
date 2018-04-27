/*  usernames: lbelyi and ncarvajalerker
 *  team name: lbelyi-ncarvajalerker
 *  REAL team name : The Nutella Haters
 */

#define _GNU_SOURCE

#include "cachelab.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>

#define dbflag 0
#define uint8_t unsigned char

//need to fix the order
struct CacheLine;
struct CacheSet;
struct Cache;
struct Cache* initCache(unsigned int E, unsigned int b, unsigned int s);
struct CacheSet initSet(unsigned int E);
struct CacheLine initLine();
unsigned int pw2(unsigned int p);
char add_to_cache(struct Cache *cache, unsigned int tag, unsigned int index);
void free_cache(struct Cache *cache);
void printUsage();

unsigned int hits = 0;
unsigned int misses = 0;
unsigned int evictions = 0;
unsigned int time = 0;

struct CacheLine {
	unsigned long tag; // 8 bytes?
	uint8_t is_valid;   // determines if it's been set
	unsigned int timeAdded;
	//uint8_t *data;     // we actually probably don't need this
};

struct CacheSet {
	struct CacheLine *lines; // must malloc()
	unsigned int numLines;
};

struct Cache {
	struct CacheSet *sets; // must malloc()
	unsigned int numSets;
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
	cache->numSets = numSets;
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
	set.lines = malloc(E * sizeof(struct CacheLine));
	set.numLines = E;
	for (int i = 0; i < E; i++) {
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
	return (1 << p);
}

/** Returns the char of the result from the adding
 *
 */
char add_to_cache(struct Cache *cache, unsigned int tag, unsigned int index) {
	// Use the index to get where ya gotta go, then use the tag to verify
	struct CacheSet *set = cache->sets + index;

	for (int i = 0; i < set->numLines; i++) {
		struct CacheLine *line = set->lines + i;
		// Check each tag in the set
		if (line->tag == tag && line->is_valid) {
			// Then it's a hit
			line->timeAdded = time++;
			return 'h';
		}
	}

	int firstEmptyIndex = -1;
	for (int i = 0; i < set->numLines; i++) {
		struct CacheLine *line = set->lines + i;
		if (!line->is_valid) {
			firstEmptyIndex = i;
			break;
		}
	}

	// If it reaches here, then it's a miss
	if (firstEmptyIndex != -1) {
		// Then no eviction necessary, just a miss
		struct CacheLine *emptyLine = set->lines + firstEmptyIndex;
		emptyLine->tag = tag;
		emptyLine->is_valid = 1;
		emptyLine->timeAdded = time++;
		return 'm';
	}

	// Now it's an eviction, decide whom'st'd've to evict

	// Then ya gotta deal with the gucci eviction stuff
	// LRU (least recently used)
	unsigned int lowestTime = INT_MAX;
	struct CacheLine *lowestLine = NULL;
	for (int i = 0; i < set->numLines; i++) {
		struct CacheLine *line = set->lines + i;
		unsigned timeAdded = line->timeAdded;
		if (timeAdded < lowestTime) {
			lowestTime = timeAdded;
			lowestLine = line;
		}
	}

	// Then you evict the lowestLine
	lowestLine->tag = tag;
	lowestLine->timeAdded = time++;
	return 'e';
}

void free_cache(struct Cache *cache){
	int setIndex;
	for (setIndex = 0; setIndex < cache->numSets; setIndex ++){
		struct CacheSet set = cache->sets[setIndex];
		if (set.lines != NULL){
			free(set.lines);
		}
	}
	// If you wanted to be safe in your freeing, you should do this check before
	if (cache->sets != NULL) {
		free(cache->sets);
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
	printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}


int main(int argc, char* argv[])
{
	// ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>
	struct Cache *cache;
	char *filename = NULL;
	uint8_t vflag = 0;
	unsigned int s = 0, b = 0, E = 0;

	if (argc == 1) {
		printUsage();
		return EXIT_FAILURE;
	}

	char option;
	while ((option = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
		// option is the char value, optarg is the char* value after
		switch (option) {
			case 'h':
				// Actually wants the help message
				printf("help\n");
				printUsage();
				return EXIT_FAILURE;
			case 'v':
				// verbose flag
				vflag = 1;
				break;
			case 's':
				// # of set index bits
				s = atoi(optarg);
				if (s <= 0) {
					printf("%i not a good value for s\n", s);
					return EXIT_FAILURE;
				}
				break;
			case 'E':
			// # of lines per set
				E = atoi(optarg);
				if (E <= 0) {
					printf("%i not a good value for E\n", E);
					return EXIT_FAILURE;
				}
				break;
			case 'b':
			// # of block bits
				b = atoi(optarg);
				if (b <= 0) {
					printf("%i not a good value for b\n", b);
					return EXIT_FAILURE;
				}
				break;
			case 't':
				// name of file
				filename = optarg;
				break;
			default:
				printf("unrecognized character %c\n", option);
				printUsage();
				return EXIT_FAILURE;
		}
	}

	// Check the arguments
	if (E == 0 || b == 0 || s == 0 || filename == NULL) {
		printf("not initialized variables\n");
		printUsage();
		return EXIT_FAILURE;
	}

	// Initialize the cache
	cache = initCache(E, b, s);

	// File reading variables
	FILE *file;
	char *line = NULL;
	size_t len = 0;
	size_t read;
	//int size;

	file = fopen(filename, "r");
	if (file == NULL) {
		perror("Could not open file...");
		return EXIT_FAILURE;
	}

	while ((read = getline(&line, &len, file)) != -1) {
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
		char *hex = calloc(sizeof(char), 10);
		char c;
		//strtok(line, " ,");
		for (; (c = line[i]) != ','; i++) {
			hex[i - hexStartIndex] = c;
		}
		hex[i] = '\0';

		unsigned long hexNum = strtol(hex, NULL, 16);

		free(hex);

		unsigned tag = hexNum >> (s + b);
		unsigned index = ((hexNum << (64 - (s + b))) >> (64 - s)); // TODO check this :p

		//unsigned JTAG = hexNum >> (s + b); //get the tag portion of the address
    //unsigned JINDEX = (hexNum^(JTAG<<(b+s)))>>b; //get the set index of the address

		//from i+1 until the end, that string represents the size
		i++;
		int dataSize = atoi(&line[i]);
		// Ends at the endline anyways, so will be the string

		// done reading from the line, now process it
		// Process the hex

		if (operation != 'I') {
			char result = add_to_cache(cache, tag, index);

			if (dbflag) {
				printf("(index: %i, tag: %i) ", index, tag);
			}
			if (vflag) {
				printf("%c %lx,%i ",operation, hexNum, dataSize);
			}

			switch (result) {
				case 'h':
					printf("hit");
					hits++;
					break;
				case 'm':
					printf("miss");
					misses++;
					break;
				case 'e':
					printf("miss eviction");
					misses++;
					evictions++;
					break;
			}
			if (operation == 'M') {
				hits++;
				printf(" hit");
			}
			printf("\n");
		}
	}

	fclose(file);
	if (line)
		free(line);
  printSummary(hits, misses, evictions);
  hits = 0;
  misses = 0;
  evictions = 0;
  free_cache(cache);

  return EXIT_SUCCESS;
}
