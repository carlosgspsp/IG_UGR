// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Informática Gráfica
// ** Definición de los métodos de Objeto3D
// **
// *********************************************************************

#include <iostream>
#include "objeto3d.h"
#include "seleccion.h"

using namespace std ;

// -----------------------------------------------------------------------------
Objeto3D::Objeto3D()
{
}

// -----------------------------------------------------------------------------
// devuelve el nombre del objeto
std::string Objeto3D::leerNombre()
{
   return nombre_obj ;
}

// -----------------------------------------------------------------------------
// poner nombre al objeto
void Objeto3D::ponerNombre( const std::string & nuevoNombre )
{
   nombre_obj = nuevoNombre ;
}
// -----------------------------------------------------------------------------
// devuelve el nombre del objeto
int Objeto3D::leerIdentificador()
{
   return identificador ;
}

// -----------------------------------------------------------------------------
// ponerle un identificador al objeto
void Objeto3D::ponerIdentificador( int nuevoIdent )
{
   identificador = nuevoIdent ;
}

// -----------------------------------------------------------------------------
// 'true' si el objeto tiene asignado un color, 'false' en otro caso
// (por defecto, no tiene asignado color)

bool Objeto3D::tieneColor()
{
   return tiene_color ;
}
// -----------------------------------------------------------------------------
// si 'tieneColor()==true', devuelve el color actual del objeto, en otro caso aborta

Tupla3f Objeto3D::leerColor()
{
   assert( tieneColor() );
   return color_objeto ;
}

// -----------------------------------------------------------------------------
// le asigna un color al objeto, desde la llamada, 'tieneColor()' devuelve true.
void Objeto3D::ponerColor( const Tupla3f & nuevo_color )
{
   tiene_color = true ;
   color_objeto = nuevo_color ;
}

// -----------------------------------------------------------------------------
// poner el centro del objeto:

void Objeto3D::ponerCentroOC( const Tupla3f & nuevoCentro )
{
   centro_oc = nuevoCentro ;
}

// -----------------------------------------------------------------------------
// Fijar (en el cauce) el color de los vértices usando el color o identificador
// del objeto, y devolver el color actual del cauce

Tupla4f Objeto3D::leerFijarColVertsCauce( ContextoVis & cv )
{
   assert( cv.cauce_act != nullptr );

   // leer color previo
   Tupla4f color_previo = cv.cauce_act->leerColorActual();

   if (cv.visualizando_normales)
       return color_previo;

   if ( cv.modo_seleccion )
   {
      // modo seleccion: usar el identificador del objeto, si no es -1
      const int ident_nodo = leerIdentificador();
      if ( ident_nodo != -1 )
         FijarColVertsIdent( *(cv.cauce_act), ident_nodo );
   }
   else
   {
      // modo dibujo: fijar el color si el objeto tiene color
      if ( tieneColor() )
         glColor3fv( leerColor() );
   }

   // devolver color previo
   return color_previo ;
}

// -----------------------------------------------------------------------------
// leer el centro del objeto

Tupla3f Objeto3D::leerCentroOC()
{
   return centro_oc ;
}

// -----------------------------------------------------------------------------
// (re)calcular centro del objeto, si es necesario

void Objeto3D::calcularCentroOC()
{
   // Por defecto no se hace nada: se asume que el centro está bien calculado
   // en el constructor. Se puede redefinir en clases derivadas, por ejemplo,
   // se redefine en los nodos de grafo de escena (y en las mallas indexadas)



}

// -----------------------------------------------------------------------------
// buscar un identificador (implementación por defecto para todos los Objeto3D)

bool Objeto3D::buscarObjeto
(
   const int        ident_busc,
   const Matriz4f & mmodelado,
   Objeto3D **      objeto,
   Tupla3f &        centro_wc
)
{
   assert( 0 < ident_busc );

   if ( identificador == ident_busc )
   {
      centro_wc = mmodelado*leerCentroOC();
      if ( objeto != nullptr )
         *objeto = this ;
      return true ;
   }
   else
      return false ;
}

// -----------------------------------------------------------------------------
// Devuelve el número de parámetros de este objeto
// (por defecto no hay ningún parámetro: devuelve 0)
// (virtual: redefinir en las clases derivadas)

unsigned Objeto3D::leerNumParametros() const
{
   return 0 ;
}

// -----------------------------------------------------------------------------
// cambia el parámetro activo actualmente (variable 'ind_par_act')
// si d == +1 pasa al siguiente (o del último al primero)
// si d == -1 pasa al anterior (o del primero al último)
// (si hay 0 parámetros, no hace nada)

void Objeto3D::modificarIndiceParametroActivo( const int d )
{
   const int n = leerNumParametros() ;

   using namespace std ;

   if ( n == 0 )
   {  cout << "no es posible cambiar el parámetro activo, este objeto no tiene ningún parámetro definido." << endl ;
      return ;
   }
   assert( 0 <= ind_par_act );
   ind_par_act = ( ind_par_act + n + d ) % n ;
   cout << "parámetro actual del objeto cambiado a " << (ind_par_act+1) << " / " << n << endl ;
}

// -----------------------------------------------------------------------------
// Incrementa o decrementa el valor base de uno de los parámetros del objeto
//  iParam : índice del parámetro (entre 0 y numParametros()-1 )
//  delta  : número de unidades de incremento o decremento (típicamente +1 o -1)
// (por defecto produce un error: número de parámetro fuera de rango)

void Objeto3D::modificarParametro( const unsigned iParam, const int delta )
{
   assert( iParam < leerNumParametros() );
   initTP();
   tiempo_par_sec[iParam] += float(delta)/10.0 ; // cada unidad es equivalente a una décima de segundo
   actualizarEstadoParametro( iParam, tiempo_par_sec[iParam] );
}

// -----------------------------------------------------------------------------
// igual que el anterior, pero cambia el parámetro activo (iParam --> ind_par_act)
void Objeto3D::modificarParametro( const int delta )
{
   assert( 0 <= ind_par_act && ind_par_act < int(leerNumParametros()) );
   initTP();
   tiempo_par_sec[ind_par_act] += float(delta)/10.0 ; // cada unidad es equivalente a una décima de segundo
   actualizarEstadoParametro( ind_par_act, tiempo_par_sec[ind_par_act] );
}

// -----------------------------------------------------------------------------
// Actualiza el valor de un parámetro a un instante de tiempo
//  iParam : índice del parámetro (entre 0 y numParametros()-1 )
//  tSec   : tiempo en segundos desde el estado inicial
// (por defecto produce un error)
// (virtual: redefinir en las clases derivadas)

void Objeto3D::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   cerr << "Se ha invocado el método: " << __PRETTY_FUNCTION__  << endl
        << "(este método se debe redefinir para clases de objetos que tengan algún parámetro)" << endl ;
   exit(1);
}

// -----------------------------------------------------------------------------
// Actualiza el estado del objeto tras transcurrir un intervalo de tiempo
// (recorre todos los parámetros y los actualiza uno a uno, para ello
// llama a actualizarEstadoParametro)
// dtSec : es el tiempo en segundos transcurrido desde el inicio de las
//         animaciones o la última llamada a este método

void Objeto3D::actualizarEstado( const float dtSec )
{
   initTP();
   for( unsigned i = 0 ; i < leerNumParametros() ; i++  )
   {
      tiempo_par_sec[i] += dtSec ;
      actualizarEstadoParametro( i, tiempo_par_sec[i] );
   }
}

// -----------------------------------------------------------------------------
// Pone los valores de los parámetros a cero (estado inicial)

void Objeto3D::resetParametros()
{
   initTP();
   for( unsigned i = 0 ; i < leerNumParametros() ; i++  )
   {
      tiempo_par_sec[i] = 0.0 ;
      actualizarEstadoParametro( i, tiempo_par_sec[i] );
   }
}

// -----------------------------------------------------------------------------
// la primera vez que se llama, inicializa los valores base de los
// parámetros (a cero), las siguientes comprueba que tiempo_par_sec tiene tantas
// entradas como parámetros.

void Objeto3D::initTP()
{
   if ( tiempo_par_sec.size() == 0  )
      for( unsigned i = 0 ; i < leerNumParametros() ; i++ )
         tiempo_par_sec.push_back( 0.0 );
   else
      assert( tiempo_par_sec.size() == leerNumParametros() );
}

// -----------------------------------------------------------------------------
// Destructor
Objeto3D::~Objeto3D()
{
   using namespace std ;
   cout << "destruyendo objeto3D de nombre: " << nombre_obj << endl << flush ;
}
// -----------------------------------------------------------------------------


