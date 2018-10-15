CXX			?= g++
CXXFLAGS	= -std=c++11 -c
BUILDDIR	?= build

EXTERNAL_SRCS	:=	png.cpp \
					rgbapixel.cpp
EXTERNAL_OBJS	:=	$(addprefix $(BUILDDIR)/external/, $(EXTERNAL_SRCS:%.cpp=%.o))


GEOMETRIC_SRCS	:=	GeometricObject.cc \
					Plane.cc \
					Sphere.cc \
					Triangle.cc
GEOMETRIC_OBJS	:=	$(addprefix $(BUILDDIR)/geometricObjects/, $(GEOMETRIC_SRCS:%.cc=%.o))

SAMPLERS_SRCS	:=	Sampler2D.cc \
					MultiJitteredSampler2D.cc
SAMPLERS_OBJS	:=	$(addprefix $(BUILDDIR)/samplers/, $(SAMPLERS_SRCS:%.cc=%.o))

SHADERS_SRCS	:=	Shader.cc
SHADERS_OBJS	:=	$(addprefix $(BUILDDIR)/shaders/, $(SHADERS_SRCS:%.cc=%.o))

UTILS_SRCS	:=	RGBColor.cc \
				ShadeRec.cc \
				Vec3.cc \
				io/loadMeshes.cc
UTILS_OBJS	:=	$(addprefix $(BUILDDIR)/utils/, $(UTILS_SRCS:%.cc=%.o))

CORE_SRCS	:=	Camera.cc \
				Light.cc \
				main.cc \
				Object.cc \
				Ray.cc \
				ViewPlane.cc \
				World.cc 
CORE_OBJS	:=	$(addprefix $(BUILDDIR)/, $(CORE_SRCS:%.cc=%.o))

.PHONY: all clean

all: $(BUILDDIR)/sample

clean:
	rm -rf $(BUILDDIR)

$(BUILDDIR)/sample : $(CORE_OBJS) $(UTILS_OBJS) $(SHADERS_OBJS) $(SAMPLERS_OBJS) $(GEOMETRIC_OBJS) $(EXTERNAL_OBJS)
	$(CXX) -lpng -o $@ $^

$(BUILDDIR)/%.o : %.c*
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $<
