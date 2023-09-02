# For each command, find all of the platform directories and run the command in those

build:
	@for dir in $(shell find . -maxdepth 1 -type d -not -name '.' -print); do \
		echo "Building $$dir"; \
		cd $$dir && make build && cd ..; \
		echo "Done with $$dir!\n"; \
	done

clean:
	@for dir in $(shell find . -maxdepth 1 -type d -not -name '.' -print); do \
		echo "Cleaning $$dir"; \
		cd $$dir && make clean && cd ..; \
	done
