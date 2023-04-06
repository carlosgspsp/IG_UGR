// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)

#ifndef LATAPEONES_HPP
#define LATAPEONES_HPP

#include "grafo-escena.h"
#include <string>



class  LataPeones : public NodoGrafoEscena 
{
public:
	LataPeones();

};

class VariasLataPeones : public NodoGrafoEscena
{
public:
	VariasLataPeones();

};

class PeonMadera : public NodoGrafoEscena
{
public:
	PeonMadera();

};

class PeonBlanco : public NodoGrafoEscena
{
public:
	PeonBlanco();

};

class PeonNegro : public NodoGrafoEscena
{
public:
	PeonNegro();

};

class LataCola : public NodoGrafoEscena
{
public:
	LataCola(char * textura);

};

class LataPepsi : public NodoGrafoEscena
{
public:
	LataPepsi(char* textura);

};

class LataUGR : public NodoGrafoEscena
{
public:
	LataUGR(char* textura);

};

class CuerpoLata : public NodoGrafoEscena
{
public:
	CuerpoLata(char * textura);

};

class BaseLata : public NodoGrafoEscena
{
public:
	BaseLata();

};

class TapaLata : public NodoGrafoEscena
{
public:
	TapaLata();

};

class NodoCubo24 : public NodoGrafoEscena
{
public:
	NodoCubo24();

};



#endif // LATAPEONES_HPP