// *********************************************************************
// **
// ** Conjunto de funciones auxiliares
// ** Copyright (C) 2014 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#ifndef AUX_HPP
#define AUX_HPP

// includes de la librería estandard y otras

#include <cctype>     // toupper
#include <algorithm>  // std::min y std::max
#include <cassert>    // assert
#include <cctype>     // toupper
#include <cstdlib>
#include <cmath>      // std::sin, std::cos, etc...
#include <fstream>    // std::ifstream
#include <iostream>   // std::cout
#include <iomanip>    // std::setprecision, std::fixed, etc....
#include <map>        // std::map<K,V>
#include <string>     // std::string
#include <vector>     // std::vector
#include <chrono>     // std::chrono::time_point, std::chrono::duration (para medir FPS)

// includes de OpenGL/Glut/Glew/Glu (dependen del S.O.)
#ifdef LINUX
//#pragma message "Usando includes para LINUX"
#include <GL/glew.h>   // nota: define GLEW_OK
#include <GLFW/glfw3.h>  // glfw (incluye OpenGL?)
#include <GL/glu.h>
#else
#ifdef OSX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLFW/glfw3.h>  // glfw
#else
#ifdef WINDOWS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <jpeglib.h>
#else
#pragma error "No está definido el símbolo 'OSX' ni 'LINUX', ni 'WINDOWS'."
#endif
#endif
#endif

// por si acaso ...(en windows se usa)
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751
#endif

// en windows, si no está definida __PRETTY_FUNCTION__, definirla:
#ifdef WINDOWS
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__ // https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=vs-2019
#endif
#endif


// ignora advertencias por uso de funciones obsoletas (sirve en OSX)
#ifdef OSX
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif


#ifdef OSX  // los VAO están disponibles como una extensión en macOS con OpenGL 2.1
#define glGenVertexArrays     glGenVertexArraysAPPLE
#define glBindVertexArray     glBindVertexArrayAPPLE
#define glDeleteVertexArrays  glDeleteVertexArraysAPPLE
#endif

// includes en 'recursos/include'
#include <lector-jpg.h>
#include <fbo.h>
#include <cauce.h>

// ---------------------------------------------------------------------
// número aleatorio entre 0 y 1
inline float alea01() { return float(std::rand())/float(RAND_MAX); }

// ---------------------------------------------------------------------
// traza la ejecución
#define traza() { std::cout << "archivo: " << __FILE__ << ", linea: " << __LINE__ << std::endl << std:: flush ;}

// ---------------------------------------------------------------------
// comprobación de errores de OpenGL
#define CError() CompruebaErrorOpenGL(__FILE__,__LINE__)
void CompruebaErrorOpenGL( const char * nomArchivo, int linea ) ;

// ---------------------------------------------------------------------
// gestion de un error
#define Error(msg)   GestionarError(msg,__FILE__,__LINE__)
void GestionarError( const char * msg, const char * nomArchivo, int linea ) ;

// ---------------------------------------------------------------------
// true solo si el rendering context activo soporta al menos una versión de openGL
bool SoportaGL( const int min_major, const int min_minor ) ;

// ---------------------------------------------------------------------
// true solo si el rendering context activo soporta al menos una versión de GLSL
bool SoportaGLSL( const int min_major, const int min_minor ) ;

//----------------------------------------------------------------------
// aborta si no se soporta una versión de OpenGL
void ExigirVersionGL( const int min_major, const int min_minor, const char * msg ) ;

//----------------------------------------------------------------------
// comprobar si está GLEW, si está inicializarlo, en otro caso error y aborta
void Inicializa_GLEW() ;

// ----------------------------------------------------------------------------
// fijar el icono que se usa para la ventana GLFW, usando imagen UGR
void FijarIconoVentanaGLFW( GLFWwindow * glfw_window ) ;

// ---------------------------------------------------------------------
// GLFW: especificar 'hints' para inicialización del rendering
// context de la ventana GLFW (se puede pedir uOpenGL 4 aquí)
void ConfigurarGLFW() ;

// ----------------------------------------------------------------------------------
// calcula posicion y tamaño de la ventana GLFW en función de la resolucion del escritorio
void TamPosVentanaGLFW( int & tamx, int & tamy, int & posx, int & posy );

// ----------------------------------------------------------------------------
// función que gestiona un error en GLFW, se fija con 'glfwSetErrorCallback'
// se imprimer código, mensaje, y se aborta
void ErrorGLFW( int codigo, const char * mensaje );

// ---------------------------------------------------------------------
// escribe en stdout características de opengl
void InformeOpenGL() ;

// ---------------------------------------------------------------------
// devuelve 'true' si está disponible el buffer de acumulación para
// usar super-sampling de 'fuerza bruta'
bool BufferAcumulacionDisponible();

// ---------------------------------------------------------------------
// crear una ventana glut centrada en el escritorio
void CrearVentanaGLUT( const char * titulo, const  float fr_alto ) ;

// ---------------------------------------------------------------------
// quita el path de un nombre de archivo con path
std::string QuitarPath( const std::string & path_arch ) ;

// ----------------------------------------------------------------------------
// funciones auxiliares para dibujar los ejes

void CrearObjetoCuadrica() ;
void DibujarCono( GLdouble base, GLdouble height, GLdouble slices, GLdouble stacks );
void DibujarEsfera( GLdouble radius, GLdouble slices, GLdouble stacks ) ;
void DibujarCilindro( GLdouble radius, GLdouble height, int slices, int stacks );
void DibujarEjeZ( Cauce & cauce );
void DibujarEjesSolido( Cauce & cauce );
void DibujarEjesLineas( Cauce & cauce );

// -----------------------------------------------------------------------------
// imprimie en 'cout' una línea con información de la frecuencia con la que se llama
// a la función

void ImprimirFPS();

// -----------------------------------------------------------------------------
// dibujar una cruz
void DibujarCruz( Cauce & cauce, const float d );

// -----------------------------------------------------------------------------
// dibuja un FBO en un rectángulo en la parte superior izquierda
void DibujarFBO( Cauce & cauce, Framebuffer & fbo );



#endif // AUX_HPP
