#ifndef _OPTIONS_
	#define _OPTIONS_

	//#define DEBUG
	//#define PHONE_HAS_HTTPPEBBLE
	//#define ANDROID

	#define BOY_WATCH true 					//you should only be changing this when compiling for both watches

	//options
	#define BOY_ON_LEFT true
	#define LEFT_SENDER true				//determines if the character that is on the left will be designated as the "sender"
	
	#define START_EVERY_HOUR 0
	#define START_EVERY_MINUTE 1
	#define START_EVERY_X_HOURS 2
	#define START_EVERY_X_MINUTES 3
	
	#define SENDER_START_OPTIONS START_EVERY_HOUR
	#define SENDER_START 0 					//"sender" character sends heart every hour at XX:00:00 (this will make the character on the other watch receive heart at this time)
	
	#define RECEIVER_START_OPTIONS START_EVERY_HOUR
	#define RECEIVER_START 30 				//"sender" character receives heart every hour at XX:30:00 (this will make the character on the other watch send heart at this time)

	#define DEFAULT_NAMES true				//if true, the other character will be called "Him" or "Her", otherwise the value of the X_NAME_CUSTOM will be used
	#define BOY_NAME_CUSTOM "Him"
	#define GIRL_NAME_CUSTOM "Her"	
	
	#define INVERT_COLOR false

	#define EYES_BLINK true
	#define EYES_BLINK_RATE 60				//chance to blink
	#define EYES_BLINK2_RATE 40				//chance to blink twice (this goes off only if the character is about to blink)
	#define EYES_BLINK_START 30 			//character blinks every minute at XX:XX:30

	#define SCREEN_HEIGHT 168
	#define SCREEN_WIDTH 144

#endif
