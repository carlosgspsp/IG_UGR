// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Carlos Ureña Almagro
// **
// ** Función 'main', inicialización y gestores de eventos
// **
// *********************************************************************


#include "ig-aux.h"  // include cabeceras de opengl / glut / glut / glew
#include "camara.h"
#include "practicas.h"
#include "seleccion.h"
#include "animacion.h"


// evita la necesidad de escribir std::
using namespace std ;

// *********************************************************************
// **
// ** Variables y constantes globales (contienen el estado de la aplicación)
// **
// *********************************************************************

int                               //
   practica_actual     = 1 ,      // practica actual (cambiable por teclado) (1,2,3,4 o 5)
   mouse_pos_factor    = 1 ,      // factor de conversión para displays "retina" en macOS
   x_ant_mabd,                    // coord. X anterior del ratón en modo arrastrar con botón derecho pulsado
   y_ant_mabd,                    // coord. Y anterior del ratón en modo arrastrar con botón derecho pulsado
   ind_cauce_act       = -1 ;     // índice del cauce activo actual (-1 si aun no se han creado los cauces)
unsigned                          //
   ind_escena_act = 0  ;          // índice de la escena actual en el vector de escenas
bool                              //
   revisualizar_escena = true,    // usado para indicar que hay que redibujar la ventana antes de procesar los siguientes eventos
   terminar_programa   = false,   // puesto a 'true' para salir
   imprimir_tiempos    = false;   // 'true' imprimir tiempo por frame, 'false', no imprimir
GLFWwindow *                      //
   ventana_glfw        = nullptr; // puntero a la ventana GLFW que está usando la aplicación
ContextoVis                       // contexto de visualización actual
   cv ;                           // (incluye parámetros de visualización)
std::vector<Cauce *>              // vector de punteros a los cauces gráficos disponibles
   cauces ;                       // (0=fijo, 1=programable), inicialmente se usa el 1
std::vector<Escena *>             //
   escenas ;                      // vector con las distintas escenas que gestiona la aplicación

// ---------------------------------------------------------------------
// Iniciaización de OpenGL (se hace justo antes de cada cuadro o 'frame')
// esta inicialización es necesaria en cada cuadro puesto que en algunos
// modos (especialmente el modo selección) modifican el estado de OpenGL de
// diversas formas

void InicializaOpenGLCuadro()
{
   glEnable( GL_DEPTH_TEST );   // habilitar test de comparación de profundidades para 3D (y 2D)
                                // (no está por defecto: https://www.opengl.org/wiki/Depth_Buffer)
   glDisable( GL_CULL_FACE );   // deshabilitar filtrado de triangulos por su orientación:

   glColor3f( 0.7, 0.2, 0.4 ) ; // establecer color inicial para todas las primitivas
   glLineWidth( 1.7 );          // establecer ancho de línea
   glPointSize( 2.3  );         // establecer tamaño de los puntos
   CError();

   // establecer color de fondo

      glClearColor( 0.2, 0.25, 0.3, 1.0 );

   // dibujar en modo 'suave' (antialiasing de lineas y puntos)
   
   if ( cv.suavizar_aristas )
   {
      glEnable( GL_POINT_SMOOTH );
      glEnable( GL_LINE_SMOOTH );
      glEnable( GL_POLYGON_SMOOTH );

      // habilitar 'blending' (tiene en cuenta transparencia, componente alpha del color)
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      CError();
   }
   else
   {
      glDisable( GL_POINT_SMOOTH );
      glDisable( GL_LINE_SMOOTH );
      glDisable( GL_POLYGON_SMOOTH );
      glDisable( GL_BLEND );
   }

   // para dibujar los polígonos rellenos más atrás que las aristas
   glEnable( GL_POLYGON_OFFSET_FILL );
   glPolygonOffset( 1.0, 1.0 );
   CError();
}

// ---------------------------------------------------------------------
// función que visualiza un frame o cuadro

void VisualizarEscena()
{
   using namespace std ;

   // inicialización de OpenGL (por cuadro)
   InicializaOpenGLCuadro();

   // limpiar la ventana
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // visualizar la escena actual
   escenas[ind_escena_act]->visualizarGL( cv );

   // visualizar en pantalla el buffer trasero (donde se han dibujado las primitivas)
   glfwSwapBuffers( ventana_glfw );

   // si queremos imprimir los tiempos por cuadro, hacerlo.
   if ( imprimir_tiempos )
      ImprimirFPS();
}
// ---------------------------------------------------------------------
// F.G. del evento de cambio de tamaño de la ventana

void FGE_CambioTamano( GLFWwindow* ventana_glfw, int nuevo_ancho, int nuevo_alto )
{
   // actualizar 'ventana_tam_x' y 'ventana_tam_y' en 'cv'
   int fbx =0, fby=0 ;
   glfwGetFramebufferSize( ventana_glfw, &fbx, &fby );
   cv.ventana_tam_x  = fbx > 0 ? fbx : nuevo_ancho ;
   cv.ventana_tam_y  = fby > 0 ? fby : nuevo_alto ;
   // forzar un nuevo evento de redibujado, para actualizar ventana
   revisualizar_escena = true ;
}
// ---------------------------------------------------------------------
// F.G. del evento de pulsar o levantar una tecla

void FGE_PulsarLevantarTecla( GLFWwindow* window, int key, int scancode, int action, int mods )
{
   using namespace std ;

   if ( action == GLFW_PRESS ) // solo estamos interesados en el evento de levantar una tecla
      return ;                 // (ignoramos el evento generado al pulsar)

   bool redib = true ; // true sii al final de esta función es necesario redibujar

   // recuperar y comprobar camara y objeto actual
   CamaraInteractiva * camara = escenas[ind_escena_act]->camaraActual(); assert( camara != nullptr );
   Objeto3D *          objeto = escenas[ind_escena_act]->objetoActual(); assert( objeto != nullptr );

   // si está pulsada la tecla 'L', actualizar la colección de fuentes de luz y terminar
   if ( glfwGetKey( window, GLFW_KEY_L) == GLFW_PRESS )
   {
      // COMPLETAR: Práctica 4: procesar la tecla 'key' para actualizar la colección de fuentes
      // de luz actual (usar método 'colFuentes' de la escena activa para obtener un puntero), llamar a
      // 'ProcesaTeclaFuenteLuz', si devuelve 'true', forzar revisualizar escena.
      // .....

      return ; // finalizar la f.g.e, ya que si está la tecla L pulsada no se mira ninguna otra tecla.
   }

   // si está pulsada la tecla 'A', la tecla se procesa en 'animacion.cpp'
   // actua sobre el objeto que se está visualizando
   if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
   {
      // COMPLETAR: Práctica 3: procesar la tecla 'key' para actualizar estado de animación
      // del objeto actual ('objeto'), se debe usar 'ProcesarTeclaAnimacion' si devuelve
      // 'true', forzar revisualizar escena (asignando valor a 'revisualizar_escena')

      return ; // finalizar la f.g.e, ya que si está la tecla A pulsada no se mira ninguna otra tecla.
   }

   constexpr float
      cam_d_incre_tecla   = 1.0,     // incremento de 'cam_d' por teclado (debe ser >= 1.0)
      cam_ab_incre_tecla  = 3.0;     // incremento de ángulos por teclado

   switch ( key )
   {
      // teclas de camara :

      case GLFW_KEY_LEFT:
         camara->desplRotarXY( +cam_ab_incre_tecla, 0.0 );
         break;

      case GLFW_KEY_RIGHT:
         camara->desplRotarXY( -cam_ab_incre_tecla, 0.0 );
         break;

      case GLFW_KEY_UP:
         camara->desplRotarXY( 0.0, +cam_ab_incre_tecla  );
         break;

      case GLFW_KEY_DOWN:
         camara->desplRotarXY( 0.0, -cam_ab_incre_tecla );
         break;

      case GLFW_KEY_KP_SUBTRACT :  // tecla '-' en el teclado numérico
         camara->moverZ( +cam_d_incre_tecla );
         break;

      case GLFW_KEY_KP_ADD :        // tecla '+' en el teclado numérico ¿?
         camara->moverZ( -cam_d_incre_tecla );
         break;

      case GLFW_KEY_C :
         camara->siguienteModo();
         break ;

      case GLFW_KEY_V :
         escenas[ind_escena_act]->siguienteCamara() ;
         break ;

      // teclas para cambiar de escena y de objeto dentro de la escena

      case GLFW_KEY_O :
         escenas[ind_escena_act]->siguienteObjeto() ;
         break ;

      case GLFW_KEY_P :
         assert( ind_escena_act < escenas.size());
         ind_escena_act = (ind_escena_act+1) % escenas.size();
         cout << "Escena actual cambiada a: " << (ind_escena_act+1) << endl << flush ;
         break ;


      // tecla para terminar
      case GLFW_KEY_ESCAPE:
      case GLFW_KEY_Q:
         terminar_programa = true ;
         break ;

      // teclas variadas:

      case GLFW_KEY_E:
         cv.dibujar_ejes = ! cv.dibujar_ejes ;
         cout << "Dibujar ejes: " << (cv.dibujar_ejes? "activado." : "desactivado.") << endl << flush ;
         break ;

      case GLFW_KEY_N :
         cv.visualizar_normales = ! cv.visualizar_normales ;
         cout << "Visualizar normales: " << (cv.visualizar_normales ? "activado." : "desactivado.") << endl << flush ;
         break ;

      case GLFW_KEY_M :
         cv.modo_visu = ModosVisu( (int(cv.modo_visu)+1) % int(ModosVisu::num_modos) );
         cout << "Modo de visualización cambiado a: '" << nombreModoVisu[int(cv.modo_visu)] << "'" << endl << flush ;
         break ;

      case GLFW_KEY_S :
         assert( 0 <= ind_cauce_act && ind_cauce_act < int(cauces.size()) );
         ind_cauce_act = (ind_cauce_act+1) % cauces.size() ;
         cv.cauce_act  = cauces[ind_cauce_act] ;
         assert( cv.cauce_act != nullptr );
         cout << "Activado cauce " << cv.cauce_act->descripcion() << " (" << ind_cauce_act+1 << " de " << cauces.size() << ")" << endl ;
         break ;

      case GLFW_KEY_W :
         cv.dibujar_aristas = ! cv.dibujar_aristas ;
         cout << "Dibujar aristas: " << (cv.dibujar_aristas? "activado" : "desactivado" ) << endl ;
         break ;

      case GLFW_KEY_B :
         cv.modo_envio = ModosEnvio( (int(cv.modo_envio)+1) % int(ModosEnvio::num_modos) );
         cout << "Modo de envío cambiado a: " << nombreModoEnvio[ int(cv.modo_envio) ] << endl << flush ;
         break ;

      case GLFW_KEY_I :
         cv.iluminacion = ! cv.iluminacion ;
         cout << "iluminación : " << (cv.iluminacion ? "activada" : "desactivada") << endl << flush ;
         break ;

      case GLFW_KEY_F :
         cv.sombr_plano = ! cv.sombr_plano ;
         cout << "modo de iluminación : " << (cv.sombr_plano ? "plano" : "smooth") << endl << flush ;
         break ;

      case GLFW_KEY_G :
         cv.suavizar_aristas = ! cv.suavizar_aristas ;
         cout << "suavizar_aristas : " << (cv.suavizar_aristas ? "activado" : "desactivado") << endl << flush ;
         break ;





      // COMPLETAR; Práctica 5. conmutar 'cv.visualizar_fbo' con la tecla 'Y'


      case GLFW_KEY_T :
         imprimir_tiempos = ! imprimir_tiempos ;
         cout << "imprimir tiempos : " << (imprimir_tiempos ? "activado" : "desactivado") << endl << flush ;
         break ;

      default:
         redib = false ; // para otras teclas, no es necesario redibujar
         break ;
   }
   // si se ha cambiado algo, forzar evento de redibujado
   if ( redib )
      revisualizar_escena = true ;
}
// --------------------------------------------------------------------
// función gestora del evento de hacer scroll
// (se registra con 'glfwSetScrollCallback'

void FGE_Scroll( GLFWwindow* window, double xoffset, double yoffset  )
{
   if ( fabs( yoffset) < 0.05 ) // si hay poco movimiento vertical, ignorar el evento
      return ;

   constexpr float cam_d_z_incre_scroll = 1.0 ;
   const     float direccion            = 0.0 < yoffset ? +1.0 : -1.0 ;

   CamaraInteractiva * camara = escenas[ind_escena_act]->camaraActual() ; assert( camara!= nullptr );

   camara->moverZ( direccion*cam_d_z_incre_scroll ) ;
   revisualizar_escena = true ;
}

// --------------------------------------------------------------------
// función gestora del evento de pulsar/levantar tecla del ratón
// (se registra con 'glfwSetMouseButtonCallback'

void FGE_PulsarLevantarBotonRaton( GLFWwindow* window, int button, int action, int mods )
{
   // solo estamos interesados en eventos de pulsar algún botón (no levantar)
   if ( action != GLFW_PRESS )
      return ;

   // leer la posición del puntero de ratón en x,y (enteros)
   double x_f,y_f ;
   glfwGetCursorPos( window, &x_f, &y_f );
   const int x = int(x_f), y = int(y_f);

   if ( button == GLFW_MOUSE_BUTTON_LEFT )
   {
      // pulsar botón izquierdo: selección
      if ( Seleccion( x, cv.ventana_tam_y - y, escenas[ind_escena_act], cv ) )
         revisualizar_escena = true ;
   }
   else if ( button == GLFW_MOUSE_BUTTON_RIGHT )
   {
      // pulsar botón derecho: inicio de modo arrastrar con botón derecho pulsado:
      x_ant_mabd = x ; // registrar posición de inicio en X
      y_ant_mabd = y ; // registrar posición de inicio en Y
      revisualizar_escena = true ;
   }
}

// --------------------------------------------------------------------
// función gestora del evento de ratón movido a una nueva posición
// (se registra con 'glfwSetCursorPosCallback'

void FGE_MovimientoRaton( GLFWwindow* window, double xpos, double ypos )
{
   // Ignorar evento si no está pulsado el botón derecho
   if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) != GLFW_PRESS )
      return ;

   // Estamos en modo arrastrar con el botón derecho pulsado:
   // Actualizar la cámara actual.

   constexpr float delta = 1.0; // incremento de ángulos con el ratón (por pixel)

   // calcular el desplazamiento en pixels en X y en Y respecto de la última posición
   const int
      x  = int( xpos ),
      y  = int( ypos ),
      dx = x - x_ant_mabd ,
      dy = y - y_ant_mabd ;

   // usar desplazamientos para desplazar/rotar la cámara actual en X y en Y
   CamaraInteractiva * camara = escenas[ind_escena_act]->camaraActual() ; assert( camara!= nullptr );
   camara->desplRotarXY( -dx*delta, dy*delta );

   // registrar (x,y) como la última posición del ratón
   x_ant_mabd = x ;
   y_ant_mabd = y ;

   // forzar que la escena se vuelva a visualizar
   revisualizar_escena = true ;

}
// ---------------------------------------------------------------------
// inicialización de GLFW: creación de la ventana, designar FGEs

void InicializaGLFW( int argc, char * argv[] )
{
   int fbx, fby, wx, wy, px, py ; // posicion y tamaño de la ventana y el framebuffer

   // inicializacion y configuracion de la librería GLFW:
   glfwSetErrorCallback( ErrorGLFW ); // fijar función llamada ante error (aborta, ver 'ig-aux.cpp')
   glfwInit() ;      // inicializacion de GLFW
   ConfigurarGLFW(); // especificar algunos parámetros de GLFW (ver 'ig-aux.cpp' en 'recursos')

   // crear y posicionar la ventana,
   TamPosVentanaGLFW( wx, wy, px, py ); // calcula pos. y tam., usando tam. escritorio (ver 'ig-aux.cpp')
   ventana_glfw = glfwCreateWindow( wx, wy, "Practicas IG GIM (19-20)", nullptr, nullptr ); // crea ventana
   assert( ventana_glfw != nullptr );
   glfwSetWindowPos( ventana_glfw, px, py ); // posiciona la ventana

   // establecer el 'rendering context' de la ventana como el 'context' actual
   glfwMakeContextCurrent( ventana_glfw );

   // fijar el icono de la ventana (en macOS no funciona) (ver 'aux.cpp')
   // (en linux/mac tampoco va bien, no dibuja bien el icono)

   FijarIconoVentanaGLFW( ventana_glfw );

   // leer el tamaño real actual de la ventana (ventana_tam_x/_y)
   // (en pantallas retina con macOS usamos el tamaño del framebuffer)
   glfwGetWindowSize( ventana_glfw, &wx, &wy );
   glfwGetFramebufferSize( ventana_glfw, &fbx, &fby );
   cv.ventana_tam_x    = fbx > 0 ? fbx : wx ;
   cv.ventana_tam_y    = fby > 0 ? fby : wy ;
   mouse_pos_factor = fbx>0 ? fbx/wx : 1 ;  // en displays macos retina, esto es 2
   cout << "Mouse pos. factor == " << mouse_pos_factor << endl ;

   // definir las diversas funciones gestoras de eventos que GLFW debe invocar
   // (lo hace en glfwPollEvents o en glfwWaitEvents)
   glfwSetWindowSizeCallback ( ventana_glfw, FGE_CambioTamano );
   glfwSetKeyCallback        ( ventana_glfw, FGE_PulsarLevantarTecla );
   glfwSetMouseButtonCallback( ventana_glfw, FGE_PulsarLevantarBotonRaton );
   glfwSetCursorPosCallback  ( ventana_glfw, FGE_MovimientoRaton );
   glfwSetScrollCallback     ( ventana_glfw, FGE_Scroll );
}



// ---------------------------------------------------------------------
// inicialización de OpenGL

void InicializaOpenGL( )
{
   CError();           // borrar posibles errores anteriores
   Inicializa_GLEW();  // ver 'aux.cpp' (dejar esta llamada siempre: en macOS no hace nada)
   InformeOpenGL() ;   // escribe características de OpenGL en pantalla

   InicializaOpenGLCuadro() ; // parte de la inicialización que luego se repite en cada cuadro

   // inicializar la lista de cauces gráficos
   // (se añade el cauce fijo y el cauce programable, se requiere que haya un
   //  rendering context ya creado, es decir, que esté creada la ventana GLFW)
   assert( cauces.size() == 0 );
   cauces.push_back( new CauceFijo() );
   cauces.push_back( new CauceProgramable() );
   ind_cauce_act = 1 ;
   cv.cauce_act = cauces[ind_cauce_act] ;
}

// ---------------------------------------------------------------------
// Código de inicialización (llama a los dos anteriores, entre otros)

void Inicializar( int argc, char *argv[] )
{
   // Inicializar GLFW (incluye crear la ventana)
   InicializaGLFW( argc, argv ) ;

   // inicializar el estado de openGL y el cauce gráfico, una vez
   // que se ha creado la ventana y se dispone de un rendering context activo
   InicializaOpenGL() ;

   // crear los objetos y las escenas que forman, insertar escenas en 'escenas'

   escenas.push_back( new Escena1() );

   // COMPLETAR: Prácticas 2,3,4 y 5
   // hacer 'push_back' en el vector de escenas de instancias de 'Escena2',
   // 'Escena3', etc..
   // ......


}

// ---------------------------------------------------------------------
// bucle principal  de gestion de eventos GLFW

void BucleEventosGLFW()
{
   terminar_programa = false ;
   revisualizar_escena = true ;

   while ( ! terminar_programa  )
   {
      if ( revisualizar_escena )      // si ha cambiado algo:
      {  VisualizarEscena();          //    dibujar la escena
         revisualizar_escena = false; //    evitar que se redibuje continuamente
      }

      // hay una animación en curso si están las animaciones activdas por el usuario y
      // además el objeto actual tiene al menos un parámetro animable
      Objeto3D * objeto = escenas[ind_escena_act]->objetoActual() ; assert( objeto != nullptr );
      const bool animacion_activa = AnimacionesActivadas() && 0 < objeto->leerNumParametros() ;

      if ( animacion_activa )         // si hay alguna animación en curso
      {  glfwPollEvents();            //    procesar todos los eventos pendientes, y llamar a la función correspondiente, si está definida
         if ( ! revisualizar_escena ) //    si no es necesario redibujar la ventana
            if ( ActualizarEstado( *objeto ) ) // actualiza estado, devuelve 'true' si 'objeto' tiene parámetros animables
                  revisualizar_escena = true ; // es necesario redibujar pq. el objeto se ha actualizado
      }                               //
      else                            // si no hay una animacion en curso
         glfwWaitEvents();            //    esperar hasta que haya un evento y llamar a la función correspondiente, si está definida

      // actualizar 'terminar_programa' si GLFW indica que se debe cerrar la ventana
      terminar_programa = terminar_programa || glfwWindowShouldClose( ventana_glfw ) ;
   }

}
// ---------------------------------------------------------------------
// punto de entrada al programa

int main( int argc, char *argv[] )
{
   using namespace std ;
   cout << "Prácticas de IG (GIM) - curso 2019-20." << endl ;

   Inicializar( argc, argv ) ; // inicializar las GLFW y OpenGL
   BucleEventosGLFW();         // ejecutar el bucle principal de gestión de eventos de GLFW
   glfwTerminate();            // finalizar la librería GLFW (no se va usar más)

   cout << "Programa terminado normalmente." << endl ;

   return 0;
}
