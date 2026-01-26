#include <game.h>

class dCharacterChangeSelectContents_c : public dBase_c {
public:
	m2d::EmbedLayout_c layout;
	dStateWrapper_c<dCharacterChangeSelectContents_c> state;
	u32 unk;

	nw4r::lyt::Pane *RootPane;
	nw4r::lyt::Pane *N_cSelectCon_00, *N_cSelectCon_01;
	nw4r::lyt::Picture
		*PbuttonShadow_00, *PbuttonShadow_01,
		*P_lui_00, *P_lui_01,
		*P_kinoB_00, *P_kinoB_01,
		*P_kinoY_00, *P_kinoY_01;
	nw4r::lyt::TextBox
		*T_zanki_01, *T_zanki_00,
		*T_kakeru_00, *T_kakeru_01;

	u32 _284, _288;
	u32 _28C, _290, _294, _298;

	bool layoutCreated, visible;
	u8 unk2[0x6];

	int marioLives_maybe, luigiLives, yellowToadLives, blueToadLives;

	// funcRun changes from 0 to 1 between the run, so it updates the next "button", then the old one
	void setRestLuigi(int funcRun);
	void setRestKinoY(int funcRun);
	void setRestKinoB(int funcRun);
};

static const int fieldLength = 2;

void dCharacterChangeSelectContents_c::setRestLuigi(int funcRun) {
	if (funcRun == 0) {
		WriteNumberToTextBox(&luigiLives, &fieldLength, T_zanki_01, false);
		T_kakeru_00->colour2 = luigiColor;
		T_zanki_01->colour2 = luigiColor;
		P_lui_00->SetVisible(true);
	} else {
		WriteNumberToTextBox(&luigiLives, &fieldLength, T_zanki_00, false);
		T_kakeru_01->colour2 = luigiColor;
		T_zanki_00->colour2 = luigiColor;
		P_lui_01->SetVisible(true);
	}
}

void dCharacterChangeSelectContents_c::setRestKinoY(int funcRun) {
	if (funcRun == 0) {
		WriteNumberToTextBox(&yellowToadLives, &fieldLength, T_zanki_01, false);
		T_kakeru_00->colour2 = kinoYColor;
		T_zanki_01->colour2 = kinoYColor;
		P_kinoY_00->SetVisible(true);
	} else {
		WriteNumberToTextBox(&yellowToadLives, &fieldLength, T_zanki_00, false);
		T_kakeru_01->colour2 = kinoYColor;
		T_zanki_00->colour2 = kinoYColor;
		P_kinoY_01->SetVisible(true);
	}
}

void dCharacterChangeSelectContents_c::setRestKinoB(int funcRun) {
	if (funcRun == 0) {
		WriteNumberToTextBox(&blueToadLives, &fieldLength, T_zanki_01, false);
		T_kakeru_00->colour2 = kinoBColor;
		T_zanki_01->colour2 = kinoBColor;
		P_kinoB_00->SetVisible(true);
	} else {
		WriteNumberToTextBox(&blueToadLives, &fieldLength, T_zanki_00, false);
		T_kakeru_01->colour2 = kinoBColor;
		T_zanki_00->colour2 = kinoBColor;
		P_kinoB_01->SetVisible(true);
	}
}