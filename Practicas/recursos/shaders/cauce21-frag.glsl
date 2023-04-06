#version 120 // pedir glsl ver. 1.2


// *********************************************************************
//
// fragment shader sencillo
//
// *********************************************************************

// ver:
//
// buen ejemplo aquí:
// http://joshbeam.com/articles/getting_started_with_glsl/
//
// para ver como hacer bind de una (o varias) texturas:
// https://www.opengl.org/wiki/Sampler_(GLSL)#Language_Definition

const int max_num_luces = 8 ; // debe coincidir con 'int Cauce::maxNumFuentesLuz()'

// Parámetros de entrada ('uniform')

uniform int       eval_mil ;       // evaluar el MIL si (1) o no (0) --> si es que no, usar color plano actual
uniform int       sombr_plano ;    // 0 --> usar smooth shading, 1 --> usar flat shading
uniform int       eval_text ;      // 0 --> no evaluar texturas, 1 -> evaluar textura en FS, sustituye a (var_color)
uniform sampler2D tex ;            // textura: al ser el primer 'sampler', está ligado a la unidad 0 de texturas
uniform vec3      mil_ka ;         // color de la componente ambiental del MIL (Ka)
uniform vec3      mil_kd ;         // color de la componente difusa del MIL (Kd)
uniform vec3      mil_ks ;         // color de la componente pseudo-especular del MIL (Ks)
uniform float     mil_exp ;        // exponente de la componente pseudo-especular del MIL (e)
uniform int       num_luces ;      // número de luces activas, si eval_mil == true
uniform vec4      pos_dir_luz_ec[max_num_luces] ;  // idem (ec)
uniform vec3      color_luz[max_num_luces] ;       // color o intensidad de cada fuente de luz


// Parámetros de entrada ('varying' o 'in')

varying vec4 var_posic_ec ;   // posicion del punto (en coords de camara)
varying vec3 var_normal_ec;   // normal del punto central del pixel  (en coords. de camara)
varying vec3 var_vec_obs_ec ; // vector hacia el observador en coords de cámara
varying vec4 var_color;       // color del vértice (interpolado a los pixels) (obtenido de glColor)
varying vec2 var_coord_text;  // coordenadas de textura (enviadas o calculadas)

// -----------------------------------------------------------------------------------------------
// vector normalizado hacia el observador

vec3 VectorHaciaObs()
{
   return normalize( var_vec_obs_ec );
}
// -----------------------------------------------------------------------------------------------
// devuelve la normal al triángulo (usa la tangente y la bi-tangente, en el plano del triángulo)

vec3 NormalTriangulo()
{
   vec4 tx, ty ;

   tx = dFdx( var_posic_ec );
   ty = dFdy( var_posic_ec );

   return normalize( cross( tx.xyz, ty.xyz ) );
}
// -----------------------------------------------------------------------------------------------
// vector normal, normalizado

vec3 Normal()
{
   vec3 n ;
   if ( sombr_plano == 1 )
      n = NormalTriangulo() ;
   else
      n = normalize( var_normal_ec );

   if ( gl_FrontFacing )
      return n ;
   else
      return -n ;
}
// -----------------------------------------------------------------------------------------------
// vector normalizado en la dirección de la i-ésima fuente de luz

vec3 VectorHaciaFuente( int i )
{
   if ( pos_dir_luz_ec[i].w == 1.0  )
      return normalize( pos_dir_luz_ec[i].xyz - var_posic_ec.xyz );
   else
      return normalize( pos_dir_luz_ec[i].xyz );
}
// -----------------------------------------------------------------------------------------------
// Función que evalúa un MIL sencillo
// color_obj  == color del objeto en el punto central al pixel

vec3 EvalMIL(  vec3 color_obj )
{
   vec3 n        = Normal(),              // vector normal, normalizado
        v        = VectorHaciaObs() ,     // vector hacia el observador, normalizado
        col_suma = vec3( 0.0, 0.0, 0.0 ); // suma de los colores debidos a cada fuente de luz

   // para cada fuente de luz, sumar en 's' el color debido a esa fuente:
   for( int i = 0 ; i < num_luces ; i++ )
   {
      // sumar la componente ambiental debida a esta fuente de luz
      col_suma = col_suma + color_luz[i]*color_obj*mil_ka ;

      // si la normal apunta al hemisferio de la fuente, añadir
      // componentes difusa y especular

      vec3  l   = VectorHaciaFuente( i ) ;
      float nl  = dot( n, l ) ;

      if ( 0.0 < nl )
      {  float hn  = max( 0.0, dot( n, normalize( l+v ) ));
         vec3  col = color_obj*(mil_kd*nl) + pow(hn,mil_exp)*mil_ks ;
         col_suma = col_suma + (color_luz[i] * col);
      }
      // nota: esto es lo más lógico y parecido al cauce fijo,
      // (causa terminadores de sombra "duros" en la componente pseudo-especular)
      // otra opción es simplemente añadir la componente pseudo-especular aún cuando
      // nl < 0.0, (siempre con hn no-negativo, por supuesto).
   }
   return col_suma ;
}
// -----------------------------------------------------------------------------------------------
// función principal (escribe en 'gl_FragColor', que es una variable de salida predefinida)

void main()
{
   // consultar color del objeto en el centro del pixel ('color_obj')
   vec4 color_obj ;
   if ( eval_text == 1 ) // si hay textura:
      color_obj = texture2D( tex, var_coord_text );  // es el color de la textura en las coordenadas de textura actuales
   else  // si no hay textura:
      color_obj = var_color ; // es el color enviado con 'glColor' o 'glColorPointer'

   // calcular el color del pixel (gl_FragColor)
   if ( eval_mil == 0 ) // si está desactivada iluminación:
      gl_FragColor = color_obj ; // el color del pixel es el color del objeto
   else // si está activada iluminación
      gl_FragColor = vec4( EvalMIL( color_obj.rgb ), 1.0 ); // el color del pixel es el resultado de evaluar iluminación
}
