// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)


#include "modelo-jer.h"
#include "malla-ind.h"
#include "malla-revol.h"

	Mike::Mike() {
		unsigned ind_anda = agregar(MAT_Traslacion(0.0, 0.0, 0.0));
		unsigned ind_gira = agregar(MAT_Rotacion(0.0, 0.0, 1.0, 0.0));
		agregar(new Cuerpo());
		Ojo* ojo = new Ojo;
		agregar(ojo);
		pm_rot_parpado = ojo->getMatrizParpado();
		BrazoIzq* brazo_izquierda = new BrazoIzq;
		agregar(brazo_izquierda);
		pm_rot_brazo_izq = brazo_izquierda->getMatrizBrazoIzquierdo();
		BrazoDer* brazo_derecha = new BrazoDer;
		agregar(brazo_derecha);
		pm_rot_brazo_der = brazo_derecha->getMatrizBrazoDerecho();
		PiernaIzq* pierna_izquierda = new PiernaIzq;
		agregar(pierna_izquierda);
		pm_rot_pierna_izq = pierna_izquierda->getMatrizPiernaIzquierda();
		PiernaDer* pierna_derecha = new PiernaDer;
		agregar(pierna_derecha);
		pm_rot_pierna_der = pierna_derecha->getMatrizPiernaDerecha();
		pm_mike_anda = leerPtrMatriz(ind_anda);
		pm_mike_gira = leerPtrMatriz(ind_gira);
		agregar(new CuernoIzq);
		agregar(new CuernoDer);
		

		ponerNombre("Mike El Wacho");
		ponerIdentificador(0);
		
	}	

	unsigned Mike::leerNumParametros() const{
		return 3;
	}
	void Mike::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
		float ang;
		switch (iParam) {
		case 0: 
			ang = 90.0*sin(10.0*t_sec);
			*pm_rot_parpado = MAT_Rotacion(ang, 1, 0, 0);
			break;

		case 1:
			ang = 45.0 * sin(5.0 * t_sec);
			*pm_rot_brazo_izq = MAT_Rotacion(ang, 1, 0, 0);
			*pm_rot_brazo_der = MAT_Rotacion(-ang, 1, 0, 0);
			*pm_rot_pierna_izq = MAT_Rotacion(-ang, 1, 0, 0);
			*pm_rot_pierna_der = MAT_Rotacion(ang, 1, 0, 0);
			break;
		case 2:
			ang = 45.0 * sin(0.5 * t_sec);
			*pm_mike_anda = MAT_Traslacion(0.0, 0.0, ang);
			if (ang >= 44.8) {
				*pm_mike_gira = MAT_Rotacion(180.0, 0.0, 1.0, 0.0);
			}

			if (ang <= -44.8) {
				*pm_mike_gira = MAT_Rotacion(0.0, 0.0, 1.0, 0.0);
			}
			
			break;
		}
	}

	Cuerpo::Cuerpo() {
		agregar(new Material(new Textura("../recursos/imgs/mike.jpg"), 0.5, 0.5, 0.5, 60));
		ponerColor({ 0.71, 0.84, 0.26 });
		agregar(MAT_Escalado(3.0, 3.0, 3.0));
		agregar(new Esfera(20, 40));

		ponerNombre("Cuerpo de Mike");
		ponerIdentificador(7);
	}

	Ojo::Ojo() {

		ponerColor({ 1.0,1.0,1.0 });
		agregar(MAT_Traslacion(0.0, 0.7, 2.2));
		agregar(new Esfera(20,40));
		agregar(new Pupila());
		agregar(new Iris());
		Parpado* parpado = new Parpado;
		agregar(parpado);
		ptr_matriz_parpado = parpado->getMatrizRotacion();

		ponerNombre("Ojo de Mike");
		ponerIdentificador(8);
	}

	Matriz4f* Ojo::getMatrizParpado() {
		return ptr_matriz_parpado;
	}

	Iris::Iris() {
		ponerColor({ 0.3647,0.7569,0.7255 });
		agregar(MAT_Traslacion(0.0, 0.0, 0.65));
		agregar(MAT_Escalado(0.5, 0.5, 0.4));
		

		agregar(new Esfera(20, 40));
	}

	Pupila::Pupila(){
	ponerColor({ 0.0,0.0,0.0 });
	agregar(MAT_Traslacion(0.0, 0.0, 1.0));
	agregar(MAT_Escalado(0.2, 0.2, 0.1));
	agregar(new Esfera(20,40));
	
	}

	Parpado::Parpado() {
		agregar(new Material(new Textura("../recursos/imgs/mike.jpg"), 0.5, 0.5, 0.5, 60));
		ponerColor({ 0.64, 0.73, 0.33 });
		agregar(MAT_Traslacion(0.0, 0.0, 0.2));
		agregar(MAT_Escalado(1.0, 1.0, 1.0));
		ind = agregar(MAT_Rotacion(-50, 1, 0, 0));
		agregar(new SemiEsfera(20, 40));

	}

	Matriz4f* Parpado::getMatrizRotacion() {
		return leerPtrMatriz(ind);
	}

	BrazoIzq::BrazoIzq() {
		agregar(new Material(new Textura("../recursos/imgs/mike.jpg"), 0.5, 0.5, 0.5, 60));
		ponerColor({ 0.71, 0.84, 0.26 });
		agregar(MAT_Traslacion(3.0, 0.5, 0.0));
		ind = agregar(MAT_Rotacion(-25, 1, 0, 0));
		agregar(MAT_Escalado(0.5, -4.5, 0.5));
		agregar(new Cilindro(20, 40));

		ponerNombre("Brazo Izquierdo de Mike");
		ponerIdentificador(9);
	}

	Matriz4f* BrazoIzq::getMatrizBrazoIzquierdo() {
		return leerPtrMatriz(ind);
	}

	BrazoDer::BrazoDer() {
		agregar(new Material(new Textura("../recursos/imgs/mike.jpg"), 0.5, 0.5, 0.5, 60));
		ponerColor({ 0.71, 0.84, 0.26 });
		agregar(MAT_Traslacion(-3.0, 0.5, 0.0));
		ind = agregar(MAT_Rotacion(-25, 1, 0, 0));
		agregar(MAT_Escalado(0.5, -4.5, 0.5));
		agregar(new Cilindro(20, 40));

		ponerNombre("Brazo Derecho de Mike");
		ponerIdentificador(10);

	}

	Matriz4f* BrazoDer::getMatrizBrazoDerecho() {
		return leerPtrMatriz(ind);
	}

	PiernaIzq::PiernaIzq() {
		agregar(new Material(new Textura("../recursos/imgs/mike.jpg"), 0.5, 0.5, 0.5, 60));
		ponerColor({ 0.71, 0.84, 0.26 });
		agregar(MAT_Traslacion(1.0, -2.5, 0.0));
		ind = agregar(MAT_Rotacion(0.0, 1, 0, 0));
		agregar(MAT_Escalado(0.5, -3.5, 0.5));
		agregar(new Cilindro(20, 40));

		ponerNombre("Pierna Izquierda de Mike");
		ponerIdentificador(11);

	}

	Matriz4f* PiernaIzq::getMatrizPiernaIzquierda() {
		return leerPtrMatriz(ind);
	}

	PiernaDer::PiernaDer() {
		agregar(new Material(new Textura("../recursos/imgs/mike.jpg"), 0.5, 0.5, 0.5, 60));
		ponerColor({ 0.71, 0.84, 0.26 });
		agregar(MAT_Traslacion(-1.0, -2.5, 0.0));
		ind = agregar(MAT_Rotacion(0.0, 1, 0, 0));
		agregar(MAT_Escalado(0.5, -3.5, 0.5));

		agregar(new Cilindro(20, 40));

		ponerNombre("Pierna Derecha de Mike");
		ponerIdentificador(12);

	}

	Matriz4f* PiernaDer::getMatrizPiernaDerecha() {
		return leerPtrMatriz(ind);
	}

	CuernoIzq::CuernoIzq() {
		agregar(new Material(new Textura("../recursos/imgs/cuerno.jpg"), 0.5, 0.5, 0.5, 60));
		ponerColor({ 0.75, 0.65, 0.56 });
		agregar(MAT_Traslacion(-1.5, 2.5, 0.0));
		agregar(MAT_Rotacion(25.0, 0, 0, 1));
		agregar(MAT_Escalado(0.5, 1.0, 0.5));

		agregar(new Cono(20, 40));

		ponerNombre("Cuerno Izquierdo de Mike");
		ponerIdentificador(13);

	}



	CuernoDer::CuernoDer() {
		agregar(new Material(new Textura("../recursos/imgs/cuerno.jpg"), 0.5, 0.5, 0.5, 60));
		ponerColor({ 0.75, 0.65, 0.56 });
		agregar(MAT_Traslacion(1.5, 2.5, 0.0));
		agregar(MAT_Rotacion(-25.0, 0, 0, 1));
		agregar(MAT_Escalado(0.5, 1.0, 0.5));

		agregar(new Cono(20, 40));


		ponerNombre("Cuerno Derecho de Mike");
		ponerIdentificador(14);

	}
