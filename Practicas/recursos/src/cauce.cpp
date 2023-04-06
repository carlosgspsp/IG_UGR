// *********************************************************************
// **
// ** Gestión del cauce gráfico (clase ShaderProg) (implementación)
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


#include "ig-aux.h"
#include "cauce.h"

//*******************************************************************************
// Cauce
// -----------------------------------------------------------------------------

Cauce::Cauce()
{
   // las variables de instancia se inicializan en su declaración
   // (en principio no es necesario hacer nada aquí)
}

// -----------------------------------------------------------------------------
// devuelve el color actual

Tupla4f Cauce::leerColorActual() const
{
   float c[4];
   glGetFloatv( GL_CURRENT_COLOR, c );
   return Tupla4f { c[0], c[1], c[2], c[3] };
}

//*******************************************************************************
// Cauce Fijo
// -----------------------------------------------------------------------------

void CauceFijo::activar()
{
   CError();
   glUseProgram( 0 );
   CError();
}
// -----------------------------------------------------------------------------

void CauceFijo::fijarMatrizVista( const Matriz4f & nue_mat_vista )
{
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity() ;
   glMultMatrixf( nue_mat_vista );
   CError();
}
// -----------------------------------------------------------------------------

void CauceFijo::fijarMatrizProyeccion( const Matriz4f & nue_mat_proyeccion )
{
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glMultMatrixf( nue_mat_proyeccion );
   CError();
}
//-----------------------------------------------------------------------------

void CauceFijo::fijarEvalText( const bool nue_eval_text, const int nue_text_id  )
{
   CError();
   if ( nue_eval_text )
   {
      assert( -1 < nue_text_id );
      glEnable( GL_TEXTURE_2D );
      glBindTexture( GL_TEXTURE_2D, nue_text_id );
      glColor3f( 1.0, 1.0, 1.0 ); // CUA: revisar .....
   }
   else
      glDisable( GL_TEXTURE_2D );
   CError();
}
//-----------------------------------------------------------------------------

void CauceFijo::fijarTipoGCT( const int nue_tipo_gct,
                              const float * coefs_s, const float * coefs_t )
{
   assert( 0 <= nue_tipo_gct && nue_tipo_gct < 3 );

   if ( nue_tipo_gct != 0 )
   {
      assert( coefs_s != nullptr );
      assert( coefs_t != nullptr );
   }

   switch ( nue_tipo_gct )
   {
      case 0 :
         glDisable( GL_TEXTURE_GEN_S );
         glDisable( GL_TEXTURE_GEN_T );
         //tipoGCT = 0 ;
         break ;
      case 1 :
         glEnable( GL_TEXTURE_GEN_S );
         glEnable( GL_TEXTURE_GEN_T );
         glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR ) ;
         glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR ) ;
         glTexGenfv( GL_S, GL_OBJECT_PLANE, coefs_s ) ;
         glTexGenfv( GL_T, GL_OBJECT_PLANE, coefs_t) ;
         //tipoGCT = 1 ;
         break ;
      case 2 :
         glEnable( GL_TEXTURE_GEN_S );
         glEnable( GL_TEXTURE_GEN_T );
         glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR ) ;
         glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR ) ;
         glTexGenfv( GL_S, GL_EYE_PLANE, coefs_s ) ;
         glTexGenfv( GL_T, GL_EYE_PLANE, coefs_t ) ;
         //tipoGCT = 2 ;
         break ;
   }
   CError();
}
// -----------------------------------------------------------------------------

void CauceFijo::fijarEvalMIL( const bool nue_eval_mil  )
{
   if ( nue_eval_mil )
      glEnable( GL_LIGHTING );
   else
      glDisable( GL_LIGHTING );

   CError();
}
// -----------------------------------------------------------------------------

void CauceFijo::fijarModoSombrPlano( const bool nue_sombr_plano )
{
   const GLenum modo = nue_sombr_plano ? GL_FLAT : GL_SMOOTH ;
   glShadeModel( modo );
   CError();
}
// -----------------------------------------------------------------------------

void CauceFijo::fijarParamsMIL( const Tupla3f &ambiente, const Tupla3f &difuso,
                                const Tupla3f &especular, const float exp  )
{
   assert( 0 < colores_fuentes.size() && colores_fuentes.size() < maxNumFuentesLuz() );

   auto to4  = []( const Tupla3f & t3 ) { return Tupla4f ( t3(0), t3(1), t3(2), 1.0 ); };
   auto prod = []( const Tupla3f & c1, const Tupla3f & c2 )
               { return Tupla4f ( c1(0)*c2(0), c1(1)*c2(1), c1(2)*c2(2), 1.0 ); } ;

   glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,  Tupla4f{ 0.0, 0.0, 0.0, 0.0 } );
   glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  to4(especular) );
   glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, exp );

   // definir los colores de las fuentes de luz en OpenGL: son el producto de
   // los colores de las fuentes que hay registradas por las reflectividades que aparecen aquí:
   for( unsigned i = 0 ; i < colores_fuentes.size() ; i++ )
   {
      glLightfv( GL_LIGHT0+i, GL_AMBIENT,  prod( ambiente,  colores_fuentes[i] ) );
      glLightfv( GL_LIGHT0+i, GL_DIFFUSE,  prod( difuso,    colores_fuentes[i] ) );
      glLightfv( GL_LIGHT0+i, GL_SPECULAR, prod( especular, colores_fuentes[i] ) );
   }

   CError();
}
// -----------------------------------------------------------------------------
// inserta copia de la matriz de modelado actual (en el tope de la pila)

void CauceFijo::pushMM()
{
   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();
}
// -----------------------------------------------------------------------------
// extrae tope de la pila y sustituye la matriz de modelado actual

void CauceFijo::popMM()
{
   glMatrixMode( GL_MODELVIEW );
   glPopMatrix();
   CError();
}
// -----------------------------------------------------------------------------
// compone matriz de modelado actual con la matriz dada.

void CauceFijo::compMM( const Matriz4f & mat_componer )
{
   glMatrixMode( GL_MODELVIEW );
   glMultMatrixf( mat_componer );
   CError();
}
// -----------------------------------------------------------------------------
// fijar los parámetros que definen las fuentes de luz

void CauceFijo::fijarFuentesLuz( const std::vector<Tupla3f> & color,
                                 const std::vector<Tupla4f> & pos_dir_wc  )
{
   assert( 0 < color.size() && color.size() <= maxNumFuentesLuz() );
   assert( color.size() == pos_dir_wc.size() );
   const GLfloat color_negro[4] = { 0.0, 0.0, 0.0, 1.0 };

   glEnable( GL_NORMALIZE );
   glEnable( GL_COLOR_MATERIAL );
   glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

   // fijar parámetros del cauce fijo que afectan a la iluminación y materiales
   glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
   glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR ) ;
   glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
   glLightModelfv( GL_LIGHT_MODEL_AMBIENT, color_negro ); // por defecto es (0.2, 0.2, 0.2, 1.0)

   // habilitar fuentes, desde 0  hasta color.size()-1,
   // para cada una de ellas, se envía su posición (el color de la fuente se enviará
   // cuando se especifiquen los parámetros del material, más adelante)
   for( unsigned i = 0 ; i < color.size() ; i++ )
   {
      glEnable( GL_LIGHT0+i );
      glLightfv( GL_LIGHT0+i, GL_POSITION, pos_dir_wc[i] );
   }
   // deshabilitar restantes fuentes de luz hasta 'maxNumFuentesLuz' no incluido
   for( unsigned i = color.size() ; i < maxNumFuentesLuz() ; i++ )
      glDisable( GL_LIGHT0 + i );

   // registrar los colores de las fuentes de luz, se usarán más adelante cuando
   // se active el material y se conozcan todos los parámetros del M.I.L.
   colores_fuentes.clear();
   for( unsigned i = 0 ; i < color.size()  ; i++ )
      colores_fuentes.push_back( color[i] );

}
// -----------------------------------------------------------------------------
// lee el nombre o descripción del cauce

std::string CauceFijo::descripcion()
{
   using namespace std ;
   return "de funcionalidad fija." ;
}


// *****************************************************************************
// Cauce programable
// -----------------------------------------------------------------------------

CauceProgramable::CauceProgramable()
{
   using namespace std ;

   // inicializar los nombres de los archivos (gl 2.1 por ahora)
   frag_fn = "../recursos/shaders/cauce21-frag.glsl" ;
   vert_fn = "../recursos/shaders/cauce21-vert.glsl" ;

   // crear y compilar shaders, crear el programa
   id_frag_shader = compilarShader( frag_fn, GL_FRAGMENT_SHADER );
   id_vert_shader = compilarShader( vert_fn, GL_VERTEX_SHADER );
   id_prog        = glCreateProgram();

	// asociar shaders al programa
	glAttachShader( id_prog, id_frag_shader );
	glAttachShader( id_prog, id_vert_shader );

	// enlazar programa y ver errores
	glLinkProgram( id_prog );
   GLint resultado ;
   glGetProgramiv( id_prog, GL_LINK_STATUS, &resultado );

   // si ha habido errores, abortar
   if ( resultado != GL_TRUE )
   {
      // ver errores al enlazar
      const GLsizei maxt = 1024L*10L ;
      GLsizei       tam ;
      GLchar        buffer[maxt] ;

      glGetProgramInfoLog( id_prog, maxt, &tam, buffer);
      cout << "error al enlazar:" << endl
           << buffer << flush
           << "programa abortado" << endl << std::flush ;
      exit(1);
   }
   CError();

   // obtener las 'locations' de los parámetros uniform

   loc_mat_modelado     = glGetUniformLocation( id_prog, "mat_modelado");
   loc_mat_modelado_nor = glGetUniformLocation( id_prog, "mat_modelado_nor");
   loc_mat_vista        = glGetUniformLocation( id_prog, "mat_vista");
   loc_mat_proyeccion   = glGetUniformLocation( id_prog, "mat_proyeccion");
   loc_eval_mil         = glGetUniformLocation( id_prog, "eval_mil" );
   loc_sombr_plano      = glGetUniformLocation( id_prog, "sombr_plano" );
   loc_eval_text        = glGetUniformLocation( id_prog, "eval_text" );
   loc_tipo_gct         = glGetUniformLocation( id_prog, "tipo_gct" );
   loc_coefs_s          = glGetUniformLocation( id_prog, "coefs_s" );
   loc_coefs_t          = glGetUniformLocation( id_prog, "coefs_t" );
   loc_mil_ka           = glGetUniformLocation( id_prog, "mil_ka");
   loc_mil_kd           = glGetUniformLocation( id_prog, "mil_kd");
   loc_mil_ks           = glGetUniformLocation( id_prog, "mil_ks");
   loc_mil_exp          = glGetUniformLocation( id_prog, "mil_exp");
   loc_num_luces        = glGetUniformLocation( id_prog, "num_luces");
   loc_pos_dir_luz_ec   = glGetUniformLocation( id_prog, "pos_dir_luz_ec");
   loc_color_luz        = glGetUniformLocation( id_prog, "color_luz");

   assert( -1 < loc_mat_modelado );
   assert( -1 < loc_mat_modelado_nor );
   assert( -1 < loc_mat_vista );
   assert( -1 < loc_mat_proyeccion );
   assert( -1 < loc_eval_mil );
   assert( -1 < loc_sombr_plano );
   assert( -1 < loc_eval_text );
   assert( -1 < loc_tipo_gct );
   assert( -1 < loc_coefs_s );
   assert( -1 < loc_coefs_t );
   assert( -1 < loc_mil_ka );
   assert( -1 < loc_mil_kd );
   assert( -1 < loc_mil_ks );
   assert( -1 < loc_mil_exp );
   assert( -1 < loc_num_luces );
   assert( -1 < loc_pos_dir_luz_ec );
   assert( -1 < loc_color_luz );

   // dar valores iniciales por defecto a los parámetros uniform
   // (se intenta seguir la especificacion de OpenGL)

   glUseProgram( id_prog );
   CError();

   glUniformMatrix4fv( loc_mat_modelado,     1, GL_FALSE, mat_modelado );
   glUniformMatrix4fv( loc_mat_modelado_nor, 1, GL_FALSE, mat_modelado_nor );
   glUniformMatrix4fv( loc_mat_vista,        1, GL_FALSE, mat_vista );
   glUniformMatrix4fv( loc_mat_proyeccion,   1, GL_FALSE, mat_proyeccion );

   glUniform1i ( loc_eval_mil,    eval_mil ? 1 : 0 );
   glUniform1i ( loc_sombr_plano, sombr_plano ? 1 : 0 );
   glUniform1i ( loc_eval_text,   eval_text ? 1 : 0 );
   glUniform1i ( loc_tipo_gct,    tipo_gct );

   glUniform4fv( loc_coefs_s, 1, coefs_s );
   glUniform4fv( loc_coefs_t, 1, coefs_t );

   glUniform3fv( loc_mil_ka,  1, Tupla3f { 0.2, 0.2, 0.2 } );
   glUniform3fv( loc_mil_kd,  1, Tupla3f { 0.8, 0.8, 0.8 } );
   glUniform3fv( loc_mil_ks,  1, Tupla3f { 0.0, 0.0, 0.0 } );
   glUniform1f ( loc_mil_exp, 0.0 );

   glUniform1i( loc_num_luces, 0 ); // por defecto: 0 fuentes de luz activas
   CError();

   glUseProgram( 0 );
}
//----------------------------------------------------------------------

void CauceProgramable::activar()
{
   CError();
   assert( 0 < id_prog );
   glUseProgram( id_prog );
   CError();
}

//----------------------------------------------------------------------
// lee un archivo completo como texto  y devuelve una cadena terminada
// en cero con el contenido
// (el resultado está en el heap y debería borrarse con 'delete [] p')

char * CauceProgramable::leerArchivo( const std::string & nombreArchivo )
{
	using namespace std ;

	ifstream file( nombreArchivo.c_str(), ios::in|ios::binary|ios::ate );

	if ( ! file.is_open())
	{  cout << "imposible abrir archivo para lectura (" << nombreArchivo << ")" << endl << flush ;
		exit(1);
	}
	size_t	numBytes	= file.tellg();
	char *	bytes		= new char [numBytes+2];

	file.seekg (0, ios::beg);
	file.read (bytes, numBytes);
	file.close();

	bytes[numBytes]=0 ;
	bytes[numBytes+1]=0 ;

	return bytes ;
}
//----------------------------------------------------------------------
// crea, carga y compila un shader nuevo
// si hay algún error, aborta
// si todo va bien, devuelve el código de opengl del shader

GLuint CauceProgramable::compilarShader( const std::string & nombreArchivo, GLenum tipoShader )
{
   using namespace std ;
   GLuint idShader ; // resultado: identificador de shader

   CError() ;

   // crear shader nuevo, obtener identificador (tipo GLuint)
   idShader = glCreateShader( tipoShader );

   // leer archivo fuente de shader en memoria, asociar fuente al shader, liberar memoria:
   const GLchar * fuente = leerArchivo( nombreArchivo );
   glShaderSource( idShader, 1, & fuente, nullptr );
   delete [] fuente ;
   fuente = nullptr ;

   // compilar y comprobar errores
   glCompileShader( idShader );
   GLint resultado ;
   glGetShaderiv( idShader, GL_COMPILE_STATUS, &resultado );

   // si hay errores, abortar
   if ( resultado != GL_TRUE )
   {
      const GLsizei maxt = 1024L*10L ;
      GLsizei       tam ;
      GLchar        buffer[maxt] ;

      glGetShaderInfoLog( idShader, maxt, &tam, buffer);
      cout << "error al compilar archivo '" << nombreArchivo << "'" << endl
           << buffer << flush
           << "programa abortado" << endl << std::flush ;
      exit(1);
   }
   CError() ;
	return idShader ;
}
// -----------------------------------------------------------------------------
// fijar la matriz de vista y resetear las matrices de modelado y vista,
// (vacía la pila de modelado y de normales, igual que el cauce fijo)

void CauceProgramable::fijarMatrizVista( const Matriz4f & nue_mat_vista )
{
   assert( 0 < id_prog );

   mat_vista     = nue_mat_vista ;
   //mat_vista_inv = MAT_Inversa( mat_vista ) ;

   glUseProgram( id_prog );
   glUniformMatrix4fv( loc_mat_vista, 1, GL_FALSE, mat_vista );
   //glUniformMatrix4fv( loc_mat_vista_inv, 1, GL_FALSE, mat_vista_inv );

   pila_mat_modelado.clear();
   pila_mat_modelado_nor.clear();

   mat_modelado     = MAT_Ident();
   mat_modelado_nor = MAT_Ident();

   actualizarMatricesMN();

   CError();
}
// -----------------------------------------------------------------------------

void CauceProgramable::fijarMatrizProyeccion( const Matriz4f & nue_mat_proyeccion )
{
   assert( 0 < id_prog );
   mat_proyeccion = nue_mat_proyeccion ;
   glUseProgram( id_prog );
   glUniformMatrix4fv( loc_mat_proyeccion, 1, GL_FALSE, mat_proyeccion );
   CError();
}
//-----------------------------------------------------------------------------

void CauceProgramable::fijarEvalText( const bool nue_eval_text, const int nue_text_id  )
{
   CError();
   eval_text = nue_eval_text ;

   if ( eval_text )
   {
      assert( -1 < nue_text_id );
      glEnable( GL_TEXTURE_2D );  // necesario ? (probar)
      glActiveTexture( GL_TEXTURE0 ) ; // creo que necesario en el cauce prog., probar
      glBindTexture( GL_TEXTURE_2D, nue_text_id );
      glUniform1i( loc_eval_text, 1 );
   }
   else
   {
      glDisable( GL_TEXTURE_2D );  // necesario ? (probar)
      glUniform1i( loc_eval_text, 0 );
   }
   CError();
}
//-----------------------------------------------------------------------------

void CauceProgramable::fijarTipoGCT( const int nue_tipo_gct,
                           const float * coefs_s, const float * coefs_t )
{
   assert( 0 <= nue_tipo_gct && nue_tipo_gct <= 2 );

   tipo_gct = nue_tipo_gct ;

   if ( tipo_gct != 0 )
   {
      assert( coefs_s != nullptr );
      assert( coefs_t != nullptr );
   }

   glUniform1i( loc_tipo_gct, tipo_gct  ? 1 : 0 );

   if ( tipo_gct == 1 || tipo_gct == 2 )
   {
      glUniform4fv( loc_coefs_s, 1, coefs_s );
      glUniform4fv( loc_coefs_t, 1, coefs_t );
   }
   CError();
}


// -----------------------------------------------------------------------------

void CauceProgramable::fijarEvalMIL( const bool nue_eval_mil  )
{
   eval_mil = nue_eval_mil ;
   glUseProgram( id_prog );
   glUniform1i( loc_eval_mil, eval_mil ? 1 : 0 );
   CError();

}
// -----------------------------------------------------------------------------

void CauceProgramable::fijarModoSombrPlano( const bool nue_sombr_plano )
{
   sombr_plano = nue_sombr_plano ;
   glUseProgram( id_prog );
   glUniform1i( loc_sombr_plano, sombr_plano ? 1 : 0 );
   CError();
}

// -----------------------------------------------------------------------------

void CauceProgramable::fijarParamsMIL( const Tupla3f &ambiente, const Tupla3f &difuso,
                                       const Tupla3f &especular, const float exp  )
{
   assert( -1 < loc_mil_ka );
   assert( -1 < loc_mil_kd );
   assert( -1 < loc_mil_ks );
   assert( -1 < loc_mil_exp );

   glUseProgram( id_prog );
   glUniform3fv( loc_mil_ka, 1, ambiente );
   glUniform3fv( loc_mil_kd, 1, difuso );
   glUniform3fv( loc_mil_ks, 1, especular );
   glUniform1f ( loc_mil_exp, exp );

   CError();
}

// -----------------------------------------------------------------------------

void CauceProgramable::fijarFuentesLuz( const std::vector<Tupla3f> & color,
                                        const std::vector<Tupla4f> & pos_dir_wc  )
{
   const unsigned nl = color.size();
   assert( 0 < nl && nl <= maxNumFuentesLuz() );
   assert( nl == pos_dir_wc.size() );

   assert( 0 < id_prog );
   glUseProgram( id_prog );

   std::vector<Tupla4f> pos_dir_ec ;

   for( unsigned i = 0 ; i < nl ; i++ )
      pos_dir_ec.push_back( mat_vista * pos_dir_wc[i ]);

   glUniform1i( loc_num_luces, nl );
   glUniform3fv( loc_color_luz, nl, (const float *)color.data() );
   glUniform4fv( loc_pos_dir_luz_ec, nl, (const float *)pos_dir_ec.data() );
}
// -----------------------------------------------------------------------------
// inserta copia de la matriz de modelado actual (en el tope de la pila)

void CauceProgramable::pushMM()
{
   pila_mat_modelado.push_back( mat_modelado );
   pila_mat_modelado_nor.push_back( mat_modelado_nor );
}

// -----------------------------------------------------------------------------
// extrae tope de la pila y sustituye la matriz de modelado actual

void CauceProgramable::popMM()
{
   const unsigned n = pila_mat_modelado.size() ;
   assert( 0 < n );
   assert( n == pila_mat_modelado_nor.size() );

   mat_modelado     = pila_mat_modelado[n-1] ;
   mat_modelado_nor = pila_mat_modelado_nor[n-1] ;

   pila_mat_modelado.pop_back();
   pila_mat_modelado_nor.pop_back();

   actualizarMatricesMN();
}
// -----------------------------------------------------------------------------
// compone matriz de modelado actual con la matriz dada.

void CauceProgramable::compMM( const Matriz4f & mat_componer )
{
   const Matriz4f mat_componer_nor = MAT_Transpuesta3x3( MAT_Inversa( mat_componer ) );

   mat_modelado     = mat_modelado * mat_componer ;
   mat_modelado_nor = mat_modelado_nor * mat_componer_nor ;

   actualizarMatricesMN();
}
// -----------------------------------------------------------------------------

void CauceProgramable::actualizarMatricesMN()
{
   assert( 0 < id_prog );
   assert( -1 < loc_mat_modelado );
   assert( -1 < loc_mat_modelado_nor );

   glUseProgram( id_prog );
   glUniformMatrix4fv( loc_mat_modelado,     1, GL_FALSE, mat_modelado );
   glUniformMatrix4fv( loc_mat_modelado_nor, 1, GL_FALSE, mat_modelado_nor );
   CError();
}
// -----------------------------------------------------------------------------
// lee el nombre o descripción del cauce

std::string CauceProgramable::descripcion()
{
   return "programable" ;
}
