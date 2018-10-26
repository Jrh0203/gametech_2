noinst_HEADERS = Bootstrap.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir)
assignment2_SOURCES = Bootstrap.cpp Object.cpp Paddle.cpp 
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) -std=c++11 -I/lusr/opt/cegui-0.8.7/include/cegui-0 -I/lusr/opt/sdl2-net-2.0.1/include/SDL2 
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) -L/lusr/opt/sdl2-net-2.0.1/lib 
assignment2_LDFLAGS = -lOgreOverlay -lboost_system -I/lusr/opt/cegui-0.8.7/lib -lCEGUIBase-0 -lCEGUIOgreRenderer-0 -lSDL -lSDL_mixer -lSDL_net

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
