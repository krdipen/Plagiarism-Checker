all:
	@gcc -c ./src/plagChecker.c
	@mv plagChecker.o ./obj
	@gcc -o plagChecker ./obj/plagChecker.o
	@mv plagChecker ./bin

run:
	@./bin/plagChecker ./testcase/test1.txt ./corpus

clean:
	@rm ./bin/plagChecker
	@rm ./obj/plagChecker.o
