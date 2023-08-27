#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "ansi.h"

int wire_blue = 1;

typedef struct
{
	void *left;
	uint32_t value;
	void *right;
} node;

#include "bluegraph.c"

char buffer[16];

void blue_preflight()
{
	printf(A(DB,CY) " " RST A(DF, CY) " ENTER CIRCUIT TRAVERSAL PATH: " RST);
	
	// Read in a few directions
	fgets(buffer, 16, stdin);
}

void blue()
{
	blue_preflight();
	
	// Initialize to the root node
	node *ptr = &graph[0];
	uint32_t xyzzy = ptr->value;
	
	// Read the sequence they input
	int i;
	for (i = 0; i < 15; i ++)
	{
		int z = 0;
		switch (buffer[i])
		{
		case 'L':
			ptr = ptr->left;
			break;
		case 'R':
			ptr = ptr->right;
			break;
		case '\n':
			z = 1;
			break;
		default:
			z = 1;
			printf("boom\n");
			break;
		}
		if (z) break;
		xyzzy ^= ptr->value;
	}
	
	printf(A(DB,CY) " " RST A(DF, CY) " PROGRAMMING GATE ARRAY... ");
	fflush(stdout);
	sleep(1);
	printf("SUCCEEDED" RST "\n");
	usleep(500000);
	
	// What is the value?
	if (xyzzy == solution)
	{
		printf(A(DB,CY) " " RST A(DF, CY) " VOLTAGE REROUTED FROM REMOTE DETONATION RECEIVER " RST "\n");
		-- wire_blue;
	}
	else
	{
		++ wire_blue;
	}
}