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

