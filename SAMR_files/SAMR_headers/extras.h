#ifndef EXTRAS_H
#define EXTRAS_H

#include "SAMR_headers\build.h"

// Its weird that the original code included so many unrelated functions
// in main, so I moved them here instead

#pragma warning(push)
#pragma warning(disable: PW_ALL_WARNINGS)
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#pragma warning(pop)

#include "SAMR_headers\sam.h"

#pragma warning(push)
#pragma warning(disable: PW_ALL_WARNINGS)
#include <SDL.h>
#include <SDL_audio.h>
#pragma warning(pop)

void WriteWav(char* filename, char* buffer, int bufferlength);
void PrintUsage();
void MixAudio(void* unused, uint8_t* stream, int len);
void OutputSound();

#endif // !EXTRAS_H
