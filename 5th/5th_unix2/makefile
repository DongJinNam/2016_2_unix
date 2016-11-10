OBJS = main.o copyfile.o

copyfile: $(OBJS)
	$(CC) -o copyfile main.o copyfile.o

main.o : $(*.c)

copyfile.o : $(*.c)

clean:
	rm *.o

