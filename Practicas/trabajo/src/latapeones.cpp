// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)

#include "latapeones.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include <string>

LataPeones::LataPeones()
{
	agregar(new LataCola("./imgs/lata-coke.jpg"));
	agregar(new PeonMadera);
	agregar(new PeonBlanco);
	agregar(new PeonNegro);
}


VariasLataPeones::VariasLataPeones()
{
	agregar(new LataCola("./imgs/lata-coke.jpg"));
	agregar(new LataPepsi("./imgs/lata-pepsi.jpg"));
	agregar(new LataUGR("./imgs/window-icon.jpg"));
	agregar(new PeonMadera);
	agregar(new PeonBlanco);
	agregar(new PeonNegro);
}


PeonMadera::PeonMadera()
{
	agregar(MAT_Traslacion(0.0, 1.5, 0.0));
	agregar(new Material(new Textura("./imgs/text-madera.jpg"), 0.5, 0.5, 0.5, 60));
	agregar(new MallaRevolPLY("../recursos/plys/peon.ply", 100));

	ponerNombre("Peón Madera");
	ponerIdentificador(1);

}

PeonBlanco::PeonBlanco()
{
	ponerColor({ 1.0,1.0,1.0 });
	agregar(MAT_Traslacion(-2.5, 1.5, 0.0));
	agregar(new MallaRevolPLY("../recursos/plys/peon.ply", 100));
	agregar(new Material(0.5, 0.5, 0.5, 0.5));

	ponerNombre("Peon Blanco");
	ponerIdentificador(2);
}

PeonNegro::PeonNegro()
{
	ponerColor({ 0.0,0.0,0.0 });
	agregar(MAT_Traslacion(2.5, 1.5, 0.0));
	agregar(new MallaRevolPLY("../recursos/plys/peon.ply", 100));
	agregar(new Material( 0.5, 0.5, 0.5, 0.5));

	ponerNombre("Peon Negro");
	ponerIdentificador(3);
}





LataCola::LataCola(char * textura)
{
	agregar(MAT_Escalado(5.0, 5.0, 5.0));
	agregar(MAT_Traslacion(1.0, 0.0, 1.0));
	agregar(new CuerpoLata(textura));
	agregar(new BaseLata);
	agregar(new TapaLata);

	ponerNombre("Lata de Coca-Cola");
	ponerIdentificador(4);
}

LataPepsi::LataPepsi(char* textura)
{
	agregar(MAT_Escalado(5.0, 5.0, 5.0));
	agregar(MAT_Traslacion(-1.0, 0.0, 1.0));
	agregar(new CuerpoLata(textura));
	agregar(new BaseLata);
	agregar(new TapaLata);

	ponerNombre("Lata de Pepsi");
	ponerIdentificador(5);
}

LataUGR::LataUGR(char* textura)
{
	agregar(MAT_Escalado(5.0, 5.0, 5.0));
	agregar(MAT_Traslacion(0.0, 0.0, 1.0));
	agregar(new CuerpoLata(textura));
	agregar(new BaseLata);
	agregar(new TapaLata);

	ponerNombre("Lata de la UGR");
	ponerIdentificador(6);

}

CuerpoLata::CuerpoLata(char * textura)
{
	agregar(new Material(new Textura(textura), 0.5, 0.5, 0.5, 60));
	agregar(new MallaRevolPLY("../recursos/plys/lata-pcue.ply", 100));
	
}

BaseLata::BaseLata()
{
	agregar(new Material(0.2, 0.8, 0.4, 20));
	agregar(new MallaRevolPLY("../recursos/plys/lata-pinf.ply", 100));
}

TapaLata::TapaLata()
{
	agregar(new Material(0.2, 0.8, 0.4, 20));
	agregar(new MallaRevolPLY("../recursos/plys/lata-psup.ply", 100));
}

NodoCubo24::NodoCubo24()
{
	agregar(new Material(new Textura("./imgs/window-icon.jpg"), 0.5, 0.5, 0.5, 60));
	agregar(MAT_Traslacion(0.0, 0.0, 0.0));
	agregar(new Cubo24);
	
}

