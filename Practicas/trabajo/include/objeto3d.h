// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Informática Gráfica
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_OBJETO3D_HPP
#define IG_OBJETO3D_HPP

#include <string>          // usar std::string
#include "practicas.h"   // declaración de 'ContextoVis'


// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class Objeto3D
{

   public:
      // pone el identificador a '0', siempre
      Objeto3D() ;

      // visualizar el objeto con OpenGL, usando la configuración especificada en 'cv'
      virtual void visualizarGL( ContextoVis & cv ) = 0 ;



      // devuelve el nombre del objeto
      std::string leerNombre() ;

      // poner nombre al objeto
      void ponerNombre( const std::string & nuevoNombre );

      // devuelve el identificador del objeto:
      int leerIdentificador() ;

      // ponerle un identificador al objeto
      void ponerIdentificador( int nuevoIdent );

      // 'true' si el objeto tiene asignado un color, 'false' en otro caso
      // (por defecto, no tiene asignado color)
      bool tieneColor();

      // si 'tieneColor()==true', devuelve el color actual del objeto
      Tupla3f leerColor();

      // le asigna un color al objeto, desde la llamada, 'tieneColor()' devuelve true.
      void ponerColor( const Tupla3f & nuevo_color );

      // Fijar (en el cauce) el color de los vértices usando el color o identificador
      // del objeto, y devolver el color actual del cauce
      // si 'cv.modo_seleccion' es 'true', usa el identificador del objeto (si no es -1)
      // si 'cv.modo_seleccion' es 'false', usa el color del objeto (si el
      //      objeto tiene color y 'cv.fijar_colores_objetos' es 'true')

      Tupla4f leerFijarColVertsCauce( ContextoVis & cv  );


      // devuelve el número de parámetros de este objeto
      // (por defecto no hay ningún parámetro: devuelve 0)
      // (virtual: redefinir en clases derivadas)
      virtual unsigned leerNumParametros() const ;

      // cambia el parámetro activo actualmente (variable 'ind_par_act')
      // si d == +1 pasa al siguiente (o del último al primero)
      // si d == -1 pasa al anterior (o del primero al último)
      // (si hay 0 parámetros, no hace nada)
      void modificarIndiceParametroActivo( const int d );

      // incrementa o decrementa el valor de uno de los parámetros del objeto
      //  iParam : índice del parámetro (entre 0 y numParametros()-1 )
      //  delta  : número de unidades de incremento o decremento (típicamente +1 o -1)
      // (por defecto produce un error: número de parámetro fuera de rango)
      void modificarParametro( const unsigned iParam, const int delta );

      // igual que el anterior, pero cambia el parámetro activo (iParam == ind_par_act)
      void modificarParametro( const int delta );

      // actualiza el estado del objeto tras transcurrir un intervalo de tiempo
      // (recorre todos los parámetros y los actualiza uno a uno, para ello
      // llama a actualizarEstadoParametro)
      // dtSec : es el tiempo en segundos transcurrido desde el inicio de las
      //         animaciones o la última llamada a este método
      void actualizarEstado( const float dtSec );

      // pone los valores de los parámetros a cero (estado inicial)
      // (por defecto no hace nada)
      void resetParametros();

      // poner el centro del objeto:
      void ponerCentroOC( const Tupla3f & nuevoCentro );

      // leer el centro del objeto
      Tupla3f leerCentroOC();

      // (re)calcular centro del objeto, si es necesario
      virtual void calcularCentroOC() ;

      // destructor
      virtual ~Objeto3D();

      // método para buscar un objeto con un identificador y devolver
      // un puntero al objeto y el punto central
      //
      // resultado:
      //    true si se ha encontrado, false si no.
      //
      // parámetros de entrada:
      //    ident_busc: identificador a buscar
      //    modelview:  matriz de modelado del padre del objeto (pasa coords.loc. a WC)
      //                (en el nodo raíz es la matriz identidad)
      //
      // parámetros de salida
      //    objeto: si encontrado y no es nullptr: apunta al puntero al objeto
      //    centro_wc: punto central del nodo en coords. de mundo (si encontrado)

      virtual bool buscarObjeto( const int ident_busc,
         const Matriz4f & mmodelado, Objeto3D ** objeto, Tupla3f & centro_wc )  ;







   protected: // -----

      // actualiza el valor de un parámetro a un instante de tiempo
      //  iParam : índice del parámetro (entre 0 y numParametros()-1 )
      // 't_sec' :  tiempo en segundos desde el estado inicial
      // (por defecto produce un error: número de parámetro fuera de rango, esto se debe
      // a que por defecto un objeto 3D declara tener 0 parámetros, y esta función no se
      // debe llamar, a no ser que se declaren tener más parámetros).
       // (virtual: redefinir en clases derivadas)
      virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

   private: //-----

      // la primera vez que se llama, inicializa los valores base de los
      // parámetros (a cero), las siguientes comprueba que tpSec tiene tantas
      // entradas como parámetros.
      void initTP();

      // valores actuales de 'tiempo' de cada parámetro (en segundos)
      // (inicialmente está vacío, así todos los objetos tienen 0 parámetros por defecto)
      std::vector<float> tiempo_par_sec ;

      // índice del parámetro activo (solo sirve si 'leerNumParametros() > 0')
      int ind_par_act = 0 ;

      // nombre asignado al objeto
      std::string nombre_obj = "no asignado";

      // punto central o representativo del objeto, en coordenadas de objeto
      Tupla3f centro_oc = { 0.0, 0.0, 0.0 };

      // identificador de este objeto
      //  0 : no seleccionable
      // -1 : mismo identificador que el padre en la jerarquía
      // >0 : tiene este identificador (es seleccionable)
      int identificador = -1 ;

      // 'true' el objeto tiene asignado un color
      // 'false' el objeto no tiene asignado un color
      bool tiene_color = false ;

      // si 'tiene_color' es 'true', color actual del objeto
      Tupla3f color_objeto = { 1.0, 1.0, 1.0 } ;

} ;



#endif
