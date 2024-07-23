root_dir := $(PWD)
bld_dir := ./build
$(bld_dir):
	mkdir -p $(bld_dir)

partA: | $(bld_dir)
	cd $(bld_dir); \
	ncverilog ../top_tb.sv +access+r +incdir+$(root_dir) +prog_path=$(root_dir)/DRAM_INPUT ;

clean:
	rm -rf $(bld_dir);
