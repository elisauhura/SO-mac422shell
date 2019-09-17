OBJS=main.o

mac422shell: $(OBJS)
	$(CC) $(OBJS) -o mac422shell

install: mac422shell
	cp mac422shell /usr/local/bin


