#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
using std::string;

class Exception {
public:
    enum eErrorCode {
        Unexpected,         // �\�����ʃG���[
        FailToAlloc,        // ���������蓖�Ď��s
        LoadingNotDone,     // ���[�h�̒��f
        FuncError,          // �֐��G���[
        InputError,         // ���̓G���[
        OutputError,        // �o�̓G���[
        OtherError          // ���̑��̃G���[
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
    void ExportError();         // �G���[�����t�@�C���ɋL�^����
};

#endif // !EXCEPTION_H
