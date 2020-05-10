.PHONY: clean

#Definition de variable
#Nom de la commande du compilateur
COMPILATEUR = gcc
#Nom de l'executable
PJNAME = automate
#Variable pour ranger les fichiers objets etc..
SRCDIR = ./src/
OBJDIR = ./obj/
BINDIR = ./bin/

$(BINDIR)$(PJNAME): $(OBJDIR)main.o 
	$(COMPILATEUR) -o $(BINDIR)automate $(OBJDIR)argument.o $(OBJDIR)automaton.o $(OBJDIR)parcours.o $(OBJDIR)determinisation.o $(OBJDIR)overide.o $(OBJDIR)main.o 

$(OBJDIR)automaton.o: $(SRCDIR)automaton.c $(SRCDIR)automaton.h $(SRCDIR)type.h
	$(COMPILATEUR) -c -o $(OBJDIR)automaton.o $(SRCDIR)automaton.c

$(OBJDIR)parcours.o: $(SRCDIR)parcours.c $(SRCDIR)parcours.h $(OBJDIR)automaton.o
	$(COMPILATEUR) -c -o $(OBJDIR)parcours.o $(SRCDIR)parcours.c

$(OBJDIR)argument.o: $(SRCDIR)argument.c $(SRCDIR)argument.h $(OBJDIR)parcours.o
	$(COMPILATEUR) -c -o $(OBJDIR)argument.o $(SRCDIR)argument.c

$(OBJDIR)determinisation.o: $(SRCDIR)automaton.c $(SRCDIR)automaton.h $(SRCDIR)determinisation.h $(SRCDIR)determinisation.c
	$(COMPILATEUR) -c -o $(OBJDIR)determinisation.o $(SRCDIR)determinisation.c

$(OBJDIR)overide.o: $(SRCDIR)overide.c $(SRCDIR)overide.h $(SRCDIR)automaton.h $(SRCDIR)type.h
	$(COMPILATEUR) -c -o $(OBJDIR)overide.o $(SRCDIR)overide.c

$(OBJDIR)main.o: $(OBJDIR)argument.o $(OBJDIR)automaton.o $(OBJDIR)parcours.o $(OBJDIR)determinisation.o $(OBJDIR)overide.o $(SRCDIR)main.c
	$(COMPILATEUR) -c -o $(OBJDIR)main.o $(SRCDIR)main.c

clean:
	rm -f $(OBJDIR)*.o
