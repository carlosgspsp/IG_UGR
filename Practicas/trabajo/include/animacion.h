// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Informática Gráfica - curso 2020-21
// ** Declaración de funciones de animación
// **
// *********************************************************************

#ifndef ANIMACION_H
#define ANIMACION_H

#include "objeto3d.h"
#include "practicas.h"

// #####################################
// Funciones usadas para animacion:
// ####################################



// Función  que actuliza periodicamente el estado del objeto
// Si el objeto no tiene parámetros: no hace nada y devuelve false
// En otro caso:
//    calcula el tiempo real transcurrido desde la última llamada a esta función
//    actualiza el estado del objeto, usando ese tiempo transcurrido

bool ActualizarEstado( Objeto3D & objeto ) ;

// devuelve true sii las nimaciones están activadas
bool AnimacionesActivadas() ;

// gestiona una tecla correspondiente a animaciones (se llama cuando se pulsa una
// tecla la vez que la tecla 'A' está pulsada)
bool ProcesarTeclaAnimacion( Objeto3D * objeto, int glfw_key ) ;

#endif // ANIMACION_H
