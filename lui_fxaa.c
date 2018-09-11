
#include "lui_fxaa.h"
#include <emmintrin.h>
#include <string.h>


#if defined(__GNUC__) && ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#   define restrict __restrict
#elif defined(_MSC_VER) && _MSC_VER >= 1400
#   define restrict __restrict
#else
#   define restrict
#endif

//
// Implemented from spec:
//  http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf
//
// Other sources used:
//  Intel SIMD intrinsics guide
//  Nvidia Graphics SDK 11 (for shader implementation as reference)
//

// the higher the span the less area of screen is actually
// anti-aliased
#define SW_FXAA_SPAN_MAX 8
#define SW_FXAA_OFFS (((SW_FXAA_SPAN_MAX*8)>>4))

#define MM_MUL_IMPL(A, B, AI, BI) \
    _mm_shuffle_epi32( \
        _mm_shuffle_ps( \
            _mm_mul_epu32(A, B), \
            _mm_mul_epu32(AI,BI), \
            _MM_SHUFFLE(2,0,2,0) \
        ), \
        _MM_SHUFFLE(3,1,2,0) \
    )

// some utility simd macros
#define MM_MULI_N(A, B) MM_MUL_IMPL((A),(B), _mm_shuffle_epi32((A), _MM_SHUFFLE(3,3,1,1)), _mm_shuffle_epi32((B), _MM_SHUFFLE(3,3,1,1)))
#define MM_MULI_I(A, B) MM_MUL_IMPL((A),(B), _mm_shuffle_epi32((A), _MM_SHUFFLE(3,3,1,1)), (B))
#define MM_LANE_I(A, B) (((int*)(A))[(B)])

// luma operator:
#define LUMA(RGB, AND1, SRLI1, AND2, SRLI2, AND3, SRLI3) \
    _mm_add_epi32( \
        _mm_add_epi32( \
            _mm_srli_epi32(_mm_and_si128((RGB), _mm_set1_epi32(AND1)), SRLI1), \
            _mm_srli_epi32(_mm_and_si128((RGB), _mm_set1_epi32(AND2)), SRLI2) \
        ), \
        _mm_srli_epi32(_mm_and_si128((RGB), _mm_set1_epi32(AND3)), SRLI3) \
    )

// bilinear interpolation filter (32bit)
__m128i bilinear_filter32(
    const unsigned int* const restrict fb,
    __m128i offs,
    __m128i dx,
    __m128i dy,
    __m128i pt,
    unsigned int p
) {

    const __m128i f128  = _mm_set1_epi32(0xFu);
    const __m128i mask1 = _mm_set1_epi32(0xFF00FFu);
    const __m128i mask2 = _mm_set1_epi32(0xFF00u);
    const __m128i fe1   = _mm_set1_epi32(0xFE00FE00u);
    const __m128i fe2   = _mm_set1_epi32(0x00FE0000u);
    const __m128i dy4n  = _mm_add_epi32(_mm_srai_epi32(dx,4),MM_MULI_I(_mm_srai_epi32(dy,4),pt));
    const __m128i osa   = _mm_add_epi32(offs,dy4n);
    const __m128i osb   = _mm_sub_epi32(offs,dy4n);

    dx = _mm_and_si128(dx, f128);
    dy = _mm_and_si128(dy, f128);

    const __m128i xy          = MM_MULI_N(dx,dy);
    const __m128i x16         = _mm_slli_epi32(dx,4);
    const __m128i invxy       = _mm_sub_epi32(_mm_slli_epi32(dy,4),xy);
    const __m128i xinvy       = _mm_sub_epi32(x16,xy);
    const __m128i invxinvy    = _mm_sub_epi32(_mm_sub_epi32(_mm_set1_epi32(256),x16),invxy);
    const unsigned int oa0    = MM_LANE_I(&osa, 0);
    const unsigned int oa1    = MM_LANE_I(&osa, 1);
    const unsigned int oa2    = MM_LANE_I(&osa, 2);
    const unsigned int oa3    = MM_LANE_I(&osa, 3);
    const __m128i r00a        = _mm_set_epi32(fb[oa3],     fb[oa2],     fb[oa1],     fb[oa0]);
    const __m128i r10a        = _mm_set_epi32(fb[oa3+1],   fb[oa2+1],   fb[oa1+1],   fb[oa0+1]);
    const __m128i r01a        = _mm_set_epi32(fb[oa3+p],   fb[oa2+p],   fb[oa1+p],   fb[oa0+p]);
    const __m128i r11a        = _mm_set_epi32(fb[oa3+p+1], fb[oa2+p+1], fb[oa1+p+1], fb[oa0+p+1]);

    const __m128i lerp0 =
        _mm_srli_epi32(
            _mm_or_si128(
                _mm_and_si128(
                    _mm_add_epi32(
                        _mm_add_epi32(
                            MM_MULI_N(_mm_and_si128(r00a, mask1), invxinvy),
                            MM_MULI_N(_mm_and_si128(r10a, mask1), xinvy)
                        ),
                        _mm_add_epi32(
                            MM_MULI_N(_mm_and_si128(r01a, mask1), invxy),
                            MM_MULI_N(_mm_and_si128(r11a, mask1), xy)
                        )
                    ),
                    fe1
                ),
                _mm_and_si128(
                    _mm_add_epi32(
                        _mm_add_epi32(
                            MM_MULI_N(_mm_and_si128(r00a, mask2), invxinvy),
                            MM_MULI_N(_mm_and_si128(r10a, mask2), xinvy)
                        ),
                        _mm_add_epi32(
                            MM_MULI_N(_mm_and_si128(r01a, mask2), invxy),
                            MM_MULI_N(_mm_and_si128(r11a, mask2), xy)
                        )
                    ),
                    fe2
                )
            ),
            9
        );

    const unsigned int ob0 = MM_LANE_I(&osb, 0);
    const unsigned int ob1 = MM_LANE_I(&osb, 1);
    const unsigned int ob2 = MM_LANE_I(&osb, 2);
    const unsigned int ob3 = MM_LANE_I(&osb, 3);
    const __m128i r11b     = _mm_set_epi32(fb[ob3-1-p], fb[ob2-1-p], fb[ob1-1-p], fb[ob0-1-p]);
    const __m128i r01b     = _mm_set_epi32(fb[ob3-p],   fb[ob2-p],   fb[ob1-p],   fb[ob0-p]);
    const __m128i r10b     = _mm_set_epi32(fb[ob3-1],   fb[ob2-1],   fb[ob1-1],   fb[ob0-1]);
    const __m128i r00b     = _mm_set_epi32(fb[ob3],     fb[ob2],     fb[ob1],     fb[ob0]);

    return _mm_add_epi32(
                lerp0,
                _mm_srli_epi32(
                    _mm_or_si128(
                        _mm_and_si128(
                            _mm_add_epi32(
                                _mm_add_epi32(
                                    MM_MULI_N(_mm_and_si128(r00b, mask1),invxinvy),
                                    MM_MULI_N(_mm_and_si128(r10b, mask1),xinvy)
                                ),
                                _mm_add_epi32(
                                    MM_MULI_N(_mm_and_si128(r01b, mask1),invxy),
                                    MM_MULI_N(_mm_and_si128(r11b, mask1),xy)
                                )
                            ),
                            fe1
                        ),
                        _mm_and_si128(
                            _mm_add_epi32(
                                _mm_add_epi32(
                                    MM_MULI_N(_mm_and_si128(r00b, mask2),invxinvy),
                                    MM_MULI_N(_mm_and_si128(r10b, mask2),xinvy)
                                ),
                                _mm_add_epi32(
                                    MM_MULI_N(_mm_and_si128(r01b, mask2),invxy),
                                    MM_MULI_N(_mm_and_si128(r11b, mask2),xy)
                                )
                            ),
                            fe2
                        )
                    ),
                    9
                )
            );
}

// fxaa filter
void fxaa_32bit(
    const int ystart,
    const int yend,
    const int xstart,
    const int xend,

    const unsigned int width,
    const unsigned int owidth,
    const unsigned int nwidth,
    const unsigned int height,

    unsigned int*  const restrict buffercopy,
    unsigned int*  const restrict bufferfinal,
    unsigned char* const restrict mask

) {
    const __m128i t4444     = _mm_set1_epi32(4);
    const __m128i fefefe    = _mm_set1_epi32(0xFEFEFE);
    const __m128i fcfcfc    = _mm_set1_epi32(0xFCFCFC);
    const __m128i w0        = _mm_set_epi32(0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0);
    const __m128  x7fffffff = ((__m128)_mm_set1_epi32(0x7FFFFFFF));
    const __m128  pspanmax  = _mm_set1_ps( (float)(SW_FXAA_SPAN_MAX*8));
    const __m128  nspanmax  = _mm_set1_ps(-(float)(SW_FXAA_SPAN_MAX*8));
    const __m128i owidth128 = _mm_set1_epi32(owidth);

    // borders
    for(int y = 0; y < ystart; ++y)
        memcpy(bufferfinal+y*nwidth,buffercopy+y*owidth,width*4);
    for(int y = yend; y < (int)height; ++y)
        memcpy(bufferfinal+y*nwidth,buffercopy+y*owidth,width*4);
    

    // this can be threaded
    // e.g:
    #pragma omp parallel for schedule(dynamic)
    for(int y = ystart; y < yend; ++y)
    {
        unsigned int offsm1   = (y-1)*owidth - 1 + xstart;
        unsigned int offsn    = (y*nwidth + xstart);
        unsigned int offsmask = (y*width  + xstart)>>2;
        __m128i      pitch    = _mm_add_epi32(_mm_set_epi32(3,2,1,0),_mm_set1_epi32(y*owidth + xstart));


        // borders
        for(int x = 0; x < xstart; ++x)
            bufferfinal[offsn-xstart+x] = buffercopy[offsm1-xstart+owidth+1+x];
        for(int x = xend; x < (int)width; ++x)
            bufferfinal[offsn-xstart+x] = buffercopy[offsm1-xstart+owidth+1+x];


        for(int x = xstart; x < xend; x+=4, offsm1+=4, offsn+=4, ++offsmask, pitch = _mm_add_epi32(pitch, t4444)) {
            if(mask[offsmask] == 0)
            {
                const unsigned int offs   = offsm1+owidth;
                const unsigned int offsp1 = offs+owidth;

                // Directional stages:
                #define DEFINE_STAGE(N1, N2, N3, N4, N5, OFFSET) \
                    const __m128i N1 = _mm_and_si128((__m128i)_mm_load_ss((float*)(buffercopy+OFFSET)), fcfcfc); \
                    const __m128i N2 = _mm_and_si128(_mm_load_si128((__m128i*)(buffercopy+OFFSET+1)), fcfcfc); \
                    const __m128i N3 = _mm_and_si128((__m128i)_mm_load_ss((float*)(buffercopy+OFFSET+5)), fcfcfc); \
                    const __m128i N4 = _mm_or_si128(N1,_mm_and_si128(_mm_shuffle_epi32(N2,_MM_SHUFFLE(2,1,0,0)), w0)); \
                    const __m128i N5 = _mm_shuffle_epi32(_mm_or_si128(N3,_mm_and_si128(N2, w0)),_MM_SHUFFLE(0,3,2,1))

                // NW = texture2D(First_Texture, TexCoord1 + (vec2(-1.0, -1.0) * PixelSize)).xyz
                // NE = texture2D(First_Texture, TexCoord1 + (vec2(+1.0, -1.0) * PixelSize)).xyz
                DEFINE_STAGE(NW, rN, NE, rNW, rNE, offsm1);

                // SW = texture2D(First_Texture, TexCoord1 + (vec2(-1.0, +1.0) * PixelSize)).xyz
                // SE = texture2D(First_Texture, TexCoord1 + (vec2(+1.0, +1.0) * PixelSize)).xyz
                DEFINE_STAGE(SW, rS, SE, rSW, rSE, offsp1);

                // M  = texture2D(First_Texture, TexCoord1).xyz
                DEFINE_STAGE(W,  rM, E,  rW,  rE,  offs);
                
                #undef DEFINE_STAGE

                const __m128i rMrN   = _mm_add_epi32(rM,rN);
                const __m128i rMrS   = _mm_add_epi32(rM,rS);
                const __m128i lNW    = LUMA(_mm_add_epi32(_mm_add_epi32(rMrN,rNW),rW), 0x3FCu, 5, 0x3FC00u, 11, 0xFE0000u, 20);
                const __m128i lNE    = LUMA(_mm_add_epi32(_mm_add_epi32(rMrN,rNE),rE), 0x3FCu, 5, 0x3FC00u, 11, 0xFE0000u, 20);
                const __m128i lSW    = LUMA(_mm_add_epi32(_mm_add_epi32(rMrS,rSW),rW), 0x3FCu, 5, 0x3FC00u, 11, 0xFE0000u, 20);
                const __m128i lSE    = LUMA(_mm_add_epi32(_mm_add_epi32(rMrS,rSE),rE), 0x3FCu, 5, 0x3FC00u, 11, 0xFE0000u, 20);
                const __m128i mS     = _mm_cmpgt_epi32(lSW,lSE);
                const __m128i mN     = _mm_cmpgt_epi32(lNW,lNE);
                const __m128i tMax   = _mm_or_si128(_mm_and_si128(mS,lSW), _mm_andnot_si128(mS,lSE));
                const __m128i tMin   = _mm_or_si128(_mm_and_si128(mS,lSE), _mm_andnot_si128(mS,lSW));
                const __m128i tMax2  = _mm_or_si128(_mm_and_si128(mN,lNW), _mm_andnot_si128(mN,lNE));
                const __m128i tMin2  = _mm_or_si128(_mm_and_si128(mN,lNE), _mm_andnot_si128(mN,lNW));
                const __m128i SWSE   = _mm_add_epi32(lSW,lSE);
                const __m128i NWNE   = _mm_add_epi32(lNW,lNE);
                const __m128 fdirx   = _mm_cvtepi32_ps(_mm_sub_epi32(SWSE,NWNE));
                const __m128 fdiry   = _mm_cvtepi32_ps(_mm_sub_epi32(_mm_add_epi32(lNW,lSW),_mm_add_epi32(lNE,lSE)));
                const __m128 temp    = _mm_rcp_ps(
                                            _mm_add_ps(
                                                _mm_mul_ps(
                                                    _mm_min_ps(
                                                        _mm_and_ps(fdirx, x7fffffff),
                                                        _mm_and_ps(fdiry, x7fffffff)
                                                    ),
                                                    _mm_set1_ps((float)(1.0/8.0))
                                                ),
                                                _mm_max_ps(
                                                    _mm_mul_ps(
                                                        _mm_cvtepi32_ps(_mm_add_epi32(NWNE, SWSE)),
                                                        _mm_set1_ps((float)(1.0/256.0))
                                                    ),
                                                    _mm_set1_ps((float)(1.0/4.0))
                                                )
                                            )
                                        );

                const __m128i dirx = _mm_cvtps_epi32(_mm_min_ps(pspanmax, _mm_max_ps(nspanmax, _mm_mul_ps(fdirx, temp))));
                const __m128i diry = _mm_cvtps_epi32(_mm_min_ps(pspanmax, _mm_max_ps(nspanmax, _mm_mul_ps(fdiry, temp))));
                const __m128i virx = _mm_srai_epi32(dirx, 2);
                const __m128i viry = _mm_srai_epi32(diry, 2);
            
                __m128i rB = bilinear_filter32(buffercopy, pitch, dirx, diry, owidth128, owidth);
                __m128i rA = bilinear_filter32(buffercopy, pitch, virx, viry, owidth128, owidth);
                rB         = _mm_srli_epi32(
                                _mm_add_epi32(
                                    _mm_and_si128(rA, fefefe),
                                    _mm_and_si128(rB, fefefe)
                                ),
                                1
                            );

                const __m128i lM = LUMA(rM, 0xFFu, 3, 0xFF00u, 9, 0x00FE0000u, 18);
                const __m128i lB = LUMA(rB, 0xFFu, 3, 0xFF00u, 9, 0x00FE0000u, 18);
                const __m128i mL =  _mm_or_si128(
                                            _mm_and_si128(
                                                _mm_and_si128(
                                                    _mm_cmplt_epi32(lB,lM),
                                                    _mm_cmplt_epi32(lB,tMin)
                                                ),
                                                _mm_cmplt_epi32(lB,tMin2)
                                            ),
                                            _mm_and_si128(
                                                _mm_and_si128(
                                                    _mm_cmpgt_epi32(lB,lM),
                                                    _mm_cmpgt_epi32(lB,tMax)
                                                ),
                                                _mm_cmpgt_epi32(lB,tMax2)
                                            )
                                        );

                _mm_store_si128(
                    (__m128i*)(bufferfinal+offsn),
                    _mm_or_si128(
                        _mm_and_si128   (mL, rA),
                        _mm_andnot_si128(mL, rB)
                    )
                );
                mask[offsmask] = 1;
            }
        }
    }
}

#undef MM_MUL_IMPL
#undef MM_MULI_I
#undef MM_MULI_N
#undef MM_LANE_I
#undef LUMA

// to use fxaa32 you need to know what you're going to be actually applying
// fxaa to, the first time around you'd apply to the whole frame, that can
// be acomplished by allocating a chunk of memory exactly (w * h) >> 2 which
// is used for a mask, then you need your input and output render target
// memory as RGBA32, and finally you need to know the width, height, and
// pitch of what you're operating on.
//
// e.g
// unsigned char *mask = malloc((w * h) >> 2);
// fxaa_32bit(
//        SW_FXAA_OFFS,
//        (h-SW_FXAA_OFFS),
//        SW_FXAA_OFFS&0xFFFFFFFC,
//        (w-SW_FXAA_OFFS)&0xFFFFFFFC,
//        w,
//        (pitch >> 2),
//        (pitch >> 2),
//        h,
//        (unsigned int *)from_rgba_32,
//        (unsigned int *)to_rgba_32,
//        mask
// );
//
// keeping this mask alongside you can thus update just REGIONS of the
// screen that change (instead of all per frame), to do this you need
// to know the left, right, top, AND bottom of the rectangular region
// to update, e.g
// int left   = element.left   + update.left;
// int right  = element.right  + update.right;
// int top    = element.top    + update.top
// int bottom = element.bottom + update.bottom;
// if (!((left >= right) || (top >= bottom))) {
//     // update the region (and area around it that changes)
//     fxaa_32bit(
//          max(top-(int)SW_FXAA_OFFS,(int)SW_FXAA_OFFS),
//          min((unsigned int)bottom+SW_FXAA_OFFS,h-(SW_FXAA_OFFS)),
//          max(left-(int)SW_FXAA_OFFS,(int)SW_FXAA_OFFS)&0xFFFFFFFC,
//          min((unsigned int)r+SW_FXAA_OFFS,w-SW_FXAA_OFFS)&0xFFFFFFFC,
//          w,
//          pitch >> 2,
//          pitch >> 2,
//          h,
//          (unsigned int   *)from_rgba_32,
//          (unsigned int   *)to_rgab_32,
//          mask
//     );
// }
//
//#define TEST_APP
#ifdef  TEST_APP
#include <SDL.h>
#include <SDL_image.h>
#include <png.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// SDL_api oriented PNG writing of surfaces
static void pngError(png_structp ctx, png_const_charp str) {
    SDL_SetError("%s", str);
}

static void pngWrite(png_structp pngPtr, png_bytep data, png_size_t len) {
    SDL_RWops  *rw = (SDL_RWops*)png_get_io_ptr(pngPtr);
    SDL_RWwrite(rw, data, sizeof(png_byte), len);
}

SDL_Surface *pngFormat(SDL_Surface *src) {
    if (src->format->BitsPerPixel <= 24 || src->format->Amask) {
        src->refcount++;
        return src;
    }

    SDL_Surface *surf = SDL_CreateRGBSurface(
        src->flags,
        src->w,
        src->h,
        24,
        src->format->Rmask,
        src->format->Gmask,
        src->format->Bmask,
        0 // no alpha
    );

    // conv blit
    SDL_BlitSurface(src, NULL, surf, NULL);

    return surf;
}

#define SUCCESS 0
#define ERROR  -1
int pngSaveRW(SDL_Surface *surface, SDL_RWops *dst, int freedst) {
    png_structp png_ptr;
    png_infop   info_ptr;
    png_colorp  pal_ptr;
    SDL_Palette *pal;
    int i, colortype;
    png_bytep *row_pointers;

    /* Initialize and do basic error checking */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, pngError, NULL); /* err_ptr, err_fn, warn_fn */
    if (!png_ptr)
    {
        SDL_SetError("Unable to png_create_write_struct on %s\n", PNG_LIBPNG_VER_STRING);
        if (freedst) SDL_FreeRW(dst);
        return (ERROR);
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        SDL_SetError("Unable to png_create_info_struct\n");
        png_destroy_write_struct(&png_ptr, NULL);
        if (freedst) SDL_FreeRW(dst);
        return (ERROR);
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        if (freedst) SDL_FreeRW(dst);
        return (ERROR);
    }

    png_set_write_fn(png_ptr, dst, pngWrite, NULL);

    colortype = PNG_COLOR_MASK_COLOR;
    if (surface->format->BytesPerPixel > 0
    && surface->format->BytesPerPixel <= 8
    && (pal = surface->format->palette))
    {
        colortype |= PNG_COLOR_MASK_PALETTE;
        pal_ptr = (png_colorp)malloc(pal->ncolors * sizeof(png_color));
        for (i = 0; i < pal->ncolors; i++) {
            pal_ptr[i].red   = pal->colors[i].r;
            pal_ptr[i].green = pal->colors[i].g;
            pal_ptr[i].blue  = pal->colors[i].b;
        }
        png_set_PLTE(png_ptr, info_ptr, pal_ptr, pal->ncolors);
        free(pal_ptr);
    }
    else if (surface->format->BytesPerPixel > 3 || surface->format->Amask)
        colortype |= PNG_COLOR_MASK_ALPHA;

    png_set_IHDR(png_ptr, info_ptr, surface->w, surface->h, 8, colortype,
    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_bgr(png_ptr);

    png_write_info(png_ptr, info_ptr);
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep)*surface->h);
    for (i = 0; i < surface->h; i++)
        row_pointers[i] = (png_bytep)(Uint8*)surface->pixels + i * surface->pitch;

    png_write_image(png_ptr, row_pointers);
    free(row_pointers);
    png_write_end(png_ptr, info_ptr);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    if (freedst) SDL_FreeRW(dst);
    return (SUCCESS);
}

SDL_Surface *loadImage(const char *file) {
    SDL_Surface *tmp = NULL;
    SDL_Surface *ret = NULL;

    if (!(tmp = IMG_Load(file)))
        return NULL;

    ret = SDL_DisplayFormat(tmp); // convert to display format
    SDL_FreeSurface(tmp);

    return ret;
}

int main(int argc, char **argv) {
    argc--;
    argv++;

    if (!argc)
        return 0;


    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    SDL_WM_SetCaption("FXAA via SSE, By: Dale Weiler", "FXAA");

    // 1600x600 == (2 side by side 800x600 images)
    SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    SDL_Surface *load   = loadImage(*argv);
    SDL_Surface *copy   = NULL;

    if (!load) {
        fprintf(stderr, "failed to open image: %s for FXAA %s\n", *argv, SDL_GetError());
        return EXIT_FAILURE;
    }

    // scale video mode for two side/by/side images
    if (!(screen = SDL_SetVideoMode(load->w * 2, load->h, 32, SDL_SWSURFACE))) {
        fprintf(stderr, "failed to resize window for comparision %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // create surface for AA filter
    copy = SDL_CreateRGBSurface(
        SDL_SWSURFACE,
        load->w,
        load->h,
        load->format->BitsPerPixel,
        load->format->Rmask,
        load->format->Gmask,
        load->format->Bmask,
        load->format->Amask
    );

    unsigned char *mask = (unsigned char *)malloc((copy->w * copy->h) >> 2);
    memset(mask, 0, (copy->w * copy->h) >> 2);
    assert(copy->format->BitsPerPixel == 32);

    // lock for write
    SDL_LockSurface(copy);
        fxaa_32bit(
            (SW_FXAA_OFFS),
            (copy->h-(SW_FXAA_OFFS)),
            (SW_FXAA_OFFS)&0xFFFFFFFC,
            (copy->w-(SW_FXAA_OFFS))&0xFFFFFFFC,
            copy->w,
            (copy->pitch >> 2),
            (copy->pitch >> 2),
            copy->h,
            (unsigned int *)load->pixels,
            (unsigned int *)copy->pixels,
            mask
        );
    SDL_UnlockSurface(copy);

    int          running = 1;
    SDL_Rect     before  = { 0,       0, 0, 0 };
    SDL_Rect     after   = { load->w, 0, 0, 0 };
    SDL_Surface *shot    = NULL;
    SDL_Event    event;
    while (running) {
        SDL_BlitSurface(load, NULL, screen, &before);
        SDL_BlitSurface(copy, NULL, screen, &after);
        SDL_Flip(screen);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0; break;
                case SDL_KEYDOWN:
                    shot = pngFormat(screen);
                    if (pngSaveRW(shot, SDL_RWFromFile("screenshot.png", "wb"), 1) == ERROR)
                        fprintf(stderr, "failed saving screenshot %s\n", SDL_GetError());
                    SDL_FreeSurface(shot);
                    break;
            }
        }
    }

    free(mask);
    SDL_FreeSurface(load);
    SDL_FreeSurface(copy);
    SDL_Quit();
    return 0;
}
#endif