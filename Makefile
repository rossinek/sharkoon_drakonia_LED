main:	clean
	mkdir bin
	gcc sharkoonLED.c -o bin/sharkoonLED -lusb-1.0


clean:
	rm -rf bin
