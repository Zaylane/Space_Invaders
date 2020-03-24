/*
 * fonction.c
 *
 *  Created on: 12 dec. 2019
 *      Author: Salles.l
 */
#include "vt100.h"
#include "serial.h"
#include "fonction.h"

unsigned char depla;
unsigned char vx = 43;
uint8_t debut_tir = 0;
static float vy = 2;
static unsigned char conteur_sens = 0;
static unsigned char direction = 0;

void delai(unsigned long n) {
	int i = 0;
	unsigned long int max = n * 100000;
	do {
		i++;
	} while (i <= max);
}

void fenetre(unsigned char character1, unsigned char character2) {

	unsigned char i;

	for (i = 1; i < 90; i++) {
		vt100_move(i, 1);
		serial_putchar(character1);
		vt100_move(i, 29);
		serial_putchar(character1);
	}
	for (i = 1; i < 30; i++) {
		vt100_move(1, i);
		serial_putchar(character2);
		vt100_move(90, i);
		serial_putchar(character2);
	}
}

void menu_accueil(void) {
	fenetre('#', '#');
	vt100_move(38, 10);
	serial_puts("Space Invader");
	vt100_move(43, 15);
	serial_puts("PLAY");
	vt100_move(70, 28);
	serial_puts("By SALLES Lucas");
	vt100_move(34, 20);
	serial_puts("Appuyer sur une touche");
	appuye_touche();
}

void appuye_touche(void) {
	signed char touche = -1;
	while (touche == -1) {
		touche = serial_get_last_char();
	}
}

void recap_regle(void) {
	vt100_clear_screen();
	fenetre('#', '#');
	vt100_move(5, 4);
	serial_puts("Tuez tous les ennemis pour gagner la partie");
	vt100_move(5, 6);
	serial_puts("Les ennemis descendent dans la zone de jeu");
	vt100_move(5, 8);
	serial_puts("Si ils arrivent en bas en premier vous avez perdu");
	vt100_move(5, 10);
	serial_puts("Tirez dessus et protegez-vous pour gagner :");
	vt100_move(5, 16);
	serial_puts("Fonctionnement :");
	vt100_move(5, 18);
	serial_puts("D : Deplacement a droite");
	vt100_move(5, 20);
	serial_puts("Q : Deplacement a gauche");
	vt100_move(5, 22);
	serial_puts("V : Tirer");
	vt100_move(65, 28);
	serial_puts("Appuyer sur une touche");
	appuye_touche();
}

void interface(unsigned char x, unsigned char y) {
	vt100_clear_screen();
	fenetre('#', '#');
	unsigned char k;

	for (k = 3; k < 89; k = k + 20) {
		vt100_move(k, 20);
		serial_puts("_____");
	}
	vt100_move(43, 25);
	serial_puts("('0')");
}

void deplacement_vaisseau(void) {

	vt100_move(vx, 25);
	serial_puts("('0')");

	depla = serial_get_last_char();

	if (depla == 'd') {
		vt100_move(vx, 25);
		serial_puts("     ");
		vx = vx + 1;
		if (vx > 85) {
			vx = 85;
		}
		vt100_move(vx, 25);
		serial_puts("('0')");
	}

	if (depla == 'q') {
		vt100_move(vx, 25);
		serial_puts("     ");
		vx = vx - 1;
		if (vx < 2) {
			vx = 2;
		}
		vt100_move(vx, 25);
		serial_puts("('0')");
	}
}

typedef struct alien {
	uint8_t x;
	uint8_t y;
	uint8_t statut;
} t_alien;
t_alien aliens[5] = { 0 };

void init_alien(void) {
	uint8_t va = 0;
	direction = 0;
	for (va = 0; va < 5; va++) {
		aliens[va].y = 4;
		aliens[va].x = (va + 1) * 8;
		aliens[va].statut = 1;
	}
}

uint8_t deplacement_ennemi(void) {
	uint8_t va = 0;
	uint8_t doit_descendre = 0;

	for (va = 0; va < 5; va++) {
		if (direction == 0) {
			if (aliens[va].x == 83)
				doit_descendre = 1;
		} else {
			if (aliens[va].x == 2)
				doit_descendre = 1;
		}
	}

	if (doit_descendre) {
		conteur_sens += 1;
		direction = conteur_sens % 2;
		for (va = 0; va < 5; va++) {
			vt100_move(aliens[va].x, aliens[va].y);
			serial_puts("       ");
			aliens[va].y++;
		}
	}

	for (va = 0; va < 5; va++) {
		if (direction == 0) {
			aliens[va].x++;
		} else {
			aliens[va].x--;
		}
		if (aliens[va].y == 14) {
			return 1;
		}
		if (aliens[va].statut == 1) {

			vt100_move(aliens[va].x, aliens[va].y);
			serial_puts(" [-O-] ");
			delai(2);

		}
	}
	return 0;
}

void collision(void) {
	uint8_t va = 0;
	static uint8_t score = 0;
	for (va = 0; va < 6; va++) {
		if (vy == aliens[va].y && debut_tir >= aliens[va].x
				&& debut_tir <= aliens[va].x + 4) {
			// collision détectée !
			vt100_move(aliens[va].x, aliens[va].y);
			serial_puts("       ");
			aliens[va].statut = 0;
			score += 10;
			vt100_move(3, 3);
			serial_puts("Score : ");
			serial_putchar('0' + (score / 10));
			serial_putchar('0' + (score % 10));

		}
	}
}
	void fin_de_partie(void) {
		vt100_clear_screen();
		fenetre('#', '#');
		vt100_move(38, 10);
		serial_puts("Game Over");
	}

	void shoot(void) {
		if (depla == 'v') {
			debut_tir = vx + 2;
			vy = 25;
			vt100_move(debut_tir, vy);
			serial_puts(" ");
		}
		if (vy >= 3) {
			vt100_move(debut_tir, vy);
			serial_puts(" ");
			vy -= 1;
			vt100_move(debut_tir, vy);
			serial_puts("o");
			delai(2);
		} else {
			vt100_move(debut_tir, vy);
			serial_puts(" ");
		}
	}

