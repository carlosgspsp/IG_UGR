// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2020-21
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

//#include <set>   // std::set
#include "ig-aux.h"
#include "tuplasg.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"
#include <iostream>
#include <fstream>


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....


   for (int i = 0; i < triangulos.size(); i++)
   {
       Tupla3f vector_a, vector_b, vector_perpendicular, vector_normalizado;

       vector_a = vertices[triangulos[i](1)] - vertices[triangulos[i](0)];
       vector_b = vertices[triangulos[i](2)] - vertices[triangulos[i](0)];
     
       vector_perpendicular = vector_a.cross(vector_b);
       vector_normalizado = vector_perpendicular.normalized();
       nor_tri.push_back(vector_normalizado);

   }



}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......
    calcularNormalesTriangulos();
    Tupla3f suma;
    for (int i = 0; i < vertices.size(); i++)
    {
        for (int j = 0; j < triangulos.size(); j++)
        {
            if (i == triangulos[j](0) || i == triangulos[j](1) || i == triangulos[j](2))
            {
                suma = suma + nor_tri[j];
            }
        }
        if (suma.lengthSq() == 0)
           nor_ver.push_back({ 0.0,0.0,0.0 });
        else
        {
            suma = suma.normalized();
            nor_ver.push_back(suma);
            
        }
        suma = { 0.0,0.0,0.0 };
    }
        

}




// ----------------------------------------------------------------------------


void MallaInd::visualizarGL(ContextoVis& cv)
{

    using namespace std;
    assert(cv.cauce_act != nullptr);

    if (triangulos.size() == 0 || vertices.size() == 0)
    {
        cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush;
        return;
    }

    if (cv.visualizando_normales)
    {
        visualizarNormales();
    }

    // guardar el color previamente fijado
    const Tupla4f color_previo = leerFijarColVertsCauce(cv);

    // COMPLETAR: práctica 1: si el puntero 'array_verts' es nulo, crear el objeto ArrayVerts
    //   * en el constructor se dan los datos de la tabla de coordenadas de vértices (tabla 'vertices')
    //   * después hay que invocar a 'fijarIndices', usando el formato y datos de la tabla de triángulos ('triangulos')
    //   * si las tablas 'col_ver', 'cc_tt_ver' o 'nor_ver' no están vacías, hay que invocar los métodos 
    //     'fijarColores', 'fijarCoordText' y 'fijarNormales', como corresponda.
    if (array_verts == nullptr) {

        array_verts = new ArrayVertices(GL_FLOAT, 3, vertices.size(), vertices.data());
    

        array_verts->fijarIndices(GL_UNSIGNED_INT, triangulos.size() * 3, triangulos.data());

        if (!col_ver.empty()) {
            array_verts->fijarColores(GL_FLOAT, 3, col_ver.data());
        }

        if (!cc_tt_ver.empty()) {
            array_verts->fijarCoordText(GL_FLOAT, 2, cc_tt_ver.data());
        }

        if (!nor_ver.empty()) {
            array_verts->fijarNormales(GL_FLOAT, nor_ver.data());
        }

    }

    
   // COMPLETAR: práctica 1: visualizar según el modo (en 'cv.modo_envio')
   //   ** inmediato begin/end       : usar método 'visualizarGL_MI_BVE' de 'ArrayVerts'
   //   ** inmediato con una llamada : usar método 'visualizarGL_MI_DAE' de 'ArrayVerts'
   //   ** diferido (con un VAO)     : usar método 'visualizarGL_MD_VAO' de 'ArrayVerts'
   // (en cualquier caso hay que pasar como parámetro el tipo de primitiva adecuada a una malla de triángulos).
   // .....
   switch (cv.modo_envio) {
   case ModosEnvio::inmediato_begin_end:
       array_verts->visualizarGL_MI_BVE(GL_TRIANGLES);
           break;
   case ModosEnvio::inmediato_drawelements:
       array_verts->visualizarGL_MI_DAE(GL_TRIANGLES);
           break;
   case ModosEnvio::diferido_vao:
       array_verts->visualizarGL_MD_VAO(GL_TRIANGLES);
           break;
   }


   // restaurar el color previamente fijado
   glColor4fv( color_previo );

}






// *****************************************************************************


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................

   LeerPLY(nombre_arch, vertices, triangulos);
   MallaInd("Archivo PLY");


   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();


}

void MallaInd::visualizarNormales()
{
    using namespace std;

    if (nor_ver.size() == 0)
    {
        cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl;
        return;
    }
    if (array_verts_normales == nullptr)
    {
        for (unsigned i = 0; i < vertices.size(); i++)
        {
            segmentos_normales.push_back(vertices[i]);
            segmentos_normales.push_back(vertices[i] + 0.35f * (nor_ver[i]));
        }
        array_verts_normales = new ArrayVertices(GL_FLOAT, 3, segmentos_normales.size(), segmentos_normales.data());
    }

    array_verts_normales->visualizarGL_MI_DAE(GL_LINES);
    CError();
}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;
   
}
// -----------------------------------------------------------------------------------------------

// Clase 'Tetraedro

Tetraedro::Tetraedro()
    : MallaInd("Piramide de 4 vértices")
{
    
    vertices =
    { { -1.0, -1.0, -1.0 }, // 0
       { -1.0, -1.0, +1.0 }, // 1
       { -1.0, +1.0, -1.0 }, // 2
       { +1.0, +1.0, +1.0 } // 3
    };



    triangulos =
    { {0,1,3}, {0,3,2}, // X-
      {3,1,0}, {2,3,1} // X+ (+4)
    };

    ponerColor({ 0.75, 0.2, 0.69 });
    
}
// -----------------------------------------------------------------------------------------------
// Clase 'Cubo de Colores

CuboColores::CuboColores()
    : MallaInd("cubo de colores con 8 vértices")
{

    vertices =
    { { -1.0, -1.0, -1.0 }, // 0
       { -1.0, -1.0, +1.0 }, // 1
       { -1.0, +1.0, -1.0 }, // 2
       { -1.0, +1.0, +1.0 }, // 3
       { +1.0, -1.0, -1.0 }, // 4
       { +1.0, -1.0, +1.0 }, // 5
       { +1.0, +1.0, -1.0 }, // 6
       { +1.0, +1.0, +1.0 }, // 7
    };



    triangulos =
    { {0,1,3}, {0,3,2}, // X-
       {4,7,5}, {4,6,7}, // X+ (+4)

       {0,5,1}, {0,4,5}, // Y-
       {2,3,7}, {2,7,6}, // Y+ (+2)

       {0,6,4}, {0,2,6}, // Z-
       {1,5,7}, {1,7,3}  // Z+ (+1)
    };

    col_ver =
 { { 0.0, 0.0, 0.0 }, // 0
   { 0.0, 0.0, 1.0 }, // 1
   { 0.0, 1.0, 0.0 }, // 2
   { 0.0, 1.0, 1.0 }, // 3
   { 1.0, 0.0, 0.0 }, // 4
   { 1.0, 0.0, 1.0 }, // 5
   { 1.0, 1.0, 0.0 }, // 6
   { 1.0, 1.0, 1.0 }, // 7
    };
}

CasaX::CasaX()
    : MallaInd("cubo 8 vértices")
{

    vertices =
    { { 0.0, 0.0, 0.0 }, // 0
     { 0.0, 0.0, +0.8 }, // 1
     { 0.0, +0.6, 0.0 }, // 2
     { 0.0, +0.6, +0.8 }, // 3
     { +1.0, 0.0, 0.0 }, // 4
     { +1.0, 0.0, +0.8 }, // 5
     { +1.0, +0.6, 0.0 }, // 6
     { +1.0, +0.6, +0.8 }, // 7
     { 0.0 , 1.0, 0.4 },    //8
     { 1.0, 1.0, 0.4 }     //9

    };



    triangulos =
    { {0,1,3}, {0,3,2}, // X-
   {4,7,5}, {4,6,7}, // X+ (+4)

   {0,6,4}, {0,2,6}, // Z-
   {1,5,7}, {1,7,3},  // Z+ (+1)

    {2,3,8}, {2,6,9},
    {3,7,8},{6,7,9} ,
    {2,8,9},{8,9,7}

    };

    col_ver =
    { { 0.0, 0.0, 0.0 }, // 0
       { 0.0, 0.0, +0.8 }, // 1
       { 0.0, +0.6, 0.0 }, // 2
       { 0.0, +0.6, +0.8 }, // 3
       { +1.0, 0.0, 0.0 }, // 4
       { +1.0, 0.0, +0.8 }, // 5
       { +1.0, +0.6, 0.0 }, // 6
       { +1.0, +0.6, +0.8 }, // 7
        {0.0, 1.0, 0.4},
        {1.0, 1.0, 0.4}

    };


}



RejillaZ::RejillaZ(unsigned n, unsigned m) : MallaInd("cubo 8 vértices")
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            
            float x = (1.0*i)/(n-1);
            float y = (1.0*j)/(m-1);
            float z = 0.0;

            vertices.push_back({ x, y ,z });
        }
    int tam = m;

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < m - 1; j++)
        {
            int k = i * tam + j;
            triangulos.push_back({ k, k + tam, k + tam + 1 });
            triangulos.push_back({ k, k + tam + 1, k + 1 });
        }
    for (unsigned i = 0; i < vertices.size(); i++) {
        col_ver.push_back(vertices.at(i));
    }
}


Cubo24::Cubo24() : MallaInd("cubo 24 vértices") {
    using namespace std;
    vertices =
    {  { -1.0, -1.0, -1.0 }, // 0
       { -1.0, -1.0, +1.0 }, // 1
       { -1.0, +1.0, -1.0 }, // 2
       { -1.0, +1.0, +1.0 }, // 3
       { +1.0, -1.0, -1.0 }, // 4
       { +1.0, -1.0, +1.0 }, // 5
       { +1.0, +1.0, -1.0 }, // 6
       { +1.0, +1.0, +1.0 }, // 7
       { -1.0, -1.0, -1.0 }, // 8 (0)
       { -1.0, -1.0, +1.0 }, // 9 (1)
       { -1.0, +1.0, -1.0 }, // 10 (2)
       { -1.0, +1.0, +1.0 }, // 11 (3)
       { +1.0, -1.0, -1.0 }, // 12 (4)
       { +1.0, -1.0, +1.0 }, // 13 (5)
       { +1.0, +1.0, -1.0 }, // 14 (6)
       { +1.0, +1.0, +1.0 }, // 15 (7)
       { -1.0, -1.0, -1.0 }, // 16 (0)
       { -1.0, -1.0, +1.0 }, // 17 (1)
       { -1.0, +1.0, -1.0 }, // 18 (2)
       { -1.0, +1.0, +1.0 }, // 19 (3)
       { +1.0, -1.0, -1.0 }, // 20 (4)
       { +1.0, -1.0, +1.0 }, // 21 (5)
       { +1.0, +1.0, -1.0 }, // 22 (6)
       { +1.0, +1.0, +1.0 }, // 23 (7)
    };


    triangulos =
    { {0,1,3}, {0,3,2}, // X-
       {4,7,5}, {4,6,7}, // X+ (+4)

       {8,13,9}, {8,12,13}, // Y-
       {10,11,15}, {10,15,14}, // Y+ (+2)

       {16,22,20}, {16,18,22}, // Z-
       {17,21,23}, {17,23,19}  // Z+ (+1)
    };


    cc_tt_ver =
    {
       {0.0,1.0}, //0
       {1.0,1.0}, //1
       {0.0,0.0}, //2
       {1.0,0.0}, //3
       {1.0,1.0}, //4
       {0.0,1.0}, //5
       {1.0,0.0}, //6
       {0.0,0.0}, //7
       {0.0,1.0}, //8
       {0.0,0.0}, //9
       {0.0,0.0}, //10
       {0.0,1.0}, //11
       {1.0,1.0}, //12
       {1.0,0.0}, //13
       {1.0,0.0}, //14
       {1.0,1.0}, //15
       {1.0,1.0}, //16
       {0.0,1.0}, //17
       {1.0,0.0}, //18
       {0.0,0.0}, //19
       {0.0,1.0}, //20
       {1.0,1.0}, //21
       {0.0,0.0}, //22
       {1.0,0.0}  //23
    };

    calcularNormales();

}


// -----------------------------------------------------------------------------------------------
