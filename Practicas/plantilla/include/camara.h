// *********************************************************************
// **
// ** Informática Gráfica - curso 2020-21
// ** Declaración de la clase 'Viewport', 'Camara' y derivadas de 'Camara':
// **
// **  + Camara: clase base, cámara básica.
// **      + CamaraInteractiva: cámaras con métodos para modificar sus parámetros
// **          + CamaraOrbitalSimple: cámara orbital usada en las prácticas (implementada al 100%)
// **          + Camara3Modos: cámara con tres modos de funcionamiento (a completar en la práctica 5)
// **
// *********************************************************************

#ifndef CAMARA_HPP
#define CAMARA_HPP

#include "tuplasg.h"
#include "matrices-tr.h"
#include "cauce.h"



// *********************************************************************
// clase: Viewport

class Viewport
{
  public:
  int       org_x, org_y, // origen en pixels (esquina inferior izquierda)
            ancho, alto ; // dimensiones en pixels (núm. columnas, núm. filas)
  float     ratio_yx ;    // == alto/ancho (relación de aspecto)
  Matriz4f  matrizVp ,    // matriz de viewport ( pasa: NDC ==> DC )
            matrizVpInv ; // matriz inversa ( pasa: DC ==> NDC )
  // constructor
  Viewport() ; // crea viewport de 512 x 512 con origen en (0,0)
  Viewport( int p_org_x, int p_org_y, int p_ancho, int p_alto );
} ;

// ******************************************************************
// clase base para cámaras

class Camara
{
   public: // ----------------------------

   // fija las matrices model-view y projection en el cauce
   void activar( Cauce & cauce )  ;

   // cambio el valor de 'ratio_vp' (alto/ancho del viewport)
   void fijarRatioViewport( const float nuevo_ratio ) ;

   // lee la descripción de la cámara (y probablemente su estado)
   virtual std::string descripcion() ;

   protected: // ------------------------------

   bool     matrices_actualizadas = false ;        // true si matrices actualizadas
   Matriz4f matriz_vista = MAT_Ident() , // matriz de vista
            matriz_proye = MAT_Ident() ; // matriz de proyección
   float    ratio_vp     = 1.0 ;         // alto del viewport dividido por el ancho del viewport

   // actualiza 'matriz_vista' y 'matriz_proye' a partir de los parámetros
   // específicos de cada tipo de cámara
   virtual void actualizarMatrices() ;
} ;

// ******************************************************************
// clase (abstracta) para cámaras interactivas (cámaras manipulables)

class CamaraInteractiva : public Camara
{
   public:
   // desplazar o rotar la cámara en horizontal 'da' unidades y en vertical 'db' unidades
   // (en horizontal y en vertical aquí se entiende relativo al marco de camara)
   virtual void desplRotarXY( const float da, const float db ) = 0 ;

   // acercar/alejar o desplazar la camara en el eje Z un total de 'dz' unidades
   // (el eje es el eje Z, relativo al marco de la camara, es decir, perpendicular al plano de visión)
   virtual void moverZ( const float dz ) = 0 ;

   // hacer que apunte hacía el punto de atención 'paten' y
   // cambiar de modo a modo examinar
   // por defecto imprime mensaje de advertencia de que la cámra no ofrece esta funcionalidad
   virtual void mirarHacia( const Tupla3f & paten )  ;

   // cambiar el modo de la camara al siguiente modo o al primero
   // por defecto imprime mensaje de advertencia de que la cámra no ofrece esta funcionalidad
   virtual void siguienteModo()  ;
} ;

// ******************************************************************
// clase para una cámara orbital sencilla que siempre tiene el origen de
// coordenadas del mundo en el centro de la imagen (tiene un modo único)

class CamaraOrbitalSimple : public CamaraInteractiva
{
   public:

   // cambia la longitud usando 'da' y la latitud usando 'db'
   virtual void desplRotarXY( const float da, const float db )   ;

   // acercar/alejar 'dz' unidades al origen (sin 'atravesarlo' nunca)
   virtual void moverZ( const float dz )  ;

   // lee la descripción de la cámara (y probablemente su estado)
   virtual std::string descripcion() ;

   protected:
      // posición de la cámara en coordenadas esféricas relativas al origen de
      // coordenadas del mundo (y a sus ejes)
      float a = 0.0,   // ángulo horizontal (angulo del eje Z con el plano z==0 en cc.m.)
            b = 0.0,   // ángulo vértical (angulo del eje Z con el plano y==0 en cc.m.)
            d = 4.0 ;  // distancia entre origen de cámara y origen de cc. de mundo

   virtual void actualizarMatrices() ;
} ;

// tipo enumerado para los modos de las cámaras interactivas
enum class ModoCam { examinar, prim_pers_rot, prim_pers_despl } ;

// ******************************************************************
// cámara interactiva completa, que puede funcionar en tres modos, y
// que puede centrarse en un punto (pasa a modo examinar)
//
// Los modos son:
//
//   ** modo examinar (o modo orbital), relativo a un punto de atención
//   ** modo primera persona con rotaciones (típico de los videojuegos)
//   ** modo primera persona con desplazamientos  (horizontal o vertical)



class Camara3Modos : public CamaraInteractiva
{
   public:

   // crea una cámara perspectiva con valores por defecto
   Camara3Modos() ;

   // crea una cámara, inicialmente en modo examinar, con el punto
   // de atención en el origen, se especifica:
   // * perspectiva_ini : true si es una camara perspectiva, false si es ortográfica
   // * origen_ini      : punto de vista inicial (origen marco de cámara)
   // * punto_aten_ini  : punto de atención
   // * fovy_grad_ini   : si es perspectiva, la apertura de campo vertical, en grados

   Camara3Modos( const bool perspectiva_ini,
                 const Tupla3f & origen_ini, const float ratio_vp_ini,
                 const Tupla3f & punto_aten_ini, const float fovy_grad_ini = 70.0 ) ;

   // desplazar o rotar la cámara en horizontal 'dx' unidades y en vertical 'dy' unidades
   // (en horizontal y en vertical aquí se entiende relativo al marco de camara)
   virtual void desplRotarXY( const float da, const float db )  ;

   // acercar/alejar la cámara respecto al punto de atención
   // (el eje es el eje Z, relativo al marco de la camara, es decir, perpendicular al plano de visión)
   virtual void moverZ( const float dz )  ;

   // hacer que apunte hacía el punto de atención 'paten' y pasar a modo examinar
   virtual void mirarHacia( const Tupla3f & nuevo_punto_aten )  ;

   // cambiar el modo de la camara al siguiente modo o al primero
   virtual void siguienteModo()  ;

   // devuelve el punto de atencion actual
   virtual Tupla3f puntoAtencion() ;

   // lee la descripción de la cámara (y probablemente su estado)
   virtual std::string descripcion() ;

private:

      // modo actual de la cámara,
      // (0=examinar, 1=prim.persona con desplazamientos, 2=prim.persona con rotaciones)
      ModoCam modo_actual = ModoCam::examinar ;

      // true si la cámara es perspectiva, false en otro caso:
      bool perspectiva = true ;

      // apertura de campo vertical en grados
      float fovy_grad = 60.0 ;

      // distancia inicial entre el origen del marco y el punto de atención
      const float d_ini = 3.0 ;

      // punto de atención
      Tupla3f punto_atencion = { 0.0, 0.0, 0.0 } ;

      // coordenadas polares del origen de coordenadas del mcv, respecto del punto de atención
      // (ángulos 'a' y 'b' son cero, 'r' es d_ini)
      Tupla3f org_polares = { 0.0, 0.0, d_ini } ;

      // coordenadas cartesianas del origen de coordenadas del mcv, respecto del punto de atención
      Tupla3f org_cartesianas = { 0.0, 0.0, d_ini };

      // ejes del marco de coordenadas de vista
      Tupla3f eje[3] = {  { 1.0, 0.0, 0.0 }, // eje X
                          { 0.0, 1.0, 0.0 }, // eje Y
                          { 0.0, 0.0, 1.0 }  // eje Z
                        };

      virtual void actualizarMatrices() ;
      void actualizarEjesMCV() ;

} ;

#endif
