# makefile The Boys
# Carlos Maziero - DINF/UFPR, 2024/2

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -std=c99
LDLIBS  = -lm
MAIN    = theboys
ENTREGA = $(MAIN)

HDR = lista.h fprio.h eventos.h conjunto.h entidades.h auxiliares.h

# lista de arquivos-objeto (sem conjunto.o)
OBJ = lista.o fprio.o eventos.o entidades.o auxiliares.o

$(MAIN): $(MAIN).o $(OBJ) conjunto.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# construir o arquivo principal
$(MAIN).o: $(MAIN).c $(HDR)

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c $<

fprio.o: fprio.c fprio.h
	$(CC) $(CFLAGS) -c $<

entidades.o: entidades.c $(HDR)
	$(CC) $(CFLAGS) -c $<

eventos.o: eventos.c $(HDR)
	$(CC) $(CFLAGS) -c $<

auxiliares.o: auxiliares.c $(HDR)
	$(CC) $(CFLAGS) -c $<

run: $(MAIN)
	./$(MAIN)

valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)

# gerar arquivo TGZ para entregar
tgz: clean
	-mkdir -p /tmp/$(USER)/$(ENTREGA)
	chmod 0700 /tmp/$(USER)/$(ENTREGA)
	cp *.c *.h makefile conjunto.o /tmp/$(USER)/$(ENTREGA)
	tar czvf $(ENTREGA).tgz -C /tmp/$(USER) $(ENTREGA)
	rm -rf /tmp/$(USER)
	@echo "Arquivo $(ENTREGA).tgz criado para entrega"

# limpar arquivos temporários
clean:
	rm -f *~ $(OBJ) $(MAIN).o $(MAIN) $(ENTREGA).tgz

