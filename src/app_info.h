#include "app_options.h"

#ifndef _INFO_
	#define _INFO_

	#define APP_VER_MAJOR 1	
	#define APP_VER_MINOR 0
	
	#ifndef DEBUG
		#ifdef PHONE_HAS_HTTPPEBBLE
			#ifdef ANDROID
				#if BOY_ON_LEFT
					#if BOY_WATCH
						#if INVERT_COLOR
							#define APP_NAME "Couple Watch [BLAV]"
						#else
							#define APP_NAME "Couple Watch [BLA]"
						#endif
					#else
						#if INVERT_COLOR
							#define APP_NAME "Couple Watch [GRAV]"
						#else
							#define APP_NAME "Couple Watch [GRA]"
						#endif
					#endif
				#else
					#if BOY_WATCH
						#if INVERT_COLOR
							#define APP_NAME "Couple Watch [BRAV]"
						#else
							#define APP_NAME "Couple Watch [BRA]"
						#endif
					#else
						#if INVERT_COLOR
							#define APP_NAME "Couple Watch [GLAV]"
						#else
							#define APP_NAME "Couple Watch [GLA]"
						#endif
					#endif
				#endif
			#else
				#if BOY_ON_LEFT
					#if BOY_WATCH
						#if INVERT_COLOR
							#define APP_NAME "Couple Watch [BLIV]"
						#else
							#define APP_NAME "Couple Watch [BLI]"
						#endif
					#else
						#if INVERT_COLOR
							#define APP_NAME "Couple Watch [GRIV]"
						#else
							#define APP_NAME "Couple Watch [GRI]"
						#endif
					#endif
				#else
					#if BOY_WATCH
						#if INVERT_COLOR
							#define APP_NAME "Couple Watch [BRIV]"
						#else
							#define APP_NAME "Couple Watch [BRI]"
						#endif
					#else
						#if INVERT_COLOR
							#define APP_NAME "Couple Watch [GLIV]"
						#else
							#define APP_NAME "Couple Watch [GLI]"
						#endif
					#endif
				#endif
			#endif
		#else
			#if BOY_ON_LEFT
				#if BOY_WATCH
					#if INVERT_COLOR
						#define APP_NAME "Couple Watch [BLV]"
					#else
						#define APP_NAME "Couple Watch [BL]"
					#endif
				#else
					#if INVERT_COLOR
						#define APP_NAME "Couple Watch [GRV]"
					#else
						#define APP_NAME "Couple Watch [GR]"
					#endif
				#endif
			#else
				#if BOY_WATCH
						#if INVERT_COLOR
						#define APP_NAME "Couple Watch [BRV]"
					#else
						#define APP_NAME "Couple Watch [BR]"
					#endif
				#else
					#if INVERT_COLOR
						#define APP_NAME "Couple Watch [GLV]"
					#else
						#define APP_NAME "Couple Watch [GL]"
					#endif
				#endif
			#endif
		#endif
	#else
		#ifdef PHONE_HAS_HTTPPEBBLE
			#ifdef ANDROID
				#if BOY_ON_LEFT
					#if BOY_WATCH
						#if INVERT_COLOR
							#define APP_NAME "DEBUG: Couple Watch [BLAV]"
						#else
							#define APP_NAME "DEBUG: Couple Watch [BLA]"
						#endif
					#else
						#if INVERT_COLOR
							#define APP_NAME "DEBUG: Couple Watch [GRAV]"
						#else
							#define APP_NAME "DEBUG: Couple Watch [GRA]"
						#endif
					#endif
				#else
					#if BOY_WATCH
						#if INVERT_COLOR
							#define APP_NAME "DEBUG: Couple Watch [BRAV]"
						#else
							#define APP_NAME "DEBUG: Couple Watch [BRA]"
						#endif
					#else
						#if INVERT_COLOR
							#define APP_NAME "DEBUG: Couple Watch [GLAV]"
						#else
							#define APP_NAME "DEBUG: Couple Watch [GLA]"
						#endif
					#endif
				#endif
			#else
				#if BOY_ON_LEFT
					#if BOY_WATCH
						#if INVERT_COLOR
							#define APP_NAME "DEBUG: Couple Watch [BLIV]"
						#else
							#define APP_NAME "DEBUG: Couple Watch [BLI]"
						#endif
					#else
						#if INVERT_COLOR
							#define APP_NAME "DEBUG: Couple Watch [GRIV]"
						#else
							#define APP_NAME "DEBUG: Couple Watch [GRI]"
						#endif
					#endif
				#else
					#if BOY_WATCH
						#if INVERT_COLOR
							#define APP_NAME "DEBUG: Couple Watch [BRIV]"
						#else
							#define APP_NAME "DEBUG: Couple Watch [BRI]"
						#endif
					#else
						#if INVERT_COLOR
							#define APP_NAME "DEBUG: Couple Watch [GLIV]"
						#else
							#define APP_NAME "DEBUG: Couple Watch [GLI]"
						#endif
					#endif
				#endif
			#endif
		#else
			#if BOY_ON_LEFT
				#if BOY_WATCH
					#if INVERT_COLOR
						#define APP_NAME "DEBUG: Couple Watch [BLV]"
					#else
						#define APP_NAME "DEBUG: Couple Watch [BL]"
					#endif
				#else
					#if INVERT_COLOR
						#define APP_NAME "DEBUG: Couple Watch [GRV]"
					#else
						#define APP_NAME "DEBUG: Couple Watch [GR]"
					#endif
				#endif
			#else
				#if BOY_WATCH
						#if INVERT_COLOR
						#define APP_NAME "DEBUG: Couple Watch [BRV]"
					#else
						#define APP_NAME "DEBUG: Couple Watch [BR]"
					#endif
				#else
					#if INVERT_COLOR
						#define APP_NAME "DEBUG: Couple Watch [GLV]"
					#else
						#define APP_NAME "DEBUG: Couple Watch [GL]"
					#endif
				#endif
			#endif
		#endif
	#endif
	
	#define APP_AUTHOR "ihopethisnamecounts"

#endif
