#projs = $(sort $(wildcard 0*)) $(sort $(wildcard 1*))
projs = $(sort $(wildcard 0*))
projs += $(sort $(wildcard 1*))

test:
	echo the projects are:
	$(foreach d, $(projs), echo  $(d); )

builddir:
	$(foreach name, $(projs), mkdir -p $(name)/build; )

all:
	$(foreach name, $(projs), \
		cd $(name)/build && cmake .. && make && cd ../../;)

runall:
	#$(foreach name, $(projs), ./$(name)/build/$(name) ../Media;)
	$(foreach name, $(projs), ./$(name)/build/$(name);)

clearBuild:
	$(foreach name, $(projs), rm -rf $(name)/build/*;)

clearKDEV:
	$(foreach name, $(projs), rm -rf $(name)/*.kdev4;)
	$(foreach name, $(projs), rm -rf $(name)/.kdev4;)

clearVS:
	$(foreach name, $(projs), rm -rf $(name)/Release;)
	$(foreach name, $(projs), rm -rf $(name)/Debug;)
	$(foreach name, $(projs), rm -rf $(name)/*.sdf;)
	$(foreach name, $(projs), rm -rf $(name)/*.suo;)

clean: clearBuild clearVS clearKDEV

