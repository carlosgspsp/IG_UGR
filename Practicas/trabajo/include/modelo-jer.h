// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)


#ifndef MODELO_JER_HPP
#define MODELO_JER_HPP
#include "grafo-escena.h"


class Mike : public NodoGrafoEscena {
public:
	Mike();
	 virtual unsigned leerNumParametros() const;
protected:
	virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);

private:
	Matriz4f* pm_rot_parpado = nullptr;
	Matriz4f* pm_rot_brazo_izq = nullptr;
	Matriz4f* pm_rot_brazo_der = nullptr;
	Matriz4f* pm_rot_pierna_izq = nullptr;
	Matriz4f* pm_rot_pierna_der = nullptr;
	Matriz4f* pm_mike_anda = nullptr;
	Matriz4f* pm_mike_gira = nullptr;
};

class Cuerpo : public NodoGrafoEscena {
public:
	Cuerpo();
};

class Ojo : public NodoGrafoEscena {
public:
	Ojo();
	Matriz4f* getMatrizParpado();
private:
	Matriz4f* ptr_matriz_parpado;
};

class Pupila : public NodoGrafoEscena {
public:
	Pupila();
};

class Iris : public NodoGrafoEscena {
public:
	Iris();
};

class Parpado : public NodoGrafoEscena {
public:
	Parpado();
	Matriz4f* getMatrizRotacion();

private:
	
	unsigned ind;
};

class BrazoIzq : public NodoGrafoEscena {
public:
	BrazoIzq();
	Matriz4f* getMatrizBrazoIzquierdo();
private:

	unsigned ind;
};

class BrazoDer : public NodoGrafoEscena {
public:
	BrazoDer();
	Matriz4f* getMatrizBrazoDerecho();
private:

	unsigned ind;
};

class PiernaIzq : public NodoGrafoEscena {
public:
	PiernaIzq();
	Matriz4f* getMatrizPiernaIzquierda();
private:

	unsigned ind;
};

class PiernaDer : public NodoGrafoEscena {
public:
	PiernaDer();
	Matriz4f* getMatrizPiernaDerecha();
private:

	unsigned ind;
};

class CuernoIzq : public NodoGrafoEscena {
public:
	CuernoIzq();

};

class CuernoDer : public NodoGrafoEscena {
public:
	CuernoDer();

};
#endif // MODELO_JER_HPP