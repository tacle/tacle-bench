all: run_lift test_lift

SRCS=lift_io.c lift_control.c
TEST_SRCS=$(SRCS) test_lift.data.c

# executables
run_lift: run_lift.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^
test_lift: test_lift.c $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

# coverage test
covtest: test_lift.c $(TEST_SRCS) $(SRCS)
	$(CC) -fprofile-arcs -ftest-coverage -O0 -o test_lift.covtest $^
	./test_lift.covtest
	gcov lift_control.c

# single file (necessary for some analysis tools)
run_lift.onefile.c: lift_io.h lift_io.c lift_control.h lift_control.c run_lift.c
	echo "/* "$@", generated on `date` */" > $@
	echo '#include <stdio.h>' >> $@
	echo '#include <stdint.h>' >> $@
	(for f in $^; do grep -v '#include' $$f >> $@ ; done)

test_lift.onefile.c: lift_io.h lift_io.c lift_control.h lift_control.c test_lift.data.h test_lift.data.c  test_lift.c
	echo "/* "$@", generated on `date` */" > $@
	echo '#include <stdio.h>' >> $@
	echo '#include <stdint.h>' >> $@
	(for f in $^; do grep -v '#include' $$f >> $@ ; done)

# cleanup
clean:
	rm -f run_lift test_lift test_lift.covtest *.gcov *.gcda *.gcno
distclean: clean
	rm -f *~ run_lift.onefile.c test_lift.onefile.c
.PHONY: cov clean distclean

