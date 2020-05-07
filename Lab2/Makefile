METHOD = DES
CFLAGS = -O2 -DDEBUG 

target: main.o des.o aes128.o sm4.o rc4.o bm.o sha2.o sha3.0
	g++ $(CFLAGS) -o target main.o des.o aes128.o sm4.o rc4.o bm.o sha2.o sha3.o
main.o: main.cpp des.h aes128.h sm4.h rc4.h bm.h sha2.h sha3.h
	g++ $(CFLAGS) -o main.o -c main.cpp
des.o: des.cpp des.h
	g++ $(CFLAGS) -o des.o -c des.cpp
aes128.o: aes128.cpp aes128.h
	g++ $(CFLAGS) -o aes128.o -c aes128.cpp
sm4.o: sm4.cpp sm4.h
	g++ $(CFLAGS) -o sm4.o -c sm4.cpp
rc4.o: rc4.cpp rc4.h
	g++ $(CFLAGS) -o rc4.o -c rc4.cpp
bm.o: bm.cpp bm.h
	g++ $(CFLAGS) -o bm.o -c bm.cpp
sha2.o: sha2.cpp sha2.h
	g++ $(CFLAGS) -o sha2.o -c sha2.cpp
sha3.o: sha3.cpp sha3.h
	g++ $(CFLAGS) -o sha3.o -c sha3.cpp
run:
	make
	./target $(METHOD)
	make clean
gen:
	python3 generate_plaintext.py
clean:
	rm *.o target

# target: main.o des.o
# 	g++ -o target main.o des.o
# main.o: main.cpp des.h
# 	g++ -o main.o -c main.cpp
# des.o: des.cpp des.h
# 	g++ -o des.o -c des.cpp
# run:
# 	make
# 	./target
# 	make clean
# gen:
# 	python3 generate_plaintext.py
# clean:
# 	rm *.o target