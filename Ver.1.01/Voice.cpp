#include "DxLib.h"
#include "Voice.h"
#include "Clock.h"
#include <sstream>
#include <iomanip>
using std::pair;

Voice::Voice()
{
	std::ostringstream file_name;
	const std::string empty_string;

	// 年を読み上げる音声をロード
	hYear[0] = LoadSoundMem("voice_B\\kei2_voice_001.wav");
	hYear[1] = LoadSoundMem("voice_B\\kei2_voice_002.wav");
	hYear[2] = LoadSoundMem("voice_B\\kei2_voice_003.wav");
	hYear[3] = LoadSoundMem("voice_B\\kei2_voice_003_1.wav");
	hYear[4] = LoadSoundMem("voice_B\\kei2_voice_003_2.wav");
	hYear[5] = LoadSoundMem("voice_B\\kei2_voice_003_3.wav");
	hYear[6] = LoadSoundMem("voice_B\\kei2_voice_003_4.wav");
	if (ProcessMessage() == -1) throw 1;

	// 月を読み上げる音声をロード
	for (int i = 0; i < MONTH; i++) {
		file_name.str(empty_string);
		file_name << "voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 4 << ".wav";
		hMonth[i] = LoadSoundMem(file_name.str().c_str());
		if (ProcessMessage() == -1) throw 2;
	}

	// 日を読み上げる音声をロード
	for (int i = 0; i < DAY; i++) {
		file_name.str(empty_string);
		file_name << "voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 16 << ".wav";
		hDay[i] = LoadSoundMem(file_name.str().c_str());
		if (ProcessMessage() == -1) throw 3;
	}

	// 曜日を読み上げる音声をロード
	for (int i = 0; i < WEEK; i++) {
		file_name.str(empty_string);
		file_name << "voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 47 << ".wav";
		hWeek[(i + 1) % WEEK] = LoadSoundMem(file_name.str().c_str());
		if (ProcessMessage() == -1) throw 4;
	}

	// 午前・午後を読み上げる音声をロード
	for (int i = 0; i < NOON; i++) {
		file_name.str(empty_string);
		file_name << "voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 54 << ".wav";
		hNoon[i] = LoadSoundMem(file_name.str().c_str());
		if (ProcessMessage() == -1) throw 5;
	}

	// 時を読み上げる音声をロード
	for (int i = 0; i < HOUR; i++) {
		file_name.str(empty_string);
		file_name << "voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 56 << ".wav";
		hHour[i] = LoadSoundMem(file_name.str().c_str());
		if (ProcessMessage() == -1) throw 6;
	}

	// 時報を読み上げる音声をロード
	for (int i = 0; i < HOUR; i++) {
		file_name.str(empty_string);
		file_name << "voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 81 << ".wav";
		hTime_signal[i] = LoadSoundMem(file_name.str().c_str());
		if (ProcessMessage() == -1) throw 7;
	}

	// 分を読み上げる音声をロード
	for (int i = 0; i < MINUTE; i++) {
		file_name.str(empty_string);
		file_name << "voice_B\\kei2_voice_" << std::setw(3) << std::setfill('0') << i + 106 << ".wav";
		hMinute[i] = LoadSoundMem(file_name.str().c_str());
		if (ProcessMessage() == -1) throw 8;
	}

	// 語尾の音声をロード
	hTail[0] = LoadSoundMem("voice_B\\kei2_voice_166.wav");			// …だね！
	hTail[1] = LoadSoundMem("voice_B\\kei2_voice_167.wav");			// …だよ！
	hTail[2] = LoadSoundMem("voice_B\\kei2_voice_168.wav");			// …になったよ！
	hTail[3] = LoadSoundMem("voice_B\\kei2_voice_247.wav");			// …だって！
	if (ProcessMessage() == -1) throw 9;

	// 日付依存の台詞をロード
	hSeason[0] = LoadSoundMem("voice_A\\kei_voice_041_1.wav");		// あけましておめでとう！　今年もよろしくね。
	hSeason[1] = LoadSoundMem("voice_A\\kei_voice_053.wav");		// トリックオアトリート！
	hSeason[2] = LoadSoundMem("voice_A\\kei_voice_054.wav");		// メリークリスマス！
	hSeason[3] = LoadSoundMem("voice_B\\kei2_voice_212.wav");		// お誕生日おめでとう！
	hSeason[4] = LoadSoundMem("voice_B\\kei2_voice_213.wav");		// ハッピーバースデー！
	hSeason[5] = LoadSoundMem("voice_B\\kei2_voice_251.wav");		// あたたかい格好して出かけよう！
	hSeason[6] = LoadSoundMem("voice_B\\kei2_voice_252.wav");		// 水分補給をこまめにとるといいよ
	hSeason[7] = LoadSoundMem("voice_B\\kei2_voice_253.wav");		// 熱中症に気を付けてね
	hSeason[8] = LoadSoundMem("voice_B\\kei2_voice_255.wav");		// 風邪ひかないよう気を付けてね
	hSeason[9] = LoadSoundMem("voice_B\\kei2_voice_263.wav");		// 帰ったら手洗いうがいだよ！
	if (ProcessMessage() == -1) throw 10;

	// システム音声をロード
	hSystem[0] = LoadSoundMem("voice_A\\kei_voice_088.wav");		// んー…バッテリー減ってきてる こまめにセーブしてね
	hSystem[1] = LoadSoundMem("voice_A\\kei_voice_089.wav");		// あー！　もう限界っ！ バッテリー切れちゃうよ！
	hSystem[2] = LoadSoundMem("voice_A\\kei_voice_037.wav");		// 設定変えてみる？
	if (ProcessMessage() == -1) throw 11;

	// キーボードキーの音声をロード
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_SPACE,  LoadSoundMem("voice_A\\kei_voice_103.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_RETURN, LoadSoundMem("voice_A\\kei_voice_104.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_BACK,   LoadSoundMem("voice_A\\kei_voice_105.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_ESCAPE, LoadSoundMem("voice_A\\kei_voice_106.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_WIN,    LoadSoundMem("voice_A\\kei_voice_107.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_CTRL,   LoadSoundMem("voice_A\\kei_voice_108.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_ALT,    LoadSoundMem("voice_A\\kei_voice_109.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_SHIFT,  LoadSoundMem("voice_A\\kei_voice_110.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_TAB,    LoadSoundMem("voice_A\\kei_voice_111.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_INSERT, LoadSoundMem("voice_A\\kei_voice_112.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_DELETE, LoadSoundMem("voice_A\\kei_voice_113.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_SYSRQ,  LoadSoundMem("voice_A\\kei_voice_114.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_HOME,   LoadSoundMem("voice_A\\kei_voice_115.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_END,    LoadSoundMem("voice_A\\kei_voice_116.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_PGUP,   LoadSoundMem("voice_A\\kei_voice_117.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_PGDN,   LoadSoundMem("voice_A\\kei_voice_118.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_UP,     LoadSoundMem("voice_A\\kei_voice_119.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_DOWN,   LoadSoundMem("voice_A\\kei_voice_120.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_RIGHT,  LoadSoundMem("voice_A\\kei_voice_121.wav")));
	keyboard.insert(pair<KeyCode, int>(KEY_INPUT_LEFT,   LoadSoundMem("voice_A\\kei_voice_122.wav")));
	if (ProcessMessage() == -1) throw 12;

	// キャラの台詞をロード
	words[0]   = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_005.wav"));
	words[1]   = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_005_2.wav"));
	words[2]   = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_006.wav"));
	words[3]   = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_007.wav"));
	words[4]   = Words(6,    10,   true,  LoadSoundMem("voice_A\\kei_voice_008.wav"));
	words[5]   = Words(5,    10,   false, LoadSoundMem("voice_A\\kei_voice_008_1.wav"));
	words[6]   = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_008_2.wav"));
	words[7]   = Words(11.5, 15.0, false, LoadSoundMem("voice_A\\kei_voice_009.wav"));
	words[8]   = Words(10,   19,   false, LoadSoundMem("voice_A\\kei_voice_009_1.wav"));
	words[9]   = Words(19,   3,    true,  LoadSoundMem("voice_A\\kei_voice_010.wav"));
	words[10]  = Words(19,   5,    false, LoadSoundMem("voice_A\\kei_voice_010_1.wav"));
	words[11]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_010_2.wav"));
	words[12]  = Words(23,   3,    true,  LoadSoundMem("voice_A\\kei_voice_011.wav"));
	words[13]  = Words(23,   3,    false, LoadSoundMem("voice_A\\kei_voice_011_1.wav"));
	words[14]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_012.wav"));
	words[15]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_013.wav"));
	words[16]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_014.wav"));
	words[17]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_014_1.wav"));
	words[18]  = Words(0,    24,   false, true, LoadSoundMem("voice_A\\kei_voice_015.wav"));
	words[19]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_015_2.wav"));
	words[20]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_016.wav"));
	words[21]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_016_1.wav"));
	words[22]  = Words(16,   20,   false, LoadSoundMem("voice_A\\kei_voice_017.wav"));
	words[23]  = Words(16,   20,   false, LoadSoundMem("voice_A\\kei_voice_017_1.wav"));
	words[24]  = Words(16,   24,   false, LoadSoundMem("voice_A\\kei_voice_017_2.wav"));
	words[25]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_018.wav"));
	words[26]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_018_1.wav"));
	words[27]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_019.wav"));
	words[28]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_019_1.wav"));
	words[29]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_020.wav"));
	words[30]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_020_1.wav"));
	words[31]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_021.wav"));
	words[32]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_021_1.wav"));
	words[33]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_022.wav"));
	words[34]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_022_1.wav"));
	words[35]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_023.wav"));
	words[36]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_023_1.wav"));
	words[37]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_024.wav"));
	words[38]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_024_1.wav"));
	words[39]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_025.wav"));
	words[40]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_026.wav"));
	words[41]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_027.wav"));
	words[42]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_028.wav"));
	words[43]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_029.wav"));
	words[44]  = Words(0,    24,   true, true, LoadSoundMem("voice_A\\kei_voice_030.wav"));
	words[45]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_030_1.wav"));
	words[46]  = Words(0,    24,   true, true, LoadSoundMem("voice_A\\kei_voice_031.wav"));
	words[47]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_031_1.wav"));
	words[48]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_032.wav"));
	words[49]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_034.wav"));
	words[50]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_035.wav"));
	words[51]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_038.wav"));
	words[52]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_039.wav"));
	words[53]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_040.wav"));
	words[54]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_040_1.wav"));
	words[55]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_059_2.wav"));
	words[56]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_060.wav"));
	words[57]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_064.wav"));
	words[58]  = Words(10,   24,   true,  LoadSoundMem("voice_A\\kei_voice_084.wav"));
	words[59]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_085.wav"));
	words[60]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_085_1.wav"));
	words[61]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_086_2.wav"));
	words[62]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_087_1.wav"));
	words[63]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_090_1.wav"));
	words[64]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_091_1.wav"));
	words[65]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_092.wav"));
	words[66]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_095_2.wav"));
	words[67]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_096.wav"));
	words[68]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_097.wav"));
	words[69]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_097_1.wav"));
	words[70]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_098.wav"));
	words[71]  = Words(0,    24,   false, LoadSoundMem("voice_A\\kei_voice_098_1.wav"));
	words[72]  = Words(0,    24,   true,  LoadSoundMem("voice_A\\kei_voice_099.wav"));
	words[73]  = Words(7.5,  9.0,  false, LoadSoundMem("voice_B\\kei2_voice_169.wav"));
	words[74]  = Words(7.5,  9.0,  false, LoadSoundMem("voice_B\\kei2_voice_170.wav"));
	words[75]  = Words(17,   20,   false, LoadSoundMem("voice_B\\kei2_voice_171.wav"));
	words[76]  = Words(17,   20,   false, LoadSoundMem("voice_B\\kei2_voice_172.wav"));
	words[77]  = Words(5,    10,   false, LoadSoundMem("voice_B\\kei2_voice_173.wav"));
	words[78]  = Words(10,   19,   false, LoadSoundMem("voice_B\\kei2_voice_174.wav"));
	words[79]  = Words(19,   5,    false, LoadSoundMem("voice_B\\kei2_voice_175.wav"));
	words[80]  = Words(23,   3,    false, LoadSoundMem("voice_B\\kei2_voice_176.wav"));
	words[81]  = Words(17,   24,   false, LoadSoundMem("voice_B\\kei2_voice_177.wav"));
	words[82]  = Words(21,   1,    false, LoadSoundMem("voice_B\\kei2_voice_178.wav"));
	words[83]  = Words(1,    5,    false, LoadSoundMem("voice_B\\kei2_voice_179.wav"));
	words[84]  = Words(2,    5,    false, LoadSoundMem("voice_B\\kei2_voice_180.wav"));
	words[85]  = Words(1,    4,    false, LoadSoundMem("voice_B\\kei2_voice_181.wav"));
	words[86]  = Words(0,    5,    true,  LoadSoundMem("voice_B\\kei2_voice_182.wav"));
	words[87]  = Words(2,    5,    false, LoadSoundMem("voice_B\\kei2_voice_183.wav"));
	words[88]  = Words(5,    7,    false, LoadSoundMem("voice_B\\kei2_voice_184.wav"));
	words[89]  = Words(6,    10,   false, LoadSoundMem("voice_B\\kei2_voice_185.wav"));
	words[90]  = Words(6,    12,   false, LoadSoundMem("voice_B\\kei2_voice_186.wav"));
	words[91]  = Words(10,   18,   false, LoadSoundMem("voice_B\\kei2_voice_187.wav"));
	words[92]  = Words(12,   14,   false, LoadSoundMem("voice_B\\kei2_voice_188.wav"));
	words[93]  = Words(12,   14,   false, LoadSoundMem("voice_B\\kei2_voice_189.wav"));
	words[94]  = Words(13,   16,   false, LoadSoundMem("voice_B\\kei2_voice_190.wav"));
	words[95]  = Words(14.5, 15.5, false, LoadSoundMem("voice_B\\kei2_voice_191.wav"));
	words[96]  = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_192.wav"));
	words[97]  = Words(16,   19,   false, LoadSoundMem("voice_B\\kei2_voice_193.wav"));
	words[98]  = Words(17,   24,   false, LoadSoundMem("voice_B\\kei2_voice_194.wav"));
	words[99]  = Words(19,   23,   false, LoadSoundMem("voice_B\\kei2_voice_195.wav"));
	words[100] = Words(19,   22,   false, LoadSoundMem("voice_B\\kei2_voice_196.wav"));
	words[101] = Words(21,   23,   false, LoadSoundMem("voice_B\\kei2_voice_197.wav"));
	words[102] = Words(21,   1,    false, LoadSoundMem("voice_B\\kei2_voice_198.wav"));
	words[103] = Words(23,   2,    false, LoadSoundMem("voice_B\\kei2_voice_199.wav"));
	words[104] = Words(10,   24,   false, LoadSoundMem("voice_B\\kei2_voice_200.wav"));
	words[105] = Words(11,   23,   false, LoadSoundMem("voice_B\\kei2_voice_201.wav"));
	words[106] = Words(11,   3,    false, LoadSoundMem("voice_B\\kei2_voice_202.wav"));
	words[107] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_203.wav"));
	words[108] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_204.wav"));
	words[109] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_205.wav"));
	words[110] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_206.wav"));
	words[111] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_207.wav"));
	words[112] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_208.wav"));
	words[113] = Words(18,   24,   false, LoadSoundMem("voice_B\\kei2_voice_209.wav"));
	words[114] = Words(9,    17,   false, LoadSoundMem("voice_B\\kei2_voice_210.wav"));
	words[115] = Words(11,   1,    false, LoadSoundMem("voice_B\\kei2_voice_211.wav"));
	words[116] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_216.wav"));
	words[117] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_262.wav"));
	words[118] = Words(16,   19,   false, LoadSoundMem("voice_B\\kei2_voice_263.wav"));
	words[119] = Words(6,    24,   false, LoadSoundMem("voice_B\\kei2_voice_264.wav"));
	words[120] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_265.wav"));
	words[121] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_266.wav"));
	words[122] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_267.wav"));
	words[123] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_268.wav"));
	words[124] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_269.wav"));
	words[125] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_270.wav"));
	words[126] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_271.wav"));
	words[127] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_277.wav"));
	words[128] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_297.wav"));
	words[129] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_298.wav"));
	words[130] = Words(0,    24,   false, LoadSoundMem("voice_B\\kei2_voice_299.wav"));
	words[131] = Words(0,    24,   true,  LoadSoundMem("voice_B\\kei2_voice_302.wav"));

	hPlaying = -1;
}

void Voice::SoundTime(eTmData data, int index)
{
	if (pCfgData->GetCfg(CfgData::Sound)) {
		switch (data) {
		case Year:
			hPlaying = hYear[index];
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			break;
		case Month:
			hPlaying = hMonth[index];
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			break;
		case Day:
			hPlaying = hDay[index];
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			break;
		case Week:
			hPlaying = hWeek[index];
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			break;
		case Noon:
			hPlaying = hNoon[index];
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			break;
		case Hour:
			hPlaying = hHour[index];
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			break;
		case TimeSignal:
			hPlaying = hTime_signal[index];
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			break;
		case Minute:
			hPlaying = hMinute[index];
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			break;
		case Tail:
			hPlaying = hTail[index];
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			break;
		}
	}
}

void Voice::SoundWords()
{
	if (pCfgData->GetCfg(CfgData::Sound)) {
		// 初回起動時の最初の再生は固定
		if (pCfgData->GetCfg(CfgData::FirstTime)) {
			hPlaying = words[18].GetHandle();
			PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
			pCfgData->SetCfg(CfgData::FirstTime, false);
			return;
		}

		std::vector<int> cand;				// 再生される音声の候補

		// 候補に時刻に応じた音声を追加
		for (int i = 0; i < WORDS; i++)
			if (words[i].IsAppropriate())
				cand.push_back(words[i].GetHandle());

		// 日付に応じた音声を追加(出現確率10倍)
		if (pDate->IsNewYear())
			for (int i = 0; i < 10; i++)
				cand.push_back(hSeason[0]);
		else if (pDate->IsHalloween())
			for (int i = 0; i < 10; i++)
				cand.push_back(hSeason[1]);
		else if (pDate->IsChristmas())
			for (int i = 0; i < 10; i++)
				cand.push_back(hSeason[2]);
		else if (pDate->IsBirthday()) {
			for (int i = 0; i < 5; i++)
				cand.push_back(hSeason[3]);
			for (int i = 0; i < 5; i++)
				cand.push_back(hSeason[4]);
		}
		// 季節に応じた音声を追加
		if (pDate->IsSummer()) {
			cand.push_back(hSeason[6]);
			cand.push_back(hSeason[7]);
		}
		else if (pDate->IsWinter()) {
			cand.push_back(hSeason[5]);
			cand.push_back(hSeason[8]);
			cand.push_back(hSeason[9]);
		}

		// 時間に合ったセリフをランダムで再生
		hPlaying = cand[GetRand(cand.size() - 1)];
		PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
	}
}

void Voice::SoundSys(int index)
{
	if (pCfgData->GetCfg(CfgData::Sound)) {
		hPlaying = hSystem[index];
		PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
	}
}

void Voice::SoundKey(KeyCode key)
{
	if (pCfgData->GetCfg(CfgData::Sound)) {
		hPlaying = keyboard.at(key);
		PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
	}
}

void Voice::Repeat()
{
	if (pCfgData->GetCfg(CfgData::Sound))
		PlaySoundMem(hPlaying, DX_PLAYTYPE_BACK);
}

void Voice::StopSound()
{
	StopSoundMem(hPlaying);
}

Words::Words(int minh, int maxh, bool prg, int hw)
{
	min_hour = minh;
	max_hour = maxh;
	min_minute = 0;
	max_minute = 0;
	programmer = prg;
	kurei_kei = false;
	hWord = hw;
}

Words::Words(double minh, double maxh, bool prg, int hw)
{
	min_hour = (int)minh;
	max_hour = (int)maxh;
	min_minute = (int)((minh - min_hour) * MINUTE);
	max_minute = (int)((maxh - max_hour) * MINUTE);
	programmer = prg;
	kurei_kei = false;
	hWord = hw;
}

Words::Words(int minh, int maxh, bool prg, bool k, int hw)
{
	min_hour = minh;
	max_hour = maxh;
	min_minute = 0;
	max_minute = 0;
	programmer = prg;
	kurei_kei = k;
	hWord = hw;
}

bool Words::IsAppropriate()
{
	int h = pTime->Geth();
	int m = pTime->Getm();

	/*
	   キャラが暮井慧か、また、プログラマ用かの判定
	   words[44]とwords[46]はプログラマ用だが、キャラが暮井慧の時は候補に含める
	   条件はカルノー図より求めた
	*/
	if (kurei_kei && pCfgData->GetCfg(CfgData::ChrName) != CfgData::KureiKei) {
		if(!programmer)
			return false;
		if (!pCfgData->GetCfg(CfgData::ForPrg))
			return false;
	}
	if (!kurei_kei && programmer && !pCfgData->GetCfg(CfgData::ForPrg))
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
