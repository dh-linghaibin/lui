/*
 * This file is part of the main.c
 *
 *  Copyright (c) : 2018年8月16日 linghaibin
 *      Author: a6735
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include "lui.h"

#define min(a, b) ( (a) < (b) ? (a) : (b) )

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

static unsigned short screen[LCD_WIDTH * LCD_LENGTH];

static unsigned scroll_pos;
static unsigned scroll_top;
static unsigned scroll_bottom;

static void update_texture(void)
{
    SDL_UpdateTexture(texture, NULL, screen, LCD_WIDTH * sizeof(screen[0]));
}

static void render_texture(void)
{
    SDL_Rect src, dst;
    unsigned w = LCD_WIDTH - (scroll_bottom + scroll_top);

    SDL_RenderClear(renderer);

    /* bottom */
    if (scroll_bottom) {
        src.x = 0;
        src.y = 0;
        src.w = scroll_bottom;
        src.h = LCD_LENGTH;

        SDL_RenderCopy(renderer, texture, &src, &src);
    }

    /* scrolling area */
    src.x = scroll_bottom + scroll_pos;
    src.y = 0;
    src.w = w - scroll_pos;
    src.h = LCD_LENGTH;
    dst.x = scroll_bottom;
    dst.y = 0;
    dst.w = w - scroll_pos;
    dst.h = LCD_LENGTH;

    SDL_RenderCopy(renderer, texture, &src, &dst);

    if (scroll_pos) {
        src.x = scroll_bottom;
        src.y = 0;
        src.w = scroll_pos;
        src.h = LCD_LENGTH;
        dst.x = scroll_bottom + w - scroll_pos;
        dst.y = 0;
        dst.w = scroll_pos;
        dst.h = LCD_LENGTH;
        SDL_RenderCopy(renderer, texture, &src, &dst);
    }

    /* top */
    if (scroll_top) {
        src.x = LCD_WIDTH - scroll_top;
        src.y = 0;
        src.w = scroll_top;
        src.h = LCD_LENGTH;

        SDL_RenderCopy(renderer, texture, &src, &src);
    }

    SDL_RenderPresent(renderer);
}

void lsdl_box(int x, int y, int width, int length, uint16_t * color) {
    for(int j = y; j < y+length; j++) {
        for(int i = x; i < x+width; i++) {
            screen[j*LCD_WIDTH+i] = *color++;
        }
    }
}

void tft_init(void)
{
    Uint32 flags = 0;
    /* Initialize SDL. */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(1);

    if (getenv("FULLSCREEN"))
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    window = SDL_CreateWindow("lui",
                  SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED,
                  LCD_WIDTH, LCD_LENGTH, flags);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(renderer, LCD_WIDTH, LCD_LENGTH);

    texture = SDL_CreateTexture(renderer,
                    SDL_PIXELFORMAT_RGB565,
                    SDL_TEXTUREACCESS_STREAMING,
                    LCD_WIDTH, LCD_LENGTH);
    if (!window || !renderer || !texture)
        exit(2);

    scroll_pos = scroll_top = scroll_bottom = 0;

    memset(screen, 0x08a3, sizeof(screen));
    update_texture();
    render_texture();
}

void tft_fill(int x, int y, int w, int h, unsigned color)
{
    int i, j;

    w = min(w, LCD_WIDTH - x);
    h = min(h, LCD_LENGTH - y);

    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++)
            screen[x + j + (y + i) * LCD_WIDTH] = color;
}

void tft_update(void)
{
    update_texture();
    render_texture();
}

SDL_Event even2;
uint8_t running = 1 ;
uint8_t touch_flag = 0;
static int tick_thread(void *data) {
    SDL_Delay(1);
    return 0;
}

int main(int argc, char *args[]) {
    tft_init();
    lui_init();
    lui_loop();
    SDL_CreateThread(tick_thread, "tick", NULL);
    tft_update();
    SDL_Delay(10);
    while(running) {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            switch( event.type ) {
                case SDL_QUIT: {
                    running = 0;
                } break;
                case SDL_MOUSEMOTION: {
                    if(touch_flag == 1) {
                        lui_obj_even(event.button.x, event.button.y, touch_flag);
                        lui_loop();
                    }
                } break;
                case SDL_MOUSEWHEEL: {

                } break;
                case SDL_MOUSEBUTTONDOWN: {
                    if( event.button.button == SDL_BUTTON_LEFT ){
                        //printf("down\n");
                        touch_flag = 1;
                        lui_obj_even(event.button.x, event.button.y, 2);
                        lui_loop();
                    }
                } break;
                case SDL_MOUSEBUTTONUP: {
                    if( event.button.button == SDL_BUTTON_LEFT ){
                        //printf("up\n");
                        touch_flag = 0;
                        lui_obj_even(event.button.x, event.button.y, touch_flag);
                        lui_loop();
                    }
                } break;
            }
            SDL_Delay(20);
            tft_update();
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
