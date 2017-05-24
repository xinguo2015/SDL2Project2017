projs = $(sort HanoiTower Maze Sweep Tetris WordHit)
test:
	echo the projects are:
	$(foreach d, $(projs), echo  $(d); )

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

