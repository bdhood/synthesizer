#ifndef NOTE_H
#define NOTE_H

typedef struct NOTE {
	int note;
	char sharp;
	char octave;
} NOTE;

void note_from_string(NOTE* n, char* str);
int note_to_int(NOTE* n);
void note_from_int(NOTE* n, int value);

#endif