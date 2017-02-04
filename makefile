all:
	$(MAKE) -C build all
install:
	$(MAKE) -C build install
backend: 
	$(MAKE) -C build backend
test: 
	$(MAKE) -C build test
