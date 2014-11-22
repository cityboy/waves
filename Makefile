CC = c++
RM = rm

BASEDIR = /home/localadmin/Study/OpenGL-tutorial
GLEWDIR = /home/localadmin/Study/Packages/glew
GLFWDIR = /home/localadmin/Study/Packages/glfw
GLMDIR = /home/localadmin/Study/Packages/glm
CFLAGS = -g -Wall -Wextra -pedantic -rdynamic 
CDEFS = -DGLEW_STATIC -DTW_NO_DIRECT3D -DTW_NO_LIB_PRAGMA -DTW_STATIC -D_CRT_SECURE_NO_WARNINGS
IPATH = -I.. -I$(GLEWDIR)/include -I$(GLFWDIR)/include -I$(GLMDIR) -I/usr/include -I/usr/include/x86_64-linux-gnu
LPATH = -L$(GLEWDIR)/lib -L$(GLFWDIR)/src -L/lib64 -L/usr/lib/x86_64-linux-gnu 
LFLAGS = -rdynamic
#LDPATH = -Wl,-R/usr/pkg/lib
#-- have to link the same libraries multiple times because for the interdependency
LIBS = -lGLU -lGL -lX11 -lXext -lglfw3 -lGLEW -lGLU -lGL -lGLU -lXrandr -lXext -lX11 -lrt -ldl -lpthread -lXrandr -lXi -lXxf86vm -lrt -lm -lGL -lX11 -lXext 

all: waves

waves: waves.o ArcBall.o ../common/shader.o
	@$(CC) -o $@ $^ $(LFLAGS) $(LPATH) $(LIBS)
	
.cpp.o: 
	@$(CC) $(CDEFS) $(CFLAGS) -c -o $@ $^ $(IPATH)
	
clean:
	@$(RM) -f *.o *.gch waves



