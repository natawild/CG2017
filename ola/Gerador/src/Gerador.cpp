#include <iostream>
#include <string>
#include <vector>
#include "headers\Figura.h"
#include "..\..\utils\Ponto3D.h"




int main(int argc, char** argv) {
	// Objecto onde vão ser colocados os pontos da figura a criar.
	Figura figura;
	// Facilita controlo de fluxo, evita if..then...else aninhados.
	bool figuraCriada = false;

	// Esta string vai ser concatenada com o ficheiro recebido como argumento
	// Isto significa que se se indicar o ficheiro "figura.3d" por exemplo,
	// o ficheiro criado será colocado na pasta Motor/Modelos.
	// O ficheiro ser colocado directamente na pasta do motor evita o copy/paste
	// manual.
	std::string pastaModelos = "../../Motor/Modelos/";

	if (!figuraCriada && argc == 5 && ( strcmp(argv[1], "plane") == 0 || 
										 strcmp(argv[1], "plano") == 0)   ) {
		// argv:     0       1        2         3       4  
		//       > Gerador plane comprimento largura ficheiro
		Ponto3D centroPlano = { 0,0,0 };
		string nomeFicheiro(argv[4]);
		string filePathCompleto(pastaModelos + nomeFicheiro);

		std::cout << "A criar plano..."; cout.flush();

		// Passa argumentos para variaveis e chama funcao para criar plano.
		float comprimento = stof(argv[2]);
		float largura = stof(argv[3]);
		figura.criaPlanoEmY(centroPlano,comprimento, largura, 1);

		std::cout << "concluido!" << std::endl; cout.flush();

		std::cout << "A escrever pontos em " << filePathCompleto << " ..." ; cout.flush();
		// Escreve pontos da figura em ficheiro dado
		figura.toFicheiro(filePathCompleto);
		std::cout << "concluido!" << std::endl; cout.flush();
		figuraCriada = true;
	}

	if (!figuraCriada && argc == 6 && (strcmp(argv[1], "caixa") == 0 ||
										strcmp(argv[1], "box") == 0 )   ) {
		// argv:     0       1        2         3       4      5
		//       > Gerador caixa comprimento largura altura ficheiro
		string nomeFicheiro(argv[5]);
		string filePathCompleto(pastaModelos + nomeFicheiro);

		std::cout << "A criar caixa..."; cout.flush();
		Ponto3D centroCaixa = { 0.0,0.0,0.0 };
		float comprimento = std::stof(argv[2]);
		float largura = std::stof(argv[3]);
		float altura = std::stof(argv[4]);
		
		figura.criaCaixa(centroCaixa, comprimento, altura, largura);
		std::cout << "concluido!" << std::endl; cout.flush();

		std::cout << "A escrever pontos em " << filePathCompleto << " ..."; cout.flush();
		
		figura.toFicheiro(filePathCompleto);
		std::cout << "concluido!" << std::endl; cout.flush();
		figuraCriada = true;
	}

	if (!figuraCriada && argc == 7 && strcmp(argv[1], "cone") == 0) {
		// argv:     0       1    2     3      4      5       6
		//       > Gerador cone raio altura fatias camadas ficheiro
		string nomeFicheiro(argv[6]);
		string filePathCompleto(pastaModelos + nomeFicheiro);

		std::cout << "A criar cone..."; cout.flush();
		Ponto3D centroCone = {0.0,0.0,0.0};
		float raio = std::stof(argv[2]);
		float altura = std::stof(argv[3]);
		int fatias = stoi(argv[4]);
		int camadas = stoi(argv[5]);
		figura.criaCone(centroCone, altura, raio, camadas, fatias);

		std::cout << "concluido!" << std::endl; cout.flush();

		std::cout << "A escrever pontos em " << filePathCompleto << " ..."; cout.flush();
		figura.toFicheiro(filePathCompleto);
		std::cout << "concluido!" << std::endl; cout.flush();
		figuraCriada = true;
	}

	if (!figuraCriada && argc == 6 && (strcmp(argv[1], "sphere") == 0 ||
										strcmp(argv[1], "esfera") == 0)) {
		// argv:     0       1     2     3       4      5       
		//       > Gerador sphere raio fatias camadas ficheiro
		string nomeFicheiro(argv[5]);
		string filePathCompleto(pastaModelos + nomeFicheiro);

		std::cout << "A criar esfera..."; cout.flush();
		float raio = std::stof(argv[2]);
		int fatias = std::stoi(argv[3]);
		int camadas = std::stoi(argv[4]);
		figura.criaEsfera(fatias, camadas, raio);
		std::cout << "concluido!" << std::endl; cout.flush();
		
		std::cout << "A escrever pontos em " << filePathCompleto << " ..."; cout.flush();
		figura.toFicheiro(filePathCompleto);
		std::cout << "concluido!" << std::endl; cout.flush();
		figuraCriada = true;
	}

	
	// Se programa for corrido com nº de argumentos nao esperado ou figura invalida
	if (!figuraCriada) {
		if (argc == 1) {
			std::cout << "Programa corrido sem argumentos!" << std::endl;
		}
		else {
			// Neste ponto sabe-se que:
			//     1) Foi indicado mais que 1 argumento, mas que o numero de argumentos e invalido.
			//     OU
			//     2) A figura indicada (argv[1]) e invalida.
			std::cout << "Numero de argumentos nao esperado ou figura invalida." << std::endl;
			std::cout << "    Numero argumentos: " << argc << std::endl;
			std::cout << "    Figura: " << argv[1] << std::endl;
		}
		// Imprime ajuda, independentemente do erro.
		std::cout << "Como usar:" << std::endl;
		std::cout << "    > Gerador (plane|plano) comprimento largura ficheiro" << std::endl;
		std::cout << "    > Gerador (box|caixa) comprimento largura altura ficheiro" << std::endl;
		std::cout << "    > Gerador cone raio altura fatias camadas ficheiro" << std::endl;
		std::cout << "    > Gerador (esfera|sphere) raio fatias camadas ficheiro" << std::endl;
	}


	return 0;
}
