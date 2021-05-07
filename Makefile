ALL=brute-force-size-3 brute-force-size-4 dynamic-programming-size-4

all: $(ALL)

brute-force-size-3: brute-force-size-3.cc
	g++ -O3 -std=c++11 -o $@ $< -lpthread

brute-force-size-4: brute-force-size-4.cc
	g++ -O3 -std=c++11 -o $@ $< -lpthread

dynamic-programming-size-4: dynamic-programming-size-4.cc
	g++ -O3 -std=c++11 -o $@ $< -lpthread

clean:
	rm -f $(ALL)

.PHONY: clean
