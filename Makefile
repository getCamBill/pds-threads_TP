CC      = gcc
CFLAGS  = -Wall -Wextra
CFLAGS += -g
LDFLAGS = -pthread

TOUS := aleazard compteur-gc test

tout: ${TOUS}

aleazard: aleazard.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

compteur-gc: compteur-gc.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

rdv_q3: rdv_q3.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

rdv_q4: rdv_q4.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

test: test.sh
	# Le test a une durée moyenne de 6/7 minutes ...
	./test.sh

sem_test: yoda.sh
	# Le test a une durée moyenne de 6/10 secondes ...
	./yoda.sh

clean:
	rm -f core *.o

realclean: clean
	rm -f ${TOUS}

.PHONY: all clean realclean
