CXXFLAGS := -g -Wall -std=c++0x -lm
CXX=g++

all: procsim

bp.o: bp.cpp bp.h
dispatch.o: dispatch.cpp dispatch.h fetch.h regfile.h schedule.h
execute.o: execute.cpp execute.h
fetch.o: fetch.cpp fetch.h cache.h bp.h procsim.hpp fetched.h
fetched.o: fetched.cpp fetched.h
procsim.o: procsim.cpp procsim.hpp cache.h fetch.h fetched.h dispatch.h\
 schedule.h execute.h stateupdate.h
procsim_driver.o: procsim_driver.cpp procsim.hpp cache.h bp.h
regfile.o: regfile.cpp regfile.h
result_bus.o: result_bus.cpp result_bus.h
schedule.o: schedule.cpp schedule.h result_bus.h execute.h
stateupdate.o: stateupdate.cpp stateupdate.h execute.h result_bus.h\
 fu.h procsim.hpp bp.h schedule.h dispatch.h



procsim: bp.o dispatch.o execute.o fetch.o fetched.o procsim.o procsim_driver.o\
 regfile.o result_bus.o schedule.o stateupdate.o
	$(CXX) -o procsim bp.o dispatch.o execute.o fetch.o fetched.o procsim.o\
 procsim_driver.o regfile.o result_bus.o schedule.o stateupdate.o
clean:
	rm -f procsim *.o
