// *********************************************************************
// **
// ** Informática Gráfica
// ** Declaraciones públicas auxiliares, comunes a todas las prácticas
// **
// *********************************************************************


#ifndef PRACTICAS_H
#define PRACTICAS_H

class Escena ;
class ColFuentesLuz ;
class ContextoVis ;
class CamaraInteractiva ;
class Material ;
class Objeto3D ;

#include <string>
#include "matrizg.h"
#include "materiales-luces.h"
#include "cauce.h"
#include "camara.h"
#include "escena.h"

// ---------------------------------------------------------------------
// tipo de datos enumerado para los modos de visualización:

enum class ModosVisu
{
   relleno,
   lineas,
   puntos,
   num_modos
} ;
const std::string nombreModoVisu[ int(ModosVisu::num_modos)+1 ] =
{
   "relleno",
   "líneas",
   "puntos",
   "*** NUM. MODOS. VIS. ***"
} ;


// ---------------------------------------------------------------------
// tipo de datos enumerado para los modos de envío de primitivas

enum class ModosEnvio
{
   inmediato_begin_end,
   //inmediato_vao, // CAMBIAR el nombre de este valor
   inmediato_drawelements,
   diferido_vao,
   num_modos
} ;
const std::string nombreModoEnvio[ int(ModosEnvio::num_modos)+1 ] =
{
   "inmediato (con 'glBegin/glEnd')",
   "inmediato (con 'glDrawElements')", // CAMBIAR
   "diferido con VAO (con 'glDrawElements')",
   "*** NUM MODOS ENV. ***"
} ;

// --------------------------------------------------------------------
// contexto de visualización

class ContextoVis
{
   public:

   // modo de visualización actual
   ModosVisu modo_visu = ModosVisu::relleno ;

   // modo de envío actual
   ModosEnvio modo_envio = ModosEnvio::diferido_vao ;
   //ModosEnvio modo_envio = ModosEnvio::inmediato_drawelements ;

   // true -> usar 'flat shading', 'false' -->  usar 'smooth shading'
   bool sombr_plano = false ;

   // true -> dibujando en modo selección con FBO invisible (puede ser el back-buffer)
   bool modo_seleccion = false ;

   // true -> usar iluminación, false --> usar colores fijados con 'glColor' 'glColorPointer'
   bool iluminacion = true ;

   // dibujar aristas sí/no
   bool dibujar_aristas = false ;

   // visualizar normales si/no (útil para depurar objetos)
   bool visualizar_normales = false ;

   // true sii se está en proceso de visualizar normales durante una llamada a 'Escena::visualizarNormales'
   bool visualizando_normales = false ;

   // 'true'-> dibujar ejes, 'false' -> no dibujar ejes
   bool dibujar_ejes = true ;

   // 'true' -> visualizar FBO de seleccion, 'false' -> no visualizarlo
   bool visualizar_fbo = false ;

   // material actualmente activado (cuando 'iluminacion==true')
   Material * material_act = nullptr ;

   // puntero al cauce activo actual
   Cauce * cauce_act = nullptr; // puntero al cauce activo

   // numero de columnas (tam_x) y filas (tam_y) del viewport actual
   int ventana_tam_x = -1,
       ventana_tam_y = -1 ;
   
   // suavizar aristas (y puntos) si/no 
   bool suavizar_aristas = false ;

   



};

#endif // PRACTICAS_H
