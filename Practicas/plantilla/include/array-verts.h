// *********************************************************************
// **
// ** Informática Gráfica - curso 2020-21
// ** Declaraciones de las clases 'DescrTabla' y 'ArrayVerts'
// **
// *********************************************************************

#ifndef ARRAY_VERTS_H
#define ARRAY_VERTS_H

#include <ig-aux.h> 


// -----------------------------------------------------------------------------------------------------------------
//
// Clase DescrTabla (descriptor de buffer): 
//
// contiene información sobre el formato y localización en memoria 
// de un buffer con una tabla de datos de vértices (una tabla de coordenadas, de algún atributo, o de índices)
//
// -----------------------------------------------------------------------------------------------------------------

class DescrTabla
{
   public:

   
   DescrTabla(  const GLenum  p_tipo_tabla,      const GLenum   p_atributo, 
                 const GLenum  p_tipo_valores,    const GLint    p_num_vals_tupla,
                 const GLsizei p_num_tuplas_ind,  const GLvoid * p_datos );

   ~DescrTabla(); // destructor (no libera memoria con los datos, solo en la GPU si procede)

   void activar_md() ; // Activa el buffer (modo inmediato)
   void activar_mi() ; // Activa el buffer (modo diferido)


   // inline GLenum         lee_tipo_valores() const { return tipo_valores; }
   // inline const GLvoid * lee_datos()        const { return datos; }

   private:

   // Variables que describen el buffer

   GLenum tipo_tabla = 0 ;
   // Indica el tipo de tabla, es decir, si la tabla tiene atributos o índices, debe valer:
   //   GL_ARRAY_BUFFER   - atributos de vértices (incluyendo coordenadas), 
   //   GL_ELEMENT_BUFFER - índices

   GLenum atributo = 0 ;
   // Indica cual es el atributo de vértice almacenado en la tabla (solo si tipo_tabla es 'GL_ARRAY_BUFFER'). 
   // Debe valer una de estas constantes:
   //    GL_VERTEX_ARRAY          -  coordenadas (posiciones)  
   //    GL_TEXTURE_COORD_ARRAY   - coordenadas de textura 
   //    GL_COLOR_ARRAY           - colores de vértices
   //    GL_NORMAL_ARRAY          - normales 
   
   GLenum tipo_valores = 0 ;
   // Indica el tipo de los valores numéricos almacenados en la tabla, 
   // Puede valer, típicamente: GL_FLOAT, GL_DOUBLE, para atributos como las coordenadas, las normales, 
   // las coordenadas de textura o los colores, o bien GL_INT o GL_UNSIGNED_INT, para índices (en algunos casos 
   // para los colores)

   GLsizeiptr num_bytes_valor = 0 ; 
   // Número de bytes que ocupa cada valor numérico (se puede obtener con sizeof(...) exclusivamente 
   // en función del tipo de los de valores.
   
   GLint num_vals_tupla = 0 ;
   // Número de valores numéricos para cada vértice o tupla 
   // Para las tablas de atributos, indica cuantos valores numéricos hay por cada vértice
   // Para una tabla de índices, vale 1.

   GLsizei num_tuplas_ind = 0 ;
   // Número total de tuplas (vértices) o índices en la tabla

   GLsizeiptr tamano_en_bytes = 0 ;
   // Tamaño total de la tabla en bytes, asumiendo que 'paso_en_bytes' es 0 (tuplas contiguas)
   // (coincide con el numero de tuplas x número de valores por tupla x tamaño de cada valor en bytes)
   
   const GLvoid * datos = nullptr ;  
   // Puntero a los datos en la memoria (RAM) de la aplicación, incialmente vale null para indicar que 
   // la tabla no contiene datos. El objeto 'DescrTabla' no crea ni destruye este array de datos, simplemente 
   // los lee. Los datos deben estar en esa dirección todo el tiempo de vida del objeto  'DescrTabla'.

   GLuint nombre_vbo = 0 ; 
   // Nombre  de VBO. Es un valor numérico entero que OpenGL usa para identificar el DescrTabla cuando ya se ha copiado 
   // la tabla a la memoria de la GPU para modo diferido. Si tiene el valor 0, es que la tabla no se ha transferido a la GPU 
   // o bien está vacía o sin inicializar. Si tiene un valor > 0 es que la tabla se ha transferido a un 
   // VBO en la GPU para visualización en modo diferido.  

   // ---------------
   // Métodos privados: 

   // comprueba que el objeto está en un estado correcto (que los valores son correctos)
   void comprobar() ; 

   // fijar la localización del puntero de datos para un buffer:
   void fijarPuntero( const GLvoid * ptr_offset );

   // conceder acceso a elementos privados desde la clase 'ArrayVertices'.
   friend class ArrayVertices ; 

} ;

// -----------------------------------------------------------------------------------------------------------------
//
// Clase ArrayVerts
//
// Una clase para un array de coordenadas de vértices, incluyendo además (opcionalmente) sus atributos y sus índices
// (las instancias de esta clase contienen los descriptores de buffers necesarios, pero no crean ni
//  destruyen la memoria de esos buffers)
//
// -----------------------------------------------------------------------------------------------------------------

class ArrayVertices
{

   public:
   // constructor: crea un array de vértices, indicando las coordenadas de los mismos
   ArrayVertices ( const GLenum  tipo_valores,   const GLint num_vals_tupla, 
                   const GLsizei p_num_vertices, const GLvoid * datos );

   // destructor: destruye los buffers (libera memoria ocupada en la GPU, no destruye las tablas en RAM)
   ~ArrayVertices();
   
   // métodos para establecer las tablas de atributos (son opcionales)
   // (se asume que el numero de tuplas en cada tabla coincide con el número de vértices especificados en el constructor)
   void fijarColores  ( const GLenum tipo_valores, const GLint num_vals_tupla, const GLvoid *datos );
   void fijarCoordText( const GLenum tipo_valores, const GLint num_vals_tupla, const GLvoid *datos );
   void fijarNormales ( const GLenum tipo_valores, const GLvoid *datos ); // se asumen 3 reales por normal  

   // métodos para establecer los índices (es opcional) 
   void fijarIndices( const GLenum tipo_valores, const GLsizei p_num_indices, const GLvoid * datos ); 

   // métodos que visualizan el array de vértices (no modifcan estado de OpenGL, excepto los punteros a buffers)
   // 'mode' indica el tipo de primitiva: GL_TRIANGLES, GL_LINES, GL_POLYGON, etc....
   void visualizarGL_MI_BVE( const GLenum tipo_primitiva ); // modo inmediato, usando  una llamada por vértice (glBegin/glVertex/glEnd)
   void visualizarGL_MI_DAE( const GLenum tipo_primitiva ); // modo inmediato, con una única llamada a 'DrawArrays' (no indexado) o 'DrawElements' (indexado)
   void visualizarGL_MD_VAO( const GLenum tipo_primitiva ); // modo diferido, con VBOs, VAO y con una única llamada 
   
   private:

   GLuint nombre_vao = 0 ; // nombre del VAO para M.D., se crea bajo demanda la prim. vez que se visualiza en MD

   unsigned num_vertices = 0, // número de vértices (inicializado en el constructor, >0)
            num_indices  = 0; // número de índices (opcional, es 0 si no hay índices)

   // Descriptores de tablas de datos (son propiedad de este 'ArrayVertices')
   DescrTabla * coordenadas    = nullptr, // debe ser inicializado en el constructor
              * colores        = nullptr, // inicializado en 'fijarColores', nullptr si no hay colores
              * normales       = nullptr, // inicializado en 'fijarNormales',  nullptr si no hay normales
              * coords_textura = nullptr, // inicializado en 'fijarCoordText',  nullptr si no hay coords. de textura
              * indices        = nullptr ; // inicializado en 'fijarIndices', nullptr si no hay índices.
               
   void deshabilitar_tablas();
   void comprobarFormato_MI_BVE();
} ;

#endif // ARRAY_VERTS_H