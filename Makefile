model-converter:model-converter.o
	# dynamic
	# g++ build/model-converter.o -o bin/model-converter -l assimp
	# static 
	g++ build/model-converter.o -o bin/model-converter -L /usr/local/third_party/assimp-master/lib -static -l assimp -l stdc++ -l m -l gcc -l c -l z -l draco -l minizip

model-converter.o:src/model-converter.cpp
	g++ -c src/model-converter.cpp -o build/model-converter.o

.PHONY:clean
clean:
	-rm build/*.o bin/*
