/*  usernames: lbelyi and ncarvajalerker
 *  team name: lbelyi-ncarvajalerker
 *  REAL team name : The Nutella Haters
 */

#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>

void clear_cache(){
	if(){
		
	}
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
	if (argc != 10 || argv[1] == "-h") {
		printUsage();
		return EXIT_FAILURE;
	}

	int index_bits = atoi(argv[3]);
	int lines_set = atoi(argv[5]);
	int block_offset = atoi(argv[7]);
	char *filename = argv[9];

	// File reading variables
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(filename, 'r');
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
		strtok(line, " ,");
		for (; line[i] != ','; i++) {

		}
	}

	fclose(fp);
	if (line)
		free(line);
    printSummary(0, 0, 0);

    return EXIT_SUCCESS;
}

