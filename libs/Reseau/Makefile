#
# Makefile pour la bibliotheque graphique
#

SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
CIBLE = libreseau.a

#
# Nom de la cible principale
#

all: $(CIBLE)

#
# Cible de nettoyage
#

clean:
	rm -f core *.o *.a $(CIBLE)

#
# Dependances pour la bibliotheque
#

$(CIBLE): $(OBJETS)
	$(AR) rs $@ $?



format:
	clang-format -i -style="WebKit" libreseau.c libreseau.h
