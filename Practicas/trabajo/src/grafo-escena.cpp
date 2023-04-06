// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
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
#include "matrices-tr.h"
#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 3: recorrer las entradas y visualizar cada nodo.
   // ........
    // guarda modelview actual
    const Tupla4f color_previo = leerFijarColVertsCauce(cv);
    cv.cauce_act->pushMM();

    Material* material_pre = cv.iluminacion ? cv.material_act : nullptr;
    // recorrer todas las entradas del array que hay en el nodo:
    for (unsigned i = 0; i < entradas.size(); i++)
        switch (entradas[i].tipo)
        {
        case TipoEntNGE::objeto: // entrada objeto:
            entradas[i].objeto->visualizarGL(cv); // visualizar objeto
            break;
        case TipoEntNGE::transformacion: // entrada transf.:
            cv.cauce_act->compMM(*(entradas[i].matriz)); // componer matriz
            break;
        case TipoEntNGE::material:
            if (cv.iluminacion && !cv.modo_seleccion)
            {
                cv.material_act = entradas[i].material;
                cv.material_act->activar(*cv.cauce_act);
            }
            break;
        }
    // restaura modelview guardada
  
    glColor4fv(color_previo);

    if (material_pre != nullptr)
    {
        cv.material_act = material_pre;
        cv.material_act->activar(*cv.cauce_act);

    }

    cv.cauce_act->popMM();

   // COMPLETAR: práctica 4: en la práctica 4, si 'cv.iluminacion' es 'true',
   // se deben de gestionar los materiales:
   //   1. guardar puntero al material activo al inicio (está en cv.material_act)
   //   2. si una entrada des de tipo material, activarlo y actualizar 'cv.material_act'
   //   3. al finalizar, restaurar el material activo al inicio (si es distinto del actual)



}




// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
// ignora el colo o identificador del nodo, ignora las entradas de tipo material
// (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)




// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
    entradas.push_back(entrada);

   return entradas.size()-1 ; // sustituir por lo que corresponda ....

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: devolver puntero la matriz en ese índice
   //   (debe de dar error y abortar si no hay una matriz en esa entrada)
   // ........(sustituir 'return nullptr' por lo que corresponda)
    assert(indice < entradas.size());
    assert(entradas[indice].tipo == TipoEntNGE::transformacion);
    assert(entradas[indice].matriz != nullptr);
    return entradas[indice].matriz;


}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
   

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

        if (!centro_calculado) {
            Matriz4f mat = MAT_Ident();
            Tupla3f suma = { 0,0,0 };
            float num_centros = 0;

            for (unsigned i = 0; i < entradas.size(); i++) {
                if (entradas[i].tipo == TipoEntNGE::objeto) {
                    entradas[i].objeto->calcularCentroOC();
                    suma = suma + (mat * entradas[i].objeto->leerCentroOC());
                    num_centros++;
                }
                else if (entradas[i].tipo == TipoEntNGE::transformacion) {
                    mat = mat * (*entradas[i].matriz);
                }
            }

            Tupla3f centro = suma / num_centros;

            ponerCentroOC(centro);
            centro_calculado = true;
        }
    }

// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   assert( 0 < ident_busc );

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........
   calcularCentroOC();

   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........
   if (leerIdentificador() == ident_busc)
   {
       centro_wc = mmodelado * leerCentroOC();
       if (objeto == nullptr)
       {
           cout << "\n El identificador del objeto es nulo" << endl;
       }
       *objeto = this;
       return true;
   }

   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........
   Matriz4f mat = mmodelado;


   bool encontrado = false;
   for (int i = 0; i < entradas.size() && !encontrado; i++)
   {
       if (entradas[i].tipo == TipoEntNGE::objeto)
           encontrado = entradas[i].objeto->buscarObjeto(ident_busc, mat, objeto, centro_wc);
       else if (entradas[i].tipo == TipoEntNGE::transformacion)
           mat = mat * (*entradas[i].matriz);
   }


   // ni este nodo ni ningún hijo es el buscado: terminar
   return encontrado;
}


GrafoCubos::GrafoCubos() {


    agregar(new Cara1());
    agregar(new Cara2());
   agregar(new Cara3());
    agregar(new Cara4());
   agregar(new Cara5());
    agregar(new Cara6());

}

unsigned GrafoCubos::leerNumParametros() const {
	return 1;
}
void GrafoCubos::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
	float ang;
	switch (iParam) {
	case 0:

		break;

	}
}

Cara1::Cara1() {
    agregar(MAT_Traslacion(-0.5, -0.5, 0.5));
    agregar(new RejillaZ(7, 7));
    agregar(MAT_Traslacion(0.5, -0.5, -0.5));
    agregar(MAT_Escalado(0.2, 0.5, 0.2));
    agregar(new Cubo());
    
}

Cara2::Cara2() {
    agregar(MAT_Traslacion(-0.5, -0.5, -0.5));
    agregar(new RejillaZ(7, 7));
    agregar(MAT_Traslacion(0.5, 1.5, 0.5));
    agregar(MAT_Escalado(0.2, 0.5, 0.2));
    agregar(new Cubo());
}

Cara3::Cara3() {
    agregar(MAT_Traslacion(-0.5, 0.5, 0.5));
    agregar(MAT_Rotacion(-90, 1, 0, 0));
    agregar(new RejillaZ(7, 7));
    agregar(MAT_Traslacion(0.5, 1.5, -0.5));
    agregar(MAT_Escalado(0.2, 0.5, 0.2));
    agregar(new Cubo());
}

Cara4::Cara4() {
    agregar(MAT_Traslacion(-0.5, -0.5, 0.5));
    agregar(MAT_Rotacion(-90, 1, 0, 0));
    agregar(new RejillaZ(7, 7));
    agregar(MAT_Traslacion(0.5, -0.5, 0.5));
    agregar(MAT_Escalado(0.2, 0.5, 0.2));
    agregar(new Cubo());
}

Cara5::Cara5() {
    agregar(MAT_Traslacion(-0.5, -0.5, -0.5));
    agregar(MAT_Rotacion(-90, 0, 1, 0));
    agregar(new RejillaZ(7, 7));
    agregar(MAT_Traslacion(0.5, 0.5, 0.5));
    agregar(MAT_Rotacion(-90, 1, 0, 0));
    agregar(MAT_Escalado(0.2, 0.5, 0.2));
    agregar(new Cubo());
}

Cara6::Cara6() {
    agregar(MAT_Traslacion(0.5, -0.5, -0.5));
    agregar(MAT_Rotacion(-90, 0, 1, 0));
    agregar(new RejillaZ(7, 7));
}

GrafoEsferasP5::GrafoEsferasP5()
{
    const unsigned
        n_filas_esferas = 8,
        n_esferas_x_fila = 5;

    agregar(MAT_Escalado(2.5 / n_esferas_x_fila, 2.5 / n_esferas_x_fila, 2.5 / n_esferas_x_fila));

    for (unsigned i = 0; i < n_filas_esferas; i++)
    {
        NodoGrafoEscena* fila_esferas = new NodoGrafoEscena();
        fila_esferas->agregar(MAT_Traslacion(3.0, 0.0, 0.0));
        fila_esferas->ponerIdentificador(i);
        for (unsigned j = 0; j < n_esferas_x_fila; j++)
        {
            MallaInd* esfera = new Esfera(20, 40);
            fila_esferas->agregar(MAT_Traslacion(2.5, 0.0, 0.0));
            ponerIdentificador(j);
            esfera->ponerIdentificador(j);
            fila_esferas->agregar(esfera);
        }
        agregar(fila_esferas);
        agregar(MAT_Rotacion(360.0 / n_filas_esferas, { 0.0, 1.0, 0.0 }));
    }

    ponerNombre("Grafo de Esferas");
    //ponerIdentificador(20);
}