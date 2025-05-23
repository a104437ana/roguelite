#include <stdlib.h>

#include "mapa.h"

/**
 * a104437 - Ana Sá Oliveira
 * Determina o número de paredes que existem num determinado raio.
 */
int raio (STATE *st, int x, int y, int r) {
	int n = 0;
	signed int rX, rY;
	if (r>=0) {
		for (rX = r; rX >= - r ; rX--) {
			for (rY = r; rY >= - r ; rY--) {
				if (st->map[x+rX][y+rY].caracterAnterior == '#') n++;
			}
		}
	}
	return n;
}

/**
 * a104437 - Ana Sá Oliveira
 * Gera números aleatórios, que serão usados posteriormente no movimento aleatório dos monstros (quando distantes do jogador).
 */
void gerar_seeds (STATE *st) {
	st->seed[0][0] = lrand48();
	st->seed[1][0] = lrand48();
	st->seed[2][0] = lrand48();
	st->seed[3][0] = lrand48();
	st->seed[4][0] = lrand48();
	st->seed[5][0] = lrand48();
	st->seed[6][0] = lrand48();
	st->seed[7][0] = lrand48();
	st->seed[0][1] = lrand48();
	st->seed[1][1] = lrand48();
	st->seed[2][1] = lrand48();
	st->seed[3][1] = lrand48();
	st->seed[4][1] = lrand48();
	st->seed[5][1] = lrand48();
	st->seed[6][1] = lrand48();
	st->seed[7][1] = lrand48();
}

/**
 * a104437 - Ana Sá Oliveira
 * a104170 - Beatriz Peixoto
 * a72481 - Sara Ramalho
 * Gera as coordenadas e inicializa o jogador, as escadas, os monstros, as armas, as poções e as bombas.
 */
void gerar_coordenadas (STATE *st) {
	/**
     * a104437 - Ana Sá Oliveira
     * Gera coordenadas aleatórias para o jogador.
     */
	st->jogador.coord.X = lrand48() % st->jogo.X;
	st->jogador.coord.Y = lrand48() % st->jogo.Y;
	while (raio(st,st->jogador.coord.X,st->jogador.coord.Y,1) != 0 || st->map[st->jogador.coord.X][st->jogador.coord.Y].acessivel != 1) {
		st->jogador.coord.X = lrand48() % st->jogo.X;
	    st->jogador.coord.Y = lrand48() % st->jogo.Y;
	}
	st->map[st->jogador.coord.X][st->jogador.coord.Y].acessivel = 0;

    /**
     * a104437 - Ana Sá Oliveira
     * Gera coordenadas aleatórias para as escadas.
     */
	st->escada.X = lrand48() % st->jogo.X;
	st->escada.Y = lrand48() % st->jogo.Y;
	while (st->map[st->escada.X][st->escada.Y].acessivel != 1) {
		st->escada.X = lrand48() % st->jogo.X;
	    st->escada.Y = lrand48() % st->jogo.Y;
    }
	st->map[st->escada.X][st->escada.Y].acessivel = 0;

    /**
     * a104170- Beatriz Peixoto
     * Criação aleatória das coordenadas dos monstros.
     */
	for (int i = 0; i < 8; i++)
    {
		st->monstro[i].coord.X = lrand48() % (st->jogo.X);
		st->monstro[i].coord.Y = lrand48() % (st->jogo.Y);

	    while (st->map[st->monstro[i].coord.X][st->monstro[i].coord.Y].acessivel != 1)
	    {
			st->monstro[i].coord.X = lrand48() % (st->jogo.X);
			st->monstro[i].coord.Y = lrand48() % (st->jogo.Y);
	    }

		st->map[st->monstro[i].coord.X][st->monstro[i].coord.Y].acessivel = 0;

		st->monstro[i].vida = MAX_VIDA_MONSTRO;
    }

	/**
	 * a72481 - Sara Ramalho
	 * Criação de armas.
	 */
	for (int i = 0; i < 10; i++)
    {   if (st->arma[i].equipada == 0) {
		do
		{
			st->arma[i].coord.X = lrand48() % (st->jogo.X);
			st->arma[i].coord.Y = lrand48() % (st->jogo.Y);
		}
		while (st->map[st->arma[i].coord.X][st->arma[i].coord.Y].acessivel != 1);
		
		st->map[st->arma[i].coord.X][st->arma[i].coord.Y].objeto = 1;
	}
    }

	/**
	 * a72481 - Sara Ramalho
	 * Criação de poções.
	 */
	for (int i = 0; i < NUM_MAX_POCOES; i++) {
		st->pocao[i].coord.X = 0;
		st->pocao[i].coord.Y = 0;
		st->pocao[i].tipo = -1;
		st->pocao[i].gerada = 0;
	}
	for (int i = 0; i < NUM_MAX_POCOES; i++)
    {
		do 
		{
			st->pocao[i].coord.X = lrand48() % (st->jogo.X);
			st->pocao[i].coord.Y = lrand48() % (st->jogo.Y);
		}
	    while (st->map[st->pocao[i].coord.X][st->pocao[i].coord.Y].acessivel != 1 && st->map[st->pocao[i].coord.X][st->pocao[i].coord.Y].objeto == 0);
	    
		st->pocao[i].tipo = lrand48() % NUM_MAX_POCOES;
		st->pocao[i].gerada = 1;
        st->map[st->pocao[i].coord.X][st->pocao[i].coord.Y].objeto = 1;
    }

	/**
	 * a72481 - Sara Ramalho
	 * Criação de bombas.
	 */
    for (int i = 0; i < NUM_MAX_BOMBAS; i++) {
		st->bomba[i].coord.X = 0;
		st->bomba[i].coord.Y = 0;
		st->bomba[i].gerada = 0;
	}
	for (int i = 0; i < NUM_MAX_BOMBAS; i++)
    {
		do 
		{
			st->bomba[i].coord.X = lrand48() % (st->jogo.X);
			st->bomba[i].coord.Y = lrand48() % (st->jogo.Y);
		}
	    while (st->map[st->bomba[i].coord.X][st->bomba[i].coord.Y].acessivel != 1 && st->map[st->bomba[i].coord.X][st->bomba[i].coord.Y].objeto == 0);
	    
		st->bomba[i].gerada = 1; 
		st->map[st->bomba[i].coord.X][st->bomba[i].coord.Y].objeto = 1;
    }
}

/**
 * a104437 - Ana Sá Oliveira
 * Gera um primeiro mapa aleatório com aproximadamente 40% de paredes e com duas camadas de paredes a sua volta.
 */
void first_map (STATE *st) {
	int x, y;
	x = 0;
	y = 0;
	for (;x<st->jogo.X;x++) {
		st->map[x][y].caracterAtual = '#';
	}
	x = st->jogo.X - 1;
	for (;y<st->jogo.Y;y++) {
		st->map[x][y].caracterAtual = '#';
	}
	y = st->jogo.Y - 1;
	for (;x>=0;x--) {
		st->map[x][y].caracterAtual = '#';
	}
	x = 0;
	for (;y>0;y--) {
		st->map[x][y].caracterAtual = '#';
	}
	x = 1;
	y = 1;
	for (;x<st->jogo.X-1;x++) {
		st->map[x][y].caracterAtual = '#';
	}
	x = st->jogo.X - 2;
	for (;y<st->jogo.Y-1;y++) {
		st->map[x][y].caracterAtual = '#';
	}
	y = st->jogo.Y - 2;
	for (;x>=1;x--) {
		st->map[x][y].caracterAtual = '#';
	}
	x = 1;
	for (;y>1;y--) {
		st->map[x][y].caracterAtual = '#';
	}
	x = 2;
	for (;x<st->jogo.X - 2;x++) {
		y = 2;
		for (;y<st->jogo.Y - 2;y++) {
			if (drand48() < 0.34) {
				st->map[x][y].caracterAtual = '#';
			}
			else {
				st->map[x][y].caracterAtual = '.';
			}
		}
	}
}

/**
 * a104437 - Ana Sá Oliveira
 * Primeiro algoritmo para transformar a aparência do mapa, faz com que um ponto com 5 ou mais paredes a volta e nele vire parede,
  um ponto com 2 ou menos paredes a volta e nele vire parede e torna os restantes pontos não paredes.
 */
void first_alg (STATE *st) {
	int x, y;
	for (x=0;x<st->jogo.X;x++) {
		for (y=0;y<st->jogo.Y;y++) {
			st->map[x][y].caracterAnterior = st->map[x][y].caracterAtual;
		}
	}
	x = 2;
	for (;x<st->jogo.X - 2;x++) {
		y = 2;
		for (;y<st->jogo.Y - 2;y++) {
			if (raio(st,x,y,1)>=5) {
				st->map[x][y].caracterAtual = '#';
			}
			else {
				if (raio(st,x,y,2)<=2) {
				    st->map[x][y].caracterAtual = '#';
				}
				else {
				    st->map[x][y].caracterAtual = '.';
				}
			}
		}
	}
}

/**
 * a104437 - Ana Sá Oliveira
 * Segundo algoritmo para transformar a aparência do mapa, faz com que um ponto com 5 ou mais paredes a volta e nele vire parede
 e torna os restantes pontos não paredes.
 */
void second_alg (STATE *st) {
	int x, y;
	for (x=0;x<st->jogo.X;x++) {
		for (y=0;y<st->jogo.Y;y++) {
			st->map[x][y].caracterAnterior = st->map[x][y].caracterAtual;
		}
	}
	x = 2;
	for (;x<st->jogo.X - 2;x++) {
		y = 2;
		for (;y<st->jogo.Y - 2;y++) {
			if (raio(st,x,y,1)>=5) {
				st->map[x][y].caracterAtual = '#';
			}
			else {
				st->map[x][y].caracterAtual = '.';
			}
		}
	}
}

/**
 * a104437 - Ana Sá Oliveira
 * Algoritmo flood fill que recebe um ponto, e trata esse ponto e os pontos adjacentes. Se o ponto a tratar ainda não foi percorrido pelo algoritmo 
 e se esse ponto não for uma parede nem estiver fora do mapa, então iremos mudar o seu valor acessível para o pretendido e iremos chamar a mesma função 
 (recursividade) para agora tratar deste ponto e dos seus pontos adjacentes. Esta função percorre uma área de pontos e muda o seu valor acessível para o
 pretendido.
 */
void flood_fill_alg (STATE *st, int x, int y, int valor1,int valor2) {
	signed int rX, rY;
	for (rX = 1; rX >= - 1 ; rX--) {
			for (rY = 1; rY >= - 1 ; rY--) {
				if (st->map[x+rX][y+rY].acessivel==valor2 && x+rX>=0 && y+rY>=0 && x+rX<st->jogo.X && y+rY<st->jogo.Y) {
						st->map[x+rX][y+rY].acessivel = valor1;
						flood_fill_alg (st,x+rX,y+rY,valor1,valor2);
				}
			}
		}
}

/**
 * a104437 - Ana Sá Oliveira
 * Põe todos os pontos do mapa com valor acessível igual a 0, menos as paredes que ficam com valor acessível -1.
 */
void reset_acesso (STATE *st) {
	int x, y;
	for (x = 0; x < st->jogo.X; x++) {
		for (y = 0; y < st->jogo.Y ; y++) {
			if (st->map[x][y].caracterAtual == '#') st->map[x][y].acessivel = -1;
			else st->map[x][y].acessivel = 0;
		}
	}
}

/**
 * a104437 - Ana Sá Oliveira
 * Procura pontos com valor acessível 0 (ainda não percorridos), guarda as coordenadas desse ponto e utiliza a função com o algoritmo flood fill para 
 prencher esse ponto e a área adjacente com o valor acessível adequado. Assim, cada área isolada terá um determinado valor acessível e terá uma coordenada
 inicial correspondente (utéis para depois formar os corredores que unirão todas as áreas).
 */
int prencher_mapa_acesso (STATE *st) {
	int x,y,i;
	i = 1;
	for (x = 0; x < st->jogo.X; x++) {
		for (y = 0; y < st->jogo.Y ; y++) {
			if (st->map[x][y].acessivel == 0) {
				st->sala[i-1].X = x;
				st->sala[i-1].Y = y;
				st->map[x][y].acessivel = i;
				flood_fill_alg(st,x,y,i,0);
				i++;
			}
		}
	}
	return i;
}

/**
 * a104437 - Ana Sá Oliveira
 * Gera corredores horizontais (ou verticais) para unir áreas de valor acessivel diferente de 1 com áreas de valor acessível igual a 1.
 */
void gerar_corredores (STATE *st, int i) {
	int j,x,y;
	j = 1;
	while (j<i) {
		x = st->sala[j].X;
		y = st->sala[j].Y - 1;
		for (;y>2 && st->map[x][y].acessivel != 1;y--) {
		}
		if (y>2) {
			st->map[x][y].caracterAtual = '+';
			y++;
			st->map[x][y].caracterAtual = '+';
			y++;
			for (;y<st->jogo.Y - 2 && st->map[x][y].acessivel != j+1; y++) {
				st->map[x][y].caracterAtual = '+';
			}
			while (y<st->jogo.Y - 2 && st->map[x][y].caracterAtual != '#') {
				st->map[x][y].caracterAtual = '+';
				y++;
			}
			y--;
			st->map[x][y].caracterAtual = '+';
			flood_fill_alg(st,x,y,1,j+1);
		}
		else {
		x = st->sala[j].X;
		y = st->sala[j].Y + 1;
		for (;y<st->jogo.Y - 2 && st->map[x][y].acessivel != 1;y++) {
		}
		if (y<st->jogo.Y - 2) {
			st->map[x][y].caracterAtual = '+';
			y--;
			st->map[x][y].caracterAtual = '+';
			y--;
			for (;y>2 && st->map[x][y].acessivel != j+1; y--) {
				st->map[x][y].caracterAtual = '+';
			}
			while (y>2 && st->map[x][y].caracterAtual != '#') {
				st->map[x][y].caracterAtual = '+';
				y--;
			}
			y++;
			st->map[x][y].caracterAtual = '+';
			flood_fill_alg(st,x,y,1,j+1);
		}
		else {
		x = st->sala[j].X + 1;
		y = st->sala[j].Y;
		for (;x<st->jogo.X - 2 && st->map[x][y].acessivel != 1;x++) {
		}
		if (x<st->jogo.X - 2) {
			st->map[x][y].caracterAtual = '+';
			x--;
			st->map[x][y].caracterAtual = '+';
			x--;
			for (;x>2 && st->map[x][y].acessivel != j+1; x--) {
				st->map[x][y].caracterAtual = '+';
			}
			while (x>2 && st->map[x][y].caracterAtual != '#') {
				st->map[x][y].caracterAtual = '+';
				x--;
			}
			x++;
			st->map[x][y].caracterAtual = '+';
			flood_fill_alg(st,x,y,1,j+1);
		}
		else {
		x = st->sala[j].X - 1;
		y = st->sala[j].Y;
		for (;x>2 && st->map[x][y].acessivel != 1;x--) {
		}
		if (x>2) {
			st->map[x][y].caracterAtual = '+';
			x++;
			st->map[x][y].caracterAtual = '+';
			x++;
			for (;x<st->jogo.X - 2 && st->map[x][y].acessivel != j+1; x++) {
				st->map[x][y].caracterAtual = '+';
			}
			while (x<st->jogo.X - 2 && st->map[x][y].caracterAtual != '#') {
				st->map[x][y].caracterAtual = '+';
				x++;
			}
			x--;
			st->map[x][y].caracterAtual = '+';
			flood_fill_alg(st,x,y,1,j+1);
		}
		}
		}
		}
		j++;
	}
}

/**
 * a104437 - Ana Sá Oliveira
 * Gera um mapa novo utilizando as funções anteriores e inicializa ou incrementa alguns valores do mapa e do jogo (por exemplo, incrementa o nivel do jogo).
 */
void gerar(STATE *st) {
	int i,x,y;
	first_map(st);
	for (i=1;i<=5;i++) {
		first_alg(st);
	}
	for (i=1;i<=15;i++) {
		second_alg(st);
	}
	reset_acesso(st);
	i = prencher_mapa_acesso (st);
	gerar_corredores (st,i);

	for (x = 0; x < st->jogo.X; x++) {
		for (y = 0; y < st->jogo.Y ; y++) {
			st->map[x][y].objeto = 0;
			if (st->map[x][y].caracterAtual == '#') st->map[x][y].acessivel = -1;
			if (st->map[x][y].caracterAtual == '+') st->map[x][y].acessivel = 1;
		}
	}
	for (i=0;i<50;i++) {
		st->sala[i].X = 0;
		st->sala[i].Y = 0;
	}
	gerar_coordenadas(st);
	st->map[st->escada.X][st->escada.Y].caracterAtual = '/';
	st->map[st->jogador.coord.X][st->jogador.coord.Y].caracterAtual = '>';
	gerar_seeds(st);
	st->nivel ++;
	st->acontecimento = 0;

	for (int x = 0; x < st->jogo.X; x++)
		{
			for (int y = 0; y < st->jogo.Y; y++)
			{
				st->map[x][y].acessado = 0;
			}
		}
}

