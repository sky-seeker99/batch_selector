/*

 * Copyright (c) 2003 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
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

#ifndef editorH
#define editorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TMemo *memo;
	TFontDialog *font_diag;
	TColorDialog *color_diag;
	TPopupMenu *PopupMenu1;
	TMenuItem *undo1;
	TMenuItem *paste1;
	TMenuItem *copy1;
	TMenuItem *cut1;
	TMenuItem *selectall1;
	TMenuItem *clearall1;
	TMenuItem *font1;
	TMenuItem *backcolor1;
	void __fastcall backcolor1Click(TObject *Sender);
	void __fastcall selectall1Click(TObject *Sender);
	void __fastcall clearall1Click(TObject *Sender);
	void __fastcall copy1Click(TObject *Sender);
	void __fastcall cut1Click(TObject *Sender);
	void __fastcall undo1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall paste1Click(TObject *Sender);
	void __fastcall font1Click(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TForm2(TComponent* Owner);
	__fastcall ~TForm2();
	void __fastcall WMDropFiles(TWMDropFiles & Msg);
	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
	END_MESSAGE_MAP(TForm)


};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
