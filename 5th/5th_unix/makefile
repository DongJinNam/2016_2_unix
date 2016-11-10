OBJS = main.o read.o write.o

IO = io.h
ARGS = $(IO) $(*.c)

test : $(OBJS)
	$(CC) -o $(@) $(OBJS)

$(OBJS) : $(ARGS)

clean :
	rm $(OBJS)

