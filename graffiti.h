/*
 * ISC License
 * 
 * © 2017 Mattias Andrée <m@maandree.se>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <stddef.h>

#include "utf.h"

enum graffitotype {
	GRAF_END,         /* End of sequence */
	GRAF_RUNE,        /* Rune */
	GRAF_RUNE_INVAL,  /* Rune from invalid encoding (e.g. xC0 x80)
			     or with invalid code point */
	GRAF_ESC_SEQ,     /* Escape sequence */
	GRAF_BYTE         /* A byte */
};

typedef struct {
	char *bytes;
	size_t nbytes;
	enum graffitotype type;
	Rune rune;
} Graffito;

/* Returns NULL if more bytes are needed. Cannot fail. */
char *getgraffito(char *str, size_t n, int final, Graffito *graf);
