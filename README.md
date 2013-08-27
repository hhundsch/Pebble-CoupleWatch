Pebble-CoupleWatch
==================

A watch face intended to be used by couples.

The idea of the watch face is that there are two characters -- a boy and a girl on its own watch face and installed on a separate watches (preferably by owned couples).

These characters send hearts to each other at specified times and the animation is setup such that it would look like the heart "travels over" to the other watch (and received by the other character) when sent.

For variation, each character also blinks randomly every minute.

Several options can be easily customized so that when sent, the heart won't "show up" on other watches using the same watch face.

These options are the following (at the moment these can only be set prior to compiling):
- Character names for both (boy and girl)
- Position of both characters (boy on left and girl on right, or vice versa)
- Who sends the heart first (the character on the left, or the character on the right)
- Frequency of sending the heart (for both characters)
- Frequency of blinking

Usage instructions:
- Import this Pebble project to CloudPebble.net (optional).
- Open the app_options.h file and set the various options as needed.
- Compile this project for a "boy" watch face (set BOY_WATCH = true) and install to your watch (if you are a boy) or to your partner's watch.
- Compile this project for a "girl" watch face (set BOY_WATCH = false) and install to your watch (if you are a girl) or to your partner's watch.
