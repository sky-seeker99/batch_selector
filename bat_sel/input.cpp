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

#include <vcl.h>
#pragma hdrstop

#include <process.h>
#include <stdio.h>


#include "file_ctrl.h"
#include "_bat_sel.h"
#include "editor.h"
#include "input.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
extern TForm1 *Form1;
extern TForm2 *Form2;

//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button1Click(TObject *Sender)
{
Store();
Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button2Click(TObject *Sender)
{
Visible = false;
}
//---------------------------------------------------------------------------

void  TForm3::Store(){
  if (commandKbn == 0){
    if (Form1->list->ItemIndex >= 0){
   	  Form1->list->Items->Insert(Form1->list->ItemIndex,in->Text);
    } else {
	  Form1->list->Items->Add(in->Text);
    }
  } else {
    if (Form1->list->ItemIndex >= 0){
	  Form1->list->Items->Strings[Form1->list->ItemIndex] = in->Text;
	  Form1->contextRename(in->Text.c_str());
    }
  }

}


void __fastcall TForm3::inKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if (Key==VK_RETURN) {
    Store();
    Key=0x00;
    Visible = false;
  }
  if (Key==VK_ESCAPE) {
    Key=0x00;
    Visible = false;
  }
}
//---------------------------------------------------------------------------

