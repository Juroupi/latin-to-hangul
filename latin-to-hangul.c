#include <stddef.h>
#include <ctype.h>

#include "latin-to-hangul.h"

// http://www.gernot-katzers-spice-pages.com/var/korean_hangul_unicode.html

typedef struct {
	long num;
	const char* str;
	int skip;
	long code;
} hangul_part_t;

hangul_part_t leads[] = {
	{  2, "GG", 2, 0x1101 },
	{  5, "DD", 2, 0x1104 },
	{  9, "BB", 2, 0x1108 },
	{ 11, "SS", 2, 0x110A },
	{ 14, "JJ", 2, 0x110D },
	{  1, "G",  1, 0x1100 },
	{  3, "N",  1, 0x1102 },
	{  4, "D",  1, 0x1103 },
	{  6, "R",  1, 0x1105 },
	{  7, "M",  1, 0x1106 },
	{  8, "B",  1, 0x1107 },
	{ 10, "S",  1, 0x1109 },
	{ 13, "J",  1, 0x110C },
	{ 15, "C",  1, 0x110E },
	{ 16, "K",  1, 0x110F },
	{ 17, "T",  1, 0x1110 },
	{ 18, "P",  1, 0x1111 },
	{ 19, "H",  1, 0x1112 },
	{ 12, "A",  0, 0x110B },
	{ 12, "Y",  0, 0x110B },
	{ 12, "E",  0, 0x110B },
	{ 12, "O",  0, 0x110B },
	{ 12, "W",  0, 0x110B },
	{ 12, "U",  0, 0x110B },
	{ 12, "I",  0, 0x110B }
};

static hangul_part_t vowels[] = {
	{  4, "YAE", 3, 0x1164 },
	{  7, "YEO", 3, 0x1167 },
	{ 11, "WAE", 3, 0x116B },
	{ 15, "WEO", 3, 0x116F },
	{  2, "AE",  2, 0x1162 },
	{  3, "YA",  2, 0x1163 },
	{  5, "EO",  2, 0x1165 },
	{  8, "YE",  2, 0x1168 },
	{ 10, "WA",  2, 0x116A },
	{ 12, "OE",  2, 0x116C },
	{ 13, "YO",  2, 0x116D },
	{ 16, "WE",  2, 0x1170 },
	{ 17, "WI",  2, 0x1171 },
	{ 18, "YU",  2, 0x1172 },
	{ 19, "EU",  2, 0x1173 },
	{ 20, "YI",  2, 0x1174 },
	{  1, "A",   1, 0x1161 },
	{  6, "E",   1, 0x1166 },
	{  9, "O",   1, 0x1169 },
	{ 14, "U",   1, 0x116E },
	{ 21, "I",   1, 0x1175 }
};

static hangul_part_t tails[] = {
	{  0, "",   0, 0x0000 },
	{  2, "GG", 2, 0x11A9 },
	{  3, "GS", 2, 0x11AA },
	{  5, "NJ", 2, 0x11AC },
	{  6, "NH", 2, 0x11AD },
	{  9, "LG", 2, 0x11B0 },
	{ 10, "LM", 2, 0x11B1 },
	{ 11, "LB", 2, 0x11B2 },
	{ 12, "LS", 2, 0x11B3 },
	{ 13, "LT", 2, 0x11B4 },
	{ 14, "LP", 2, 0x11B5 },
	{ 15, "LH", 2, 0x11B6 },
	{ 18, "BS", 2, 0x11B9 },
	{ 20, "SS", 2, 0x11BB },
	{ 21, "NG", 2, 0x11BC },
	{  1, "G",  1, 0x11A8 },
	{  4, "N",  1, 0x11AB },
	{  7, "D",  1, 0x11AE },
	{  8, "L",  1, 0x11AF },
	{ 16, "M",  1, 0x11B7 },
	{ 17, "B",  1, 0x11B8 },
	{ 19, "S",  1, 0x11BA },
	{ 22, "J",  1, 0x11BD },
	{ 23, "C",  1, 0x11BE },
	{ 24, "K",  1, 0x11BF },
	{ 25, "T",  1, 0x11C0 },
	{ 26, "P",  1, 0x11C1 },
	{ 27, "H",  1, 0x11C2 }
};

static int str_starts_with(const char* pre, const char* str) {

	if (*pre == '\0') {
		return 1;
	}

	if (*str == '\0' || toupper(*str) != *pre) {
		return 0;
	}

	return str_starts_with(pre + 1, str + 1);
}

static hangul_part_t* get_part(hangul_part_t* parts, int n, const char** str) {

	for (int i = 0; i < n; i++) {

		if (str_starts_with(parts[i].str, *str)) {

			(*str) += parts[i].skip;

			return &parts[i];
		}
	}

	return NULL;
}

#define PART_SIZE(name) (sizeof(name) / sizeof(hangul_part_t))

long latin_to_hangul(const char* str) {

	hangul_part_t* lead = get_part(leads, PART_SIZE(leads), &str);
	if (lead == NULL) {
		return 0;
	}

	hangul_part_t* vowel = get_part(vowels, PART_SIZE(vowels), &str);
	if (vowel == NULL) {
		return 0;
	}

	hangul_part_t* tail = &tails[0];
	if (*str != '\0') {
		tail = get_part(tails + 1, PART_SIZE(tails) - 1, &str);
		if (tail == NULL) {
			return 0;
		}
	}

	return tail->num + (vowel->num - 1) * 28 + (lead->num - 1) * 588 + 44032;
}