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

#include <vcl.h>
#pragma hdrstop

#include <process.h>
#include <stdio.h>


#include "../csvpp_xbase/xcsvi.h"
#include "../csvpp_xbase/xcsvi_anl.h"

#include "file_ctrl.h"
#include "_bat_sel.h"
#include "editor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
extern TForm1 *Form1;


//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
__fastcall TForm2::~TForm2(){

}

//---------------------------------------------------------------------------
void __fastcall TForm2::WMDropFiles(TWMDropFiles & Msg)
{
  char Buff[MAX_PATH]; //ファイル名
  char Buff_dbl[MAX_PATH+2]; //ファイル名
  int Count; //Dropされたファイル数

  Count = DragQueryFile((void*)Msg.Drop, -1, NULL, NULL ); //ファイルの数を得る
  for (int i = 0; i < Count; i++) {  //ファイルの数だけ繰り返す
	DragQueryFile((void*)Msg.Drop, i, Buff, sizeof(Buff)); //i番目のファイル名をBuffに格納

	bool sp=false;
	int len=strlen(Buff);
	for(int i=0;i<len;i++){
	  if (*(Buff+i)==' ') {sp=true; break;}
	}
	if (sp) {
	  Buff_dbl[0]='\"';
	  int i;
	  for(i=0;i<len;i++){
		Buff_dbl[i+1] = Buff[i];
	  }
	  Buff_dbl[i+1]='\"';
	  Buff_dbl[i+2]=0x00;
	  memo->Lines->Add(Buff_dbl);
	}else{
	  memo->Lines->Add(Buff);
	}
  }

  DragQueryFile((void*)Msg.Drop, 0, Buff, sizeof(Buff));
  DragFinish((void*)Msg.Drop); //終了処理
  TForm::Dispatch(&Msg);
}
void __fastcall TForm2::backcolor1Click(TObject *Sender)
{
color_diag->Color = memo->Color;
if (color_diag->Execute() == false){return;}
memo->Color = color_diag->Color;
Visible=false;
Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::selectall1Click(TObject *Sender)
{
memo->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::clearall1Click(TObject *Sender)
{
memo->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::copy1Click(TObject *Sender)
{
memo->CopyToClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::cut1Click(TObject *Sender)
{
memo->CutToClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::undo1Click(TObject *Sender)
{
memo->Undo();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
//contextWrite();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::paste1Click(TObject *Sender)
{
memo->PasteFromClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::font1Click(TObject *Sender)
{
font_diag->Font = memo->Font;
if (font_diag->Execute() == false){return;}
memo->Font = font_diag->Font;
Visible=false;
Visible=true;
}
//---------------------------------------------------------------------------

