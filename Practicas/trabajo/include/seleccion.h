// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)
#ifndef SELECCION_H
#define SELECCION_H

#include "objeto3d.h"
#include "practicas.h"
#include "escena.h"

// #####################################
// Funciones usadas para selección:
// ####################################


// Función principal de selección, se llama al hacer click con el botón izquierdo
//
// (x,y)       = posición donde se ha hecho click en coordenadas del sistema de ventanas (enteras)
// objeto_raiz = objeto raiz (no subobjeto de otro) que se está visualizando en la ventana
// cv_dib      = contexto de visualización usado para dibujar el objeto
bool Seleccion( int x, int y, Escena * escena, ContextoVis & cv_dib ) ;

// fijar el color actual en el cauce usando un identificador entero
void FijarColVertsIdent( Cauce & cauce, const int ident )  ;

// leer un identificador entero codificado en el color de un pixel en el
// framebuffer activo actualmente
int LeerIdentEnPixel( int xpix, int ypix );

// devuelve el FBO actual usado para la selección
Framebuffer * FBOActual() ;



#endif // SELECCION_H
