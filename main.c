/*#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "SSD1306.h"

#define BUTTON_UP_LEFT    PD0
#define BUTTON_DOWN_LEFT  PD1
#define BUTTON_UP_RIGHT   PD2
#define BUTTON_DOWN_RIGHT PD3

void init_buttons() {
	// Configurar PD0, PD1, PD2 y PD3 como entradas
	DDRD &= ~(1 << BUTTON_UP_LEFT);
	DDRD &= ~(1 << BUTTON_DOWN_LEFT);
	DDRD &= ~(1 << BUTTON_UP_RIGHT);
	DDRD &= ~(1 << BUTTON_DOWN_RIGHT);

	// Habilitar resistencias pull-up en PD0, PD1, PD2 y PD3
	PORTD |= (1 << BUTTON_UP_LEFT);
	PORTD |= (1 << BUTTON_DOWN_LEFT);
	PORTD |= (1 << BUTTON_UP_RIGHT);
	PORTD |= (1 << BUTTON_DOWN_RIGHT);
}

uint8_t read_button(uint8_t button) {
	// Retorna 1 si el botón está presionado, 0 de lo contrario
	return !(PIND & (1 << button));
}

void draw_vertical_bar(uint8_t position, uint8_t column, char character) {
	OLED_SetCursor(position, column);
	OLED_Printf("%c", character);
}

void init_screen() {
	i2c_init();
	OLED_Init();
	OLED_Clear();
	//OLED_SetCursor(0, 0);
	OLED_Printf("Ya empezo el despapaye");
	_delay_ms(1000);
	OLED_Clear();
}

int main(void) {
	uint8_t cursor_position_left = 0;
	uint8_t previous_position_left = 0xFF; // Un valor fuera de rango para asegurar que se dibuje inicialmente

	uint8_t cursor_position_right = 0;
	uint8_t previous_position_right = 0xFF; // Un valor fuera de rango para asegurar que se dibuje inicialmente

	uint8_t ball_position_x = 0;
	uint8_t ball_position_y = 8;
	int auxx = 0;
	int auxy = 0;
	bool boolX = 0;
	bool boolY = 0;
	
	init_screen();
	OLED_Init(); // Inicializar el OLED
	OLED_Clear(); // Limpiar el display

	init_buttons(); // Inicializar botones

	while (1) {
		// Controlar barra izquierda
		if (read_button(BUTTON_UP_LEFT) && cursor_position_left > 0) {
			cursor_position_left--;
			_delay_ms(50); // Debounce delay
		}

		if (read_button(BUTTON_DOWN_LEFT) && cursor_position_left < 7) { // Asumiendo 8 filas
			cursor_position_left++;
			_delay_ms(50); // Debounce delay
		}

		// Controlar barra derecha
		if (read_button(BUTTON_UP_RIGHT) && cursor_position_right > 0) {
			cursor_position_right--;
			_delay_ms(50); // Debounce delay
		}

		if (read_button(BUTTON_DOWN_RIGHT) && cursor_position_right < 7) { // Asumiendo 8 filas
			cursor_position_right++;
			_delay_ms(50); // Debounce delay
		}

		// Dibujar barras si han cambiado de posición
		if (cursor_position_left != previous_position_left || cursor_position_right != previous_position_right) {
			//OLED_Clear();
			draw_vertical_bar(0, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(1, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(2, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(3, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(4, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(5, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(6, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(7, 0, ' '); // Columna 0 para la barra izquierda


			draw_vertical_bar(0, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(1, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(2, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(3, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(4, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(5, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(6, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(7, 120, ' '); // Columna 0 para la barra izquierda

			draw_vertical_bar(cursor_position_left, 0, '|'); // Columna 0 para la barra izquierda
			draw_vertical_bar(cursor_position_left + 1, 0, '|'); // Columna 0 para la barra izquierda

			draw_vertical_bar(cursor_position_right, 120, '|'); // Columna 126 para la barra derecha
			draw_vertical_bar(cursor_position_right + 1, 120, '|'); // Columna 126 para la barra derecha

			previous_position_left = cursor_position_left;
			previous_position_right = cursor_position_right;
		}

		// Dibujar la pelota
		draw_vertical_bar(ball_position_x-1, ball_position_y-8, ' ');
		draw_vertical_bar(ball_position_x+1, ball_position_y+8, ' ');

		draw_vertical_bar(ball_position_x-1, ball_position_y+8, ' ');

		draw_vertical_bar(ball_position_x+1, ball_position_y-8, ' ');

		draw_vertical_bar(ball_position_x, ball_position_y, 'o');

		// Movimiento de la pelota
		if (auxx == 7) {
			boolX = 1;
		}
		if (auxx == 0) {
			boolX = 0;
		}
		if (boolX == 0) {
			ball_position_x += 1;
			auxx++;
		}
		if (boolX == 1) {
			ball_position_x -= 1;
			auxx--;
		}

		// Colisiones con las barras
		if ((auxy == 13 && ball_position_x == cursor_position_left) || (auxy == 13 && ball_position_x == cursor_position_left + 1)) {
			boolY = 1;
		}
		if ((auxy == 0 && ball_position_x == cursor_position_right) || (auxy == 0 && ball_position_x == cursor_position_right + 1)) {
			boolY = 0;
		}
		if (auxy == 0 && ball_position_x != cursor_position_right && (auxy == 0 && ball_position_x != cursor_position_right + 1)) {
			OLED_Printf("Perdiste2");
			OLED_Clear();
			_delay_ms(100);
			boolY = 0;
		}
		if (auxy == 13 && ball_position_x != cursor_position_left && (auxy == 13 && ball_position_x != cursor_position_left + 1)) {
			OLED_Printf("Perdiste1");
			OLED_Clear();
			_delay_ms(100);
			boolY = 1;
		}

		if (boolY == 0) {
			ball_position_y += 8;
			auxy++;
		}
		if (boolY == 1) {
			ball_position_y -= 8;
			auxy--;
		}

		_delay_ms(150); // Para ralentizar el movimiento y evitar parpadeos
	}

	return 0; // Nunca alcanzado
}
*/

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "SSD1306.h"

#define BUTTON_UP_LEFT    PD0
#define BUTTON_DOWN_LEFT  PD1
#define BUTTON_UP_RIGHT   PD2
#define BUTTON_DOWN_RIGHT PD3

void init_buttons() {
	// Configurar PD0, PD1, PD2 y PD3 como entradas
	DDRD &= ~(1 << BUTTON_UP_LEFT);
	DDRD &= ~(1 << BUTTON_DOWN_LEFT);
	DDRD &= ~(1 << BUTTON_UP_RIGHT);
	DDRD &= ~(1 << BUTTON_DOWN_RIGHT);

	// Habilitar resistencias pull-up en PD0, PD1, PD2 y PD3
	PORTD |= (1 << BUTTON_UP_LEFT);
	PORTD |= (1 << BUTTON_DOWN_LEFT);
	PORTD |= (1 << BUTTON_UP_RIGHT);
	PORTD |= (1 << BUTTON_DOWN_RIGHT);
}

uint8_t read_button(uint8_t button) {
	// Retorna 1 si el botón está presionado, 0 de lo contrario
	return !(PIND & (1 << button));
}

void draw_vertical_bar(uint8_t position, uint8_t column, char character) {
	OLED_SetCursor(position, column);
	OLED_Printf("%c", character);
}

void init_screen() {
	i2c_init();
	OLED_Init();
	OLED_Clear();
	OLED_SetCursor(0, 0);
	OLED_Printf("Ya empezo el despapaye");
	_delay_ms(1000);
	OLED_Clear();
}

int main(void) {
	uint8_t cursor_position_left = 0;
	uint8_t previous_position_left = 0xFF; // Un valor fuera de rango para asegurar que se dibuje inicialmente

	uint8_t cursor_position_right = 0;
	uint8_t previous_position_right = 0xFF; // Un valor fuera de rango para asegurar que se dibuje inicialmente

	uint8_t ball_position_x = 0;
	uint8_t ball_position_y = 8;
	int auxx = 0;
	int auxy = 0;
	bool boolX = 0;
	bool boolY = 0;
	uint8_t frame_count = 0; // Contador para controlar la velocidad de la pelota

	init_screen();
	OLED_Init(); // Inicializar el OLED
	OLED_Clear(); // Limpiar el display

	init_buttons(); // Inicializar botones

	while (1) {
		// Controlar barra izquierda
		if (read_button(BUTTON_UP_LEFT) && cursor_position_left > 0) {
			cursor_position_left--;
			_delay_ms(50); // Debounce delay
		}

		if (read_button(BUTTON_DOWN_LEFT) && cursor_position_left < 7) { // Asumiendo 8 filas
			cursor_position_left++;
			_delay_ms(50); // Debounce delay
		}

		// Controlar barra derecha
		if (read_button(BUTTON_UP_RIGHT) && cursor_position_right > 0) {
			cursor_position_right--;
			_delay_ms(50); // Debounce delay
		}

		if (read_button(BUTTON_DOWN_RIGHT) && cursor_position_right < 7) { // Asumiendo 8 filas
			cursor_position_right++;
			_delay_ms(50); // Debounce delay
		}

		// Dibujar barras si han cambiado de posición
		if (cursor_position_left != previous_position_left || cursor_position_right != previous_position_right) {
			// Borrar las barras anteriores
			draw_vertical_bar(0, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(1, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(2, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(3, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(4, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(5, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(6, 0, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(7, 0, ' '); // Columna 0 para la barra izquierda

			draw_vertical_bar(0, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(1, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(2, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(3, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(4, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(5, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(6, 120, ' '); // Columna 0 para la barra izquierda
			draw_vertical_bar(7, 120, ' '); // Columna 0 para la barra izquierda

			// Dibujar nuevas barras
			draw_vertical_bar(cursor_position_left, 0, '|'); // Columna 0 para la barra izquierda
			draw_vertical_bar(cursor_position_left + 1, 0, '|'); // Columna 0 para la barra izquierda

			draw_vertical_bar(cursor_position_right, 120, '|'); // Columna 126 para la barra derecha
			draw_vertical_bar(cursor_position_right + 1, 120, '|'); // Columna 126 para la barra derecha

			previous_position_left = cursor_position_left;
			previous_position_right = cursor_position_right;
		}

		// Actualizar la posición de la pelota solo cada ciertos frames
		if (frame_count == 0) {
			// Dibujar la pelota en blanco para borrar la posición anterior
			draw_vertical_bar(ball_position_x - 1, ball_position_y - 8, ' ');
			draw_vertical_bar(ball_position_x + 1, ball_position_y + 8, ' ');
			draw_vertical_bar(ball_position_x - 1, ball_position_y + 8, ' ');
			draw_vertical_bar(ball_position_x + 1, ball_position_y - 8, ' ');

			// Movimiento de la pelota
			if (auxx == 7) {
				boolX = 1;
			}
			if (auxx == 0) {
				boolX = 0;
			}
			if (boolX == 0) {
				ball_position_x += 1;
				auxx++;
			}
			if (boolX == 1) {
				ball_position_x -= 1;
				auxx--;
			}

			// Colisiones con las barras
			if ((auxy == 13 && ball_position_x == cursor_position_left) || (auxy == 13 && ball_position_x == cursor_position_left + 1)) {
				boolY = 1;
			}
			if ((auxy == 0 && ball_position_x == cursor_position_right) || (auxy == 0 && ball_position_x == cursor_position_right + 1)) {
				boolY = 0;
			}
			if (auxy == 0 && ball_position_x != cursor_position_right && (auxy == 0 && ball_position_x != cursor_position_right + 1)) {
				OLED_Printf("Perdiste2");
				OLED_Clear();
				_delay_ms(100);
				boolY = 0;
			}
			if (auxy == 13 && ball_position_x != cursor_position_left && (auxy == 13 && ball_position_x != cursor_position_left + 1)) {
				OLED_Printf("Perdiste1");
				OLED_Clear();
				_delay_ms(100);
				boolY = 1;
			}

			if (boolY == 0) {
				ball_position_y += 8;
				auxy++;
			}
			if (boolY == 1) {
				ball_position_y -= 8;
				auxy--;
			}

			// Dibujar la nueva posición de la pelota
			draw_vertical_bar(ball_position_x, ball_position_y, 'o');
		}

		frame_count = (frame_count + 1) % 5; // Ajusta el valor para cambiar la velocidad

		_delay_ms(50); // Para ralentizar el movimiento y evitar parpadeos
	}

	return 0; // Nunca alcanzado
}
