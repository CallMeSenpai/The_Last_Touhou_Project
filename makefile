all:
	gcc main.c `sdl2-config --cflags --libs`
	#first priority to fix error
	#replace include/platform.h
	##last and final hope before we switch to
	#another graphics library
	# ; , or ( lib
	# " before type " error
