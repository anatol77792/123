
lab: libmylib.so
	gcc lab.c -L. -lmylib -o lab

libmylib.so: mylib.o
	gcc -shared mylib.o -o libmylib.so

mylib.o: mylib.c
	gcc -c -fPIC mylib.c

clean:
	rm mylib.o lab libmylib.so
