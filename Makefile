.PHONY: all
all:
	cd function   && $(MAKE) build
	cd move_ctor  && $(MAKE) build
	cd shared_ptr && $(MAKE) build
	cd vector     && $(MAKE) build

.PHONY: clean
clean:
	cd function   && $(MAKE) clean
	cd move_ctor  && $(MAKE) clean
	cd shared_ptr && $(MAKE) clean
	cd vector     && $(MAKE) clean

.PHONY: compilation_database
compilation_database:
	$(MAKE) clean
	bear -- $(MAKE) all -k
