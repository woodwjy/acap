PWD_dir=$(shell pwd)
ap = $(PWD_dir)/ac-client
ac = $(PWD_dir)/ac-server
user = $(PWD_dir)/user-client
util = $(PWD_dir)/util
all:
	make -C $(util) 
	make -C $(ap) 
	make -C $(ac) 
	make -C $(user) 
	


clean:
	make clean -C $(util) 
	make clean -C $(ap) 
	make clean -C $(ac) 
	make clean -C $(user) 
