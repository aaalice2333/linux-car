.PHONY : clean

program:./src/main.o ./src/syn.o ./src/car.o ./src/tunnel.o 
	g++ ./src/main.o ./src/syn.o ./src/car.o ./src/tunnel.o  -o program	

syn.o:./src/syn.cpp
	g++ -c ./src/syn.cpp	
	
tunnel.o:./src/tunnel.cpp
	g++ -c ./src/tunnel.cpp	
	
car.o:./src/car.cpp
	g++ -c ./src/car.cpp	
	
main.o:./src/main.cpp
	g++ -c ./src/main.cpp	

clean :
	rm -rf *.o	
