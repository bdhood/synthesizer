#include "SDL.h"
#include "keyboard_ctrl.h"
#include "label.h"
#include "string.h"
#include "cJSON.h"

#include "activekeys.h"
#include <stdio.h>

/* SDL2
const unsigned char* keyboard_ctrl_keys[] = { 
	" 23 567 90 =\x08\x2d \x21", 
	"QWERTYUIOP[]\\\x7f\xf0\x22", 
	" SD GHJ L; ", 
	"\xa0ZXCVBNM,./", 0 };
const wchar_t* keyboard_ctrl_keys_mapped[] = {
	L" 23 567 90 \x3d\x08\x2d \x21",
	L"qwertyuiop\x5b\x5d\x5c\x7f\xf0\x22",
	L" sd ghj l\x3b ",
	L"\xa0zxcvbnm\x2c\x2e\x2f",
	0
};
*/

#define MAX_KEYS 0x200

#define CTRL_USED		0
#define CTRL_SPACE		1
#define CTRL_NEWLINE	2
#define CTRL_UNUSED		3

const char* keyboard_ctrl_keys[] = {
	" 23 567 90 =\x08",
	"QWERTYUIOP[]\\",
	" SD GHJ L; ",
	"\xa0ZXCVBNM,./", 0 };
const wchar_t* keyboard_ctrl_keys_mapped[] = {
	L" 23 567 90 \xbb\x08",
	L"QWERTYUIOP\xdb\xdd\xdc",
	L" SD GHJ L\xba ",
	L"\xa0ZXCVBNM\xbc\xbe\xbf",
	0
};




KEYBOARD_CTRL* keyboard_ctrl_init(cJSON *keys_root, SDL_Rect rect) {
	KEYBOARD_CTRL* kb = malloc(sizeof(KEYBOARD_CTRL));
	if (kb == 0) {
		return 0;
	}
	kb->label = label_init();
	kb->rect = rect;
	kb->keys = (KEYBOARD_CTRL_KEY*)malloc(sizeof(KEYBOARD_CTRL_KEY) * MAX_KEYS);
	if (kb->keys == 0) {
		free(kb);
		return 0;
	}
	memset(kb->keys, 0, sizeof(KEYBOARD_CTRL_KEY) * MAX_KEYS);

	unsigned char keyCode;
	char* key_text;
	float width_scalar;
	int n_key = 0, key_size = 50, padding = 2, cursor_x = kb->rect.x, cursor_y = kb->rect.y;
	int ctrl_flag;
	SDL_Rect r;
	for (cJSON* i = keys_root->child; i != 0; i = i->next) {
		keyCode = 0;
		width_scalar = 1.0f;
		key_text = 0;
		ctrl_flag = CTRL_USED;
		for (cJSON* j = i->child; j != 0; j = j->next) {
			if (strcmp(j->string, "display") == 0) {
				key_text = j->valuestring;
			}
			else if (strcmp(j->string, "width") == 0) {
				width_scalar = j->valuedouble;
			}
			else if (strcmp(j->string, "keyCode") == 0) {
				if (j->valuestring != 0) {
					keyCode = j->valuestring[0];
				}
				else {
					keyCode = (unsigned char)j->valueint;
				}
			}
			else if (strcmp(j->string, "note") == 0) {
				continue;
			}
			else if (strcmp(j->string, "ctrl") == 0) {
				if (strcmp(j->valuestring, "unused_key") == 0) {
					ctrl_flag = CTRL_UNUSED;
				}
				else if (strcmp(j->valuestring, "space") == 0) {
					ctrl_flag = CTRL_SPACE;
				}
				else if (strcmp(j->valuestring, "newline") == 0) {
					ctrl_flag = CTRL_NEWLINE;
				}
				else {
					printf("Warn: Unknown json value '%s'\n", j->valuestring);
				}
			}
			else {
				printf("Warn: Unknown json value '%s'\n", j->string);
			}
		}

		r.x = cursor_x;
		r.y = cursor_y;	
		r.w = (int)((float)key_size * width_scalar);
		r.h = key_size;

		switch (ctrl_flag) {
		case CTRL_USED: 
			kb->keys[n_key].rect = r;
			kb->keys[n_key].text = key_text;
			kb->keys[n_key].keyCode = keyCode;
			cursor_x += r.w + padding;
			n_key++;
			break;
		case CTRL_SPACE:
			cursor_x += r.w + padding;
			break;
		case CTRL_NEWLINE:
			cursor_y += key_size + padding;
			cursor_x = kb->rect.x;
			break;
		case CTRL_UNUSED:
			kb->keys[n_key].rect = r;
			kb->keys[n_key].text = 0;
			kb->keys[n_key].keyCode = keyCode;
			cursor_x += r.w + padding;
			n_key++;
			break;
		}
	}
	kb->keys[n_key].text = (char *)-1;
	return kb;
}

void keyboard_ctrl_free(KEYBOARD_CTRL* keyboard_ctrl) {
	label_free(keyboard_ctrl->label);
	free(keyboard_ctrl->keys);
	free(keyboard_ctrl);
}

void keyboard_ctrl_grow(SDL_Rect *rect, int size) {
	rect->x -= size;
	rect->y -= size;
	rect->w += size * 2;
	rect->h += size * 2;
}

void keyboard_ctrl_render(KEYBOARD_CTRL* kb, SDL_Renderer* r) {

	SDL_Rect rect;

	SDL_SetRenderDrawColor(r, kb->BACKCOLOR.r, kb->BACKCOLOR.g, kb->BACKCOLOR.b, 255);
	SDL_RenderFillRect(r, &kb->rect);

	for (int i = 0; kb->keys[i].text != (char *)-1; i++) {
		rect = kb->keys[i].rect;
		if (kb->keys[i].text == 0) {
			// unused key

			SDL_SetRenderDrawColor(r, kb->UNUSED_TRIM.r, kb->UNUSED_TRIM.g, kb->UNUSED_TRIM.b, 255);
			SDL_RenderFillRect(r, &rect);

			keyboard_ctrl_grow(&rect, -1);

			SDL_SetRenderDrawColor(r, kb->UNUSED_TOP.r, kb->UNUSED_TOP.g, kb->UNUSED_TOP.b, 255);
			SDL_RenderFillRect(r, &rect);

			rect.y += rect.h / 2;
			rect.h /= 2;

			SDL_SetRenderDrawColor(r, kb->UNUSED_BOTTOM.r, kb->UNUSED_BOTTOM.g, kb->UNUSED_BOTTOM.b, 255);
			SDL_RenderFillRect(r, &rect);
			continue;
		}
		else if (activekeys_isactive(kb->keys[i].keyCode)) {
			// active key

			SDL_SetRenderDrawColor(r, kb->ACTIVE_TRIM.r, kb->ACTIVE_TRIM.g, kb->ACTIVE_TRIM.b, 255);
			SDL_RenderFillRect(r, &rect);

			keyboard_ctrl_grow(&rect, -1);

			SDL_SetRenderDrawColor(r, kb->ACTIVE_TOP.r, kb->ACTIVE_TOP.g, kb->ACTIVE_TOP.b, 255);
			SDL_RenderFillRect(r, &rect);

			rect.y += rect.h / 2;
			rect.h /= 2;

			SDL_SetRenderDrawColor(r, kb->ACTIVE_BOTTOM.r, kb->ACTIVE_BOTTOM.g, kb->ACTIVE_BOTTOM.b, 255);
			SDL_RenderFillRect(r, &rect);
		}
		else {
			// inactive key

			SDL_SetRenderDrawColor(r, kb->INACTIVE_TRIM.r, kb->INACTIVE_TRIM.g, kb->INACTIVE_TRIM.b, 255);
			SDL_RenderFillRect(r, &rect);

			keyboard_ctrl_grow(&rect, -1);

			SDL_SetRenderDrawColor(r, kb->INACTIVE_TOP.r, kb->INACTIVE_TOP.g, kb->INACTIVE_TOP.b, 255);
			SDL_RenderFillRect(r, &rect);

			rect.y += rect.h / 2;
			rect.h /= 2;

			SDL_SetRenderDrawColor(r, kb->INACTIVE_BOTTOM.r, kb->INACTIVE_BOTTOM.g, kb->INACTIVE_BOTTOM.b, 255);
			SDL_RenderFillRect(r, &rect);
		}
		rect.y -= rect.h;
		rect.h *= 2;
		kb->label->text = kb->keys[i].text;
		kb->label->rect = rect;
		label_render(kb->label, r);
	}

	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
}
