PROJ = output.out
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
OBJS = Calculate.o Driver.o

$(PROJ): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

Calculate.o: Calculate.cpp Calculate.h
	$(CXX) $(CXXFLAGS) -c $<

Driver.o: Driver.cpp
	$(CXX) $(CXXFLAGS) -c $<

# -f removes any error messages if the file is not present
.PHONY: clean run valgrind finish
clean:
	rm -rf $(OBJS)
	rm -f $(PROJ)
	rm -f *~ *.h.gch *#
run: $(PROJ)
	./$(PROJ) $(FILE)
valgrind: $(PROJ)
	valgrind ./$(PROJ)
finish:clean
	rm –rf -f proj3.zip
	zip proj3.zip ./*