#ifndef _OPTIONS_
	#define _OPTIONS_

	//#define DEBUG								//uncomment this if you know what you are doing
	//#define PHONE_HAS_HTTPPEBBLE				//not yet implemented
	//#define ANDROID							//not yet implemented

	//you should only be changing this option when compiling for both watches
	//this option when set to true makes the compiled watch face use the boy character
	#define BOY_WATCH true 

	//general options
	#define BOY_ON_LEFT true					
	#define LEFT_SENDER true				//determines if the character that is on the left will be designated as the "sender" character
	
	#define DEFAULT_NAMES true				//if true, the other character will be called "Him" or "Her", otherwise the value of the BOY_NAME_CUSTOM/GIRL_NAME_CUSTOM will be used accordingly
	#define BOY_NAME_CUSTOM "Him"
	#define GIRL_NAME_CUSTOM "Her"	
	
	#define INVERT_COLOR false				//inverts the color of the whole watch face

	#define EYES_BLINK true
	#define EYES_BLINK_RATE 60				//chance to blink
	#define EYES_BLINK2_RATE 40				//chance to blink twice (this goes off only if the character is about to blink)
	#define EYES_BLINK_START 30 			//character blinks every minute at XX:XX:30
	
	//values to be used for SENDER_START_OPTIONS/RECEIVER_START_OPTIONS
	//do not modify!
	//option descriptions:
	//	if START_EVERY_HOUR is selected, the character sends/receives heart every hour at X minutes 
	//		(i.e.: if SENDER_START = 30 then the send animation will fire at 1:30:00, 2:30:00, 3:30:00, etc...)
	//	if START_EVERY_MINUTE is selected, the character sends/receives heart every minute at X seconds 
	//		(i.e.: if RECEIVER_START = 10 then the receive animation will fire at 1:30:10, 1:31:10, 1:32:10, etc...)
	//	if START_EVERY_X_HOURS is selected, the character sends/receives heart every X hours 
	//		(i.e.: if RECEIVER_START = 2 then the receive animation will fire at 2:00:00, 4:00:00, 6:00:00, etc...)
	//	if START_EVERY_X_MINUTES is selected, the character sends/receives heart every X minutes 
	//		(i.e.: if SENDER_START = 5 then the send animation will fire at 2:05:00, 2:10:00, 2:15:00, etc...)
	//	ALTERNATE_WITH_SEND should only be used by the receiver
	//		and if selected, the option used by the sender will be followed by both send and receive animations
	//		the difference is that the send and receive animation will alternate
	//		(i.e.: if SENDER_START_OPTIONS = START_EVERY_X_MINUTES and SENDER_START = 30 then the send animation will fire at 2:30:00, 3:30:00, 4:30:00, while the receive animation will fire at 3:00:00, 4:00:00, 5:00:00, etc...)
	//notes:
	//	if SENDER_START_OPTIONS = ALTERNATE_WITH_SEND then START_EVERY_HOUR will be used instead
	//	if the option START_EVERY_X_HOURS/START_EVERY_X_MINUTES is used and SENDER_START/RECEIVER_START = 0 then SENDER_START/RECEIVER_START will be set to 1 instead 
	//	RECEIVER_START will not be used if RECEIVER_START_OPTIONS = ALTERNATE_WITH_SEND
	//	animations will not be queued 
	//		(i.e.: if a send animation is fired while the receive animation is still going on, the send animation will not be performed after the receive animation is done)
	#define START_EVERY_HOUR 0					
	#define START_EVERY_MINUTE 1				
	#define START_EVERY_X_HOURS 2				
	#define START_EVERY_X_MINUTES 3				
	#define ALTERNATE_WITH_SEND 4				
	
	//send and receive heart options:
	//SENDER_START_OPTIONS/SENDER_START are the options used by the "sender" character
	//	the "sender" character is the one that will be using the send animation first 
	//RECEIVER_START_OPTIONS/RECEIVER_START are the options used by the "receiver" character
	//	the "receiver" character is the one that will be using the receive animation first 
	#define SENDER_START_OPTIONS START_EVERY_HOUR
	#define SENDER_START 0 					
	#define RECEIVER_START_OPTIONS START_EVERY_HOUR
	#define RECEIVER_START 30 				

	//constants
	#define SCREEN_HEIGHT 168
	#define SCREEN_WIDTH 144

#endif
