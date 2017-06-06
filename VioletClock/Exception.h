#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
using std::string;

class Exception {
public:
    enum eErrorCode {
        Unexpected,         // 予期せぬエラー
        FailToAlloc,        // メモリ割り当て失敗
        LoadingNotDone,     // ロードの中断
        FuncError,          // 関数エラー
        InputError,         // 入力エラー
        OutputError,        // 出力エラー
        OtherError          // その他のエラー
    };
private:
    eErrorCode error_code;
    int error_val;
    string error_str;
public:
    Exception(eErrorCode code);
    Exception(eErrorCode code, unsigned int val);
    Exception(eErrorCode code, string str);
    Exception(eErrorCode code, unsigned int val, string str);
    void ExportError();         // エラー情報をファイルに記録する
};

#endif // !EXCEPTION_H
