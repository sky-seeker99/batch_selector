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
