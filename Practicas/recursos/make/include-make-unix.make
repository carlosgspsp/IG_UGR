########################################################################
##
## archivo 'include2.make'
## Carlos Ureña,
##
## modificado por última vez: Julio de 2019

## los paths sn todos relativos a la carpeta donde se ejecuta 'make',
## que es en '<dir_alu>/make', donde <dir_alu> es el directorio de trabajo del
## alumno (hermano de 'recursos')


## -------------------------------------------------------------------
##
## invalidar definiciones de 'pattern rules' predefinidias, definir targets

.SUFFIXES:
.PHONY: x, all, compile_all

## -------------------------------------------------------------------
## definir variables que dependen de los paths y unidades a compilar

recursos    := ../../recursos
dir_alu     := ..
target_name := pracs_ig_exe

target      := build/bin/$(target_name)
dir_objs    := build/objs
units_rec   := $(basename $(wildcard $(recursos)/src/*.cpp))
units_alu   := $(basename $(wildcard $(dir_alu)/src/*.cpp))

## -------------------------------------------------------------------
## unidades de compilación (.o a crear en 'objs')

objs_rec   := $(notdir $(units_rec))
objs_alu   := $(notdir $(units_alu))
objs       := $(addprefix $(dir_objs)/, $(addsuffix .o, $(objs_rec) $(objs_alu)))

## -------------------------------------------------------------------
# definiciones comunes

stdver    := -std=c++11
lib_jpg   := -ljpeg
lib_glfw  := -lglfw
incl_dirs := -I$(recursos)/include -I$(dir_alu)/include

## -------------------------------------------------------------------
## definiciones dependientes del SO

uname:=$(shell uname -s)

ifeq ($(uname),Darwin)
   os          := OSX
   lib_gl      := -framework OpenGL
   lib_glu     := /System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGLU.dylib
   lib_glew    :=
   depr        := -Wdeprecated-declarations
   comp        := clang++
else
    os          := LINUX
    lib_gl      := -lGL
    lib_glu     := -lGLU
    lib_glew    := -lGLEW
    depr        :=
    comp        := g++
endif

## -------------------------------------------------------------------
## flags enlazador (librerías)
ld_flags  := $(lib_dir_loc) $(lib_glfw)  $(lib_glew)  $(lib_glu) $(lib_gl) $(lib_jpg)

## -------------------------------------------------------------------
## flags compilador
c_flags  := $(stdver) $(incl_dirs)  -D$(os) $(opt_dbg_flag) $(exit_first) $(warn_all)


## -------------------------------------------------------------------
## auxiliares:

raya:=---------------------------------------------------------------

##
## funcion para compilar
## llamar con:
##     $(call compilar, $<, $@)

compilar =\
   echo `tput bold`$(raya);\
   echo Compilando: $(notdir $(1));\
   tput sgr0 ;\
   echo $(comp) $(c_flags) -c $(1) -o $(2) ;\
   $(comp) $(c_flags) -c $(1) -o $(2)

## -------------------------------------------------------------------
## targets

x: $(target)
	@echo "S.O.       :" $(os)
	@echo "Ejecutando :" $(target)
	cd .. ; ./make-unix/$(target)

all:
	make clean
	make compile_all

compile_all: $(target_name)

## -------------------------------------------------------------------
## enlazar

$(target) : $(objs)
	@echo `tput bold`$(raya)
	@echo "S.O.    : " $(os)
	@echo Enlazando: $(target_name) ....
	@tput sgr0
	$(comp) -o $(target) $(objs) $(ld_flags)
	@echo $(raya)

## -------------------------------------------------------------------
## compilar una unidad  (nota: aquí el directorio de objs esta 'hardcoded' !!)

## fuentes en src/
$(dir_objs)/%.o: $(dir_alu)/src/%.cpp
	@$(call compilar, $<, $@ )

## fuentes en ../recursos/src
$(dir_objs)/%.o: $(recursos)/src/%.cpp
	@$(call compilar, $<, $@ )

## -------------------------------------------------------------------
## limpiar

clean:
	rm -f $(dir_objs)/*.o $(target)
