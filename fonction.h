/*
 * fonction.h
 *
 *  Created on: 12 dec. 2019
 *      Author: Salles.l
 */

#ifndef FONCTIONH
#define FONCTIONH

void fenetre(unsigned char character1, unsigned char character2);
void menu_accueil(void);
void appuye_touche(void);
void recap_regle(void);
void interface(unsigned char x, unsigned char y);
void init_alien(void);
void deplacement_vaisseau(void);
void shoot(void);
void collision(void);
uint8_t deplacement_ennemi(void);
void fin_de_partie(void);

#endif /* FONCTION_H_ */
