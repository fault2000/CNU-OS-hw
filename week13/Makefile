
my_scheduler : main.o scheduler.o
	gcc -o $@ $^

main.o : main.c
	gcc -c $^

scheduler.o : scheduler.c
	gcc -c $^

clean :
	rm main.o scheduler.o my_scheduler
