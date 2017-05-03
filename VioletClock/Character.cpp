#include "DxLib.h"
#include "Character.h"
#include "Voice.h"
#include "Exception.h"
using std::pair;

int Chr::hBase = -1;
unordered_map<eFace, int> Chr::hFace;

Chr::Chr() : Area::Area(CHR_X, CHR_Y, CHR_W, CHR_H)
{
    if (hBase == -1)
        switch (SCALEY(480)){
        case 480:   hBase = LoadGraph("image\\96dpi\\kurei_kei\\base.png");     break;
        case 600:   hBase = LoadGraph("image\\120dpi\\kurei_kei\\base.png");    break;
        case 720:   hBase = LoadGraph("image\\144dpi\\kurei_kei\\base.png");    break;
        }

    if (hFace.empty()) {
        switch (SCALEY(480)) {
        case 480:
            hFace.insert(pair<eFace, int>(Confident, LoadGraph("image\\96dpi\\kurei_kei\\confident.png")));
            hFace.insert(pair<eFace, int>(Doya,      LoadGraph("image\\96dpi\\kurei_kei\\doya.png")));
            hFace.insert(pair<eFace, int>(Excited,   LoadGraph("image\\96dpi\\kurei_kei\\excited.png")));
            hFace.insert(pair<eFace, int>(Kyoton,    LoadGraph("image\\96dpi\\kurei_kei\\kyoton.png")));
            hFace.insert(pair<eFace, int>(Laugh,     LoadGraph("image\\96dpi\\kurei_kei\\laugh.png")));
            hFace.insert(pair<eFace, int>(Normal,    LoadGraph("image\\96dpi\\kurei_kei\\normal.png")));
            hFace.insert(pair<eFace, int>(Shocked,   LoadGraph("image\\96dpi\\kurei_kei\\shocked.png")));
            hFace.insert(pair<eFace, int>(Speaking,  LoadGraph("image\\96dpi\\kurei_kei\\speaking.png")));
            hFace.insert(pair<eFace, int>(Tearful,   LoadGraph("image\\96dpi\\kurei_kei\\tearful.png")));
            hFace.insert(pair<eFace, int>(Wink,      LoadGraph("image\\96dpi\\kurei_kei\\wink.png")));
            hFace.insert(pair<eFace, int>(Blink,     LoadGraph("image\\96dpi\\kurei_kei\\blink.png")));
            if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 0);
            break;
        case 600:
            hFace.insert(pair<eFace, int>(Confident, LoadGraph("image\\120dpi\\kurei_kei\\confident.png")));
            hFace.insert(pair<eFace, int>(Doya,      LoadGraph("image\\120dpi\\kurei_kei\\doya.png")));
            hFace.insert(pair<eFace, int>(Excited,   LoadGraph("image\\120dpi\\kurei_kei\\excited.png")));
            hFace.insert(pair<eFace, int>(Kyoton,    LoadGraph("image\\120dpi\\kurei_kei\\kyoton.png")));
            hFace.insert(pair<eFace, int>(Laugh,     LoadGraph("image\\120dpi\\kurei_kei\\laugh.png")));
            hFace.insert(pair<eFace, int>(Normal,    LoadGraph("image\\120dpi\\kurei_kei\\normal.png")));
            hFace.insert(pair<eFace, int>(Shocked,   LoadGraph("image\\120dpi\\kurei_kei\\shocked.png")));
            hFace.insert(pair<eFace, int>(Speaking,  LoadGraph("image\\120dpi\\kurei_kei\\speaking.png")));
            hFace.insert(pair<eFace, int>(Tearful,   LoadGraph("image\\120dpi\\kurei_kei\\tearful.png")));
            hFace.insert(pair<eFace, int>(Wink,      LoadGraph("image\\120dpi\\kurei_kei\\wink.png")));
            hFace.insert(pair<eFace, int>(Blink,     LoadGraph("image\\120dpi\\kurei_kei\\blink.png")));
            if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 0);
            break;
        case 720:
            hFace.insert(pair<eFace, int>(Confident, LoadGraph("image\\144dpi\\kurei_kei\\confident.png")));
            hFace.insert(pair<eFace, int>(Doya,      LoadGraph("image\\144dpi\\kurei_kei\\doya.png")));
            hFace.insert(pair<eFace, int>(Excited,   LoadGraph("image\\144dpi\\kurei_kei\\excited.png")));
            hFace.insert(pair<eFace, int>(Kyoton,    LoadGraph("image\\144dpi\\kurei_kei\\kyoton.png")));
            hFace.insert(pair<eFace, int>(Laugh,     LoadGraph("image\\144dpi\\kurei_kei\\laugh.png")));
            hFace.insert(pair<eFace, int>(Normal,    LoadGraph("image\\144dpi\\kurei_kei\\normal.png")));
            hFace.insert(pair<eFace, int>(Shocked,   LoadGraph("image\\144dpi\\kurei_kei\\shocked.png")));
            hFace.insert(pair<eFace, int>(Speaking,  LoadGraph("image\\144dpi\\kurei_kei\\speaking.png")));
            hFace.insert(pair<eFace, int>(Tearful,   LoadGraph("image\\144dpi\\kurei_kei\\tearful.png")));
            hFace.insert(pair<eFace, int>(Wink,      LoadGraph("image\\144dpi\\kurei_kei\\wink.png")));
            hFace.insert(pair<eFace, int>(Blink,     LoadGraph("image\\144dpi\\kurei_kei\\blink.png")));
            if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 0);
            break;
        }
    }
}

void Chr::Draw()
{
    static int quiet_cnt = 1;       // 音の鳴っていないフレーム数を数える

    DrawGraph(x1, y1, hBase, TRUE);

    // 音が鳴っているときとその1ループ後は表情を変える
    if (pVoice->IsPlaying() || quiet_cnt == 0)
        DrawGraph(x1, y1, hFace.at(pVoice->GetFace()), TRUE);


    static int wait_frame;          // 音が鳴らないフレーム数がこの値に達したら瞬きする
    static int blink_frame = 0;     // 瞬きで目を瞑っているフレーム数を数える

    if (quiet_cnt == 1)
        wait_frame = GetRand(25 * 60) + 30;

    // 音が鳴っていないときquiet_cntをインクリメント
    if (!pVoice->IsPlaying())
        quiet_cnt++;
    else
        quiet_cnt = 0;

    // wait_frameフレームの間音が鳴っていなければ瞬きをする
    if (quiet_cnt == wait_frame) {
        blink_frame = 1;
    }

    if (blink_frame > 0) {
        // 6フレーム(0.1秒)の間目を閉じる
        if (blink_frame <= 6) {
            DrawGraph(x1, y1, hFace.at(Blink), TRUE);
            blink_frame++;
        }
        else {
            blink_frame = 0;
            quiet_cnt = 1;
        }
    }
}

// 現在のバッテリー残量を%で返す
int BatteryPercent()
{
    SYSTEM_POWER_STATUS pw_stat;

    GetSystemPowerStatus(&pw_stat);

    return pw_stat.BatteryLifePercent;
}

void Chr::Update()
{
    if (!pVoice->IsPlaying()) {
        // クリックされたら音声を流す
        if (IsClicked())
            pVoice->SoundWords();


        if (pVoice->GetCfg().sound && pVoice->GetCfg().auto_repro) {
            // もし電源がなかったらシステムサウンドを流す
            static int bat_charge;
            static int pre_bat_charge = 100;

            bat_charge = BatteryPercent();
            if (bat_charge <= BAT_CRISIS && pre_bat_charge > BAT_CRISIS)
                pVoice->SoundSys(1);
            else if (bat_charge <= BAT_LOW && pre_bat_charge > BAT_LOW)
                pVoice->SoundSys(0);
            pre_bat_charge = bat_charge;
        }


        // もしキーボードが押されていたらそのキーを再生する
        char key_state[256];

        GetHitKeyStateAll(key_state);
        if (key_state[KEY_INPUT_SPACE])     pVoice->SoundKey(KEY_INPUT_SPACE);
        if (key_state[KEY_INPUT_RETURN])    pVoice->SoundKey(KEY_INPUT_RETURN);
        if (key_state[KEY_INPUT_BACK])      pVoice->SoundKey(KEY_INPUT_BACK);
        if (key_state[KEY_INPUT_ESCAPE])    pVoice->SoundKey(KEY_INPUT_ESCAPE);
        if (key_state[KEY_INPUT_LWIN] ||
            key_state[KEY_INPUT_RWIN])      pVoice->SoundKey(KEY_INPUT_WIN);
        if (key_state[KEY_INPUT_LCONTROL] ||
            key_state[KEY_INPUT_RCONTROL])  pVoice->SoundKey(KEY_INPUT_CTRL);
        if (key_state[KEY_INPUT_LALT] ||
            key_state[KEY_INPUT_RALT])      pVoice->SoundKey(KEY_INPUT_ALT);
        if (key_state[KEY_INPUT_LSHIFT] ||
            key_state[KEY_INPUT_RSHIFT])    pVoice->SoundKey(KEY_INPUT_SHIFT);
        if (key_state[KEY_INPUT_TAB])       pVoice->SoundKey(KEY_INPUT_TAB);
        if (key_state[KEY_INPUT_INSERT])    pVoice->SoundKey(KEY_INPUT_INSERT);
        if (key_state[KEY_INPUT_DELETE])    pVoice->SoundKey(KEY_INPUT_DELETE);
        if (key_state[KEY_INPUT_SYSRQ])     pVoice->SoundKey(KEY_INPUT_SYSRQ);
        if (key_state[KEY_INPUT_HOME])      pVoice->SoundKey(KEY_INPUT_HOME);
        if (key_state[KEY_INPUT_END])       pVoice->SoundKey(KEY_INPUT_END);
        if (key_state[KEY_INPUT_PGUP])      pVoice->SoundKey(KEY_INPUT_PGUP);
        if (key_state[KEY_INPUT_PGDN])      pVoice->SoundKey(KEY_INPUT_PGDN);
        if (key_state[KEY_INPUT_UP])        pVoice->SoundKey(KEY_INPUT_UP);
        if (key_state[KEY_INPUT_DOWN])      pVoice->SoundKey(KEY_INPUT_DOWN);
        if (key_state[KEY_INPUT_RIGHT])     pVoice->SoundKey(KEY_INPUT_RIGHT);
        if (key_state[KEY_INPUT_LEFT])      pVoice->SoundKey(KEY_INPUT_LEFT);
    }
}
