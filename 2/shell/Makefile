OS=prompt.o background.o bg_pid_list.o call.o parse.o shell_main.c
EXEC=shell
%.o: %.c %.h
	gcc -c $<
$(EXEC): $(OS)
	gcc $^ -o $@
.PHONY:
clean:
	rm -f *.o
	rm $(EXEC)
