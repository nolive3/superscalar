CXXFLAGS := -g -Wall -std=c++0x -lm
CXX=g++
my_res_root=nick_res
res_root=ta_res
trace_root=trace
diff_name=errs
my_res=nick_full_res
F=2 4 8
M=2 4 8
J=1 2 3
K=1 2 3
L=1 2 3



all: procsim




traces:
	@make traceGCC
	@make traceGOBMK
	@make traceHMMER
	@make traceMCF
traceGCC: checkGCC $(my_res)
	@for f in $F;\
	do \
	for m in $M;\
	do \
	for j in $J;\
	do \
	for k in $K;\
	do \
	for l in $L;\
	do \
	    ./procsim -f$$f -m$$m -j$$j -k$$k -l$$l < $(trace_root)/gcc.100k.trace > $(my_res)/gcc.f$$f.m$$m.j$$j.k$$k.l$$l.out;\
	done; \
	done; \
	done; \
	done; \
	done
	@touch traceGCC
traceGOBMK: checkGOBMK $(my_res)
	@for f in $F;\
	do \
	for m in $M;\
	do \
	for j in $J;\
	do \
	for k in $K;\
	do \
	for l in $L;\
	do \
	    ./procsim -f$$f -m$$m -j$$j -k$$k -l$$l < $(trace_root)/gobmk.100k.trace > $(my_res)/gobmk.f$$f.m$$m.j$$j.k$$k.l$$l.out;\
	done; \
	done; \
	done; \
	done; \
	done
	@touch traceGOBMK
traceHMMER: checkHMMER $(my_res)
	@for f in $F;\
	do \
	for m in $M;\
	do \
	for j in $J;\
	do \
	for k in $K;\
	do \
	for l in $L;\
	do \
	    ./procsim -f$$f -m$$m -j$$j -k$$k -l$$l < $(trace_root)/hmmer.100k.trace > $(my_res)/hmmer.f$$f.m$$m.j$$j.k$$k.l$$l.out;\
	done; \
	done; \
	done; \
	done; \
	done
	@touch traceHMMER
traceMCF: checkMCF $(my_res)
	@for f in $F;\
	do \
	for m in $M;\
	do \
	for j in $J;\
	do \
	for k in $K;\
	do \
	for l in $L;\
	do \
	    ./procsim -f$$f -m$$m -j$$j -k$$k -l$$l < $(trace_root)/mcf.100k.trace > $(my_res)/mcf.f$$f.m$$m.j$$j.k$$k.l$$l.out;\
	done; \
	done; \
	done; \
	done; \
	done
	@touch traceMCF





check: checkGCC checkGOBMK checkHMMER checkMCF
	@echo "All the things worked :D"
checkGCC: procsim $(my_res_root)/$(diff_name) $(trace_root)/gcc.100k.trace $(res_root)/gcc.f2m2j3k1l1.out
	@echo "Checking $@"
	@./procsim -f2 -m2 -j3 -k1 -l1 < $(trace_root)/gcc.100k.trace > $(my_res_root)/gcc.out
	@diff $(my_res_root)/gcc.out $(res_root)/gcc.f2m2j3k1l1.out > $(my_res_root)/$(diff_name)/gcc
	@touch $@
checkGOBMK: procsim $(my_res_root)/$(diff_name) $(trace_root)/gobmk.100k.trace $(res_root)/gobmk.f4m2j2k1l1.out
	@echo "Checking $@"
	@./procsim -f4 -m2 -j2 -k1 -l1 < $(trace_root)/gobmk.100k.trace > $(my_res_root)/gobmk.out
	@diff $(my_res_root)/gobmk.out $(res_root)/gobmk.f4m2j2k1l1.out > $(my_res_root)/$(diff_name)/gobmk
	@touch $@
checkHMMER: procsim $(my_res_root)/$(diff_name) $(trace_root)/hmmer.100k.trace $(res_root)/hmmer.f4m4j3k3l3.out
	@echo "Checking $@"
	@./procsim -f4 -m4 -j3 -k3 -l3 < $(trace_root)/hmmer.100k.trace > $(my_res_root)/hmmer.out
	@diff $(my_res_root)/hmmer.out $(res_root)/hmmer.f4m4j3k3l3.out > $(my_res_root)/$(diff_name)/hmmer
	@touch $@
checkMCF: procsim $(my_res_root)/$(diff_name) $(trace_root)/mcf.100k.trace $(res_root)/mcf.f2m2j2k1l1.out
	@echo "Checking $@"
	@./procsim -f2 -m2 -j2 -k1 -l1 < $(trace_root)/mcf.100k.trace > $(my_res_root)/mcf.out
	@diff $(my_res_root)/mcf.out $(res_root)/mcf.f2m2j2k1l1.out > $(my_res_root)/$(diff_name)/mcf
	@touch $@




$(my_res_root):
	mkdir $(my_res_root)
$(my_res_root)/$(diff_name): $(my_res_root)
	mkdir $(my_res_root)/$(diff_name)
$(my_res): 
	mkdir $(my_res)

bp.o: bp.cpp bp.h fu.h
dispatch.o: dispatch.cpp dispatch.h fetch.h regfile.h schedule.h fu.h
execute.o: execute.cpp execute.h fu.h
fetch.o: fetch.cpp fetch.h cache.h bp.h procsim.hpp fetched.h fu.h
fetched.o: fetched.cpp fetched.h fu.h
procsim.o: procsim.cpp procsim.hpp cache.h fetch.h fetched.h dispatch.h\
 schedule.h execute.h stateupdate.h fu.h
procsim_driver.o: procsim_driver.cpp procsim.hpp cache.h bp.h fu.h
regfile.o: regfile.cpp regfile.h fu.h
result_bus.o: result_bus.cpp result_bus.h fu.h
schedule.o: schedule.cpp schedule.h result_bus.h execute.h fu.h
stateupdate.o: stateupdate.cpp stateupdate.h execute.h result_bus.h\
 fu.h procsim.hpp bp.h schedule.h dispatch.h fu.h



procsim: bp.o dispatch.o execute.o fetch.o fetched.o procsim.o procsim_driver.o\
 regfile.o result_bus.o schedule.o stateupdate.o
	$(CXX) -o procsim bp.o dispatch.o execute.o fetch.o fetched.o procsim.o\
 procsim_driver.o regfile.o result_bus.o schedule.o stateupdate.o
distclean: clean
	rm -rf $(my_res) 
	rm -f traceGCC traceGOBMK traceHMMER traceMCF
clean:
	rm -f procsim *.o checkGCC checkGOBMK checkHMMER checkMCF
	rm -rf $(my_res_root)
