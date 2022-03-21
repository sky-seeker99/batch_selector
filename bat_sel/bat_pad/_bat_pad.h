/*

 * Copyright (c) 2008 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
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
