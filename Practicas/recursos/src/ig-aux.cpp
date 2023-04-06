// *********************************************************************
// **
// ** Rutinas auxiliares:
// ** - Gestión de errores en general
// ** - Comprobación de errores de OpenGL (implementacion)
// ** - Inicialización de glew,
// ** - Comprobacion de la versión de OpenGL
// **
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

#include "ig-aux.h"

// *********************************************************************
// gestion de errores

void GestionarError( const char * msg, const char * nomArchivo, int linea )
{
   std::cout
         << std::endl
         << "-----------------------------------------------------------" << std::endl
         << "Error detectado:" << std::endl
         << "   descripción : " << msg  << "." << std::endl
         << "   archivo     : " << nomArchivo << std::endl
         << "   línea       : " << linea << std::endl
         << "-----------------------------------------------------------" << std::endl
         << "programa abortado." << std:: endl
         << std::flush ;

    exit(1) ;
}

// -----------------------------------------------------------------------------
// códigos y descripciones de errores detectado por 'glGetError'

const unsigned numErrors = 5 ;


const GLenum errCodes[numErrors] =
{
   GL_NO_ERROR ,
   GL_INVALID_ENUM ,
   GL_INVALID_VALUE ,
   GL_INVALID_OPERATION ,
   //GL_INVALID_FRAMEBUFFER_OPERATION ,  // REVISAR (pq no está definido con glfw?)
   GL_OUT_OF_MEMORY
} ;

const char * errDescr[numErrors] =
{
   "Error when trying to report an error: no error has been recorded",
   "An unacceptable value is specified for an enumerated argument",
   "A numeric argument is out of range",
   "The specified operation is not allowed in the current state",
   //"The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete (i.e. the return value from 'glCheckFramebufferStatus' is not GL_FRAMEBUFFER_COMPLETE)",
   "There is not enough memory left to execute the command"
} ;


const char * errCodeString[numErrors] =
{
   "GL_NO_ERROR",
   "GL_INVALID_ENUM",
   "GL_INVALID_VALUE",
   "GL_INVALID_OPERATION",
   //"GL_INVALID_FRAMEBUFFER_OPERATION",
   "GL_OUT_OF_MEMORY"
} ;

// ---------------------------------------------------------------------
// devuelve descripción de error dado el código de error opengl

std::string ErrorDescr( GLenum errorCode )
{
   int iErr = -1 ;
   for ( unsigned i = 0 ; i < numErrors ; i++ )
   {  if ( errCodes[i] == errorCode)
      {  iErr = i ;
         break ;
      }
   }
   if ( iErr == -1 )
      return std::string("Error when trying to report an error: error code is not a valid error code for 'glGetError'") ;
   return std::string( errDescr[iErr] ) ;
}

std::string ErrorCodeString( GLenum errorCode )
{
   int iErr = -1 ;
   for ( unsigned i = 0 ; i < numErrors ; i++ )
   {  if ( errCodes[i] == errorCode)
      {  iErr = i ;
         break ;
      }
   }
   if ( iErr == -1 )
      return std::string("** invalid error code **") ;
   return std::string( errCodeString[iErr] ) ;
}

//----------------------------------------------------------------------

void CompruebaErrorOpenGL( const char * nomArchivo, int linea )
{
   const GLint codigoError = glGetError() ;

   if ( codigoError != GL_NO_ERROR )
   {  std::cout
         << std::endl
         << "Detectado error de OpenGL. Programa abortado." << std::endl
         << "   archivo (linea) : " << QuitarPath(nomArchivo) << " (" << linea << ")" << std::endl
         << "   código error    : " << ErrorCodeString( codigoError )  << std::endl
         << "   descripción     : " << ErrorDescr( codigoError )  << "." << std::endl
         << std::endl << std::flush ;
      exit(1);
   }
}

//----------------------------------------------------------------------

void CalculaVersion(  const char *str, int & major, int & minor )
{
   // podría hacerse así, pero las primeras versiones (1.5-) al parecer no tienen GL_MAJOR_VERSION.... :-(
      // glGetIntegerv( GL_MAJOR_VERSION, &major ) ;
      // glGetIntegerv( GL_MINOR_VERSION, &minor ) ;

   using namespace std ;
   const
      std::string ver(str) ;
   size_t //auto
      blanco = ver.find_first_of(' ') ;
   if ( blanco == string::npos )
      blanco = ver.length() ;

   const size_t //auto
      punto = ver.find_first_of('.') ;
   assert( punto != string::npos ); // salta si no encuentra el punto
   assert( punto+1 < blanco );

   const string
      smajor = ver.substr( 0, punto ),
      sminor = ver.substr( punto+1, blanco-punto-1 );

   major = atoi(smajor.c_str());
   minor = atoi(sminor.c_str());
}

// ---------------------------------------------------------------------
// quita el path de un nombre de archivo con path

std::string QuitarPath( const std::string & path_arch )
{
   //const size_t

   const size_t // auto
   	barra = path_arch.find_last_of('/') ;
   if ( barra == std::string::npos ) // no tiene '/' (devolver tal cual)
      return path_arch ;
   else
      return path_arch.substr( barra+1 );

}

// ---------------------------------------------------------------------
// devuelve true si se soporta al menos una versión de OpenGL, se
// proporciona el número de version dividido en "major" y "minor"

bool SoportaGL( const int min_major, const int min_minor )
{
   static GLint major,minor ;
   static bool primera = true ;

   if ( primera )
   {
      // parser de la cadena (GL_VERSION)
      using namespace std ;
      CalculaVersion( (const char *) glGetString(GL_VERSION), major, minor ) ;
      cout << "calculada version de OpenGL: " << major << "." << minor << " (" << glGetString(GL_VERSION) << ")" << endl << flush ;
      primera = false ;
   }

   if ( min_major < major )
      return true;
   if ( min_major == major )
      if ( min_minor <= minor )
         return true;
   return false ;
}


// ---------------------------------------------------------------------

bool SoportaGLSL( const int min_major, const int min_minor )
{
   static GLint major,minor ;
   static bool primera = true ;

   if ( primera )
   {
      // parser de la cadena (GL_VERSION)
      using namespace std ;
      CalculaVersion( (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION), major, minor ) ;
      cout << "calculada version de GLSL: " << major << "." << minor << " (" << glGetString(GL_SHADING_LANGUAGE_VERSION) << ")" << endl << flush ;
      primera = false ;
   }

   if ( min_major < major )
      return true;
   if ( min_major == major )
      if ( min_minor <= minor )
         return true;
   return false ;
}


//----------------------------------------------------------------------

void Inicializa_GLEW(  ) // necesario para Linux y resto de SS.OO.
{
#ifndef OSX
   static bool primera = true ;

   if ( ! primera )
      return ;
   primera = false ;

   using namespace std ;
#ifndef GLEW_OK
   Error("no se han incluido los headers de GLEW correctamente, usa '#include <ig-aux.hpp>' para incluir símbolos de OpenGL/GLFW/GLEW") ;
#else
   GLenum codigoError = glewInit();
   if ( codigoError != GLEW_OK ) // comprobar posibles errores
   {
      const std::string errmsg =
         std::string( "Imposible inicializar ’GLEW’, mensaje recibido: ") +
         std::string( (const char *)glewGetErrorString(codigoError) ) ;
      Error(errmsg.c_str()) ;
   }
   else
      cout << "librería GLEW inicializada correctamente." << endl << flush ;
#endif
#else
   // en macOS (símbolo OSX definido) no es necesario usar 'glew'
#endif
}
// ----------------------------------------------------------------------------
// función que gestiona un error en GLFW, se fija con 'glfwSetErrorCallback'

void ErrorGLFW( int codigo, const char * mensaje )
{
   using namespace std ;
   cout << "Error en la librería GLFW: " << endl
        << "   Código  : " << codigo << endl
        << "   Mensaje : " << mensaje << endl
        << "(programa abortado)" << endl ;
   exit(1);
}

// ---------------------------------------------------------------------

void InformeOpenGL(  )
{
   GLint acc_r, acc_g, acc_b ;
   glGetIntegerv( GL_ACCUM_RED_BITS,   &acc_r );
   glGetIntegerv( GL_ACCUM_GREEN_BITS, &acc_g );
   glGetIntegerv( GL_ACCUM_BLUE_BITS,  &acc_b );

   using namespace std ;
   cout  << "datos de versión e implementación de OpenGL" << endl
         << "    implementación de : " << glGetString(GL_VENDOR)  << endl
         << "    hardware          : " << glGetString(GL_RENDERER) << endl
         << "    version de OpenGL : " << glGetString(GL_VERSION) << endl
         << "    version de GLSL   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl
         << "    bits buff.accum   : " << "(" << acc_r << "," << acc_g << "," << acc_b << ")" << endl
         << "    raw antialiasing  : " << (BufferAcumulacionDisponible() ? "disponible" : "no disponible") << endl
         << flush ;
}
// ----------------------------------------------------------------------------

bool BufferAcumulacionDisponible()
{
   // comprobar si hay o no hay disponible buffer de acumulación
   // (a pesar de pedirse al abrir ventana, como es un 'hint', podría no haberse obtenido)

   GLint nb_accum_r, nb_accum_g, nb_accum_b ;

   glGetIntegerv( GL_ACCUM_RED_BITS,   &nb_accum_r  );
   glGetIntegerv( GL_ACCUM_GREEN_BITS, &nb_accum_g  );
   glGetIntegerv( GL_ACCUM_BLUE_BITS,  &nb_accum_b  );

   return 4 <= nb_accum_r && 4 <= nb_accum_g && 4 <= nb_accum_b ;
}
// ---------------------------------------------------------------------
// GLFW: especificar 'hints' para inicialización del rendering
// context de la ventana GLFW (se puede pedir OpenGL 4 aquí)

void ConfigurarGLFW()
{

#ifdef OSX
   // se podría activar este código para pedir un rendering context 'core'
   // (no tiene compatibilidad con OpenGL 2.1)

   constexpr bool pedir_opengl_4 = false ;
   if ( pedir_opengl_4 )
   {
      glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
      glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
      glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
      glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#ifdef GLFW_COCOA_RETINA_FRAMEBUFFER
      glfwWindowHint( GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE );
#endif
   }
#endif

   // decirle a GLFW que abra la ventana con un buffer de acumulación (para antialiasing)
   glfwWindowHint( GLFW_ACCUM_RED_BITS, 32 );
   glfwWindowHint( GLFW_ACCUM_GREEN_BITS, 32 );
   glfwWindowHint( GLFW_ACCUM_BLUE_BITS, 32 );
   glfwWindowHint( GLFW_ACCUM_ALPHA_BITS, 32 );

   // imprimir info. sobre la versión de GLFW en uso:
   using namespace std ;
   cout << "Versión de GLFW: " << glfwGetVersionString() << endl ;
}

// ----------------------------------------------------------------------------------
// calcula posicion y tamaño de la ventana en función de la resolucion del escritorio

void TamPosVentanaGLFW( int & tamx, int & tamy, int & posx, int & posy )
{
   // leer el tamaño del escritorio para posicionar y dimensionar la ventana
   const GLFWvidmode *
      modo      = glfwGetVideoMode( glfwGetPrimaryMonitor( ) ); //modo act. del mon. primario.
   const int
      ancho_tot = modo->width,     // ancho total del escritorio en el modo actual
      alto_tot  = modo->height ;    // alto total del escritorio en el modo actual

   tamx  = (alto_tot*4)/5 ;     // ancho de la ventana
   tamy  = tamx ;               // alto de la ventana
   posx  = (ancho_tot-tamx)/2 ; // posicion X de la ventana
   posy  = (alto_tot-tamy)/2;   // posición Y de la ventana
}

// ----------------------------------------------------------------------------
// fijar el icono que se usa para la ventana GLFW, usando imagen UGR
// * En macOS no hace nada (no se puede)
// * Se evita intentarlo en Windows (aborta), y en versiones de GLFW
//   previas a la 3.2 (no está definido 'glfwSetWindowIcon')

void FijarIconoVentanaGLFW( GLFWwindow * glfw_window )
{
   #ifndef WINDOWS
   #if GLFW_VERSION_MAJOR >= 3
   #if GLFW_VERSION_MINOR >= 2

   using namespace std ;
   assert( glfw_window != nullptr );

   unsigned ancho, alto ;
   unsigned char * img = LeerArchivoJPEG( "../recursos/imgs/window-icon.jpg", ancho, alto );
   assert( img != nullptr );
   assert( 0 < ancho );
   assert( 0 < alto );

   static GLFWimage glfw_img  ;
   constexpr unsigned long nc = 4 ;
   //cout << "icono, ancho = " << ancho << ", alto == " << alto << endl ;

   glfw_img.width = ancho ;
   glfw_img.height = alto ;
   glfw_img.pixels = new unsigned char [nc*glfw_img.width*glfw_img.height] ;

   unsigned char * pwrite = glfw_img.pixels ;

   for( int iy = 0 ; iy < glfw_img.height ; iy++ )
   for( int ix = 0 ; ix < glfw_img.width ; ix++ )
   {
      unsigned char * pread = img + ((3*ancho*iy) + ix) ;
      //const unsigned char vwrite = ( pread[0] != 255 || pread[1] != 255 || pread[2] != 255 ) ? 255 : 0 ;
      for( unsigned i = 0 ; i < nc-1 ; i++ )
         pwrite[i] = pread[i] ;
      pwrite[nc-1] = 255 ;
      pwrite += nc ;
   }
   glfwSetWindowIcon( glfw_window, 1, &glfw_img  );

   delete [] glfw_img.pixels ;
   delete [] img ;

   #endif
   #endif
   #endif
}

// *****************************************************************************
// funciones para dibujar los ejes, con conos, cilindros y esferas:

// ----------------------------------------------------------------------------
// declaración e inicialización del objeto tipo cuadrica ('qu_ptr')

static GLUquadric * qu_ptr = nullptr ; // objeto quadrica (para glusphere y glucone)

void CrearObjetoCuadrica()
{
   // generar el objeto 'cuadrica' (una sola vez)
   if ( qu_ptr == nullptr )
      qu_ptr = gluNewQuadric() ;

   assert( qu_ptr != nullptr );
}
// ----------------------------------------------------------------------------
// dibuja un cono cuyo eje es el eje Z y el disco de la base tiene centro en el origen
// 'base' es el radio de la base, y 'height' es la altura (en el eje Z)

void DibujarCono( GLdouble base, GLdouble height, GLdouble slices, GLdouble stacks )
{
   CrearObjetoCuadrica();
   gluCylinder( qu_ptr, base, 0.0, height, slices, stacks );
}
// ----------------------------------------------------------------------------
// dibuja una esfera, se proporciona el radio, el número de meridianos (slices)
// y el de paralelos (stacks)

void DibujarEsfera( GLdouble radius, GLdouble slices, GLdouble stacks )
{
   CrearObjetoCuadrica();
   gluSphere( qu_ptr, radius, slices, stacks );

}

// ----------------------------------------------------------------------------
// dibuja un cilindro, en el eje Z, entre Z=0 y Z=1, 'radius' es el radius

void DibujarCilindro( GLdouble radius, GLdouble height, int slices, int stacks )
{
   CrearObjetoCuadrica();
   gluCylinder( qu_ptr, radius, radius, height, slices, stacks );
}
// ----------------------------------------------------------------------------
// dibuja el eje Z

void DibujarEjeZ( Cauce & cauce )
{
   const GLfloat
   	zmed     = 0.93,  // z fin del cilindro
   	rad_fle  = 0.023 ;  // radio del cono

   DibujarCilindro( rad_fle*0.3, zmed, 16,8 );

   cauce.pushMM() ;
      cauce.compMM( MAT_Traslacion( 0.0, 0.0, zmed ) );
      DibujarCono( rad_fle, 1.0-zmed, 16,16 );
   cauce.popMM();


   glBegin(GL_LINES);
   	glVertex3f( 0.0, 0.0, -50.0 );
   	glVertex3f( 0.0, 0.0, +50.0 );
   glEnd();
}

//----------------------------------------------------------------------

void DibujarEjesSolido( Cauce & cauce )
{
   PilaMatriz4f pila_m ;

   cauce.fijarEvalMIL( false );
   cauce.fijarEvalText( false );
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glShadeModel( GL_FLAT );

   // eje X, color rojo
   glColor3f( 1.0, 0.3, 0.3 );
   cauce.pushMM();
      cauce.compMM( MAT_Rotacion( 90.0, 0.0,1.0,0.0 ) );
      DibujarEjeZ( cauce ) ;
   cauce.popMM();

   // eje Y, color verde
   glColor3f( 0.3, 1.0, 0.3 );
   cauce.pushMM();
      cauce.compMM( MAT_Rotacion( -90.0, 1.0, 0.0, 0.0 ) );
      DibujarEjeZ( cauce ) ;
   cauce.popMM();

   // eje Z, color azul
   glColor3f( 0.7, 0.9, 1.0 );
   DibujarEjeZ( cauce ) ;

   // bola en el origen, negra
   glColor3f( 0.0, 0.0, 0.0 );
   DibujarEsfera( 0.04, 16, 8 );
}

// ---------------------------------------------------------------------
// dibuja los ejes utilizando la primitiva grafica de lineas

void DibujarEjesLineas( Cauce & cauce )
{
   const float long_ejes = 300.0 ;

   // establecer modo de dibujo a lineas (podría estar en puntos)
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

   // dibujar tres segmentos
   glBegin(GL_LINES);
      // eje X, color rojo
      glColor3f( 1.0, 0.0, 0.0 );
      glVertex3f( -long_ejes, 0.0, 0.0 );
      glVertex3f( +long_ejes, 0.0, 0.0 );
      // eje Y, color verde
      glColor3f( 0.0, 1.0, 0.0 );
      glVertex3f( 0.0, -long_ejes, 0.0 );
      glVertex3f( 0.0, +long_ejes, 0.0 );
      // eje Z, color azul
      glColor3f( 0.0, 0.0, 1.0 );
      glVertex3f( 0.0, 0.0, -long_ejes );
      glVertex3f( 0.0, 0.0, +long_ejes );
   glEnd();

   // bola en el origen, negra
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glColor3f( 0.0, 0.0, 0.0 );
   DibujarEsfera( 0.01, 8, 8 );

}

// -----------------------------------------------------------------------------
// imprimie en 'cout' una línea con información de la frecuencia con la que se llama
// a la función

void ImprimirFPS()
{
   using namespace std ;
   using namespace std::chrono ;

   typedef duration<float,ratio<1,1>> DuracionSegs ;

   // leer instante final de las instrucciones
   constexpr int  num_cuadros = 20 ;
   static int     count       = 0 ;
   static bool    primera_vez = true ;
   static time_point<steady_clock> fin_cuadro_anterior ;

   // saltar 'num_cuadros' veces la impresión
   if ( count % num_cuadros != 0 )
   {
      count++ ;
      return ;
   }

   // registrar instante actual
   time_point<steady_clock> fin_cuadro_actual = steady_clock::now() ;


   // si no es la primera vez, calcular e imprimir
   if ( ! primera_vez )
   {
      const float segundos_cuadro = (DuracionSegs( fin_cuadro_actual - fin_cuadro_anterior )).count()/float(num_cuadros) ,
                  fps             = 1.0/segundos_cuadro ;

      // guardar flags de formato de 'cout'
      ios estado_anterior( nullptr );
      estado_anterior.copyfmt( cout );

      // imprimir cambiando los flags
      cout << fixed << setprecision( 1 )
           << "milisegundos = " << setw(5) <<  (segundos_cuadro*1000.0)
           << " (FPS = " << setw(5) << fps << ")."
           << defaultfloat << endl ;

      // restaurar formato de 'cout'
      cout.copyfmt( estado_anterior );
   }
   count ++ ;
   primera_vez = false ;
   fin_cuadro_anterior = fin_cuadro_actual ;
}
// ----------------------------------------------------------------------------
// dibujar una cruz blanca en el centro del viewport, deja el estado de OpenGL fatal

void DibujarCruz( Cauce & cauce, const float d )
{
   glColor3f( 0.0, 0.0, 0.0 );
   cauce.fijarEvalMIL( false );
   cauce.fijarEvalText( false );
   cauce.fijarMatrizProyeccion( MAT_Ident() );
   cauce.fijarMatrizVista( MAT_Ident() );

   glDisable( GL_DEPTH_TEST );
   glBegin( GL_LINES );
      glVertex3f( -d, 0.0, 0.0 );
      glVertex3f( +d, 0.0, 0.0 );
      glVertex3f( 0.0, -d, 0.0 );
      glVertex3f( 0.0, +d, 0.0 );
   glEnd();
   glEnable( GL_DEPTH_TEST );
   CError();
}

// ----------------------------------------------------------------------------
//
void DibujarFBO( Cauce & cauce, Framebuffer & fbo )
{
   //cout << "dibujando cuadro de textura..." << endl ;
   GLuint textId = fbo.leerTextId();

   // ponemos el color a blanco para que al multiplicar por textura salga la textura tal cual.
   glColor4f( 1.0, 1.0, 1.0, 1.0 );

   cauce.fijarMatrizProyeccion( MAT_Ident() );
   cauce.fijarMatrizVista( MAT_Ident() );
   cauce.fijarEvalMIL( false );
   cauce.fijarEvalText( true, textId );
   cauce.fijarTipoGCT( 0 );
   CError();

   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glShadeModel( GL_FLAT );
   glDisable( GL_CULL_FACE );
   glDisable( GL_DEPTH_TEST );
   CError();

   glBegin( GL_QUADS );
         glTexCoord2f( 0.0, 0.0 ); glVertex3f( -0.95, +0.6, 0.0 );
         glTexCoord2f( 1.0, 0.0 ); glVertex3f( -0.6,  +0.6, 0.0 );
         glTexCoord2f( 1.0, 1.0 ); glVertex3f( -0.6,  +0.95, 0.0 );
         glTexCoord2f( 0.0, 1.0 ); glVertex3f( -0.95, +0.95, 0.0 );
   glEnd();
   CError();
   //cout << "hecho." << endl ;

   glEnable( GL_DEPTH_TEST );
}
