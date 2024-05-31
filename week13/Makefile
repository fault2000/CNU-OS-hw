
my_scheduler : main.o scheduler.o
	gcc -g -o $@ $^

main.o : main.c
	gcc -g -c $^

scheduler.o : scheduler.c
	gcc -g -c $^

clean :
	rm main.o scheduler.o my_scheduler
