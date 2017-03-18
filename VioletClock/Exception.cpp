#include "Exception.h"
#include <fstream>
#include <vector>

// エラー出力ファイル
std::ofstream ferr("error.log");

Exception::Exception(eErrorCode code)
{
    error_code = code;
    error_val = -1;
}

Exception::Exception(eErrorCode code, unsigned int val)
{
    error_code = code;
    error_val = val;
}

Exception::Exception(eErrorCode code, string str)
{
    error_code = code;
    error_val = -1;
    error_str = str;
}

Exception::Exception(eErrorCode code, unsigned int val, string str)
{
    error_code = code;
    error_val = val;
    error_str = str;
}

void Exception::ExportError()
{
    if (!ferr)
        exit(1);

    static const std::vector<string> error_statement = {
        "予期せぬエラー",
        "メモリ割り当て失敗",
        "ロードの中断",
        "関数エラー",
        "入力エラー",
        "出力エラー"
        "その他のエラー"
    };

    ferr << error_statement[error_code];
    if (error_val >= 0)
        ferr << '(' << error_val << ')';
    if (!error_str.empty())
        ferr << '(' << error_str << ')';
    ferr << std::endl;
}
