

CFLAGS := -Wall -g

main:
	@mkdir -p bin
	@$(CC) $(CFLAGS) src/*.c -o bin/stupid-http

clean:
	@echo Cleaning...
	@rm -r bin

install:
	@echo Installing...
	@cp bin/stupid-http /usr/bin

uninstall:
	@echo Uninstalling...
	@rm /usr/bin/stupid-http