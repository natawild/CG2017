#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <fstream>
#include "..\..\..\utils\Ponto3D.h"
#include "..\..\..\utils\CoordsPolares.h"
#include "..\..\..\utils\CoordsEsfericas.h"

using namespace std;



class Figura {
public:
	Figura& criaPlanoEmX(Ponto3D centroPlano, float comprimento, float altura, int orientacao) {
		float meioComp = comprimento / 2.0f;
		float meiaAltura = altura / 2.0f;

		Ponto3D A = { 0 + centroPlano.x , meiaAltura + centroPlano.y, meioComp + centroPlano.z };
		Ponto3D B = { 0 + centroPlano.x , -meiaAltura + centroPlano.y, meioComp + centroPlano.z };
		Ponto3D C = { 0 + centroPlano.x , -meiaAltura + centroPlano.y, -meioComp + centroPlano.z };
		Ponto3D D = { 0 + centroPlano.x , meiaAltura + centroPlano.y, -meioComp + centroPlano.z };

		if (orientacao == 1) {
			//Desenha para cima
			pontos.push_back(A);
			pontos.push_back(B);
			pontos.push_back(C);
			pontos.push_back(C);
			pontos.push_back(D);
			pontos.push_back(A);
		}
		else {
			pontos.push_back(A);
			pontos.push_back(D);
			pontos.push_back(C);
			pontos.push_back(C);
			pontos.push_back(B);
			pontos.push_back(A);
		}
		return *this;
	}

	Figura& criaPlanoEmY(Ponto3D centroPlano, float comprimento, float largura, int orientacao) {
		float meioComp = comprimento / 2.0f;
		float meiaLargura = largura / 2.0f;
		
		Ponto3D A = { -meioComp + centroPlano.x ,0 + centroPlano.y, -meiaLargura + centroPlano.z };
		Ponto3D B = { -meioComp + centroPlano.x ,0 + centroPlano.y, meiaLargura + centroPlano.z };
		Ponto3D C = { meioComp + centroPlano.x ,0 + centroPlano.y, meiaLargura + centroPlano.z };
		Ponto3D D = { meioComp + centroPlano.x ,0 + centroPlano.y, -meiaLargura + centroPlano.z };

		if (orientacao == 1) {
			//Desenha para cima
			pontos.push_back(A);
			pontos.push_back(B);
			pontos.push_back(C);
			pontos.push_back(C);
			pontos.push_back(D);
			pontos.push_back(A);
		}
		else {
			pontos.push_back(A);
			pontos.push_back(D);
			pontos.push_back(C);
			pontos.push_back(C);
			pontos.push_back(B);
			pontos.push_back(A);
		}

		return *this;
	}

	Figura& criaPlanoEmZ(Ponto3D centroPlano, float comprimento, float altura, int orientacao) {
		
		float meioComprimento = comprimento / 2.0f;
		float meiaAltura = altura / 2.0f;

		Ponto3D A = { -meioComprimento + centroPlano.x , meiaAltura + centroPlano.y, 0 + centroPlano.z };
		Ponto3D B = { -meioComprimento + centroPlano.x , -meiaAltura + centroPlano.y, 0 + centroPlano.z };
		Ponto3D C = { meioComprimento + centroPlano.x , -meiaAltura + centroPlano.y, 0 + centroPlano.z };
		Ponto3D D = { meioComprimento + centroPlano.x , meiaAltura + centroPlano.y, 0 + centroPlano.z };

		if (orientacao == 1) {
			//Desenha para cima
			pontos.push_back(A);
			pontos.push_back(B);
			pontos.push_back(C);
			pontos.push_back(C);
			pontos.push_back(D);
			pontos.push_back(A);
		}
		else {
			pontos.push_back(A);
			pontos.push_back(D);
			pontos.push_back(C);
			pontos.push_back(C);
			pontos.push_back(B);
			pontos.push_back(A);
		}
		return *this;
	}

	Figura& criaCirculo(Ponto3D centroCirculo, float raio, int fatias, int orientacao) {
		CoordsPolares A, B;
		float dAz = (2.0 * M_PI) / (fatias + 0.0f);

		if (orientacao == 1) {
			for (int i = 0; i < fatias; i++) {
				A = CoordsPolares(centroCirculo, raio, dAz*i);
				B = CoordsPolares(centroCirculo, raio, dAz*(i + 1));
				pontos.push_back(centroCirculo);
				pontos.push_back(A.toCartesianas());
				pontos.push_back(B.toCartesianas());
			}
		}
		else {
			for (int i = 0; i < fatias; i++) {
				A = CoordsPolares(centroCirculo, raio, dAz*i);
				B = CoordsPolares(centroCirculo, raio, dAz*(i + 1));
				pontos.push_back(centroCirculo);
				pontos.push_back(B.toCartesianas());
				pontos.push_back(A.toCartesianas());
			}
		}


		return *this;
	}

	Figura& criaCaixa(Ponto3D centroCaixa, float dx, float dy, float dz) {
		Ponto3D centroPlano;
		float meiodx = dx / 2.0f;
		float meiody = dy / 2.0f;
		float meiodz = dz / 2.0f;

		// Plano B
		centroPlano = { centroCaixa.x + 0.0f ,centroCaixa.y + 0.0f, centroCaixa.z +  meiodz };
		criaPlanoEmZ(centroPlano, dx, dy, 1);

		// Plano B'
		centroPlano = { centroCaixa.x + 0.0f ,centroCaixa.y + 0.0f, centroCaixa.z - meiodz };
		criaPlanoEmZ(centroPlano, dx, dy, 0);

		// Plano C
		centroPlano = { centroCaixa.x + meiodx, centroCaixa.y + 0.0f,centroCaixa.z + 0.0f};
		criaPlanoEmX(centroPlano, dz, dy, 1);

		// Plano C'
		centroPlano = { centroCaixa.x - meiodx, centroCaixa.y + 0.0f,centroCaixa.z + 0.0f };
		criaPlanoEmX(centroPlano, dz, dy, 0);

		// Plano A
		centroPlano = { centroCaixa.x + 0.0f, centroCaixa.y + meiody, centroCaixa.z + 0.0f };
		criaPlanoEmY(centroPlano, dx,dz, 1);

		// Plano A'
		centroPlano = { centroCaixa.x + 0.0f, centroCaixa.y - meiody, centroCaixa.z + 0.0f };
		criaPlanoEmY(centroPlano, dx, dz, 0);



		return *this;
	}

	Figura& criaCilindro(Ponto3D centroCilindro, float raio, float altura, int fatias) {

		float dAz = (2.0f * M_PI) / (fatias + 0.0f);
		float meiaAltura = altura / 2.0f;
		CoordsPolares A, B, C, D;
		Ponto3D centroBase = { centroCilindro.x, centroCilindro.y - meiaAltura, centroCilindro.z };
		Ponto3D centro2 = { centroBase.x, centroBase.y + altura, centroBase.z };
		criaCirculo(centroBase, raio, fatias, 0);
		criaCirculo(centro2, raio, fatias, 1);

		for (int i = 0; i < fatias; i++) {
			A = CoordsPolares(centro2, raio, dAz * i);
			B = CoordsPolares(centroBase, raio, dAz * i);
			C = CoordsPolares(centroBase, raio, dAz * (i + 1));
			D = CoordsPolares(centro2, raio, dAz * (i + 1));

			pontos.push_back(A.toCartesianas());
			pontos.push_back(B.toCartesianas());
			pontos.push_back(C.toCartesianas());
			pontos.push_back(C.toCartesianas());
			pontos.push_back(D.toCartesianas());
			pontos.push_back(A.toCartesianas());
		}

		return *this;
	}

	Figura& criaCone(Ponto3D centroCone, float altura, float raio, int camadas, int fatias) {
		
		CoordsPolares A, B, C, D;
		Ponto3D cBaixo, cCima;
		float dRaio = raio / (camadas + 0.0f);
		float dAltura = altura / (camadas + 0.0f);
		float dAz = (2.0 * M_PI) / (fatias + 0.0f);
		float meiaAltura = altura / 2.0f;
		Ponto3D centroBase = { centroCone.x, centroCone.y - meiaAltura, centroCone.z };
		Ponto3D& cb = centroBase;

		criaCirculo(centroBase, raio, fatias, 0);

		for (int i = 0; i < camadas; i++) {
			for (int j = 0; j < fatias; j++) {
				cBaixo = Ponto3D{ cb.x, cb.y + (dAltura*i), cb.z };
				cCima = Ponto3D{ cb.x, cb.y + (dAltura*(i + 1.0f)), cb.z };

				A = CoordsPolares(cCima, raio - (dRaio*(i + 1.0)), dAz * j);
				B = CoordsPolares(cBaixo, raio - (dRaio*(i + 0.0)), dAz * j);
				C = CoordsPolares(cBaixo, raio - (dRaio*(i + 0.0)), dAz * (j + 1.0));
				D = CoordsPolares(cCima, raio - (dRaio*(i + 1.0)), dAz * (j + 1.0));

				pontos.push_back(A.toCartesianas());
				pontos.push_back(B.toCartesianas());
				pontos.push_back(C.toCartesianas());
				pontos.push_back(C.toCartesianas());
				pontos.push_back(D.toCartesianas());
				pontos.push_back(A.toCartesianas());
			}
		}

		return *this;
	}

	Figura& criaEsfera(int fatias, int camadas, float raio) {
		CoordsEsfericas A,B,C,D;
		float deltaPolar = M_PI / (camadas+0.0f);
		float deltaAz = (2.0 * M_PI) / (fatias+0.0f);

		for (int i = 0; i < camadas; i++) {
			for (int j = 0; j < fatias; j++) {
				A = CoordsEsfericas(raio, deltaAz * (j + 0.0f), deltaPolar * (i+0.0f));
				B = CoordsEsfericas(raio, deltaAz * (j + 0.0f), deltaPolar * (i+1.0f));
				C = CoordsEsfericas(raio, deltaAz * (j + 1.0f), deltaPolar * (i+1.0f));
				D = CoordsEsfericas(raio, deltaAz * (j + 1.0f), deltaPolar * (i+0.0f));

				pontos.push_back(A.toCartesianas());
				pontos.push_back(B.toCartesianas());
				pontos.push_back(C.toCartesianas());
				pontos.push_back(C.toCartesianas());
				pontos.push_back(D.toCartesianas());
				pontos.push_back(A.toCartesianas());

			}
		}

		return *this;
	}

	void toFicheiro(std::string filePath) {
		std::ofstream ofs;
		ofs.open(filePath, std::ofstream::out | std::ofstream::trunc);

		for (auto it = pontos.begin(); it != pontos.end(); ++it) {
			ofs << it->x << " " << it->y << " " << it->z << std::endl;
		}

		ofs.close();

	}

	std::vector<Ponto3D> getPontos() {
		return pontos;
	}

private:
	std::vector<Ponto3D> pontos;

};
