#include "DxLib.h"
#include "Voice.h"
#include "Clock.h"
#include "Exception.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <fstream>
using std::pair;

Voice::Voice()
{
    std::ostringstream file_name;
    const std::string empty_string;

    // 年を読み上げる音声をロード
    hYear[0] = LoadSoundMem("sound\\voice_B\\kei2_voice_001.wav");
    hYear[1] = LoadSoundMem("sound\\voice_B\\kei2_voice_002.wav");
    hYear[2] = LoadSoundMem("sound\\voice_B\\kei2_voice_003.wav");
    hYear[3] = LoadSoundMem("sound\\voice_B\\kei2_voice_003_1.wav");
    hYear[4] = LoadSoundMem("sound\\voice_B\\kei2_voice_003_2.wav");
    hYear[5] = LoadSoundMem("sound\\voice_B\\kei2_voice_003_3.wav");
    hYear[6] = LoadSoundMem("sound\\voice_B\\kei2_voice_003_4.wav");
    if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 1);

    // 月を読み上げる音声をロード
    for (int i = 0; i < MONTH; i++) {
        file_name.str(empty_string);
        file_name << "sound\\voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 4 << ".wav";
        hMonth[i] = LoadSoundMem(file_name.str().c_str());
        if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 2);
    }

    // 日を読み上げる音声をロード
    for (int i = 0; i < DAY; i++) {
        file_name.str(empty_string);
        file_name << "sound\\voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 16 << ".wav";
        hDay[i] = LoadSoundMem(file_name.str().c_str());
        if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 3);
    }

    // 曜日を読み上げる音声をロード
    for (int i = 0; i < WEEK; i++) {
        file_name.str(empty_string);
        file_name << "sound\\voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 47 << ".wav";
        hWeek[(i + 1) % WEEK] = LoadSoundMem(file_name.str().c_str());
        if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 4);
    }

    // 午前・午後を読み上げる音声をロード
    for (int i = 0; i < NOON; i++) {
        file_name.str(empty_string);
        file_name << "sound\\voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 54 << ".wav";
        hNoon[i] = LoadSoundMem(file_name.str().c_str());
        if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 5);
    }

    // 時を読み上げる音声をロード
    for (int i = 0; i < HOUR; i++) {
        file_name.str(empty_string);
        file_name << "sound\\voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 56 << ".wav";
        hHour[i] = LoadSoundMem(file_name.str().c_str());
        if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 6);
    }

    // 時報を読み上げる音声をロード
    for (int i = 0; i < HOUR; i++) {
        file_name.str(empty_string);
        file_name << "sound\\voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 81 << ".wav";
        hTime_signal[i] = LoadSoundMem(file_name.str().c_str());
        if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 7);
    }

    // 分を読み上げる音声をロード
    for (int i = 0; i < MINUTE; i++) {
        file_name.str(empty_string);
        file_name << "sound\\voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 106 << ".wav";
        hMinute[i] = LoadSoundMem(file_name.str().c_str());
        if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 8);
    }

    // 語尾の音声をロード
    hTail[0] = LoadSoundMem("sound\\voice_B\\kei2_voice_166.wav");          // …だね！
    hTail[1] = LoadSoundMem("sound\\voice_B\\kei2_voice_167.wav");          // …だよ！
    hTail[2] = LoadSoundMem("sound\\voice_B\\kei2_voice_168.wav");          // …になったよ！
    hTail[3] = LoadSoundMem("sound\\voice_B\\kei2_voice_247.wav");          // …だって！
    if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 9);

    // 日付依存の台詞をロード
    hSeason[0] = LoadSoundMem("sound\\voice_A\\kei_voice_041_1.wav");       // あけましておめでとう！　今年もよろしくね。
    hSeason[1] = LoadSoundMem("sound\\voice_A\\kei_voice_053.wav");         // トリックオアトリート！
    hSeason[2] = LoadSoundMem("sound\\voice_A\\kei_voice_054.wav");         // メリークリスマス！
    hSeason[3] = LoadSoundMem("sound\\voice_B\\kei2_voice_212.wav");        // お誕生日おめでとう！
    hSeason[4] = LoadSoundMem("sound\\voice_B\\kei2_voice_213.wav");        // ハッピーバースデー！
    hSeason[5] = LoadSoundMem("sound\\voice_B\\kei2_voice_251.wav");        // あたたかい格好して出かけよう！
    hSeason[6] = LoadSoundMem("sound\\voice_B\\kei2_voice_252.wav");        // 水分補給をこまめにとるといいよ
    hSeason[7] = LoadSoundMem("sound\\voice_B\\kei2_voice_253.wav");        // 熱中症に気を付けてね
    hSeason[8] = LoadSoundMem("sound\\voice_B\\kei2_voice_255.wav");        // 風邪ひかないよう気を付けてね
    hSeason[9] = LoadSoundMem("sound\\voice_B\\kei2_voice_263.wav");        // 帰ったら手洗いうがいだよ！
    if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 10);

    // システム音声をロード
    hSystem[0] = LoadSoundMem("sound\\voice_A\\kei_voice_088.wav");         // んー…バッテリー減ってきてる こまめにセーブしてね
    hSystem[1] = LoadSoundMem("sound\\voice_A\\kei_voice_089.wav");         // あー！　もう限界っ！ バッテリー切れちゃうよ！
    hSystem[2] = LoadSoundMem("sound\\voice_A\\kei_voice_037.wav");         // 設定変えてみる？
    hSystem[3] = LoadSoundMem("sound\\voice_B\\kei2_voice_310.wav");        // 完了！
    if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 11);

    // アラーム関係の音声をロード
    hAlarm[0] = LoadSoundMem("sound\\alarm1.wav");                          // ピピピピピッ×4
    hAlarm[1] = LoadSoundMem("sound\\voice_A\\kei_voice_062.wav");          // 時間だよ！
    hAlarm[2] = LoadSoundMem("sound\\voice_A\\kei_voice_059_2.wav");        // 早く早く！
    hAlarm[3] = LoadSoundMem("sound\\voice_A\\kei_voice_067.wav");          // 早く起きてよ！
    hAlarm[4] = LoadSoundMem("sound\\voice_B\\kei2_voice_088_2.wav");       // さぁ起きた起きた～
    hAlarm[5] = LoadSoundMem("sound\\voice_B\\kei2_voice_089_2.wav");       // ほら早く起きなよ～
    hAlarm[6] = LoadSoundMem("sound\\voice_B\\kei2_voice_173.wav");         // おはよう！
    if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 12);

    // タイマー関係の音声をロード
    hTimer[0] = LoadSoundMem("sound\\voice_A\\kei_voice_055.wav");
    hTimer[1] = LoadSoundMem("sound\\voice_A\\kei_voice_056.wav");
    hTimer[2] = LoadSoundMem("sound\\voice_A\\kei_voice_058.wav");
    hTimer[3] = LoadSoundMem("sound\\voice_A\\kei_voice_059_1.wav");
    hTimer[4] = LoadSoundMem("sound\\voice_A\\kei_voice_069.wav");
    hTimer[5] = LoadSoundMem("sound\\voice_A\\kei_voice_070.wav");
    hTimer[6] = LoadSoundMem("sound\\voice_A\\kei_voice_071.wav");
    hTimer[7] = LoadSoundMem("sound\\voice_A\\kei_voice_073.wav");
    hTimer[8] = LoadSoundMem("sound\\voice_A\\kei_voice_074.wav");
    hTimer[9] = LoadSoundMem("sound\\voice_A\\kei_voice_075.wav");
    hTimer[10] = LoadSoundMem("sound\\voice_A\\kei_voice_076.wav");
    hTimer[11] = LoadSoundMem("sound\\voice_A\\kei_voice_077.wav");
    hTimer[12] = LoadSoundMem("sound\\voice_A\\kei_voice_079.wav");
    hTimer[13] = LoadSoundMem("sound\\voice_A\\kei_voice_080.wav");
    hTimer[14] = LoadSoundMem("sound\\voice_A\\kei_voice_060.wav");
    hTimer[15] = LoadSoundMem("sound\\voice_A\\kei_voice_061_a.wav");
    hTimer[16] = LoadSoundMem("sound\\voice_A\\kei_voice_061_b.wav");
    hTimer[17] = LoadSoundMem("sound\\voice_A\\kei_voice_081.wav");
    hTimer[18] = LoadSoundMem("sound\\voice_B\\kei2_voice_272.wav");
    hTimer[19] = LoadSoundMem("sound\\voice_B\\kei2_voice_273.wav");
    hTimer[20] = LoadSoundMem("sound\\voice_B\\kei2_voice_274.wav");
    hTimer[21] = LoadSoundMem("sound\\voice_B\\kei2_voice_275.wav");
    if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 13);

    // キーボードキーの音声をロード
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_SPACE, LoadSoundMem("sound\\voice_A\\kei_voice_103.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_RETURN, LoadSoundMem("sound\\voice_A\\kei_voice_104.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_BACK, LoadSoundMem("sound\\voice_A\\kei_voice_105.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_ESCAPE, LoadSoundMem("sound\\voice_A\\kei_voice_106.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_WIN, LoadSoundMem("sound\\voice_A\\kei_voice_107.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_CTRL, LoadSoundMem("sound\\voice_A\\kei_voice_108.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_ALT, LoadSoundMem("sound\\voice_A\\kei_voice_109.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_SHIFT, LoadSoundMem("sound\\voice_A\\kei_voice_110.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_TAB, LoadSoundMem("sound\\voice_A\\kei_voice_111.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_INSERT, LoadSoundMem("sound\\voice_A\\kei_voice_112.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_DELETE, LoadSoundMem("sound\\voice_A\\kei_voice_113.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_SYSRQ, LoadSoundMem("sound\\voice_A\\kei_voice_114.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_HOME, LoadSoundMem("sound\\voice_A\\kei_voice_115.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_END, LoadSoundMem("sound\\voice_A\\kei_voice_116.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_PGUP, LoadSoundMem("sound\\voice_A\\kei_voice_117.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_PGDN, LoadSoundMem("sound\\voice_A\\kei_voice_118.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_UP, LoadSoundMem("sound\\voice_A\\kei_voice_119.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_DOWN, LoadSoundMem("sound\\voice_A\\kei_voice_120.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_RIGHT, LoadSoundMem("sound\\voice_A\\kei_voice_121.wav")));
    keyboard.insert(pair<KeyCode, int>(KEY_INPUT_LEFT, LoadSoundMem("sound\\voice_A\\kei_voice_122.wav")));
    if (ProcessMessage() == -1) throw Exception(Exception::LoadingNotDone, 14);

    // キャラの台詞をロード
    words[0] = Words(0, 24, true, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_005.wav"));
    words[1] = Words(0, 24, false, eFace::Wink, LoadSoundMem("sound\\voice_A\\kei_voice_005_2.wav"));
    words[2] = Words(0, 24, true, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_006.wav"));
    words[3] = Words(0, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_007.wav"));
    words[4] = Words(6, 10, true, eFace::Doya, LoadSoundMem("sound\\voice_A\\kei_voice_008.wav"));
    words[5] = Words(5, 10, false, eFace::Doya, LoadSoundMem("sound\\voice_A\\kei_voice_008_1.wav"));
    words[6] = Words(5, 15, true, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_008_2.wav"));
    words[7] = Words(11.5, 15.0, false, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_009.wav"));
    words[8] = Words(10, 19, false, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_009_1.wav"));
    words[9] = Words(19, 3, true, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_010.wav"));
    words[10] = Words(19, 5, false, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_010_1.wav"));
    words[11] = Words(0, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_010_2.wav"));
    words[12] = Words(23, 3, true, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_011.wav"));
    words[13] = Words(23, 3, false, eFace::Doya, LoadSoundMem("sound\\voice_A\\kei_voice_011_1.wav"));
    words[14] = Words(0, 24, false, eFace::Excited, LoadSoundMem("sound\\voice_A\\kei_voice_012.wav"));
    words[15] = Words(0, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_013.wav"));
    words[16] = Words(0, 24, true, eFace::Kyoton, LoadSoundMem("sound\\voice_A\\kei_voice_014.wav"));
    words[17] = Words(0, 24, false, eFace::Kyoton, LoadSoundMem("sound\\voice_A\\kei_voice_014_1.wav"));
    words[18] = Words(0, 24, false, eFace::Wink, LoadSoundMem("sound\\voice_A\\kei_voice_015.wav"));
    words[19] = Words(0, 24, false, eFace::Wink, LoadSoundMem("sound\\voice_A\\kei_voice_015_2.wav"));
    words[20] = Words(0, 24, true, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_016.wav"));
    words[21] = Words(0, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_016_1.wav"));
    words[22] = Words(16, 20, false, eFace::Excited, LoadSoundMem("sound\\voice_A\\kei_voice_017.wav"));
    words[23] = Words(16, 20, false, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_017_1.wav"));
    words[24] = Words(16, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_017_2.wav"));
    words[25] = Words(0, 24, true, eFace::Wink, LoadSoundMem("sound\\voice_A\\kei_voice_018.wav"));
    words[26] = Words(0, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_018_1.wav"));
    words[27] = Words(0, 24, true, eFace::Confident, LoadSoundMem("sound\\voice_A\\kei_voice_019.wav"));
    words[28] = Words(0, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_019_1.wav"));
    words[29] = Words(0, 24, true, eFace::Normal, LoadSoundMem("sound\\voice_A\\kei_voice_020.wav"));
    words[30] = Words(0, 24, false, eFace::Confident, LoadSoundMem("sound\\voice_A\\kei_voice_020_1.wav"));
    words[31] = Words(0, 24, true, eFace::Normal, LoadSoundMem("sound\\voice_A\\kei_voice_021.wav"));
    words[32] = Words(0, 24, false, eFace::Normal, LoadSoundMem("sound\\voice_A\\kei_voice_021_1.wav"));
    words[33] = Words(0, 24, true, eFace::Tearful, LoadSoundMem("sound\\voice_A\\kei_voice_022.wav"));
    words[34] = Words(0, 24, false, eFace::Tearful, LoadSoundMem("sound\\voice_A\\kei_voice_022_1.wav"));
    words[35] = Words(0, 24, false, eFace::Confident, LoadSoundMem("sound\\voice_A\\kei_voice_023.wav"));
    words[36] = Words(0, 24, false, eFace::Kyoton, LoadSoundMem("sound\\voice_A\\kei_voice_023_1.wav"));
    words[37] = Words(0, 24, true, eFace::Normal, LoadSoundMem("sound\\voice_A\\kei_voice_024.wav"));
    words[38] = Words(0, 24, false, eFace::Normal, LoadSoundMem("sound\\voice_A\\kei_voice_024_1.wav"));
    words[39] = Words(0, 24, false, eFace::Shocked, LoadSoundMem("sound\\voice_A\\kei_voice_025.wav"));
    words[40] = Words(0, 24, false, eFace::Excited, LoadSoundMem("sound\\voice_A\\kei_voice_026.wav"));
    words[41] = Words(0, 24, false, eFace::Wink, LoadSoundMem("sound\\voice_A\\kei_voice_027.wav"));
    words[42] = Words(0, 24, false, eFace::Shocked, LoadSoundMem("sound\\voice_A\\kei_voice_028.wav"));
    words[43] = Words(0, 24, false, eFace::Confident, LoadSoundMem("sound\\voice_A\\kei_voice_029.wav"));
    words[44] = Words(0, 24, true, eFace::Tearful, LoadSoundMem("sound\\voice_A\\kei_voice_030.wav"));
    words[45] = Words(0, 24, false, eFace::Tearful, LoadSoundMem("sound\\voice_A\\kei_voice_030_1.wav"));
    words[46] = Words(0, 24, true, eFace::Excited, LoadSoundMem("sound\\voice_A\\kei_voice_031.wav"));
    words[47] = Words(0, 24, false, eFace::Excited, LoadSoundMem("sound\\voice_A\\kei_voice_031_1.wav"));
    words[48] = Words(0, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_032.wav"));
    words[49] = Words(0, 24, true, eFace::Doya, LoadSoundMem("sound\\voice_A\\kei_voice_034.wav"));
    words[50] = Words(0, 24, false, eFace::Excited, LoadSoundMem("sound\\voice_A\\kei_voice_035.wav"));
    words[51] = Words(0, 24, true, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_038.wav"));
    words[52] = Words(0, 24, true, eFace::Confident, LoadSoundMem("sound\\voice_A\\kei_voice_039.wav"));
    words[53] = Words(0, 24, false, eFace::Normal, LoadSoundMem("sound\\voice_A\\kei_voice_040.wav"));
    words[54] = Words(0, 24, false, eFace::Shocked, LoadSoundMem("sound\\voice_A\\kei_voice_040_1.wav"));
    words[55] = Words(0, 24, false, eFace::Shocked, LoadSoundMem("sound\\voice_A\\kei_voice_059_2.wav"));
    words[56] = Words(0, 24, false, eFace::Excited, LoadSoundMem("sound\\voice_A\\kei_voice_060.wav"));
    words[57] = Words(0, 24, false, eFace::Normal, LoadSoundMem("sound\\voice_A\\kei_voice_064.wav"));
    words[58] = Words(10, 24, true, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_084.wav"));
    words[59] = Words(0, 24, true, eFace::Shocked, LoadSoundMem("sound\\voice_A\\kei_voice_085.wav"));
    words[60] = Words(0, 24, false, eFace::Shocked, LoadSoundMem("sound\\voice_A\\kei_voice_085_1.wav"));
    words[61] = Words(0, 24, false, eFace::Wink, LoadSoundMem("sound\\voice_A\\kei_voice_086_2.wav"));
    words[62] = Words(0, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_087_1.wav"));
    words[63] = Words(0, 24, false, eFace::Confident, LoadSoundMem("sound\\voice_A\\kei_voice_090_1.wav"));
    words[64] = Words(0, 24, false, eFace::Kyoton, LoadSoundMem("sound\\voice_A\\kei_voice_091_1.wav"));
    words[65] = Words(0, 24, false, eFace::Doya, LoadSoundMem("sound\\voice_A\\kei_voice_092.wav"));
    words[66] = Words(0, 24, false, eFace::Confident, LoadSoundMem("sound\\voice_A\\kei_voice_095_2.wav"));
    words[67] = Words(0, 24, true, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_096.wav"));
    words[68] = Words(0, 24, true, eFace::Speaking, LoadSoundMem("sound\\voice_A\\kei_voice_097.wav"));
    words[69] = Words(0, 24, false, eFace::Normal, LoadSoundMem("sound\\voice_A\\kei_voice_097_1.wav"));
    words[70] = Words(0, 24, true, eFace::Shocked, LoadSoundMem("sound\\voice_A\\kei_voice_098.wav"));
    words[71] = Words(0, 24, false, eFace::Shocked, LoadSoundMem("sound\\voice_A\\kei_voice_098_1.wav"));
    words[72] = Words(0, 24, true, eFace::Laugh, LoadSoundMem("sound\\voice_A\\kei_voice_099.wav"));
    words[73] = Words(7.5, 9.0, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_169.wav"));
    words[74] = Words(7.5, 9.0, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_170.wav"));
    words[75] = Words(17, 20, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_171.wav"));
    words[76] = Words(17, 20, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_172.wav"));
    words[77] = Words(5, 10, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_173.wav"));
    words[78] = Words(10, 19, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_174.wav"));
    words[79] = Words(19, 5, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_175.wav"));
    words[80] = Words(23, 3, false, eFace::Doya, LoadSoundMem("sound\\voice_B\\kei2_voice_176.wav"));
    words[81] = Words(17, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_177.wav"));
    words[82] = Words(21, 1, false, eFace::Confident, LoadSoundMem("sound\\voice_B\\kei2_voice_178.wav"));
    words[83] = Words(1, 5, false, eFace::Kyoton, LoadSoundMem("sound\\voice_B\\kei2_voice_179.wav"));
    words[84] = Words(2, 5, false, eFace::Kyoton, LoadSoundMem("sound\\voice_B\\kei2_voice_180.wav"));
    words[85] = Words(1, 4, false, eFace::Normal, LoadSoundMem("sound\\voice_B\\kei2_voice_181.wav"));
    words[86] = Words(0, 5, true, eFace::Wink, LoadSoundMem("sound\\voice_B\\kei2_voice_182.wav"));
    words[87] = Words(2, 5, false, eFace::Tearful, LoadSoundMem("sound\\voice_B\\kei2_voice_183.wav"));
    words[88] = Words(5, 7, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_184.wav"));
    words[89] = Words(6, 10, false, eFace::Confident, LoadSoundMem("sound\\voice_B\\kei2_voice_185.wav"));
    words[90] = Words(6, 12, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_186.wav"));
    words[91] = Words(10, 18, false, eFace::Normal, LoadSoundMem("sound\\voice_B\\kei2_voice_187.wav"));
    words[92] = Words(12, 14, false, eFace::Excited, LoadSoundMem("sound\\voice_B\\kei2_voice_188.wav"));
    words[93] = Words(12, 14, false, eFace::Confident, LoadSoundMem("sound\\voice_B\\kei2_voice_189.wav"));
    words[94] = Words(13, 16, false, eFace::Normal, LoadSoundMem("sound\\voice_B\\kei2_voice_190.wav"));
    words[95] = Words(14.5, 15.5, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_191.wav"));
    words[96] = Words(0, 24, false, eFace::Tearful, LoadSoundMem("sound\\voice_B\\kei2_voice_192.wav"));
    words[97] = Words(16, 19, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_193.wav"));
    words[98] = Words(17, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_194.wav"));
    words[99] = Words(19, 23, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_195.wav"));
    words[100] = Words(19, 22, false, eFace::Doya, LoadSoundMem("sound\\voice_B\\kei2_voice_196.wav"));
    words[101] = Words(21, 23, false, eFace::Doya, LoadSoundMem("sound\\voice_B\\kei2_voice_197.wav"));
    words[102] = Words(20, 24, false, eFace::Normal, LoadSoundMem("sound\\voice_B\\kei2_voice_198.wav"));
    words[103] = Words(23, 2, false, eFace::Wink, LoadSoundMem("sound\\voice_B\\kei2_voice_199.wav"));
    words[104] = Words(10, 24, false, eFace::Doya, LoadSoundMem("sound\\voice_B\\kei2_voice_200.wav"));
    words[105] = Words(11, 23, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_201.wav"));
    words[106] = Words(11, 3, false, eFace::Normal, LoadSoundMem("sound\\voice_B\\kei2_voice_202.wav"));
    words[107] = Words(0, 24, false, eFace::Excited, LoadSoundMem("sound\\voice_B\\kei2_voice_203.wav"));
    words[108] = Words(0, 24, false, eFace::Confident, LoadSoundMem("sound\\voice_B\\kei2_voice_204.wav"));
    words[109] = Words(0, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_205.wav"));
    words[110] = Words(0, 24, false, eFace::Shocked, LoadSoundMem("sound\\voice_B\\kei2_voice_206.wav"));
    words[111] = Words(0, 24, false, eFace::Wink, LoadSoundMem("sound\\voice_B\\kei2_voice_207.wav"));
    words[112] = Words(0, 24, false, eFace::Wink, LoadSoundMem("sound\\voice_B\\kei2_voice_208.wav"));
    words[113] = Words(18, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_209.wav"));
    words[114] = Words(9, 17, false, eFace::Wink, LoadSoundMem("sound\\voice_B\\kei2_voice_210.wav"));
    words[115] = Words(11, 1, false, eFace::Doya, LoadSoundMem("sound\\voice_B\\kei2_voice_211.wav"));
    words[116] = Words(0, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_216.wav"));
    words[117] = Words(0, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_262.wav"));
    words[118] = Words(16, 19, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_263.wav"));
    words[119] = Words(6, 24, false, eFace::Wink, LoadSoundMem("sound\\voice_B\\kei2_voice_264.wav"));
    words[120] = Words(0, 24, false, eFace::Confident, LoadSoundMem("sound\\voice_B\\kei2_voice_265.wav"));
    words[121] = Words(0, 24, false, eFace::Excited, LoadSoundMem("sound\\voice_B\\kei2_voice_266.wav"));
    words[122] = Words(0, 24, false, eFace::Excited, LoadSoundMem("sound\\voice_B\\kei2_voice_267.wav"));
    words[123] = Words(0, 24, false, eFace::Laugh, LoadSoundMem("sound\\voice_B\\kei2_voice_268.wav"));
    words[124] = Words(0, 24, false, eFace::Confident, LoadSoundMem("sound\\voice_B\\kei2_voice_269.wav"));
    words[125] = Words(0, 24, false, eFace::Excited, LoadSoundMem("sound\\voice_B\\kei2_voice_270.wav"));
    words[126] = Words(0, 24, false, eFace::Shocked, LoadSoundMem("sound\\voice_B\\kei2_voice_271.wav"));
    words[127] = Words(0, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_277.wav"));
    words[128] = Words(0, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_297.wav"));
    words[129] = Words(0, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_298.wav"));
    words[130] = Words(0, 24, false, eFace::Speaking, LoadSoundMem("sound\\voice_B\\kei2_voice_299.wav"));
    words[131] = Words(0, 24, true, eFace::Excited, LoadSoundMem("sound\\voice_B\\kei2_voice_302.wav"));

    hPlaying = -1;
    face = Laugh;
    config.sound = true;
    config.auto_repro = true;
    config.for_prg = false;
    config.first_time = true;
}

void Voice::SoundTime(eTmData data, int index)
{
    if (config.sound) {
        face = eFace::Laugh;
        switch (data) {
        case Year:
            if (index >= 0 && index < YEAR) {
                hPlaying = hYear[index];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            }
            break;
        case Month:
            if (index >= 0 && index < MONTH) {
                hPlaying = hMonth[index];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            }
            break;
        case Day:
            if (index >= 0 && index < DAY) {
                hPlaying = hDay[index];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            }
            break;
        case Week:
            if (index >= 0 && index < WEEK) {
                hPlaying = hWeek[index];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            }
            break;
        case Noon:
            if (index >= 0 && index < NOON) {
                hPlaying = hNoon[index];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            }
            break;
        case Hour:
            if (index >= 0 && index < HOUR) {
                hPlaying = hHour[index];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            }
            break;
        case TimeSignal:
            if (index >= 0 && index < HOUR) {
                hPlaying = hTime_signal[index];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            }
            break;
        case Minute:
            if (index >= 0 && index < MINUTE) {
                hPlaying = hMinute[index];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            }
            break;
        case Tail:
            if (index >= 0 && index < TAIL) {
                hPlaying = hTail[index];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            }
            break;
        }
    }
}

void Voice::SoundWords()
{
    if (config.sound) {
        // 初回起動時の最初の再生は固定
        if (config.first_time) {
            hPlaying = words[18].GetHandle();
            face = eFace::Laugh;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            SetCfg(FirstTime, false);
            return;
        }

        std::vector<Words> cand;                // 再生される音声の候補

                                                // 候補に時刻に応じた音声を追加
        for (int i = 0; i < WORDS; i++)
            if (words[i].IsAppropriate())
                cand.push_back(words[i]);

        // 日付に応じた音声を追加(出現確率10倍)
        if (pDate->IsNewYear())
            for (int i = 0; i < 10; i++)
                cand.push_back(Words(eFace::Doya, hSeason[0]));
        else if (pDate->IsHalloween())
            for (int i = 0; i < 10; i++)
                cand.push_back(Words(eFace::Excited, hSeason[1]));
        else if (pDate->IsChristmas())
            for (int i = 0; i < 10; i++)
                cand.push_back(Words(eFace::Excited, hSeason[2]));
        else if (pDate->IsBirthday()) {
            for (int i = 0; i < 10; i++)
                cand.push_back(Words(eFace::Laugh, hSeason[3]));
            for (int i = 0; i < 10; i++)
                cand.push_back(Words(eFace::Excited, hSeason[4]));
        }
        // 季節に応じた音声を追加
        if (pDate->IsSummer()) {
            cand.push_back(Words(eFace::Normal, hSeason[6]));
            cand.push_back(Words(eFace::Normal, hSeason[7]));
        }
        else if (pDate->IsWinter()) {
            cand.push_back(Words(eFace::Speaking, hSeason[5]));
            cand.push_back(Words(eFace::Kyoton, hSeason[8]));
            cand.push_back(Words(eFace::Wink, hSeason[9]));
        }

        // 時間に合ったセリフをランダムで再生
        Words chosen = cand[GetRand(cand.size() - 1)];
        hPlaying = chosen.GetHandle();
        face = chosen.GetFace();
        PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
    }
}

void Voice::SoundSys(int index)
{
    if (config.sound) {
        switch (index) {
        case 0:
            hPlaying = hSystem[0];
            face = eFace::Kyoton;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case 1:
            hPlaying = hSystem[1];
            face = eFace::Shocked;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case 2:
            hPlaying = hSystem[2];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case 3:
            hPlaying = hSystem[3];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        default:
            break;
        }
    }
}

void Voice::SoundKey(KeyCode key)
{
    if (config.sound) {
        hPlaying = keyboard.at(key);
        face = eFace::Laugh;
        PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
    }
}

// アラーム音とボイスを交互に再生
void Voice::SoundAlarm()
{
    if (config.sound && config.auto_repro) {
        static int status = 0;          // 0:ボイス再生中　1:アラーム音再生中

        if (!IsPlaying()) {
            if (status == 0) {
                hPlaying = hAlarm[0];
                face = eFace::Speaking;
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
                status = 1;
            }
            else {
                hPlaying = hAlarm[1];
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
                status = 0;
            }
        }
    }
}

// アラーム音とボイスを交互に再生
// ボイスは複数種類からランダムに選ばれる
void Voice::SoundWakeUp()
{
    if (config.sound && config.auto_repro) {
        static int status = 0;          // 0:ボイス再生中　1:アラーム再生中

        if (!IsPlaying()) {
            if (status == 0) {
                hPlaying = hAlarm[0];
                face = eFace::Excited;
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
                status = 1;
            }
            else {
                hPlaying = hAlarm[GetRand(4) + 2];          // hAlarm[2～6]をランダムで再生
                PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
                status = 0;
            }
        }
    }
}

void Voice::SoundTimer(eTimerVoice voice)
{
    if (config.sound && config.auto_repro)
        switch (voice) {
        case Start1:
            hPlaying = hTimer[0];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Start2:
            hPlaying = hTimer[1];
            face = eFace::Excited;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last1m:
            hPlaying = hTimer[2];
            face = eFace::Confident;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last30:
            hPlaying = hTimer[3];
            face = eFace::Doya;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last1:
            hPlaying = hTimer[4];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last2:
            hPlaying = hTimer[5];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last3:
            hPlaying = hTimer[6];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last4:
            hPlaying = hTimer[7];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last5:
            hPlaying = hTimer[8];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last6:
            hPlaying = hTimer[9];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last7:
            hPlaying = hTimer[10];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last8:
            hPlaying = hTimer[11];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last9:
            hPlaying = hTimer[12];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Last10:
            hPlaying = hTimer[13];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Over1:
            hPlaying = hTimer[14];
            face = eFace::Laugh;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Over2:
            hPlaying = hTimer[15];
            face = eFace::Confident;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Over3:
            hPlaying = hTimer[16];
            face = eFace::Wink;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Over4:
            hPlaying = hTimer[17];
            face = eFace::Speaking;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Over5:
            hPlaying = hTimer[18];
            face = eFace::Laugh;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Over6:
            hPlaying = hTimer[19];
            face = eFace::Laugh;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Over7:
            hPlaying = hTimer[20];
            face = eFace::Laugh;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        case Over8:
            hPlaying = hTimer[21];
            face = eFace::Laugh;
            PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
            break;
        default:
            throw Exception(Exception::OtherError, "default in switch statement");
            break;
        }
}

bool Voice::IsPlaying()
{
    return CheckSoundMem(hPlaying) && hPlaying != -1;
}

bool Voice::IsTimerPlaying()
{
    if (IsPlaying())
        for (int i = 0; i < TIMER; i++)
            if (hPlaying == hTimer[i])
                return true;

    return false;
}

void Voice::Repeat()
{
    if (config.sound)
        PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
}

void Voice::StopSound()
{
    StopSoundMem(hPlaying);
    pDate->ResetSpeaking();
    pTime->ResetSpeaking();
}

void Voice::SetCfg(eCfg cfg, int value)
{
    switch (cfg) {
    case Sound:     config.sound = (value != 0);   break;
    case AutoRepro: config.auto_repro = (value != 0);   break;
    case ForPrg:    config.for_prg = (value != 0);   break;
    case FirstTime: config.first_time = (value != 0);   break;
    }
}

Words::Words(eFace f, int hw)
{
    min_hour = 0;
    max_hour = 0;
    min_minute = 0;
    max_minute = 0;
    programmer = false;
    face = f;
    hWord = hw;
}

Words::Words(int minh, int maxh, bool prg, eFace f, int hw)
{
    min_hour = minh;
    max_hour = maxh;
    min_minute = 0;
    max_minute = 0;
    programmer = prg;
    face = f;
    hWord = hw;
}

Words::Words(double minh, double maxh, bool prg, eFace f, int hw)
{
    min_hour = (int)minh;
    max_hour = (int)maxh;
    min_minute = (int)((minh - min_hour) * MINUTE);
    max_minute = (int)((maxh - max_hour) * MINUTE);
    programmer = prg;
    face = f;
    hWord = hw;
}

bool Words::IsAppropriate()
{
    int h = pTime->Geth();
    int m = pTime->Getm();

    // 設定がプログラマ用ではないときはプログラマ用音声をカット
    if (programmer && !pVoice->GetCfg().for_prg)
        return false;

    // 時間の範囲が日付をまたがない時
    if (min_hour < max_hour || (min_hour == max_hour && min_minute <= max_minute)) {
        if (h < min_hour || max_hour < h)
            return false;
        if (h == min_hour && m < min_minute)
            return false;
        if (h == max_hour && max_minute < m)
            return false;

        return true;
    }
    // 時間の範囲が日付をまたぐ時
    else {
        if (h > min_hour || max_hour > h)
            return true;
        if (h == min_hour && m > min_minute)
            return true;
        if (h == max_hour && max_minute > m)
            return true;

        return false;
    }
}
