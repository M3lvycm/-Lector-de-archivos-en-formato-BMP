#include "pintor.h"
#include "bmp.h"

#include <stdio.h>

void dibujar(sdl_config* config, imagen_bmp* imagen)
{
	SDL_SetRenderDrawColor(config->render, 0, 0, 0, 255);
	SDL_RenderClear(config->render);

	for(uint32_t y = 0; y < imagen->alto; y++)
		for(uint32_t x = 0; x < imagen->ancho; x++)
			pintar(config, x, y, &imagen->pixeles[y * imagen->ancho + x]);

	SDL_RenderPresent(config->render);
}

int main(void)
{
	sdl_config config;
	SDL_Event evento;
	int salir = 0;
	int espejo = 0;
	imagen_bmp* imagen = cargar_bmp("test.bmp");

	if(imagen == NULL)
	{
		fprintf(stderr, "No pude cargar el test.bmp\n");
		return 1;
	}

	iniciar(&config, imagen->ancho, imagen->alto);
	dibujar(&config, imagen);

	printf("Normal ahora. Presiona E pa el espejo, otra vez E pa quitarlo.\n");
	printf("Cierra la ventana pa salir.\n");

	while(!salir)
	{
		while(SDL_PollEvent(&evento))
		{
			if(evento.type == SDL_QUIT)
				salir = 1;

			if(evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_e)
			{
				flip_horizontal(imagen);
				espejo = !espejo;
				dibujar(&config, imagen);
				if(espejo)
					printf("Espejo ON\n");
				else
					printf("Espejo OFF\n");
			}
		}
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(config.render);
	SDL_DestroyWindow(config.ventana);
	SDL_Quit();
	destruir_bmp(imagen);
}
