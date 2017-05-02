projs = $(wildcard 0*)

clean:
	$(foreach name, $(projs), rm -rf $(name)/build/*;)

all:
	$(foreach name, $(projs), \
		cd $(name)/build && cmake .. && make && cd ../../;)

test:
	$(foreach name, $(projs), ./$(name)/build/$(name) ../Media;)



