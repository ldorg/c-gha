.PHONY: all debug release test test-ci clean install help

# Detect if we need to override the compiler
# GitHub Actions and most CI environments work fine with CMake's default detection
ifeq ($(shell uname),Darwin)
    ifneq ($(wildcard /usr/bin/clang),)
        CC_VAR = CC=/usr/bin/clang
    else
        CC_VAR =
    endif
else
    CC_VAR =
endif

# Default target
all: debug

# Debug build
debug:
	@mkdir -p build
	@cd build && $(CC_VAR) cmake -DCMAKE_BUILD_TYPE=Debug ..
	@cd build && make

# Release build
release:
	@mkdir -p build
	@cd build && $(CC_VAR) cmake -DCMAKE_BUILD_TYPE=Release ..
	@cd build && make

# Run tests
test: debug
	@cd build && make test

# Run tests and generate JUnit XML for CI using Unity framework
test-ci: debug
	@echo "Running Unity tests..."
	@mkdir -p test_results
	@for test in test_gpio test_uart test_led test_sensor test_drivers; do \
		echo "Running $$test..."; \
		if ./build/tests/$$test > test_results/$$test.testpass 2>&1; then \
			echo "✓ $$test passed"; \
		else \
			mv test_results/$$test.testpass test_results/$$test.testfail; \
			echo "✗ $$test failed"; \
		fi; \
	done
	@echo "Converting to JUnit XML..."
	@python3 third_party/unity/auto/stylize_as_junit.py test_results/ --output test-results.xml
	@echo "Professional JUnit XML generated: test-results.xml"

# Clean build artifacts
clean:
	@rm -rf build

# Install (requires prior build)
install:
	@cd build && make install

# Run the application
run: debug
	@./build/src/demo-firmware

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build debug version (default)"
	@echo "  debug    - Build debug version"
	@echo "  release  - Build release version"
	@echo "  test     - Run tests"
	@echo "  test-ci  - Run tests and generate JUnit XML"
	@echo "  run      - Build and run the application"
	@echo "  clean    - Remove build directory"
	@echo "  install  - Install built binaries"
	@echo "  help     - Show this help"
	@echo ""
	@echo "Environment variables:"
	@echo "  CC       - Override compiler (e.g., CC=gcc make)"