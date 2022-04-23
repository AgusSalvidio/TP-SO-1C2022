SHARED_MODULE = Utils
MODULES = Kernel Memory CPU Console

.PHONY: $(SHARED_MODULE) $(MODULES)

all: $(SHARED_MODULE) $(MODULES)

Utils:
	$(MAKE) -C $@

Memory: Utils
	$(MAKE) -C $@

clean:
	$(MAKE) -C $(SHARED_MODULE) clean
	$(foreach module, $(MODULES), $(MAKE) -C $(module) clean &&) true