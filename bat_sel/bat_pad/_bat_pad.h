/*
 * Copyright 2026 Shigeru Kasuya
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by me or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//---------------------------------------------------------------------------

#ifndef _bat_padH
#define _bat_padH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tpad_Form1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TPopupMenu *PopupMenu1;
	TMenuItem *Quit1;
	TFontDialog *font_diag;
	TColorDialog *color_diag;
	TLabel *label;
	TMenuItem *BackColor1;
	TMenuItem *Font1;
	TMenuItem *stay;
	TMenuItem *para;
	TMenuItem *shell;
	TMenuItem *view1;
	TMenuItem *label_c;
	TMenuItem *exec1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Quit1Click(TObject *Sender);
	void __fastcall Font1Click(TObject *Sender);
	void __fastcall BackColor1Click(TObject *Sender);
	void __fastcall stayClick(TObject *Sender);
	void __fastcall paraClick(TObject *Sender);
	void __fastcall shellClick(TObject *Sender);
	void __fastcall view1Click(TObject *Sender);
	void __fastcall FormDblClick(TObject *Sender);
	void __fastcall label_cClick(TObject *Sender);
	void __fastcall labelDblClick(TObject *Sender);
	void __fastcall exec1Click(TObject *Sender);
private:	// ユーザー宣言
	int Fmode;	// ユーザー宣言
	sChar *para_file;
	sChar *list_file;
	ShellExec_T *sh;
    sCharEX *csv_str;

public:		// ユーザー宣言
    int g_exit;
	__fastcall Tpad_Form1(TComponent* Owner,char *conf_file);
	__fastcall ~Tpad_Form1();
	void confFileRead();
	void confFileWrite();
	void fileReadToMemo();
	void CsvWrite(FILE *out_fp,char *str);
	void __fastcall WMDropFiles(TWMDropFiles & Msg);
	void __fastcall WMLButtonDown(TWMLButtonDown & Msg);
	virtual void __fastcall CreateParams(TCreateParams& Params);
	__property int mode  = { read = Fmode, write = Fmode, default = 0 };


	// Windows 終了時のルーチン

	void __fastcall	WMQueryEndSession(TWMQueryEndSession &WMQES);	//これがイベントハンドラになる



protected:
	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
		VCL_MESSAGE_HANDLER(WM_LBUTTONDOWN, TWMLButtonDown, WMLButtonDown)
		MESSAGE_HANDLER(WM_QUERYENDSESSION,TWMQueryEndSession,WMQueryEndSession)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE Tpad_Form1 *pad_Form1;
//---------------------------------------------------------------------------
#endif
