#include "SAMR_headers\build.h"

#pragma warning(push)
#pragma warning(disable: PW_ALL_WARNINGS)
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <Windows.h>
#pragma warning(pop)

#include "SAMR_headers\reciter.h"
#include "SAMR_headers\sam.h"
#include "SAMR_headers\debug.h"
#include "SAMR_headers\extras.h"

#pragma warning(push)
#pragma warning(disable: PW_ALL_WARNINGS)
#include <SDL.h>
#include <SDL_audio.h>
#pragma warning(pop)

#undef min

// Used in many places, bad practice but
// needed for legacy code
int debug = 0;
// The sound
std::vector<char> totalBuffer{};
uint8_t* tempBuffer{ nullptr };

int pos = 0;
// Modified to support totalBuffer
void MixAudio(void* unused, uint8_t* stream, int len) {
	int bufferpos = (int)totalBuffer.size();
	char* buffer = nullptr;
	if (tempBuffer != nullptr) {
		buffer = (char*)tempBuffer;
	}
	else {
		buffer = totalBuffer.data();
	}
	
	int i;
	if (pos >= bufferpos) return;
	if ((bufferpos - pos) < len) len = (bufferpos - pos);
	for (i = 0; i < len; i++)
	{
		stream[i] = buffer[pos];
		pos++;
	}
}

// Modified to support totalBuffer
void OutputSound(int volume) {
	int bufferpos = (int)totalBuffer.size();
	SDL_AudioSpec fmt;

	fmt.freq = 22050;
	fmt.format = AUDIO_U8;
	fmt.channels = 1;
	fmt.samples = 2048;
	fmt.callback = MixAudio;
	fmt.userdata = NULL;

	tempBuffer = new uint8_t[totalBuffer.size()];
	// Can't modify totalBuffer.data() so i.g we gotta make another buffer
	SDL_MixAudioFormat(tempBuffer, (const uint8_t*)totalBuffer.data(), AUDIO_U8, (int)totalBuffer.size(), volume);
	/* Open the audio device and start playing sound! */
	if (SDL_OpenAudio(&fmt, NULL) < 0)
	{
		std::cout << "SDL_OpenAudio Error" << std::endl;
		return;
	}
	SDL_PauseAudio(0);
	//SDL_Delay((bufferpos)/7);

	while (pos < bufferpos)
	{
		SDL_Delay(100);
	}

	if (tempBuffer != nullptr) {
		delete[] tempBuffer;
	}

	SDL_CloseAudio();
}

int DumpToMainBuffer(std::wstring samInput, int phonetic, bool reset = true) {
	// All text must be upper case
	for (size_t i = 0; i < samInput.size(); i++) {
		samInput.at(i) = (unsigned char)toupper((int)samInput.at(i));
	}
	std::wstring str = samInput;
	const wchar_t* tempIn{ str.c_str() };
	char* tempOut{ new(std::nothrow) char[256] };
	if (tempOut != nullptr) {
		size_t convertedChars{ 0 };
		if (str.size() + 1 < 256) {
			convertedChars = wcstombs(tempOut, tempIn, str.size() + 1);
		}
		else {
			convertedChars = wcstombs(tempOut, tempIn, str.size());
			tempOut[254] = '\0';
		}
		

		if (!phonetic) {
			strcat_s((char*)tempOut, 256, "[");
			if (!TextToPhonemes((BYTE*)tempOut)) return 1;
		}
		else {
			strcat_s((char*)tempOut, 256, "\x9b");
		}

		if (convertedChars == str.size()) {
			SetInput((BYTE*)tempOut);
		}
		delete[] tempOut;

		if (!SAMMain()) {
			PrintUsage();
			return 1;
		}
		// Why divide it by 50? Audio output is only in the 1/50th area of the buffer?
		for (size_t l = 0; l < GetBufferLength() / 50; l++) {
			totalBuffer.push_back(GetBuffer()[l]);
		}
		if (reset == true) {
			ResetSam();
		}
		return 0;
	}
	return 1;
}

int main(int argc, char** argv) {
	OPENFILENAMEW fileToOpen{};
	WCHAR szFile[MAX_PATH]{};
	HWND hwnd{ NULL };

	ZeroMemory(&fileToOpen, sizeof(fileToOpen));
	fileToOpen.lStructSize = sizeof(fileToOpen);
	fileToOpen.hwndOwner = hwnd;
	fileToOpen.lpstrFile = szFile;
	fileToOpen.lpstrFile[0] = '\0';
	fileToOpen.nMaxFile = sizeof(szFile);
	fileToOpen.lpstrFilter = L"SAM\0*.sam\0";
	fileToOpen.nFilterIndex = 1;
	fileToOpen.lpstrFileTitle = NULL;
	fileToOpen.nMaxFileTitle = 0;
	fileToOpen.lpstrInitialDir = NULL;
	fileToOpen.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&fileToOpen) == FALSE) {
		std::cerr << CommDlgExtendedError() << std::endl;
		EndSam();
		return 1;
	}
	std::wstring filePath{ fileToOpen.lpstrFile };
	fileToOpen.lpstrFileTitle;
	std::wstring content{ L"" };

	std::wifstream fileStream{ filePath, std::ios::in };

	if (!fileStream.is_open()) {
		std::cerr << "File failed to open." << std::endl;
		std::cin.get();
		EndSam();
		return 1;
	}

	std::wstring v_line{ L"" };
	while (!fileStream.eof()) {
		std::getline(fileStream, v_line);
		content.append(L" " + v_line);
	}
	fileStream.close();

	std::wstringstream tokens{ content };
	std::vector<std::wstring> phrases{};
	{
		std::wstring temp{};
		while (std::getline(tokens, temp, L' ')) {
			if (temp != L"") {
				phrases.push_back(temp);
			}
		}
	}
	// The algorithm will read settings until the 
	// break at which the text will be appended into a
	// input string separated b spaces until the next setting
	// Also wav command is no longer supported due to
	// automatically being outputted anyways
	bool settingSettings{ false };
	// Legacy
	int phonetic{ 0 };

	std::wstring samInput{};

	for (size_t i = 0; i < phrases.size(); i++) {
		std::wstring current = phrases.at(i);
		if (current == L"-sing") {
			if (settingSettings == false && i != 0) {
				if (DumpToMainBuffer(samInput, phonetic) == 1) {
					phonetic = 0;
					std::cin.get();
					EndSam();
					return 1;
				}
				samInput = std::wstring();
			}
			EnableSingmode();
			settingSettings = true;
			continue;
		}
		if (current == L"-phonetic") {
			if (settingSettings == false && i != 0) {
				if (DumpToMainBuffer(samInput, phonetic) == 1) {
					phonetic = 0;
					std::cin.get();
					EndSam();
					return 1;
				}
				samInput = std::wstring();
			}
			phonetic = 1;
			settingSettings = true;
			continue;
		}
		if (current == L"-debug") {
			if (settingSettings == false && i != 0) {
				if (DumpToMainBuffer(samInput, phonetic) == 1) {
					phonetic = 0;
					std::cin.get();
					EndSam();
					return 1;
				}
				samInput = std::wstring();
			}
			debug = 1;
			settingSettings = true;
			continue;
		}
		if (current == L"-pitch") {
			if (phrases.size() > i + 1) {
				if (settingSettings == false && i != 0) {
					if (DumpToMainBuffer(samInput, phonetic) == 1) {
						phonetic = 0;
						std::cin.get();
						EndSam();
						return 1;
					}
					samInput = std::wstring();
				}
				SetPitch((unsigned char)std::clamp(_wtoi(phrases.at(i + 1).c_str()), 0, 255));
				i++;
				settingSettings = true;
				continue;
			}
			else {
				std::cerr << "Invalid setting. -pitch" << std::endl;
				std::cin.get();
				
				EndSam();
				return 1;
			}
		}
		if (current == L"-speed") {
			if (phrases.size() > i + 1) {
				if (settingSettings == false && i != 0) {
					if (DumpToMainBuffer(samInput, phonetic) == 1) {
						phonetic = 0;
						std::cin.get();
						EndSam();
						return 1;
					}
					samInput = std::wstring();
				}
				SetSpeed((unsigned char)std::clamp(_wtoi(phrases.at(i + 1).c_str()), 0, 255));
				i++;
				settingSettings = true;
				continue;
			}
			else {
				std::cerr << "Invalid setting. -speed" << std::endl;
				std::cin.get();
				
				EndSam();
				return 1;
			}
		}
		if (current == L"-mouth") {
			if (phrases.size() > i + 1) {
				if (settingSettings == false && i != 0) {
					if (DumpToMainBuffer(samInput, phonetic) == 1) {
						phonetic = 0;
						std::cin.get();
						EndSam();
						return 1;
					}
					samInput = std::wstring();
				}
				SetMouth((unsigned char)std::clamp(_wtoi(phrases.at(i + 1).c_str()), 0, 255));
				i++;
				settingSettings = true;
				continue;
			}
			else {
				std::cerr << "Invalid setting. -mouth" << std::endl;
				std::cin.get();
				
				EndSam();
				return 1;
			}
		}
		if (current == L"-throat") {
			if (phrases.size() > i + 1) {
				if (settingSettings == false && i != 0) {
					if (DumpToMainBuffer(samInput, phonetic) == 1) {
						phonetic = 0;
						std::cin.get();
						EndSam();
						return 1;
					}
					samInput = std::wstring();
				}
				SetThroat((unsigned char)std::clamp(_wtoi(phrases.at(i + 1).c_str()), 0, 255));
				i++;
				settingSettings = true;
				continue;
			}
			else {
				std::cerr << "Invalid setting. -throat" << std::endl;
				std::cin.get();
				
				EndSam();
				return 1;
			}
		}
		// If there are no more settings then append to input string
		settingSettings = false;
		if (samInput.empty() == true) {
			samInput.append(current);
		}
		else {
			std::wstring temp1 = samInput;
			samInput.append(L" " + current);

			if (samInput.size() >= 99) {
				if (DumpToMainBuffer(temp1, phonetic, false) == 1) {
					std::cin.get();
					EndSam();
					return 1;
				}
				samInput.erase(0, temp1.size() + 1);

				while (samInput.size() >= 99) {
					std::wstring temp2 = samInput.substr(0, 99);
					if (DumpToMainBuffer(temp2, phonetic, false) == 1) {
						std::cin.get();
						EndSam();
						return 1;
					}
					samInput.erase(0, 99);
				}
			}
		}
	}
	if (samInput.size() >= 99) {
		while (samInput.size() >= 99) {
			std::wstring temp2 = samInput.substr(0, 99);
			if (DumpToMainBuffer(temp2, phonetic, false) == 1) {
				std::cin.get();
				EndSam();
				return 1;
			}
			samInput.erase(0, 99);
		}
	}
	else {
		if (DumpToMainBuffer(samInput, phonetic) == 1) {
			std::cin.get();
			EndSam();
			return 1;
		}
		bool properInput1{ false };
		do {
			std::cout << "1 : Output To Wav." << std::endl;
			std::cout << "2 : Output To Sound." << std::endl;
			std::cout << "3 : Both." << std::endl;
			int input1{ 0 };
			std::string consoleInput1{};
			std::getline(std::cin, consoleInput1);

			input1 = strtol(consoleInput1.c_str(), NULL, 10);

			if (input1 == 0) {
				continue;
			}

			switch (input1) {
			case 1: {
				if (fileToOpen.lpstrFileTitle == NULL) {
					size_t lastSlash1 = filePath.rfind(L'/');
					size_t lastSlash2 = filePath.rfind(L'\\');

					if (lastSlash2 != std::wstring::npos) {
						lastSlash1 = lastSlash2;
					}
					if (lastSlash1 != std::wstring::npos) {
						std::wstring fileName{ filePath.substr(lastSlash1 + 1, filePath.size() - lastSlash1) };
						size_t lastPeriod = fileName.find_last_of(L'.');
						if (lastPeriod != std::wstring::npos) {
							fileName = fileName.substr(0, lastPeriod);

							fileName.append(L".wav");

							const wchar_t* fileNamePtr{ fileName.c_str() };
							char* convertedFileName{ new(std::nothrow) char[fileName.size() + 1] };

							size_t convertedChars{ wcstombs(convertedFileName, fileNamePtr, fileName.size() + 1) };

							WriteWav(convertedFileName, totalBuffer.data(), (int)totalBuffer.size());

							EndSam();
							return 0;
						}
						else {
							std::cin.get();
							EndSam();
							return 1;
						}
					}
					else {
						std::cin.get();
						EndSam();
						return 1;
					}
				}
				else {
					std::wstring fileName{ fileToOpen.lpstrFileTitle };
					size_t period{ fileName.find(L".", 0) };

					if (period == std::wstring::npos) {
						return 1;
					}
					fileName = fileName.substr(0, period);
					fileName.append(L".wav");

					const wchar_t* fileNamePtr{ fileName.c_str() };
					char* convertedFileName{ new(std::nothrow) char[fileName.size() + 1] };

					size_t convertedChars{ wcstombs(convertedFileName, fileNamePtr, fileName.size() + 1) };

					WriteWav(convertedFileName, totalBuffer.data(), (int)totalBuffer.size());

					EndSam();
					return 0;
				}
				properInput1 = true;
				break;
			}
			case 2: {
				std::cout << "Volume : 0-100" << std::endl;
				std::cout << "Invalid Input Default : 0" << std::endl;
				int input2{ 0 };
				std::string consoleInput2{};
				std::getline(std::cin, consoleInput2);

				input2 = strtol(consoleInput2.c_str(), NULL, 10);

				OutputSound(std::clamp(input2, 0, 120));

				properInput1 = true;
				break;
			}
			case 3: {
				std::cout << "Volume : 0-100" << std::endl;
				std::cout << "Invalid Input Default : 0" << std::endl;
				int input2{ 0 };
				std::string consoleInput2{};
				std::getline(std::cin, consoleInput2);

				input2 = strtol(consoleInput2.c_str(), NULL, 10);

				OutputSound(std::clamp(input2, 0, 120));

				if (fileToOpen.lpstrFileTitle == NULL) {
					size_t lastSlash1 = filePath.rfind(L'/');
					size_t lastSlash2 = filePath.rfind(L'\\');

					if (lastSlash2 != std::wstring::npos) {
						lastSlash1 = lastSlash2;
					}
					if (lastSlash1 != std::wstring::npos) {
						std::wstring fileName{ filePath.substr(lastSlash1 + 1, filePath.size() - lastSlash1) };
						size_t lastPeriod = fileName.find_last_of(L'.');
						if (lastPeriod != std::wstring::npos) {
							fileName = fileName.substr(0, lastPeriod);

							fileName.append(L".wav");

							const wchar_t* fileNamePtr{ fileName.c_str() };
							char* convertedFileName{ new(std::nothrow) char[fileName.size() + 1] };

							size_t convertedChars{ wcstombs(convertedFileName, fileNamePtr, fileName.size() + 1) };

							WriteWav(convertedFileName, totalBuffer.data(), (int)totalBuffer.size());

							EndSam();
							return 0;
						}
						else {
							std::cin.get();
							EndSam();
							return 1;
						}
					}
					else {
						std::cin.get();
						EndSam();
						return 1;
					}
				}
				else {
					std::wstring fileName{ fileToOpen.lpstrFileTitle };
					size_t period{ fileName.find(L".", 0) };

					if (period == std::wstring::npos) {
						std::cin.get();
						EndSam();
						return 1;
					}
					fileName = fileName.substr(0, period);
					fileName.append(L".wav");

					const wchar_t* fileNamePtr{ fileName.c_str() };
					char* convertedFileName{ new(std::nothrow) char[fileName.size() + 1] };

					size_t convertedChars{ wcstombs(convertedFileName, fileNamePtr, fileName.size() + 1) };

					WriteWav(convertedFileName, totalBuffer.data(), (int)totalBuffer.size());

					EndSam();
					return 0;
				}

				properInput1 = true;
				break;
			}
			default: {
				continue;
			}
			}
		} while (!properInput1);
		EndSam();
		return 0;
	}
	EndSam();
	return 0;
}