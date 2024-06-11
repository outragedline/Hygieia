setup:
	meson setup build

run: setup
	meson compile -C build
	meson test -C build
	./build/src/Hygieia
