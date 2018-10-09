noinst_HEADERS = Bootstrap.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir)
assignment2_SOURCES = Bootstrap.cpp Object.cpp Paddle.cpp 
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) -I/lusr/opt/cegui-0.8.7/include/cegui-0
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system -L/lusr/opt/cegui-0.8.7/lib -lCEGUIBase-0 -lCEGUIOgreRenderer-0 -lSDL2main -lSDL2

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
