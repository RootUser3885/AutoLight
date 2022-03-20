This program is written to control neopixel LED strips by interfacing
an ESP32 with WLED installed. Communication is via serial JSON.
Docs on that protocol on WLEDs site.

I originally wrote this for a 2006 Pontiac GTO with a fun little cam
and long tube headers. Fun car, 11/10 good time.

Anyways, this program also interfaces the car via a modified ELM327
adapter. I just removed the Serial to USB adapter and soldered straight to 
the UART.
It uses the ELM to determine if it is safe to allow the user to set the
LED effect via the WLED web interface. Once the car gets over some speed
safety mode is triggered, causing the WLED web UI to be locked out (by 
forcing the preset back as soon as it does not match the desired value).

It also interfaces the vehicle lights by wiring through voltage dividers
and tapping into the relay signal lines. If I can figure out how to get
the vehicle light status via the ELM OBD I will add that but for now
its just hard wired in. :(

Ive looked into other alternatives to the ELM327 but I find it the easiest
to use. I have one in my daily driver (98 Buick LeSabre, mint condition,
neighborhood watch maroon, 3800 V6, Reliable AF, Smooth ride. 12/10 would
recommend) as a digiDash. Its been working fine for years now.

This was originally written for a Teensy 3.2 because thats what I had lying
around at the time that had 3 Serial ports. In theory this should work on 
any avr compatible microcontroller with 3 serial ports.

I am still learning C++ so it this program looks like garbage then I
sincerely apologize. If this program looks like complete incoherent 
gibberish to you then you're likely right. If you're also just learning
C++ then let me give you some advice: don't use this as an example.
You'll end up crying as much as I did. And this should be a simple program.

I haven't decided if this is going on GitHub yet or not. If not then I might
have just wasted my time. Unless future me stumbles across this. If thats the
case, sorry man. If so, if anyone ends up actually looking at this mess,
enjoy stealing my code. And may the bugs be ever on your favor.
Happy programming to all and to all a good 3am programming 
extravaganza. Cheers.