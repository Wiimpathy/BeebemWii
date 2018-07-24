#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>

#include <graphics_font.h>
#include <functions.h>
#include <graphics.h>


/* SDL_Surface locking:
 *  */
#define LOCK(s)   {if(SDL_MUSTLOCK(s))(void)SDL_LockSurface(s);}
#define UNLOCK(s) {if(SDL_MUSTLOCK(s))(void)SDL_UnlockSurface(s);}

/* Private:
 */

EG_Font* EG_Font_New(void)
{
	size_t i;
	EG_Font *f_p;

        /* Allocate some space:
         */
        if ( (f_p = (EG_Font*) EG_Malloc(sizeof(EG_Font)) ) == NULL) {
                // [TODO] Log error
                return NULL;
        }

	/* Initialize EG_Font struct:
	 */

	f_p->name_p = NULL;
	f_p->hash = 0;

	f_p->width = 0;
	f_p->pitch = 0;
	f_p->height = 0;

	f_p->data_p = NULL;

	for(i=0; i<256; i++)
		f_p->ch[i].width = 0;

	return f_p;
}

void	EG_Font_Free(EG_Font *f_p)
{
	// [TODO] Check pointer
	EG_Free(f_p);
}

unsigned int EG_Font_GetHash(EG_Font *f_p)
{
	if (f_p == NULL)
		return EG_Font_NULL;
	else
		return f_p->hash;
}

char* EG_Font_GetName(EG_Font *f_p)
{
	if (f_p == NULL)
		return NULL;
	else
		return (char*) f_p->name_p;
}

unsigned int EG_Font_GetWidth(EG_Font *f_p)
{
	if (f_p == NULL)
		return 0;
	else
		return f_p->width;
	
}

unsigned int EG_Font_GetHeight(EG_Font *f_p)
{
	if (f_p == NULL)
		return 0;
	else
		return f_p->width;
}

/* Convert an existing SDL Surface into a font:
 */

static EG_BOOL SetName(EG_Font *f_p, char *name_p)
{
	char *fontname_p, null_name[]="";

	unsigned int size;

	/* If provided font name is NULL, then use an empty string instead:
	 */
	if (name_p == NULL)
		fontname_p = null_name;
	else
		fontname_p = name_p;

	// [TODO] Small mallocs are not honored in OPEN2X???
	size = strlen(fontname_p)+1;
	if (size < 20) size = 20;

	/* Set the font name to the one provided:
	 */
	if ( (f_p->name_p=(char*)EG_Malloc(size)) == NULL ) {
		// log error
		EG_Font_Free(f_p);
		return EG_FALSE;
	}
	strcpy( (char*) f_p->name_p, fontname_p);

	/* Calculate a hash for the font name (as uppercase letters):
	 */
//	if ( (f_p->hash=EG_UpperCaseHash( (char*) f_p->name_p)) == EG_HASH_FAILED) {
//		printf("hash failed\n");
//		EG_Free(f_p->name_p);
//		return EG_FALSE;
//	}

	f_p->hash=EG_MakeStringHash(f_p->name_p);

	return EG_TRUE;
}

#define SET_BIT(var, bit) (var)|=(0|(1<<(bit)))
static EG_BOOL TranslateSurfaceIntoFont(EG_Font *f_p, SDL_Surface *surface_p
 , EG_FontMap *mapping_p, SDL_Color col)
{
	unsigned int i, u, v, x, y, pixel, col_as_pixel, u32_pitch, *p;

	/* Create 1bit depth bitmap for font data:
	 */
	if ( ( f_p->data_p=(Uint32*)EG_Malloc( f_p->height*f_p->pitch*256 ) )
	 == NULL)
		return EG_FALSE;

	/* Remember the pitch as an offset in 32bit values (I'm bound to
	 * miss one below)...
	 */
	u32_pitch = f_p->pitch>>2;

	/* Clear the buffer - as we will OR changes on a bit by bit basis:
	 */

	// TODO use memset here.
	for (i=0; i<f_p->height*u32_pitch*256; i++)
		f_p->data_p[i]=0;

	/* Determine pixel we're interested in:
	 */
	col_as_pixel = SDL_MapRGB(surface_p->format, col.r, col.g, col.b);

	/* Translate the Surface into raw font data:
	 */
	for (i=0; i<256; i++) {
		p=f_p->data_p+(i * f_p->height * u32_pitch);

		/* Get location of next character on DC:
		 */
		x = mapping_p->ch[i].column * f_p->width;
		y = mapping_p->ch[i].row * f_p->height;

		f_p->ch[i].width = mapping_p->ch[i].width;

	//	printf("[%d] %d,%d %d\n", i, x, y, f_p->ch[i].width);

		/* Copy that pixel by pixel over to the 1bit depth font:
		 */
		for (v=0; v<f_p->height; v++) {
			for (u=0; u<f_p->width; u++) {
				pixel = EG_Graphics_GetPixel(surface_p, x+u, y+v);

				//printf("%d", pixel);

				if (pixel == col_as_pixel)
					SET_BIT( *(p+(u>>5)), u&31 );
				//	printf(" %d ", *(p+(u>>5)));
			}
			p+=u32_pitch;

		//	printf("\n");
		}
	}

	return EG_TRUE;
}

/* Convert a DC into a font:
 */
EG_Font* EG_Font_Import(char *name_p, EG_FontMap *mapping_p, SDL_Surface *surface_p
, SDL_Color col)
{
	EG_Font *f_p;

	/* Handle obvious gotcha's:
	 */
	if (surface_p == NULL || mapping_p == NULL)
		return NULL;

	if (mapping_p->ncolumns == 0 || mapping_p->nrows == 0)
		return NULL;

	/* Create the font (object) struct:
	 */
	if ( (f_p=EG_Font_New()) == NULL) {
		// log error
		return NULL;
	}

	/* Configure the font:
	 */
	if (SetName(f_p, name_p) == EG_FALSE) {
		EG_Font_Free(f_p);
		return NULL;
	}

	f_p->width = surface_p->w / mapping_p->ncolumns;
	f_p->height = surface_p->h / mapping_p->nrows;
	f_p->pitch = (((f_p->width-1)>>3)&(~0-3))+4;

	/* Translate DC into a font:
	 */
	if (TranslateSurfaceIntoFont(f_p, surface_p, mapping_p, col) == EG_FALSE) {
		// log error
		EG_Free(f_p->name_p);
		EG_Font_Free(f_p);
		return NULL;
	}

	return f_p;
}

#define GET_BIT(var, bit) (var)&(0|(1<<bit))

/* Output a character onto the surface (you can output 0 too):
 */
void EG_Font_PutChar(EG_Font *f_p, SDL_Surface *surface_p, char c, int x, int y
 , unsigned int flags)
{
	unsigned int *data_p, pixel, prev_pixel, ch_index;
	int u, v, bold_x_inc, italic_slope, underline_offset;

	// [TODO] Make this variable.
	underline_offset = f_p->height +1;

	if (f_p == NULL || surface_p == NULL)
		return;

	LOCK(surface_p);

	ch_index = (int) ((unsigned char) c);

	data_p = f_p->data_p + ch_index * f_p->height * (f_p->pitch>>2) ;
	bold_x_inc=flags>>1&1;
	italic_slope=0;

	for(v=0; v < (int) f_p->height; v++) {
		prev_pixel=0;
		if (flags&4) italic_slope = (f_p->height-v)>>2;

		for(u=0; u < (int) f_p->ch[ch_index].width+bold_x_inc; u++) {
			pixel=GET_BIT( *( data_p+(u>>5)), (u&31) );

			if ( pixel || prev_pixel )
				EG_Graphics_DrawFastPoint(surface_p, x+u+italic_slope, y+v);

			if (flags&2) prev_pixel = pixel;
			}

		data_p+=f_p->pitch>>2;
	}

	if (flags&1) {
		for(u=0; u < (int) f_p->ch[ch_index].width+bold_x_inc; u++)
			EG_Graphics_DrawFastPoint(surface_p, x+u, y+underline_offset);
	}

	UNLOCK(surface_p);
}


/* Output a string of characters onto the surface (you cannot output 0):
 */
void EG_Font_PutString(EG_Font *f_p, SDL_Surface *surface_p, char *string_p, int x
 , int y, unsigned int flags)
{
	unsigned int i, ch_index;

	if ( string_p == NULL || surface_p == NULL || f_p == NULL
	 || strlen(string_p)==0 ) return;

	for(i=0; i< strlen(string_p); i++) {
		EG_Font_PutChar(f_p, surface_p, string_p[i], x, y, flags);
		
		ch_index = (unsigned int) ((unsigned char) string_p[i]);
		x+= f_p->ch[ ch_index ].width;
	}
}

/* Free up resources associated with a font:
 */
void EG_Font_Destroy(EG_Font *f_p)
{
	if (f_p == NULL) {
		// log error
		return;
	}

	EG_Free(f_p->name_p);
	EG_Free(f_p->data_p);

	EG_Font_Free(f_p);
}
