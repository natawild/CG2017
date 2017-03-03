#pragma once

#include <vector>
#include <GL\glut.h>

#include "figuras.h"
#include "Transformacao.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Primitiva {

	string nome;
	vector<Primitiva> filhos;
	vector<Ponto> pontos;
	Transformacao transformacao;

public:
	Primitiva();
	Primitiva(string n, vector<Primitiva>, vector<Ponto>, Transformacao);
	string getNome() { return nome; }
	vector<Ponto> getPontos() { return pontos; }
	vector<Primitiva> getFilhos() { return filhos; }
	Transformacao getTransformacao() { return transformacao; }
	void setNome(string n) { nome = n; }
	void setFilhos(vector<Primitiva> f) { filhos = f; }
	void setPontos(vector<Ponto> p) { pontos = p; }
	void setTransformacao(Transformacao t) { transformacao = t; }
	virtual ~Primitiva() {};
}; #pragma once
