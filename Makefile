MPI_CC=pgc++ 
MPI_FLAGS= -std=c++11  -acc -Minfo=acc

SOURCES= laplace.cpp
OBJECTS= laplace.o
LIBS=

TARGET=all
EX_NAME=laplace_acc

%.o:%.cpp
	$(MPI_CC) $(MPI_FLAGS) -c $^

$(TARGET): $(OBJECTS)
	$(MPI_CC) $(MPI_FLAGS) -o $(EX_NAME) $^ $(LIBS)

clean:
	rm -f *.o $(EX_NAME) *~
