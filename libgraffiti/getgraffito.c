/* See LICENSE file for copyright and license details. */
#include <ctype.h>
#include <string.h>

#include "../graffiti.h"

char *
getgraffito(char *str, size_t n, int final, Graffito *graf)
{
	int i, m;
	Rune rune;
	char c;

	graf->bytes = str;
	graf->rune = -1;
	if (!n) {
		graf->nbytes = 0;
		graf->type = GRAF_END;
		return str;
	} else if ((*str & 0xC0) == 0x80) {
	byte:
		graf->nbytes = 1;
		graf->type = GRAF_BYTE;
		return str;
	} else if (!(*str & 0x80)) {
		if (*str == 0x1B)
			goto esc;
		graf->nbytes = 1;
		graf->type = GRAF_RUNE;
		graf->rune = *str;
		return str + 1;
	}
	for (c = *str, m = 0; c & 0x80;)
		c <<= 1, m += 1;
	if (m > 6)
		goto byte;
	rune = c >> m;
	for (i = 1; i < m; i++) {
		if ((size_t)i == n) {
			if (final)
				goto byte;
			return NULL;
		} else if ((*str & 0xC0) != 0x80) {
			goto byte;
		}
		rune <<= 6;
		rune |= *str ^ 0x80;
	}
	graf->nbytes = 1;
	graf->type = runelen(rune) == m ? GRAF_RUNE : GRAF_RUNE_INVAL;
	graf->rune = rune;
	return str + m;

esc:
	graf->nbytes = 1, str++, n--;
	graf->type = GRAF_ESC_SEQ;
	if (!n)
		return final ? str : NULL;
	if (*str == 'N') goto ss2;
	if (*str == 'O') goto ss3;
	if (*str == 'P') goto dcs;
	if (*str == 'X') goto sos;
	if (*str == 'Y') goto esc_y;
	if (*str == '[') goto csi;
	if (*str == ']') goto osc;
	if (*str == '^') goto pm;
	if (*str == '_') goto apc;
	if (*str == '{') goto esc_lbrace;
	while (n && 0 <= *str && *str < '0')
		graf->nbytes++, str++, n--;
	goto done;

ss2: ss3:
	graf->nbytes++, str++, n--;
	goto done;

apc: sos: pm: dcs:
	graf->nbytes++, str++, n--;
	while (n > 1 && !(str[0] == 0x1B && str[1] == '\\'))
		graf->nbytes++, str++, n--;
	if (n)
		graf->nbytes++, str++, n--;
	goto done;

esc_y:
	
	graf->nbytes++, str++, n--;
	if (n)
		graf->nbytes++, str++, n--;
	goto done;

csi:
	graf->nbytes++, str++, n--;
	while (n && 0 <= *str && *str < '@')
		graf->nbytes++, str++, n--;
	goto done;

osc:
	graf->nbytes++, str++, n--;
	if (!n)
		return final ? str : NULL;
	if (*str == 'P') {
		graf->nbytes++, str++, n--;
		for (i = 0; n && i < 7; i++)
			graf->nbytes++, str++, n--;
		if (!n)
			return final ? str : NULL;
		return str;
	}
	if (!isdigit(*str) && *str != ';')
		return graf->nbytes++, str + 1;
	while (n > 1 && !(str[0] == 0x1B && str[1] == '\\')) {
		if (*str == '\a')
			return graf->nbytes++, str + 1;
		graf->nbytes++, str++, n--;
	}
	if (n) {
		if (*str == '\a')
			return graf->nbytes++, str + 1;
		graf->nbytes++, str++, n--;
	}
	goto done;

esc_lbrace:
	graf->nbytes++, str++, n--;
	if (!n)
		return final ? str : NULL;
	if (*str != 'A' && *str != 'R' && *str != 'T')
		return graf->nbytes++, str + 1;
	graf->nbytes++, str++, n--;
	if (!n)
		return final ? str : NULL;
	while (n && isdigit(*str))
		graf->nbytes++, str++, n--;
	goto done;

done:
	if (!n)
		return final ? str : NULL;
	return graf->nbytes++, str + 1;
}
