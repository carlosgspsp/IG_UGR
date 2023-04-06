// Nombre: Carlos, Apellidos: García Segura, DNI/pasaporte: 45901676R (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "ig-aux.h"
#include "tuplasg.h"
#include "lector-ply.h"
#include "matrices-tr.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   // COMPLETAR: Práctica 2: completar: creación de la malla....
	int m = perfil.size();
	
	vector<Tupla3f> vectores, vectores_perpendiculares, perfil_normal; //"vectores" son los vectores entre los vertices, "vectores_perpendiculares" son m
	vector<double> distancias, valor_t;
	double denominador = 0;


	for (int i = 0; i < perfil.size() - 1; i++)
	{
		Tupla3f vector, vector_perpendicular;
		vector = perfil[i + 1] - perfil[i];
		
		vectores.push_back(vector);
		vector_perpendicular = MAT_Rotacion(-90.0, 0.0, 0.0, 1) * vector;
		
		if (vector.lengthSq() == 0)
			vectores_perpendiculares.push_back({ 0.0,0.0,0.0 });
		else
			vectores_perpendiculares.push_back(vector_perpendicular.normalized());


		distancias.push_back(sqrt(pow(vector(0), 2) + pow(vector(1), 2)));

	}

	perfil_normal.push_back(vectores_perpendiculares[0]);
	for (int i = 1; i <= vectores_perpendiculares.size()-1; i++)
	{
		perfil_normal.push_back(vectores_perpendiculares[i-1] + vectores_perpendiculares[i]);
	}
	perfil_normal.push_back(vectores_perpendiculares[vectores_perpendiculares.size()-1]);


	for (int i = 0; i < distancias.size()-1; i++)
	{
		denominador += distancias[i];
	}

	valor_t.push_back(0.0);
	for (int i = 0; i < distancias.size()-1; i++)
	{
		double numerador = 0.0;

			for (int j = 0; j < i; j++)
			{
				numerador += distancias[j];
			}
			valor_t.push_back(numerador / denominador);
		
	}
	valor_t.push_back(1.0);


	for (int i = 0; i < num_copias; i++)
			for (int j = 0; j < perfil.size(); j++)
			{
				double angulo = (2 * M_PI * i) / (num_copias - 1);
				float x = perfil[j](2) * cos(angulo) + perfil[j](0) * sin(angulo);
				float y = perfil[j](1);
				float z = perfil[j](0) * cos(angulo) - perfil[j](2) * sin(angulo);

				vertices.push_back({ x, y ,z });

				Tupla2f vertice_textura;
				vertice_textura(0) = (i*1.0) / (num_copias - 1.0);
				vertice_textura(1) = 1.0 - valor_t[j];
				cc_tt_ver.push_back(vertice_textura);

				
				Tupla3f normal;
				normal(0) = perfil_normal[j](2) * cos(angulo) + perfil_normal[j](0) * sin(angulo);
				normal(1) = perfil_normal[j](1);
				normal(2) = perfil_normal[j](0) * cos(angulo) - perfil_normal[j](2) * sin(angulo);
				nor_ver.push_back(normal);
				
			}
		
	for (int i = 0; i < num_copias-1; i++)
		for (int j = 0; j < m-1; j++)
		{
			int k = i * m + j;
			triangulos.push_back({ k, k + m, k + m + 1 });
			triangulos.push_back({ k, k + m + 1, k + 1 });
		}
	
}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
	const std::string& nombre_arch,
	const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   // ...........................
   std::vector<Tupla3f> perfil;
   LeerVerticesPLY(nombre_arch, perfil);
   inicializar(perfil, nperfiles);

}

Cilindro::Cilindro(const int num_verts_per, const unsigned nperfiles) : MallaRevol() {
	std::vector<Tupla3f> perfil;
		Tupla3f vertice;
		float radio = 1;
		float altura = 1;

		for (int i = 0; i < num_verts_per; i++)
		{
			vertice(0) = radio;
			vertice(1) = (altura / num_verts_per) * i;
			vertice(2) = 0;
			perfil.push_back(vertice);
		}
	
	inicializar(perfil, nperfiles);
}

Cono::Cono(const int num_verts_per, const unsigned nperfiles) : MallaRevol() {
	std::vector<Tupla3f> perfil;
	Tupla3f vertice;
	float radio = -1;
	float altura = 1;
	for (int i = 0; i <= num_verts_per; i++)
		{
			vertice(0) = (radio / num_verts_per) * i;
			vertice(1) = altura-(altura / num_verts_per) * i;
			vertice(2) = 0;
			perfil.push_back(vertice);
		}

	inicializar(perfil, nperfiles);
}


Esfera::Esfera(const int num_verts_per, const unsigned nperfiles) : MallaRevol() {
	std::vector<Tupla3f> perfil;
	Tupla3f vertice, punto_inicial;
	float radio = -1;
	punto_inicial = { 0.0, radio, 0.0 };
	for (int i = 0; i <= num_verts_per; i++) {
		float angulo = ((M_PI * i) / (num_verts_per));
		vertice(0) = punto_inicial(0) * cos(angulo) - punto_inicial(1) * sin(angulo);
		vertice(1) = punto_inicial(1) * cos(angulo) + punto_inicial(0) * sin(angulo);
		vertice(2) = punto_inicial(2);
		perfil.push_back(vertice);
	}

	inicializar(perfil, nperfiles);
}


SemiEsfera::SemiEsfera(const int num_verts_per, const unsigned nperfiles) : MallaRevol() {
	std::vector<Tupla3f> perfil;
	Tupla3f vertice, punto_inicial;
	float radio = 1;
	punto_inicial = { 0.0, radio, 0.0 };
	for (int i = 0; i <= num_verts_per/2; i++) {
		float angulo = ((M_PI * i) / (num_verts_per));
		vertice(0) = punto_inicial(0) * cos(angulo) - punto_inicial(1) * sin(angulo);
		vertice(1) = punto_inicial(1) * cos(angulo) + punto_inicial(0) * sin(angulo);
		vertice(2) = punto_inicial(2);

		perfil.push_back(vertice);
	}

	inicializar(perfil, nperfiles);
}