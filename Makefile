build:
	@make -C platforms build
	@make -C tools/bof_assembler build

clean:
	@make -C platforms clean
	@make -C tools/bof_assembler clean

.PHONY: build clean