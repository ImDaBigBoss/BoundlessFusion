build:
	@echo "\nBuilding desktop..."
	@make -C desktop build

	@echo "\nBuilding web..."
	@make -C web build

clean:
	@echo "\nCleaning desktop..."
	@make -C desktop clean

	@echo "\nCleaning web..."
	@make -C web clean