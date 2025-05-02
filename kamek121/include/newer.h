#ifndef NEWER_H
#define NEWER_H 

const wchar_t* getLevelNumber(int world, int level);
const wchar_t* getWorldNumber(int world);

extern int lastLevelIDs[];

void WriteAsciiToTextBox(nw4r::lyt::TextBox *tb, const char *source);

int getUnspentStarCoinCount();
int getStarCoinCount();
void getNewerLevelNumberString(int world, int level, wchar_t *dest);


#endif /* NEWER_H */
