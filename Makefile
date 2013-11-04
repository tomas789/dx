all : binary test doc

binary :
	$(MAKE) -C src

doc :
	$(MAKE) -C doc

test : 
	$(MAKE) -C test

.PHONY = binary doc test
