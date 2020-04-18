target: main.o des.o
	g++ -o target main.o des.o
main.o: main.cpp des.h
	g++ -o main.o -c main.cpp
des.o: des.cpp des.h
	g++ -o des.o -c des.cpp
run:
	make
	./target
	make clean
clean:
	rm *.o target