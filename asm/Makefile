build-dir = ./bin
src-dir = ./src

compile:
	g++ -Wall -o $(build-dir)/mala $(src-dir)/*.cpp

run:
	$(build-dir)/mala

compile.run:
	g++ -Wall -o $(build-dir)/mala $(src-dir)/*.cpp
	$(build-dir)/mala
compile.run.ntf:
	g++ -Wall -o $(build-dir)/mala $(src-dir)/*.cpp
	$(build-dir)/mala -i t.asm -o test.hex

clean:
	rm bin/*