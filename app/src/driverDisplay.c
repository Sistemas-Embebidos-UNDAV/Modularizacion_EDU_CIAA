/* Copyright 2017, Eric Pernia.
 * Copyright 2017, Pablo Gomez.
 * Copyright 2017, Eric Pernia.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * Date: 2017-04-14 
 */

/*==================[inlcusiones]============================================*/

#include "sapi.h"                       // <= Biblioteca sAPI
#include "driverDisplay.h"


/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

static uint8_t k=0;
static delay_t refrescarDisplay;
static delay_t cambiarDigito;

uint8_t charsVec[CHARS] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','A','C','E','H','J','L','P','U'};

/*
     D1   a   f  D2  D3   b
     12  11  10   9   8   7
      |   |   |   |   |   |
   +--+---+---+---+---+---+--+
   |    _     _     _     _  |
   |  /__/  /__/  /__/  /__/ |
   | /_ /. /_ /. /_ /. /_ /. |
   |                         |
   +--+---+---+---+---+---+--+
      |   |   |   |   |   |
      1   2   3   4   5   6
      e   d   h   c   g  D4
*/


// Configuracion de pines de segmentos del display
uint8_t display7SegmentosPines[8] = {
   GPIO4, // a
   GPIO5, // b
   GPIO6, // c
   GPIO2, // d
   GPIO1, // e
   GPIO3, // f
   GPIO7, // g
   GPIO8  // h (dp)
};

// Configuracion de pines de digitos del display
uint8_t display7SegmentosPinesDigitos[4] = {
   LCD1, // D1
   LCD2, // D2
   LCD3, // D3
   LCD4  // D4
};

uint8_t display7SegmentosSalidas[CHARS] = {
   0b00111111, // 0
   0b00000110, // 1
   0b01011011, // 2
   0b01001111, // 3
   0b01100110, // 4
   0b01101101, // 5
   0b01111101, // 6
   0b00000111, // 7
   0b01111111, // 8
   0b01101111, // 9

   0b01011111, // a
   0b01111100, // b
   0b01011000, // c
   0b01011110, // d
   0b01111011, // e
   0b01110001, // f

   0b01110111, // A
   0b00111001, // C
   0b01111001, // E
   0b01110110, // H
   0b00011110, // J
   0b00111000, // L
   0b01110011, // P
   0b00111110, // U

   0b10000000, // .
   0b00000000  // blank
};

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[definiciones de funciones internas]=====================*/

void display7SegmentosEscribir( uint8_t symbolIndex ){

   uint8_t i = 0;

   for(i=0;i<=7;i++)
   #ifdef ANODO_COMUN
      gpioWrite( display7SegmentosPines[i], !(display7SegmentosSalidas[symbolIndex] & (1<<i)) );
   #elif defined(CATODO_COMUN)
      gpioWrite( display7SegmentosPines[i], (display7SegmentosSalidas[symbolIndex] & (1<<i)) );
   #endif   
}

uint8_t charToIndex( uint8_t charIndex ){

   uint8_t output = CHARS - 1;
   uint8_t i;
   
   for(i=0;i<CHARS;i++) {
      if (charIndex == charsVec[i])
         output = i;
   }
   return output; 
}

void display7SegmentosEscribirDigito( uint8_t digitIndex ){

   uint8_t i = 0;

   for(i=0;i<=3;i++)
   #ifdef ANODO_COMUN
      gpioWrite( display7SegmentosPinesDigitos[i], OFF );
      gpioWrite( display7SegmentosPinesDigitos[digitIndex], ON);    
   #elif defined(CATODO_COMUN)
      gpioWrite( display7SegmentosPinesDigitos[i], ON );
      gpioWrite( display7SegmentosPinesDigitos[digitIndex], OFF);
   #endif   
}

char* itoaDisplay(int value, char* result, int base) {

  result[3] = (value % 10) + '0';
  result[2] = ((value % 100) / 10) + '0';
  result[1] = ((value % 1000) / 100) + '0';
  result[0] = ((value % 10000) / 1000) + '0';
   
  return result;
}

void display7SegmentosConfigurarPines( void ){
   uint8_t i = 0;
   
   delayConfig( &refrescarDisplay, 3 );
   delayConfig( &cambiarDigito, 500 );
   
   for(i=0;i<=7;i++)
       gpioConfig( display7SegmentosPines[i], GPIO_OUTPUT );
       
   for(i=0;i<=3;i++)
       gpioConfig( display7SegmentosPinesDigitos[i], GPIO_OUTPUT );
}

void display7SegmentosEscribir4Digitos( uint8_t* digit ){

   if( delayRead(&refrescarDisplay) ){

      display7SegmentosEscribirDigito(k);
      
      display7SegmentosEscribir( charToIndex(digit[k]) );
      k++;
      if(k>4)
         k=0;
   }
}



/*==================[fin del archivo]========================================*/