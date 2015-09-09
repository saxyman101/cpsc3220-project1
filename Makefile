CC=clang
CFLAGS=-Wall -g

BINS=memory_shim.so leakcount sctracer stester

all: $(BINS)

memory_shim.so: shim.c
	$(CC) $(CFLAGS) -fPIC -shared -o memory_shim.so shim.c -ldl
	
leakcount: leakcount.c
	$(CC) $(CFLAGS) -o leakcount leakcount.c
	
sctracer: sctracer.c
	$(CC) $(CFLAGS) -o sctracer sctracer.c
	
stester: stester.c
	$(CC) $(CFLAGS) -o stester stester.c

clean:
	rm $(BINS) leakcount.txt
	
