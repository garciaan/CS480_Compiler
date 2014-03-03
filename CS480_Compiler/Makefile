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

stutest_pass.out: compiler
	cat stutest_pass.in
	-compiler $(RUNFLAGS) stutest_pass.in > stutest_pass.out
	cat stutest_pass.out
	
stutest_fail.out: compiler
	cat stutest_fail_1.in;
	-compiler $(RUNFLAGS) stutest_fail_1.in -v > stutest_fail_1.out;
	cat stutest_fail_1.out;

	cat stutest_fail_2.in;
	-compiler $(RUNFLAGS) stutest_fail_2.in -v > stutest_fail_2.out;
	cat stutest_fail_2.out;
	
	cat stutest_fail_3.in;
	-compiler $(RUNFLAGS) stutest_fail_3.in -v > stutest_fail_3.out;
	cat stutest_fail_3.out;
	
	cat stutest_fail_4.in;
	-compiler $(RUNFLAGS) stutest_fail_4.in -v > stutest_fail_4.out;
	cat stutest_fail_4.out;
	
	cat stutest_fail_5.in;
	-compiler $(RUNFLAGS) stutest_fail_5.in -v > stutest_fail_5.out;
	cat stutest_fail_5.out;
	
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