bomb: bomb.c yellow.c red.c green.c blue.c bluegraph.c
	$(CC) -o bomb -O0 -Wall -lpthread \
		bomb.c yellow.c red.c green.c blue.c
	@rm -f bluegraph.c

bluegraph.c: gengraph
	./gengraph

gengraph: gengraph.c
	$(CC) -o gengraph -Wall gengraph.c

clean:
	rm -f bomb bluegraph.c gengraph

.PHONY: clean
