#include "ig-aux.h"
#include "tuplasg.h"
#include "practicas.h"
#include "camara.h"
#include "fbo.h"
#include "seleccion.h"

// framebuffer object usado para selección
static Framebuffer * fbo = nullptr ;

// ---------------------------------------------------------------------

Framebuffer * FBOActual()
{
   return fbo ;
}

// ---------------------------------------------------------------------
// fijar el color actual en el cauce usando un identificador entero

void FijarColVertsIdent( Cauce & cauce, const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (glColor3ub)
   // .....

}

// ----------------------------------------------------------------------------------
// leer un identificador entero codificado en el color de un pixel en el
// framebuffer activo actualmente

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   // .....(sustituir el 'return 0' por lo que corresponda)
   // .....

   return 0 ;

}

// -------------------------------------------------------------------------------
// Función principal de selección, se llama al hacer click con el botón izquierdo
//
// (x,y)       = posición donde se ha hecho click en coordenadas del sistema de ventanas (enteras)
// objeto_raiz = objeto raiz (no subobjeto de otro) que se está visualizando en la ventana
// cv_dib      = contexto de visualización usado para dibujar el objeto
//
// devuelve: true si se ha seleccionado algún objeto, false si no

bool Seleccion( int x, int y, Escena * escena, ContextoVis & cv_dib )
{
   using namespace std ;
   assert( escena != nullptr );

   // COMPLETAR: práctica 5:
   // Visualizar escena en modo selección y leer el color del pixel en (x,y)
   // Se deben de dar estos pasos:

   //cout << "Seleccion( x == " << x << ", y == " << y << ", obj.raíz ==  " << objeto_raiz.leerNombre() << " )" << endl ;

   // 1. Crear (si es necesario) y activar el framebuffer object (fbo) de selección
   // .........


   // 2. crear un 'ContextoVis' apropiado, en ese objeto:
   //    * activar modo selecion, desactivar iluminación, poner modo relleno
   //    * usar el mismo cauce, y la misma cámara que en 'cv_dib'
   //    * fijar el tamaño de la ventana igual que en 'cv_dib'
   //
   // ..........


   // 3. Activar fbo, cauce y viewport. Configurar cauce (modo solido relleno, sin ilum.
   //    ni texturas). Limpiar el FBO (color de fondo: 0)
   // .......


   // 4. Activar la cámara (se debe leer de la escena con 'camaraActual')
   // ....


   // 5. Visualizar el objeto raiz actual (se debe leer de la escena con 'objetoActual()')
   // ........


   // 6. Leer el color del pixel (usar 'LeerIdentEnPixel')
   // (hay que hacerlo mientras está activado el framebuffer de selección)
   // .....


   // 7. Desactivar el framebuffer de selección
   // .....


   // 8. Si el identificador del pixel es 0, imprimir mensaje y terminar (devolver 'false')
   // ....


   // 9. Buscar el objeto en el objeto_raiz (puede ser un grafo de escena)
   //    e informar del nombre del mismo (si no se encuentra, indicarlo)
   //   (usar 'buscarObjeto')
   // .....


   // al final devolvemos 'true', ya que hemos encontrado un objeto
   return true ;
}
