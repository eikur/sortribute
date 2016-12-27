#ifndef __MODULEFONTS_H__
#define __MODULEFONTS_H__

#include "Module.h"

class ModuleFonts : public Module
{
public:
	ModuleFonts();
	~ModuleFonts();

	bool Init();
	bool LoadConfigFromFile(const char* file_path);

	void Print(int x, int y, int font_id, const char* text) const;
	

private:
	int *m_font_ids = nullptr;
};
#endif	// __MODULEFONTS_H__
