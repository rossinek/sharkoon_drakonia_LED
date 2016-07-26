# Sharkoon mouse LED controller
LED driver for mouses by Sharkoon and Tracer. Tested on Sharkoon Drakonia / Tracer Recon.

compilation: `gcc sharkoonLED.c -o sharkoonLED -lusb-1.0`

dependencies: `libusb-1.0`

	Usage: ./sharkoonLED [intensity] [pulsation] [color]
		intensity:
			0 - off
			1 - low (default)
			2 - middle
			3 - high
		pulsation:
			0 - steady (default)
			1 - slow
			2 - middle
			3 - fast
		color:
			0 - choose color in program (default)
			1-24 - color index
