#pragma once
#include <game.h>

class daStaffRoll_c : public dStageActor_c {
public:
    enum Anime_e {
        IN_WINDOW = 0,
        IN_TITLE,
        IN_NAMES,
        OUT_WINDOW,
    };

    daStaffRoll_c() : state(this, &StateID_DispWait) { }

    int onCreate();
    int onDelete();
    int onExecute();
    int onDraw();

    int beforeExecute();

    bool createLayout();
    void chkSkip();
    void calcTimer();
    void setInfo();
    void setMessage(nw4r::lyt::TextBox *textbox, dScript::Res_c *bmg, ulong cat, ulong message);

    static dActor_c *build();

    m2d::EmbedLayout_c layout;
    dStateWrapper_c<daStaffRoll_c> state;
    nw4r::lyt::Pane
        *N_titleOne_00, *N_titleTwo_00,
        *N_names_00, *N_thankYou_00;
    nw4r::lyt::TextBox
        *T_titleC_0X[2], *T_titleL_0X[2],
        *T_titleR_0X[2], *T_names_0X[3];

    // Sprite param
    int cardIdx;
    int dispTimer;
    int destEntrance;

    bool isDualSection;
    bool isCenterName;
    bool isFinalCard;

    bool layoutLoaded;
    bool visible;

    USING_STATES(daStaffRoll_c);
    DECLARE_STATE(DispWait);
    DECLARE_STATE(OpenAnime);
    DECLARE_STATE(TextAnime);
    DECLARE_STATE(Wait);
    DECLARE_STATE(ExitAnime);
};
