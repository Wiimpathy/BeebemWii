#ifndef _EG_FONTMAP_H_
#define _EG_FONTMAP_H_

typedef struct {
        unsigned int row, column, width;
} EG_FontMap_Char;

typedef struct {
        unsigned int nrows, ncolumns;

        EG_FontMap_Char ch[256];
} EG_FontMap;

#endif
