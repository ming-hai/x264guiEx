﻿//  -----------------------------------------------------------------------------------------
//    拡張 x264 出力(GUI) Ex  v1.xx by rigaya
//  -----------------------------------------------------------------------------------------
//   ソースコードについて
//   ・無保証です。
//   ・本ソースコードを使用したことによるいかなる損害・トラブルについてrigayaは責任を負いません。
//   以上に了解して頂ける場合、本ソースコードの使用、複製、改変、再頒布を行って頂いて構いません。
//  -----------------------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include "auo_frm.h"
#include "auo_util.h"

const int NEW_LINE_THRESHOLD = 125;
const int MAKE_NEW_LINE_THRESHOLD = 140;

static inline int check_log_type(char *mes) {
	if (strstr(mes, "warning")) return LOG_WARNING;
	if (strstr(mes, "error")) return LOG_ERROR;
	return LOG_INFO;
}

//長すぎたら適当に折り返す
static int write_log_x264_mes_line(char *const mes) {
	const int mes_len = strlen(mes);
	const int mes_type = check_log_type(mes);
	char *const fin = mes + mes_len;
	char *const prefix_ptr = strstr(mes, "]: ");
	const int prefix_len = (prefix_ptr) ? prefix_ptr - mes + strlen("]: ") : 0;
	char *p = mes, *q = NULL;
	BOOL flag_continue = FALSE;
	do {
		const int threshold = NEW_LINE_THRESHOLD - (p != mes) * prefix_len;
		flag_continue = mes_len >= MAKE_NEW_LINE_THRESHOLD
			&& (p + threshold) < fin
			&& (q = strrchr(p, ' ', threshold)) != NULL;
		if (flag_continue) *q = '\0';
		if (p != mes)
			for (char *const prefix_adjust = p - prefix_len; p > prefix_adjust; p--)
				*(p-1) = ' ';
		write_log_line(mes_type, p);
		p=q+1;
	} while (flag_continue);
	return mes_len;
}

void write_log_x264_mes(char *const msg, DWORD *log_len, int total_drop, int current_frames) {
	char *a, *b, *mes = msg;
	char * const fin = mes + *log_len; //null文字の位置
	*fin = '\0';
	while ((a = strchr(mes, '\n')) != NULL) {
		if ((b = strrchr(mes, '\r', a - mes - 2)) != NULL)
			mes = b + 1;
		*a = '\0';
		write_log_x264_mes_line(mes);
		mes = a + 1;
	}
	if ((a = strrchr(mes, '\r', fin - mes - 1)) != NULL) {
		b = a - 1;
		while (*b == ' ' || *b == '\r')
			b--;
		*(b+1) = '\0';
		if ((b = strrchr(mes, '\r', b - mes - 2)) != NULL)
			mes = b + 1;
		set_window_title_x264_mes(mes, total_drop, current_frames);
		mes = a + 1;
	}
	if (mes == msg && *log_len)
		mes += write_log_x264_mes_line(mes);
	memmove(msg, mes, ((*log_len = fin - mes) + 1) * sizeof(msg[0]));
}

void write_args(const char *args) {
	size_t len = strlen(args);
	char *const c = (char *)malloc((len+1)*sizeof(c[0]));
	char *const fin = c + len;
	memcpy(c, args, (len+1)*sizeof(c[0]));
	char *p = c;
	for (char *q = NULL; p + NEW_LINE_THRESHOLD < fin && (q = strrchr(p, ' ', NEW_LINE_THRESHOLD)) != NULL; p = q+1) {
		*q = '\0';
		write_log_line(LOG_INFO, p);
	}
	write_log_line(LOG_INFO, p);
	free(c);
}
