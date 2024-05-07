/*============================================================================
 * Licencia:
 * Autor:
 * Fecha:
 *===========================================================================*/

#ifndef _DRIVERDISPLAY_H_
#define _DRIVERDISPLAY_H_

/*==================[inclusiones]============================================*/


#define ANODO_COMUN 1
#define CHARS 26

void display7SegmentosEscribir( uint8_t symbolIndex );
uint8_t charToIndex( uint8_t charIndex );
void display7SegmentosEscribirDigito( uint8_t digitIndex );
char* itoaDisplay(int value, char* result, int base);
void display7SegmentosConfigurarPines( void );
void display7SegmentosEscribir4Digitos( uint8_t* digit );

/*==================[end of file]============================================*/
#endif /* _DRIVERDISPLAY_H_ */
