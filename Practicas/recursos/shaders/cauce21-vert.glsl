#version 120

// buen ejemplo aquí:
// http://joshbeam.com/articles/getting_started_with_glsl/
//
// para ver como hacer bind de una (o varias) texturas:
// https://www.opengl.org/wiki/Sampler_(GLSL)#Language_Definition

const int max_num_luces = 8 ;

// parámetros de entrada uniform (constantes en cada primitiva)

// 1. tipo de sombreado
uniform int   sombr_plano ;     // 0 --> smooth, 1 --> flat (0 inicialmente)

// 2. matrices de transformación y proyección
uniform mat4  mat_modelado ;    // matriz de modelado actual
uniform mat4  mat_modelado_nor; // matriz de modelado para normales (traspuesta inversa de la anterior)
uniform mat4  mat_vista ;       // matriz de vista (mundo --> camara)
uniform mat4  mat_proyeccion ;  // matriz de proyeccion

// 3. parámetros relativos a texturas
uniform int   eval_text ;       // 0 --> no evaluar texturas, 1 -> evaluar textura en FS, sustituye a (var_color)
uniform int   tipo_gct ;        // tipo gen.cc.tt. (0=desact, 1=objeto, 2=camara)
uniform vec4  coefs_s ;         // coefficientes para G.CC.TT. (coordenada 's')
uniform vec4  coefs_t ;         // coefficientes para G.CC.TT. (coordenada 't')

// 4. parámetros relativos evaluación de iluminación
uniform int   eval_mil ;        // evaluar el MIL sí (1) o no (0) --> si es que no, usar color plano actual
uniform vec3  mil_ka ;          // color de la componente ambiental del MIL (Ka)
uniform vec3  mil_kd ;          // color de la componente difusa del MIL (Kd)
uniform vec3  mil_ks ;          // color de la componente pseudo-especular del MIL (Ks)
uniform float mil_exp ;         // exponente de la componente pseudo-especular del MIL (e)

// 5. parámetros de las fuentes de luces actuales
uniform int   num_luces ;                  // número de luces activas, si eval_mil == true
uniform vec4  pos_dir_luz[max_num_luces] ; // posición/dirección de cada luz
uniform vec3  color_luz[max_num_luces] ;   // color o intensidad de cada fuente de luz

// Valores calculados como salida ('varying', o 'out')

varying vec4 var_posic_ec ;   // posicion del punto (en coords de camara)
varying vec4 var_color;       // color del vértice (interpolado a los pixels)
varying vec3 var_normal_ec;   // normal del punto central del pixel  (en coords. de camara)
varying vec2 var_coord_text;  // coordenadas de textura
varying vec3 var_vec_obs_ec ; // vector hacia el observador en coords de cámara

// ------------------------------------------------------------------------------
// calculo de los parámetros de salida (var_... y gl_Position)

vec2 CoordsTextura() // calcula las coordenadas de textura
{
   if ( eval_text == 0 )           // si no se están evaluando las coordenadas de textura
      return vec2( 0.0, 0.0 );     //   devuelve las coordenadas (0.0,0.0)
   if ( tipo_gct == 0 )            // texturas activadas, generacion desactivada
      return gl_MultiTexCoord0.st; //    devuelve las coords. de glTexCoords o tabla

   vec4 pos_ver ;
   if ( tipo_gct == 1 )         // generacion en coordenadas de objeto
      pos_ver = gl_Vertex ;        //    usar las coords originales (objeto)
   else                         // generacion en coords de cámara
      pos_ver = var_posic_ec ;     //    usar las coordenadas de cámara

   return vec2( dot(pos_ver,coefs_s), dot(pos_ver,coefs_t) );
}
// ------------------------------------------------------------------------------

void main()
{
   vec4 posic_wc   = mat_modelado*gl_Vertex ;
   vec3 normal_wc  = (mat_modelado_nor*vec4(gl_Normal,0)).xyz ;
   var_posic_ec    = mat_vista*posic_wc ;
   var_normal_ec   = (mat_vista*vec4(normal_wc,0)).xyz ;
   var_vec_obs_ec  = (-var_posic_ec).xyz ;
   var_color       = gl_Color ;  // color fijado con gl_Color .....
   var_coord_text  = CoordsTextura();

   gl_Position = mat_proyeccion * var_posic_ec ;
}
