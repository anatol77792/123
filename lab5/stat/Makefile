
lab: libmylib.a
	gcc lab.c -L. -lmylib -o lab

libmylib.a: mylib.o
	ar rc libmylib.a mylib.o
	ranlib libmylib.a

mylib.o: mylib.c
	gcc -c mylib.c

clean:
	rm mylib.o lab libmylib.a
