make:
	clang++ main.cpp -lraylib -lGL -lm -lpthread -ldl -O3 -lrt -lX11 -o cpong
	./cpong
