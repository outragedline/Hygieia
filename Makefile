setup:
	meson setup build

run: setup
	meson compile -C build
	./build/src/Hygieia
