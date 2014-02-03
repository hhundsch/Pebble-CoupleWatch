#include "pebble.h"
#include <time.h>

#include "app_options.h"

Window *window;
static bool is_animating;
static bool is_splash_showing;

#define IMAGE_COUNT 8
#define IMAGE_WIDTH 96
#define IMAGE_HEIGHT SCREEN_HEIGHT

#define IMAGE_POS_NORMAL 0
#define IMAGE_POS_PRE  1
#define IMAGE_POS_SEND 2
#define IMAGE_POS_POST 3
#define IMAGE_POS_RECEIVED 4
#define IMAGE_POS_BLINK 5
#define IMAGE_POS_POSTBLINK 6
#define IMAGE_POS_BLINK2 7

static InverterLayer *inverterlayer;

typedef struct
{
	int image_index;
	uint32_t show_interval;
} animation_details;
 
animation_details animation[IMAGE_COUNT];
animation_details animation_bg[IMAGE_COUNT] = 
{ 
	{	.image_index = RESOURCE_ID_IMAGE_BG01,	.show_interval = 100	},
	{	.image_index = RESOURCE_ID_IMAGE_BG02,	.show_interval = 100	},
	{	.image_index = RESOURCE_ID_IMAGE_BG03,	.show_interval = 400	},
	{	.image_index = RESOURCE_ID_IMAGE_BG02,	.show_interval = 100	},
	{	.image_index = RESOURCE_ID_IMAGE_BG04,	.show_interval = 400	},
	{	.image_index = RESOURCE_ID_IMAGE_BG05,	.show_interval = 200	},
	{	.image_index = RESOURCE_ID_IMAGE_BG01,	.show_interval = 200	},
	{	.image_index = RESOURCE_ID_IMAGE_BG05,	.show_interval = 200	}
};

animation_details animation_gb[IMAGE_COUNT] = 
{
	{	.image_index = RESOURCE_ID_IMAGE_GB01,	.show_interval = 100	},
	{	.image_index = RESOURCE_ID_IMAGE_GB02,	.show_interval = 100	},
	{	.image_index = RESOURCE_ID_IMAGE_GB03,	.show_interval = 400	},
	{	.image_index = RESOURCE_ID_IMAGE_GB02,	.show_interval = 100	},
	{	.image_index = RESOURCE_ID_IMAGE_GB04,	.show_interval = 400	},
	{	.image_index = RESOURCE_ID_IMAGE_GB05,	.show_interval = 200	},
	{	.image_index = RESOURCE_ID_IMAGE_GB01,	.show_interval = 200	},
	{	.image_index = RESOURCE_ID_IMAGE_GB05,	.show_interval = 200	}
};

static GBitmap* me;
static BitmapLayer* melayer;
//static GContext* appctx;
static bool boy;
static bool left;
static char *boy_name = "Hub";
static char *girl_name = "Petra";

#define FRAME_COUNT 6
#define HEART_ANIMATION_DURATION 1500

#define FRAME01 GRect(-17, 24, 17, 17)
#define FRAME02 GRect(1, 24, 17, 17)
#define FRAME03 GRect(128, 24, 17, 17)
#define FRAME04 GRect(145, 24, 17, 17)
#define FRAME05 GRect(49, 30, 17, 17)
#define FRAME06 GRect(65, 30, 17, 17)
#define FRAME07 GRect(80, 30, 17, 17)
#define FRAME08 GRect(31, 44, 17, 17)
#define FRAME09 GRect(49, 44, 17, 17)
#define FRAME10 GRect(80, 44, 17, 17)
#define FRAME11 GRect(98, 44, 17, 17)

typedef struct
{
	GRect frame;
	uint32_t duration;
} animation_frame;

animation_frame send_frames[FRAME_COUNT];
animation_frame send_frames_left[FRAME_COUNT];
animation_frame send_frames_right[FRAME_COUNT];

animation_frame receive_frames[FRAME_COUNT];
animation_frame receive_frames_left[FRAME_COUNT];
animation_frame receive_frames_right[FRAME_COUNT];

static GBitmap* heart;
static BitmapLayer* heartlayer;
struct Animation* send_animation[FRAME_COUNT];
struct Animation* receive_animation[FRAME_COUNT];
struct Animation* pop_animation;

#define IMAGE_ARROW_WIDTH 40
#define IMAGE_ARROW_HEIGHT 20
static GBitmap* top_arrow;
static GBitmap* bot_arrow;
static BitmapLayer* top_arrowlayer;
static BitmapLayer* bot_arrowlayer;

#define TEXT_WIDTH SCREEN_WIDTH - IMAGE_ARROW_WIDTH - (IMAGE_WIDTH / 2)
#define TEXT_HEIGHT 20
static TextLayer* top_textlayer;
static TextLayer* bot_textlayer;

#define TIME_FRAME_PADDING 5
#define TIME_FRAME_Y 61 + TIME_FRAME_PADDING //61 (44 + 17) is the lowest possible point of the moving heart animation and the padding for the top side
#define TIME_FRAME_WIDTH SCREEN_WIDTH - (IMAGE_WIDTH / 2) //the padding for the right/left side
#define TIME_FRAME_HEIGHT 40
static TextLayer* time_textlayer;

#define SPLASH_TEXT_WIDTH TIME_FRAME_WIDTH
#define SPLASH_TEXT_HEIGHT 60
#define SPLASH_TEXT_Y SCREEN_HEIGHT - SPLASH_TEXT_HEIGHT
static TextLayer* splash_textlayer;

void init_frames();
void setup_animation();
void setup_screen();
void setup_splash_text(bool first);
void setup_me(int current_position);
void setup_heart(bool send, bool pop);
void setup_names();
void setup_time();
void setup_background();
void animate_heart(bool send);
void animate_pop();
void set_time(const struct tm* t);

AppTimer* timer_handle;
static void handle_timer(void* cookie);

void clear_background()
{
	layer_remove_from_parent((Layer *) top_arrowlayer);
	layer_remove_from_parent((Layer *) bot_arrowlayer);
	//bmp_deinit_container(top_arrow);
	bitmap_layer_destroy(top_arrowlayer);
	gbitmap_destroy(top_arrow);
	//bmp_deinit_container(bot_arrow);
	bitmap_layer_destroy(bot_arrowlayer);
	gbitmap_destroy(bot_arrow);
	
	layer_remove_from_parent((Layer *) top_textlayer);
	layer_remove_from_parent((Layer *) bot_textlayer);
}

void clear_me()
{
	layer_remove_from_parent((Layer *) melayer);
	//bmp_deinit_container(me);
	bitmap_layer_destroy(melayer);
	gbitmap_destroy(me);
}

void clear_heart()
{
	layer_remove_from_parent((Layer *) heartlayer);
	//bmp_deinit_container(&heart);
	bitmap_layer_destroy(heartlayer);
	gbitmap_destroy(heart);
}

void clear_time()
{
	layer_remove_from_parent((Layer *) time_textlayer);
}

void clear_splash_text()
{
	layer_remove_from_parent((Layer *) splash_textlayer);
}

void clear_screen()
{
	clear_time();
	clear_me();
	clear_heart();
	clear_background();
}

void animate_send()
{
	if(is_animating) return;
	else is_animating = true;
	
	//app_timer_send_event(appctx, animation[IMAGE_POS_NORMAL].show_interval, IMAGE_POS_PRE);
	app_timer_register(animation[IMAGE_POS_NORMAL].show_interval,handle_timer,(void *)IMAGE_POS_PRE);
}

void animate_receive()
{
	if(is_animating) return;	
	else is_animating = true;
	
	clear_heart();
	setup_heart(false, false);
	animate_heart(false);
}

void animate_blink()
{
	if(EYES_BLINK == false) return;
	
	if(is_animating) return;
	else is_animating = true;

	if((rand() % 100) <= EYES_BLINK_RATE)
		//app_timer_send_event(appctx, animation[IMAGE_POS_BLINK].show_interval, IMAGE_POS_BLINK);
		app_timer_register(animation[IMAGE_POS_BLINK].show_interval,handle_timer,(void *)IMAGE_POS_BLINK);
	else
		is_animating = false;
}

void send_animation_stopped(Animation *animation, void *data)
{
	(void) animation;
	(void) data;
	
	clear_heart();
	
	if(is_splash_showing == true)
	{
		//revert to the original character
		boy = !boy;
		left = !left;
		
		clear_me();
		setup_animation();
		setup_me(IMAGE_POS_NORMAL);
		
		clear_splash_text();
		setup_splash_text(false);
		animate_receive();
	}
}

void receive_animation_stopped(Animation *animation, void *data)
{
	(void) animation;
	(void) data;
	
	clear_me();
	clear_heart();
	setup_heart(true, true);
	setup_me(IMAGE_POS_RECEIVED);
	animate_pop();
}

void pop_animation_stopped(Animation *animation, void *data)
{
	(void)animation;
	(void)data;
	
	is_animating = false;
	
	clear_me();
	clear_heart();
		
	if(is_splash_showing == true)
	{
		clear_splash_text();
		
		setup_names();
		setup_background();
		setup_time();
		
		struct tm* current;
		time_t t;
		
		//get_time(current);
		t = time(NULL);
		current = localtime(&t);
		set_time(current);
	}
	
	setup_me(IMAGE_POS_NORMAL);
	
	if(is_splash_showing == true) 
	{
		is_splash_showing = false;
		setup_animation();
	}
}

void animate_pop()
{
	pop_animation = animation_create();
	//if(left) property_animation_init_layer_frame(&pop_animation, (Layer *) heartlayer, &FRAME05, &FRAME05);
	//else property_animation_init_layer_frame(&pop_animation, (Layer *) heartlayer, &FRAME07, &FRAME07);

	animation_set_duration(pop_animation, animation[IMAGE_POS_RECEIVED].show_interval);
	animation_set_curve(pop_animation, AnimationCurveEaseInOut);
	
	animation_set_handlers(pop_animation,
						   (AnimationHandlers)
						   {
							   .stopped = (AnimationStoppedHandler)pop_animation_stopped
						   },
						   NULL);
	animation_schedule(pop_animation);
}

void animate_heart(bool send)
{
	for(int x = 0; x < FRAME_COUNT - 1; x++)
	{
		if(send) animation_schedule(send_animation[x]);
		else animation_schedule(receive_animation[x]);
	}
}

void init_frames()
{
	//send animation starts at IMAGE_POS_SEND
	//so total send animation is minus the IMAGE_POS_NORMAL and IMAGE_POS_PRE durations
	//118.0 is the total of the duration of the send animation_frame 
	double duration = (HEART_ANIMATION_DURATION - animation[IMAGE_POS_NORMAL].show_interval - animation[IMAGE_POS_PRE].show_interval) / 118.0;
	
	//the constants below are the corresponding lengths of the frames
	//this is to ensure that the speed of the animation is consistent
	memcpy(send_frames_left, 
			(animation_frame[FRAME_COUNT])
			{
				{ 	.frame = FRAME09,	.duration = 22 * duration	},
				{	.frame = FRAME06,	.duration = 22 * duration	},
				{	.frame = FRAME10,	.duration = 19 * duration	},
				{	.frame = FRAME11,	.duration = 37 * duration	},
				{	.frame = FRAME03,	.duration = 18 * duration	},
				{	.frame = FRAME04,	.duration = 0 * duration	}
			},
			sizeof send_frames_left);

	memcpy(send_frames_right,
			(animation_frame[FRAME_COUNT]) 
			{
				{	.frame = FRAME10,	.duration = 22 * duration	},
				{	.frame = FRAME06,	.duration = 22 * duration	},	
				{	.frame = FRAME09,	.duration = 19 * duration	},	
				{	.frame = FRAME08,	.duration = 37 * duration	},
				{	.frame = FRAME02,	.duration = 18 * duration	},
				{	.frame = FRAME01,	.duration = 0 * duration	}
			},
			sizeof send_frames_right);
	
	//receive animation starts immediately
	//110.0 is the total of the duration of the receive animation_frame 
	duration = HEART_ANIMATION_DURATION / 110.0;
	
	//the constants below are the corresponding lengths of the frames
	//this is to ensure that the speed of the animation is consistent
	memcpy(receive_frames_left,
			(animation_frame[FRAME_COUNT])
			{
				{	.frame = FRAME04,	.duration = 18 * duration	},
				{	.frame = FRAME03,	.duration = 37 * duration	},
				{	.frame = FRAME11,	.duration = 19 * duration	},
				{	.frame = FRAME10,	.duration = 19 * duration	},
				{	.frame = FRAME06,	.duration = 17 * duration	},
				{ 	.frame = FRAME05,	.duration = 0 * duration	}
			},
			sizeof receive_frames_left);

	memcpy(receive_frames_right,
			(animation_frame[FRAME_COUNT])
			{
				{	.frame = FRAME01,	.duration = 18 * duration	},
				{	.frame = FRAME02,	.duration = 37 * duration	},
				{	.frame = FRAME08,	.duration = 19 * duration	},
				{	.frame = FRAME09,	.duration = 19 * duration	},
				{	.frame = FRAME06,	.duration = 17 * duration	},
				{ 	.frame = FRAME07,	.duration = 0 * duration	}
			},
			sizeof receive_frames_right);
}

void setup_inverter()
{
	//inverter_layer_init(inverterlayer, GRect(0, 0, SCREEN_WIDTH, (INVERT_COLOR ? SCREEN_HEIGHT : 0)));
	inverterlayer = inverter_layer_create(GRect(0, 0, SCREEN_WIDTH, (INVERT_COLOR ? SCREEN_HEIGHT : 0)));
	layer_add_child((Layer *)window, (Layer *)inverterlayer);
}

void setup_animation()
{
	for(int x = 0; x < IMAGE_COUNT; x++)
	{
		if((boy == true && left == true) || (boy == false && left == false)) 
		{
			animation[x].image_index = animation_bg[x].image_index;
			animation[x].show_interval = animation_bg[x].show_interval;
		}
		else
		{
			animation[x].image_index = animation_gb[x].image_index;
			animation[x].show_interval = animation_gb[x].show_interval;
		}
	}
	
	for(int x = 0; x < FRAME_COUNT; x++)
	{
		if(left == true)
		{
			send_frames[x].frame = send_frames_left[x].frame;
			send_frames[x].duration = send_frames_left[x].duration;
			
			receive_frames[x].frame = receive_frames_left[x].frame;
			receive_frames[x].duration = receive_frames_left[x].duration;
		}
		else
		{
			send_frames[x].frame = send_frames_right[x].frame;
			send_frames[x].duration = send_frames_right[x].duration;
			
			receive_frames[x].frame = receive_frames_right[x].frame;
			receive_frames[x].duration = receive_frames_right[x].duration;
		}
	}
	
	int total_send_delay = 0;
	
	//since the receive animation is supposed to be the continuation of the send animation of the other watch, 
	//start the delay at HEART_ANIMATION_DURATION 
	//but if the splash screen is showing, then the delay is not needed 
	int total_receive_delay = (is_splash_showing ? 0 : HEART_ANIMATION_DURATION); 

	for(int x = 0; x < FRAME_COUNT - 1; x++) //-1 because animate_heart looks at the current frame and the next frame in the array
	{
		//property_animation_init_layer_frame(&send_animation[x], (Layer *) heartlayer, &send_frames[x].frame, &send_frames[x + 1].frame);
		send_animation[x] = animation_create();
		
		animation_set_duration(send_animation[x], send_frames[x].duration);
		animation_set_delay(send_animation[x], total_send_delay);
		total_send_delay += send_frames[x].duration;
		
		if(x == FRAME_COUNT - 2) //-2 because that is the last item when the condition to break is < X - 1
		{
			animation_set_handlers(send_animation[x],
								   (AnimationHandlers)
								   {
									   .stopped = (AnimationStoppedHandler)send_animation_stopped
								   },
								   NULL);
		}
		
		animation_set_curve(send_animation[x], AnimationCurveLinear);
		
		//property_animation_init_layer_frame(receive_animation[x], (Layer *) heartlayer, &receive_frames[x].frame, &receive_frames[x + 1].frame);
		receive_animation[x] = animation_create();
		
		animation_set_duration(receive_animation[x], receive_frames[x].duration);
		
		if(total_receive_delay != 0) animation_set_delay(receive_animation[x], total_receive_delay);
		total_receive_delay += receive_frames[x].duration;
		
		if(x == FRAME_COUNT - 2) //-2 because that is the last item when the condition to break is < X - 1
		{
			animation_set_handlers(receive_animation[x],
							   (AnimationHandlers)
							   {
								   .stopped = (AnimationStoppedHandler)receive_animation_stopped
							   },
							   NULL);
		}
			
		animation_set_curve(receive_animation[x], AnimationCurveLinear);
	}
}

void setup_heart(bool send, bool pop)
{
	if(pop) 
	{
		//bmp_init_container(RESOURCE_ID_IMAGE_POP, &heart);
		heart = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_POP);
		heartlayer = bitmap_layer_create(FRAME05);
		
		if(left) layer_set_frame((Layer *) heartlayer, FRAME05);
		else layer_set_frame((Layer *) heartlayer, FRAME07);
	}
	else
	{
		//bmp_init_container(RESOURCE_ID_IMAGE_HEART, &heart);
		heart = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_POP);
		heartlayer = bitmap_layer_create(FRAME05);
		
		if(send) layer_set_frame((Layer *) heartlayer, send_frames[0].frame);
		else layer_set_frame((Layer *) heartlayer, receive_frames[0].frame);
	}
	
	layer_insert_below_sibling((Layer *) heartlayer, (Layer *) inverterlayer);
}

void setup_names()
{
	if(boy)
	{
		boy_name = "Me";
		
		#if DEFAULT_NAMES
			girl_name = "Her";
		#else
			girl_name = GIRL_NAME_CUSTOM;
		#endif
	}
	else
	{
		girl_name = "Me";
		
		#if DEFAULT_NAMES
			boy_name = "Him";
		#else
			boy_name = BOY_NAME_CUSTOM;
		#endif
	}
}

void setup_me(int current_position)
{
	if(current_position >= IMAGE_COUNT) current_position = IMAGE_POS_NORMAL;
	
	//bmp_init_container(animation[current_position].image_index, &me);
	me = gbitmap_create_with_resource(animation[current_position].image_index);
	melayer = bitmap_layer_create(FRAME05);
		
	if(left) layer_set_frame((Layer *) melayer, GRect(-IMAGE_WIDTH / 2, 0, IMAGE_WIDTH, IMAGE_HEIGHT));
	else layer_set_frame((Layer *) melayer, GRect(SCREEN_WIDTH-(IMAGE_WIDTH / 2), 0, IMAGE_WIDTH, IMAGE_HEIGHT));
	
	layer_insert_below_sibling((Layer *) melayer, (Layer *) inverterlayer);
}

void setup_time()
{
	if(left)
		//text_layer_init(&time_text, GRect((IMAGE_WIDTH / 2) + TIME_FRAME_PADDING, TIME_FRAME_Y, TIME_FRAME_WIDTH, TIME_FRAME_HEIGHT));
		time_textlayer = text_layer_create(GRect((IMAGE_WIDTH / 2) + TIME_FRAME_PADDING, TIME_FRAME_Y, TIME_FRAME_WIDTH, TIME_FRAME_HEIGHT));
	else
		//text_layer_init(&time_text, GRect(TIME_FRAME_PADDING, TIME_FRAME_Y, TIME_FRAME_WIDTH, TIME_FRAME_HEIGHT));
	    time_textlayer = text_layer_create(GRect(TIME_FRAME_PADDING, TIME_FRAME_Y, TIME_FRAME_WIDTH, TIME_FRAME_HEIGHT));
	
	text_layer_set_font(time_textlayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HANDSEAN_26)));
	text_layer_set_text_alignment(time_textlayer, GTextAlignmentCenter);
	layer_insert_below_sibling((Layer *) time_textlayer, (Layer *) inverterlayer);
}

void setup_splash_text(bool first)
{
	if(left)
		//text_layer_init(&splash_text, GRect((IMAGE_WIDTH / 2), SPLASH_TEXT_Y, SPLASH_TEXT_WIDTH, SPLASH_TEXT_HEIGHT));
		splash_textlayer = text_layer_create(GRect((IMAGE_WIDTH / 2), SPLASH_TEXT_Y, SPLASH_TEXT_WIDTH, SPLASH_TEXT_HEIGHT));
	else
		//text_layer_init(&splash_text, GRect(0, SPLASH_TEXT_Y, SPLASH_TEXT_WIDTH, SPLASH_TEXT_HEIGHT));
		splash_textlayer = text_layer_create(GRect(0, SPLASH_TEXT_Y, SPLASH_TEXT_WIDTH, SPLASH_TEXT_HEIGHT));
	
	//static char *splash_text_value = " ";
	if(first) text_layer_set_text(splash_textlayer, "created by hhundsch");
	else text_layer_set_text(splash_textlayer, "(c) 2014 CoupleWatch");
			
	text_layer_set_font(splash_textlayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HANDSEAN_12)));
	text_layer_set_overflow_mode(splash_textlayer, GTextOverflowModeWordWrap);
	text_layer_set_text_alignment(splash_textlayer, GTextAlignmentCenter);
	layer_insert_below_sibling((Layer *) splash_textlayer, (Layer *) inverterlayer);
}

void setup_background()
{
	if(left) 
	{
		//bmp_init_container(RESOURCE_ID_IMAGE_ARROW_LEFT, &top_arrow);
		//bmp_init_container(RESOURCE_ID_IMAGE_ARROW_LEFT, &bot_arrow);
		top_arrow = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ARROW_LEFT);
		top_arrowlayer = bitmap_layer_create(FRAME05);
		bot_arrow = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ARROW_LEFT);
		bot_arrowlayer = bitmap_layer_create(FRAME05);
		
		layer_set_frame((Layer *) top_arrowlayer, GRect(SCREEN_WIDTH - IMAGE_ARROW_WIDTH, -(IMAGE_ARROW_HEIGHT / 2), IMAGE_ARROW_WIDTH, IMAGE_ARROW_HEIGHT));
		layer_set_frame((Layer *) bot_arrowlayer, GRect(IMAGE_WIDTH / 2, SCREEN_HEIGHT - (IMAGE_ARROW_HEIGHT / 2), IMAGE_ARROW_WIDTH, IMAGE_ARROW_HEIGHT));
		
		//text_layer_init(top_textlayer, GRect(IMAGE_WIDTH / 2, 0, TEXT_WIDTH, TEXT_HEIGHT));
		top_textlayer = text_layer_create(GRect(IMAGE_WIDTH / 2, 0, TEXT_WIDTH, TEXT_HEIGHT));
		//text_layer_init(bot_textlayer, GRect((IMAGE_WIDTH / 2) + IMAGE_ARROW_WIDTH, SCREEN_HEIGHT - TEXT_HEIGHT, TEXT_WIDTH, TEXT_HEIGHT));
		bot_textlayer = text_layer_create(GRect((IMAGE_WIDTH / 2) + IMAGE_ARROW_WIDTH, SCREEN_HEIGHT - TEXT_HEIGHT, TEXT_WIDTH, TEXT_HEIGHT));
		
		text_layer_set_text_alignment(top_textlayer, GTextAlignmentRight);
	}
	else 
	{
		//bmp_init_container(RESOURCE_ID_IMAGE_ARROW_RIGHT, &top_arrow);
		//bmp_init_container(RESOURCE_ID_IMAGE_ARROW_RIGHT, &bot_arrow);
		top_arrow = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ARROW_RIGHT);
		top_arrowlayer = bitmap_layer_create(FRAME05);
		bot_arrow = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ARROW_RIGHT);
		bot_arrowlayer = bitmap_layer_create(FRAME05);		
		layer_set_frame((Layer *) top_arrowlayer, GRect(0, -(IMAGE_ARROW_HEIGHT / 2), IMAGE_ARROW_WIDTH, IMAGE_ARROW_HEIGHT));
		layer_set_frame((Layer *) bot_arrowlayer, GRect(SCREEN_WIDTH - IMAGE_WIDTH, SCREEN_HEIGHT - (IMAGE_ARROW_HEIGHT / 2), IMAGE_ARROW_WIDTH, IMAGE_ARROW_HEIGHT));
		
		//text_layer_init(top_textlayer, GRect(IMAGE_ARROW_WIDTH, 0, TEXT_WIDTH, TEXT_HEIGHT));
		top_textlayer = text_layer_create(GRect(IMAGE_ARROW_WIDTH, 0, TEXT_WIDTH, TEXT_HEIGHT));
		//text_layer_init(bot_textlayer, GRect(0, SCREEN_HEIGHT - TEXT_HEIGHT, TEXT_WIDTH, TEXT_HEIGHT));
		bot_textlayer = text_layer_create(GRect(0, SCREEN_HEIGHT - TEXT_HEIGHT, TEXT_WIDTH, TEXT_HEIGHT));
		text_layer_set_text_alignment(bot_textlayer, GTextAlignmentRight);
	}
	
	if(boy)
	{
		text_layer_set_text(top_textlayer, girl_name);
		text_layer_set_text(bot_textlayer, boy_name);
	}
	else
	{
		text_layer_set_text(top_textlayer, boy_name);
		text_layer_set_text(bot_textlayer, girl_name);
	}
	
	text_layer_set_font(top_textlayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HANDSEAN_14)));
	text_layer_set_font(bot_textlayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HANDSEAN_14)));
	
	layer_insert_below_sibling((Layer *) top_arrowlayer, (Layer *) inverterlayer);
	layer_insert_below_sibling((Layer *) bot_arrowlayer, (Layer *) inverterlayer);
	
	layer_insert_below_sibling((Layer *) top_textlayer, (Layer *) inverterlayer);
	layer_insert_below_sibling((Layer *) bot_textlayer, (Layer *) inverterlayer);
}

void setup_screen()
{
	setup_inverter();
	
	if(is_splash_showing == false)
	{
		setup_names();
		setup_background();
	}
	
	init_frames();	
	setup_animation();
	setup_me(IMAGE_POS_NORMAL);
	
	if(is_splash_showing == false) setup_time();
}

static void show_splash()
{
	if (is_animating == true) return;
	
	is_splash_showing = true;
	
	//show the opposite character first
	boy = !boy;
	left = !left;
	
	clear_screen();
	setup_screen();
	setup_splash_text(true);
	animate_send();
}

//static void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie)
static void handle_timer(void *data)
{
	uint32_t cookie = (uint32_t) data;
	
	if(is_animating == false) return;
	
	clear_me();
	setup_me(cookie);
	
	static uint32_t new_position;
	if(cookie == (uint32_t) IMAGE_POS_NORMAL) 
	{
		is_animating = false;
		new_position = IMAGE_POS_NORMAL;
		return;
	}
	else if(cookie == (uint32_t) IMAGE_POS_PRE) 
	{
		new_position = cookie + 1;
	}
	else if(cookie == (uint32_t) IMAGE_POS_SEND) 
	{
		new_position = cookie + 1;
		
		clear_heart();
		setup_heart(true, false);
		animate_heart(true);
	}
	else if(cookie == (uint32_t) IMAGE_POS_POST) 
	{
		new_position = IMAGE_POS_NORMAL;
	}
	else if(cookie == (uint32_t) IMAGE_POS_RECEIVED) 
	{
		new_position = IMAGE_POS_NORMAL;
	}
	else if(cookie == (uint32_t) IMAGE_POS_BLINK) 
	{
		if((rand() % 100) <= EYES_BLINK2_RATE)
			new_position = IMAGE_POS_POSTBLINK;
		else 
			new_position = IMAGE_POS_NORMAL;
	}
	else if(cookie == (uint32_t) IMAGE_POS_POSTBLINK) 
	{
		new_position = IMAGE_POS_BLINK2;
	}
	else if(cookie == (uint32_t) IMAGE_POS_BLINK2) 
	{
		new_position = IMAGE_POS_NORMAL;
	}
	else 
	{
		is_animating = false;
		new_position = IMAGE_POS_NORMAL;
		return;
	}
	
	//app_timer_send_event(ctx, animation[cookie].show_interval, new_position);
	timer_handle = app_timer_register(animation[cookie].show_interval, handle_timer, (void *) new_position);
	return;
}

void set_time(const struct tm* t)
{
	static char hourText[] = "04:44"; //this is the longest possible text based on the font used
	if(clock_is_24h_style())
		//string_format_time(hourText, sizeof(hourText), "%H:%M", t);
		strftime(hourText, sizeof(hourText), "%H:%M", t);
	else
		//string_format_time(hourText, sizeof(hourText), "%I:%M", t);
		strftime(hourText, sizeof(hourText), "%I:%M", t);
	
	text_layer_set_text(time_textlayer, hourText);
}

void handle_second_tick(struct tm* t, TimeUnits tu)
{
	//(void)ctx;
	
	if(is_splash_showing == true) return;
	
	int seconds = t->tm_sec;
	int minutes = t->tm_min;
	int hours = t->tm_hour;
	
	if(seconds == 0)
	{
		set_time(t);	
	}
	
	if(seconds == EYES_BLINK_START)
	{
		animate_blink();
	}

	bool sender = false;
	bool receiver = false;
	
	if(SENDER_START_OPTIONS == START_EVERY_HOUR && seconds == 0) 
	{
		sender = (minutes == SENDER_START);
		if(RECEIVER_START_OPTIONS == ALTERNATE_WITH_SEND && sender)
		{
			sender = (hours % 2 == 0);
			receiver = (hours % 2 == 1);
		}
	}
	else if(SENDER_START_OPTIONS == START_EVERY_MINUTE) 
	{
		sender = (seconds == SENDER_START);
		
		if(RECEIVER_START_OPTIONS == ALTERNATE_WITH_SEND && sender)
		{
			sender = (minutes % 2 == 0);
			receiver = (minutes % 2 == 1);
		}
	}
	else if(SENDER_START_OPTIONS == START_EVERY_X_HOURS && minutes == 0 && seconds == 0) 
	{
		int send_interval = (SENDER_START == 0 ? 1 : SENDER_START);
		sender = (hours % send_interval == 0);
		
		if(RECEIVER_START_OPTIONS == ALTERNATE_WITH_SEND && sender)
		{
			sender = ((hours / send_interval) % 2 == 0);
			receiver = ((hours / send_interval) % 2 == 1);
		}
	}
	else if(SENDER_START_OPTIONS == START_EVERY_X_MINUTES && seconds == 0) 
	{
		int send_interval = (SENDER_START == 0 ? 1 : SENDER_START);
		sender = (minutes % send_interval == 0);
		
		if(RECEIVER_START_OPTIONS == ALTERNATE_WITH_SEND && sender)
		{
			sender = ((minutes / send_interval) % 2 == 0);
			receiver = ((minutes / send_interval) % 2 == 1);
		}
	}
	else if(SENDER_START_OPTIONS == ALTERNATE_WITH_SEND && seconds == 0)
	{
		sender = (minutes == SENDER_START);
		if(RECEIVER_START_OPTIONS == ALTERNATE_WITH_SEND && sender)
		{
			sender = (hours % 2 == 0);
			receiver = (hours % 2 == 1);
		}
	}
	
	if(sender)
	{
		if(LEFT_SENDER == left) animate_send();
		else animate_receive();
	}
	
	if(RECEIVER_START_OPTIONS == START_EVERY_HOUR && seconds == 0)
	{
		receiver = (minutes == RECEIVER_START);
	}
	else if(RECEIVER_START_OPTIONS == START_EVERY_MINUTE)
	{
		receiver = (seconds == RECEIVER_START);
	}
	else if(RECEIVER_START_OPTIONS == START_EVERY_X_HOURS && minutes == 0 && seconds == 0)
	{
		int receive_interval = (RECEIVER_START == 0 ? 1 : RECEIVER_START);
		receiver = (hours % receive_interval == 0);
	}
	else if(RECEIVER_START_OPTIONS == START_EVERY_X_MINUTES && seconds == 0)
	{
		int receive_interval = (RECEIVER_START == 0 ? 1 : RECEIVER_START);
		receiver = (minutes % receive_interval == 0);
	}
	
	if(receiver)
	{
		if(LEFT_SENDER == left) animate_receive();
		else animate_send();
	}
}

#ifdef DEBUG
	bool toggle_pos = true;
	void handle_up_single_click(ClickRecognizerRef recognizer, Window *window1) 
	{	
		if(toggle_pos) left = !left;
		else boy = !boy;
		
		toggle_pos = !toggle_pos;
		
		clear_time();
		clear_background();
		clear_me();
		clear_heart();
		setup_names();
		setup_background();
		init_frames();
		setup_animation();
		setup_me(IMAGE_POS_NORMAL);
		setup_time();
		
		PblTm current;
		get_time(&current);
		set_time(&current);
	}

	bool toggle_send = true;
	void handle_select_single_click(ClickRecognizerRef recognizer, Window *window1) 
	{
		if (toggle_send == false) animate_receive();
		else animate_send();
		
		toggle_send = !toggle_send;
	}
	
	void handle_down_single_click(ClickRecognizerRef recognizer, Window *window1) 
	{
		animate_blink();	
	}

	void config_provider(ClickConfig **config, Window *window) 
	{
		config[BUTTON_ID_UP]->click.handler = (ClickHandler) handle_up_single_click;
		config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) handle_down_single_click;	
		config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) handle_select_single_click;
	}
#endif

void init(void)
{
	window = window_create();
	#ifdef DEBUG
		window_set_fullscreen(window, true);
		window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
	#endif

	window_stack_push(window, true);

	//resource_init_current_app(&APP_RESOURCES);

	srand(time(NULL));
	is_animating = false;
	
	//appctx = ctx;
	boy = BOY_WATCH;
	
	if(boy && BOY_ON_LEFT) left = true;
	else left = false;
	
	show_splash();
}

void deinit(void) 
{
	clear_screen();
	window_destroy(window);
}

int main(void)
{
    init();
  	app_event_loop();
	deinit();
}
