all: make_object_for_ahrs make_object_for_fuzzy link_for_output

make_object_for_ahrs:
	g++ -O2 -std=c++11 -c ahrs.cpp -I./include/

make_object_for_fuzzy:
	g++ -c myfuzzy.cpp -I./include/

link_for_output:
	g++ -O2 myfuzzy.o ahrs.o -L./lib -lm -lpthread -lfuzzylite-static
	rm ahrs.o myfuzzy.o
	

clean :
	rm  a.out

