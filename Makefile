#Alan Benitez
main: main.o HttpServer.o
		g++ -Wall -o $@ $^
main.o: main.cpp
		g++ -Wall -c main.cpp
HttpServer.o: HttpServer.hpp HttpServer.cpp
		g++ -Wall -c HttpServer.cpp
clean:
	rm -rf *.dSYM
	$(RM) *.o *.gc* test/*.o core main