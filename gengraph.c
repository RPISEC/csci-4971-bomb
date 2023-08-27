#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef struct
{
	int left;
	uint32_t value;
	int right;
} node;

int main()
{
	srand(time(NULL));
	
	FILE *outfd = fopen("bluegraph.c", "w");
	
	fprintf(outfd, "node graph[] = {\n");
	
	node graph[16];
	
	int i;
	for (i = 0; i < 16; i ++)
	{
		graph[i].left = rand() % 16;
		graph[i].right = rand() % 16;
		graph[i].value = rand();
		
		fprintf(outfd, "\t{ &graph[%d], 0x%08X, &graph[%d] },\n", graph[i].left, graph[i].value, graph[i].right);
	}
	
	fprintf(outfd, "};\n");
	
	int pathlen = (rand() % 5) + 5;
	uint32_t value = 0;
	node *root = &graph[0];
	
	fprintf(outfd, "#warning \"BLUE: ");
	int dir = -1;
	for (i = 0; i < pathlen; i ++)
	{
		value ^= root->value;
		
		if (dir == 0) fprintf(outfd, "R");
		else if (dir == 1) fprintf(outfd, "L");
		
		dir = rand() % 2;
		if (dir)
			root = &graph[root->left];
		else
			root = &graph[root->right];
	}
	fprintf(outfd, "\"\n");
	
	fprintf(outfd, "const uint32_t solution = 0x%08X;\n", value);
	
	
	return 0;
}
