// *********************************************************************
// **
// ** Gestión del cauce gráfico (clase ShaderProg) (declaraciones)
// **
// ** Copyright (C) 2017 Carlos Ureña
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


#ifndef CAUCE_HPP
#define CAUCE_HPP

#include <vector>
#include "ig-aux.h"
#include "tuplasg.h"
#include "matrices-tr.h"


//*******************************************************************************************************
// clase abstracta que define el interfaz para un cauce gráfico
// (puede concretarse en el cauce fijo de OpenGL o en distintos cauces de distintas versiones
//  de OpenGL)

class Cauce
{
   public:
      Cauce();

      // activa el cauce gráfico: a partir de la llamada
      // OpenGL usará este cauce para cualquier operación de dibujo
      virtual void activar() = 0 ;

      // fija la matriz de vista, vacía la pila de modelado y pone la
      // matriz de modelado como la matriz identidad
      virtual void fijarMatrizVista( const Matriz4f & nue_mat_vista ) = 0 ;

      // fijar la matriz de proyección
      virtual void fijarMatrizProyeccion( const Matriz4f & nue_mat_proyeccion ) = 0 ;

      // habilita/deshabilita le evaluación del MIL
      virtual void fijarEvalMIL( const bool nue_eval_mil ) = 0 ;

      // habilita/deshabilita modo sombreado (true -> plano, false -> smooth o 'Gouroud')
      virtual void fijarModoSombrPlano( const bool nue_sombr_plano ) = 0 ; // inicialmente, 'false'

      // fija los 3 colores ('ambiente', 'difuso' y 'especular') y el valor 'exp' del M.I.L.
      virtual void fijarParamsMIL(  const Tupla3f &ambiente, const Tupla3f &difuso,
                                    const Tupla3f &especular, const float exp )  = 0 ;

      // inserta copia de la matriz de modelado actual (en el tope de la pila)
      virtual void pushMM() = 0 ;

      // extrae tope de la pila y sustituye la matriz de modelado actual
      virtual void popMM()  = 0 ;

      // compone matriz de modelado actual con la matriz dada.
      virtual void compMM( const Matriz4f & mat_componer ) = 0 ;

      // habilita/deshabilita la consulta de una textura
      // si la habilita se da el identificador de textura activa (en ese caso no puede ser -1)
      virtual void fijarEvalText( const bool nue_eval_text, const int nue_text_id  = -1 ) = 0 ;

      // fijar parámetros de generación de coordenadas de textura
      // nue_tipo_gct : tipo de generación: 0 -> ninguna, 1 -> coords de objeto, 2 -> mundo, 3 -> ....
      // coefs_s, coefs_t : si no son nulos y nu_tipo_gct != 0, entonces contienen los coeficientes (4 floats cada uno)
      virtual void fijarTipoGCT( const int nue_tipo_gct,
                                 const float * coefs_s =nullptr, const float * coefs_t =nullptr) = 0 ;

      // devuelve el máximo número de fuentes de luz permitidas:
      // (según el estándar OpenGL, ese número debe ser como minimo 8)
      unsigned maxNumFuentesLuz() { return 8 ; } ;

      // fijar los parámetros de las fuentes de luz para el MIL
      // los vectores 'color' y 'pos_dir_wc' deben tener un tamaño como máximo igual a 'maxNumFuentesLuz'
      // los vectores 'color' y 'pos_dir_wc' deben tener el mismo tamaño, y debe ser mayor que cero.
      // el vector 'color' tiene los colores de las fuentes de luz
      // el vector 'pos_dir_wc' tiene las posiciones/direcciones de las fuentes de luz (en coords de mundo)
      virtual void fijarFuentesLuz( const std::vector<Tupla3f> & color,
                                    const std::vector<Tupla4f> & pos_dir_wc  ) = 0 ;

      // devuelve el color actual
      Tupla4f leerColorActual() const ;

      // lee el nombre o descripción del cauce
      virtual std::string descripcion() = 0 ;
} ;

//*******************************************************************************************************
// clase (concreta) para el cauce de funcionalidad fija (OpenGL 2.1 o anteriores)

class CauceFijo : public Cauce
{
   private:
      std::vector<Tupla3f> colores_fuentes ; // colores de las fuentes de luz, fijadas en: fijarFuentesLuz

   public:
      virtual void activar() ;
      virtual void fijarMatrizVista     ( const Matriz4f & nue_mat_vista      );
      virtual void fijarMatrizProyeccion( const Matriz4f & nue_mat_proyeccion );

      virtual void fijarEvalMIL  ( const bool nue_eval_mil )  ;
      virtual void fijarModoSombrPlano( const bool nue_sombr_plano );
      virtual void fijarParamsMIL( const Tupla3f &ambiente, const Tupla3f &difuso,
                                   const Tupla3f &especular, const float exp );
      virtual void fijarEvalText ( const bool nue_eval_text, const int nue_text_id = -1 )  ;
      virtual void fijarTipoGCT  ( const int nue_tipo_gct,
                                   const float * coefs_s =nullptr, const float * coefs_t =nullptr)  ;

      virtual void pushMM() ;  // inserta copia de la matriz de modelado actual (en el tope de la pila)
      virtual void popMM()  ;   // extrae tope de la pila y sustituye la matriz de modelado actual
      virtual void compMM( const Matriz4f & mat_componer )  ;  // compone matriz de modelado actual con la matriz dada.

      virtual void fijarFuentesLuz( const std::vector<Tupla3f> & color,
                                    const std::vector<Tupla4f> & pos_dir_wc  ) ;

      // lee el nombre o descripción del cauce
      virtual std::string descripcion() ;
} ;

//*******************************************************************************************************
// clase (concreta) para el cauce de funcionalidad programable (OpenGL 2.1 por ahora)

class CauceProgramable : public Cauce
{
   public:

   CauceProgramable() ;

   virtual void activar();
   virtual void fijarMatrizVista     ( const Matriz4f & nue_mat_vista      );
   virtual void fijarMatrizProyeccion( const Matriz4f & nue_mat_proyeccion );

   virtual void fijarEvalMIL  ( const bool nue_eval_mil );
   virtual void fijarModoSombrPlano( const bool nue_sombr_plano );
   virtual void fijarParamsMIL( const Tupla3f &ambiente, const Tupla3f &difuso,
                                const Tupla3f &especular, const float exp )  ;
   virtual void fijarEvalText ( const bool nue_eval_text, const int nue_text_id = -1 )  ;
   virtual void fijarTipoGCT  ( const int nue_tipo_gct,
                                const float * coefs_s =nullptr, const float * coefs_t =nullptr)  ;

   virtual void pushMM() ;  // inserta copia de la matriz de modelado actual (en el tope de la pila)
   virtual void popMM()  ;   // extrae tope de la pila y sustituye la matriz de modelado actual
   virtual void compMM( const Matriz4f & mat_componer )  ;  // compone matriz de modelado actual con la matriz dada.
   virtual void fijarFuentesLuz( const std::vector<Tupla3f> & color,
                                 const std::vector<Tupla4f> & pos_dir_wc  ) ;

   // lee el nombre o descripción del cauce
   virtual std::string descripcion()  ;

   private:

   GLuint
      id_frag_shader   = 0,       // identificador del fragment shader
      id_vert_shader   = 0;       // identificador del vertex shader
   GLint
      id_prog              = 0 ,  // identificador del 'shader program'
      loc_mat_modelado     = -1,  // 'localizaciones' de los parámetros uniform
      loc_mat_modelado_nor = -1,
      loc_mat_vista        = -1,
      loc_mat_proyeccion   = -1,
      loc_tipo_gct         = -1,
      loc_eval_mil         = -1,
      loc_sombr_plano      = -1,
      loc_eval_text        = -1,
      loc_coefs_s          = -1,
      loc_coefs_t          = -1,
      loc_mil_ka           = -1,
      loc_mil_kd           = -1,
      loc_mil_ks           = -1,
      loc_mil_exp          = -1,
      loc_num_luces        = -1,
      loc_color_luz        = -1,
      loc_pos_dir_luz_ec   = -1 ;

   bool
      eval_mil         = false,        // true -> evaluar MIL, false -> usar color plano
      sombr_plano      = false,
      eval_text        = false;        // true -> eval textura, false -> usar glColor o glColorPointer
   int
      tipo_gct         = 0 ;           // tipo de generación de coordenadas de textura
   Matriz4f
      mat_modelado     = MAT_Ident(),  // matriz de modelado
      mat_modelado_nor = MAT_Ident(),  // matriz de modelado para normales
      mat_vista        = MAT_Ident(),  // matriz de vista
      mat_proyeccion   = MAT_Ident();  // matriz de proyección

   float
      coefs_s[4]       = {1.0,0.0,0.0,0.0},
      coefs_t[4]       = {0.0,1.0,0.0,0.0};

   std::vector<Matriz4f>   // pilas de la matriz de modelado y de la matriz de modelado de normales.
      pila_mat_modelado,
      pila_mat_modelado_nor ;

   std::string
      frag_fn,      // nombre del archivo con el fragment shader
      vert_fn ;     // nombre del archivo con el vertex shader

   // métodos de apoyo para compilar/enlazar un shader:
   static char * leerArchivo( const std::string & nombreArchivo ) ;
   static GLuint compilarShader( const std::string & nombreArchivo, GLenum tipoShader );

   // fijar (con glUniform) las matrices de modelado y de normales en el shader prog.
   void actualizarMatricesMN();
} ;







#endif
