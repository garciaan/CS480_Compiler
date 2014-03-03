#--------------------------------------------
# INSTRUCTION
# Quoted strings are to be filled in by student
#
CCC = g++
CCFLAGS = -std=c++0x
SOURCES = Main.cpp Token.cpp Symbol_Table.cpp Parser.cpp Lex.cpp
OBJS = $(SOURCES:.cpp=.o)
RUNFLAGS = 
PROFTEST = proftest.in

$(OBJS): $(SOURCES)
	$(CCC) $(CCFLAGS) -c $(SOURCES)

compiler: $(OBJS)
	$(CCC) $(CCFLAGS) -o compiler $(OBJS)

clean:
	rm -f $(OBJS) compiler *.out
	ls

pass_test.out: compiler
	cat Pass_Test.txt; 
	-compiler $(RUNFLAGS) Pass_Test.txt -v > Pass_Test.out;
	cat Pass_Test.out;
	
fail_test.out: compiler
	cat Fail_Test.txt;
	-compiler $(RUNFLAGS) Fail_Test.txt -v > Fail_Test.out;
	cat Fail_Test.out;

		
# Notice the next line. The `-' says to ignore the return code. This
# is a way to have multiple tests of errors that cause non-zero return
# codes.
# cat stutest2.in
# -compiler stutest2.in > stutest2.out
# cat stutest2.out

proftest.out: compiler
	cat $(PROFTEST)
	compiler $(PROFTEST) > proftest.out
	cat proftest.out