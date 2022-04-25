all: Wheeler_graph

SA_and_LCP: Wheeler_graph.cpp
	g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib Wheeler_graph.cpp -o WH -lsdsl -ldivsufsort

clean:
	rm WH.o Wheeler_graph