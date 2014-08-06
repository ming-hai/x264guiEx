﻿//  -----------------------------------------------------------------------------------------
//    拡張 x264 出力(GUI) Ex  v1.xx by rigaya
//  -----------------------------------------------------------------------------------------
//   ソースコードについて
//   ・無保証です。
//   ・本ソースコードを使用したことによるいかなる損害・トラブルについてrigayaは責任を負いません。
//   以上に了解して頂ける場合、本ソースコードの使用、複製、改変、再頒布を行って頂いて構いません。
//  -----------------------------------------------------------------------------------------

#ifndef _AUO_UTIL_H_
#define _AUO_UTIL_H_

#include <Windows.h>
#include <string.h>
#include <stddef.h>

//日本語環境の一般的なコードページ一覧
static const DWORD CODE_PAGE_SJIS        = 932; //Shift-JIS
static const DWORD CODE_PAGE_JIS         = 50220;
static const DWORD CODE_PAGE_EUC_JP      = 51932;
static const DWORD CODE_PAGE_UTF8        = CP_UTF8;
static const DWORD CODE_PAGE_UTF16_LE    = CP_WINUNICODE; //WindowsのUnicode WCHAR のコードページ
static const DWORD CODE_PAGE_UTF16_BE    = 1201;
static const DWORD CODE_PAGE_US_ASCII    = 20127;
static const DWORD CODE_PAGE_WEST_EUROPE = 1252;  //厄介な西ヨーロッパ言語
static const DWORD CODE_PAGE_UNSET       = 0xffffffff;

//BOM文字リスト
static const BYTE UTF8_BOM[]     = { 0xEF, 0xBB, 0xBF };
static const BYTE UTF16_LE_BOM[] = { 0xFF, 0xFE };
static const BYTE UTF16_BE_BOM[] = { 0xFE, 0xFF };

//関数マクロ
#define clamp(x, low, high) (((x) <= (high)) ? (((x) >= (low)) ? (x) : (low)) : (high))
#define foreach(type,it,a) \
    for (type::iterator (it)=(a)->begin();(it)!=(a)->end();(it)++)
#define const_foreach(type,it,a) \
    for (type::const_iterator (it)=(a)->begin();(it)!=(a)->end();(it)++)

//基本的な関数
static inline double pow2(double a) {
	return a * a;
}
static inline int pow2(int a) {
	return a * a;
}
static inline int ceil_div_int(int i, int div) {
	return (i + (div-1)) / div;
}
static inline __int64 ceil_div_int64(__int64 i, int div) {
	return (i + (div-1)) / div;
}

//大文字小文字を無視して、1文字検索
static const char *strichr(const char *str, int c) {
	c = tolower(c);
	for (; *str; str++)
		if (c == tolower(*str))
			return str;
	return NULL;
}
static char *strichr(char *str, int c) {
	c = tolower(c);
	for (; *str; str++)
		if (c == tolower(*str))
			return str;
	return NULL;
}

//大文字小文字を無視して、文字列を検索
static const char *stristr(const char *str, const char *substr) {
	int len = 0;
	if (substr && (len = strlen(substr)) != NULL)
		for (; (str = strichr(str, substr[0])) != NULL; str++)
			if (_strnicmp(str, substr, len) == NULL)
				return str;
	return NULL;
}
static char *stristr(char *str, const char *substr) {
	int len = 0;
	if (substr && (len = strlen(substr)) != NULL)
		for (; (str = strichr(str, substr[0])) != NULL; str++)
			if (_strnicmp(str, substr, len) == NULL)
				return str;
	return NULL;
}

//指定した場所から後ろ向きに1文字検索
static const char *strrchr(const char *str, int c, int start_index) {
	if (start_index < 0) return NULL;
	const char *result = str + start_index;
	str--;
	for (; result - str; result--)
		if (*result == c)
			return result;
	return NULL;
}
static char *strrchr(char *str, int c, int start_index) {
	if (start_index < 0) return NULL;
	char *result = str + start_index;
	str--;
	for (; result - str; result--)
		if (*result == c)
			return result;
	return NULL;
}

static int countchr(const char *str, int ch) {
	int i = 0;
	for (; *str; str++)
		if (*str == ch)
			i++;
	return i;
}

//文字列の末尾についている '\r' '\n' ' ' を削除する
static size_t deleteCRLFSpace_at_End(WCHAR *str) {
	WCHAR *pw = str + wcslen(str) - 1;
	WCHAR * const qw = pw;
	while ((*pw == L'\n' || *pw == L'\r' || *pw == L' ') && pw >= str) {
		*pw = L'\0';
		pw--;
	}
	return qw - pw;
}

static size_t deleteCRLFSpace_at_End(char *str) {
	char *pw = str + strlen(str) - 1;
	char *qw = pw;
	while ((*pw == '\n' || *pw == '\r' || *pw == ' ') && pw >= str) {
		*pw = '\0';
		pw--;
	}
	return qw - pw;
}

DWORD cpu_core_count();
BOOL check_sse2();
BOOL check_sse3();
BOOL check_ssse3();
BOOL check_sse4_1();
BOOL check_OS_Win7orLater();
void get_auo_path(char *auo_path, size_t nSize);
void get_aviutl_dir(char *aviutl_dir, size_t nSize);
size_t calc_replace_mem_required(char *str, const char *old_str, const char *new_str);
int replace(char *str, size_t nSize, const char *old_str, const char *new_str);
void apply_appendix(char *new_filename, size_t new_filename_size, const char *orig_filename, const char *appendix);
BOOL check_ext(const char *filename, const char *ext);
BOOL PathGetRoot(const char *path, char *root, size_t nSize);
BOOL PathRootExists(const char *path);
BOOL DirectoryExistsOrCreate(const char *dir);
BOOL FileExistsAndHasSize(const char *path);
void PathGetDirectory(char *dir, size_t nSize, const char *path);
BOOL GetFileSizeInt(const char *filepath, DWORD *filesize);
BOOL GetFileSizeInt64(const char *filepath, __int64 *filesize);
__int64 GetFileLastUpdate(const char *filename);
int append_str(char **dst, size_t *nSize, const char *append);
BOOL PathAddBackSlashLong(char *dir);
BOOL PathCombineLong(char *path, size_t nSize, const char *dir, const char *filename);
BOOL GetPathRootFreeSpace(const char *path, __int64 *freespace);
BOOL PathForceRemoveBackSlash(char *path);
BOOL check_process_exitcode(PROCESS_INFORMATION *pi);

//ファイル名をスワップする
BOOL swap_file(const char *fileA, const char *fileB);

//文字列先頭がBOM文字でないか確認する
DWORD check_bom(const void* chr);

//与えられた文字列から主に日本語について文字コード判定を行う
DWORD get_code_page(const void *str, DWORD size_in_byte);

//IMultipleLanguge2 の DetectInoutCodePageがたまに的外れな「西ヨーロッパ言語」を返すので
//西ヨーロッパ言語 なら Shift-JIS にしてしまう
BOOL fix_ImulL_WesternEurope(UINT *code_page);

#endif //_AUO_UTIL_H_