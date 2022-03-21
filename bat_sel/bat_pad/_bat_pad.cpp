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
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <process.h>
#include <stdio.h>

#include "..\csvpp_xbase\xcsvi_anl.h"

#define VERSION "Bat Pad for Bat-Selector Ver 0.93"


#include "..\bat_sel\shell_exec.h"
#include "..\bat_sel\file_ctrl.h"
#include "..\bat_sel\_bat_sel.h"

#include "_bat_pad.h"
#include "_edit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tpad_Form1 *pad_Form1;
extern Tpad_Form2 *pad_Form2;

//---------------------------------------------------------------------------
typedef Set<TMsgDlgBtn, mbYes, mbHelp> Ok_T;
__fastcall Tpad_Form1::Tpad_Form1(TComponent* Owner,char *conf_file)
	: TForm(Owner)
{
  pad_Form1->Width = 30;
  pad_Form1->Height = 30;
  DragAcceptFiles(Handle, true);
  AnsiString cmd;
  para_file = NULL;
  list_file = new sChar("");

/*
  int cnt = ParamCount();

  if (cnt >= 1) {
	cmd = ParamStr(1);
	para_file = new sChar(cmd.c_str());
  }
*/
  para_file = new sChar(conf_file);
  sh  = new ShellExec_T();
  confFileRead();
  Hint = label->Caption;
//  Hint = cmd;
  g_exit = 0;
  csv_str = new sCharEX("");

}

__fastcall Tpad_Form1::~Tpad_Form1(){
  if (para_file != NULL){delete para_file;}
  delete sh;
  confFileWrite();
  delete list_file;
  delete csv_str;
}

void __fastcall	Tpad_Form1::WMQueryEndSession(TWMQueryEndSession &WMQES)
{
//ここにＷindows終了直前にやりたいことを書く
//最後に、Ｗindowsが終了してもいいかを指定

confFileWrite();

	WMQES.Result = TRUE;	//Ｗindowsに「終了してもいいよ」と伝える
//	WMQES.Result = FALSE;	//Ｗindowsの終了を禁止する
}

//---------------------------------------------------------------------------
void __fastcall Tpad_Form1::WMDropFiles(TWMDropFiles & Msg)
{
  char Buff[MAX_PATH]; //ファイル名
  int Count; //Dropされたファイル数

  fileReadToMemo();
  sh->listDelete();

  Count = DragQueryFile((void*)Msg.Drop, -1, NULL, NULL ); //ファイルの数を得る
  for (int i = 0; i < Count; i++) {  //ファイルの数だけ繰り返す
	DragQueryFile((void*)Msg.Drop, i, Buff, sizeof(Buff)); //i番目のファイル名をBuffに格納
    sh->fileStore(Buff);
  }

  sh->batFileGen(pad_Form2->memo->Lines);
  sh->batExec();

  DragFinish((void*)Msg.Drop); //終了処理
  TForm::Dispatch(&Msg);

}
void __fastcall Tpad_Form1::WMLButtonDown(TWMLButtonDown & Msg)
{
	//TODO この下にコードを追加してください
	SendMessage(Handle,WM_SYSCOMMAND,SC_MOVE | 2,0);
}

void __fastcall Tpad_Form1::CreateParams(TCreateParams& Params)
{
	//TODO この下にコードを追加してください
TForm::CreateParams(Params);
Params.Style |= WS_THICKFRAME;
}

void __fastcall Tpad_Form1::FormResize(TObject *Sender)
{
  if (pad_Form1->Width < 30) {pad_Form1->Width = 30;}
  if (pad_Form1->Height < 30) {pad_Form1->Height = 30;}


}
//---------------------------------------------------------------------------
void __fastcall Tpad_Form1::Quit1Click(TObject *Sender)
{
g_exit = 1;
Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall Tpad_Form1::Font1Click(TObject *Sender)
{
font_diag->Font = label->Font;
if (font_diag->Execute() == false){return;}
label->Font = font_diag->Font;
Visible = false;
Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall Tpad_Form1::BackColor1Click(TObject *Sender)
{
color_diag->Color = pad_Form1->Color;
if (color_diag->Execute() == false){return;}
Color = color_diag->Color;
label->Color = color_diag->Color;
Visible = false;
Visible = true;
}
//---------------------------------------------------------------------------
void Tpad_Form1::confFileRead() {
  if (para_file == NULL) {return;}

  Read64_C *in_fp = new  Read64_C(para_file->c_str(),"rt",1024);
  if (in_fp->okCheck() == false) {
    delete in_fp;
    return;
  }

  CsvAnl_C *csvi = new CsvAnl_C;
  TEXT_READ_LOOP(in_fp,buff)
    csvi->Exec(buff);
    char *str   = NULL;
    char *w_str = NULL;
    CSV_LOOP_(csvi,w,true)
      if      (str   == NULL) {str = w;}
      else if (w_str == NULL) {w_str = w;}
      break;
    LOOP_END
    if (w_str == NULL) {continue;}
    if (strcmp(str,"#height"    ) == 0){Height       = SujiConvEx(w_str); continue;}
    if (strcmp(str,"#width"     ) == 0){Width        = SujiConvEx(w_str); continue;}
    if (strcmp(str,"#top"       ) == 0){Top          = SujiConvEx(w_str); continue;}
    if (strcmp(str,"#left"      ) == 0){Left         = SujiConvEx(w_str); continue;}
    if (strcmp(str,"#font_size" ) == 0){
      label->Font->Size = SujiConvEx(w_str);
      continue;
	}

	if (strcmp(str,"#label" ) == 0){
	  label->Caption = w_str;
	  continue;
	}
	if (strcmp(str,"#list" ) == 0){
	  list_file->set(w_str);
	  continue;
	}
	if (strcmp(str,"#back_color" ) == 0){
      int ii = SujiConvEx(w_str);
      if (ii < 0){ii = 16777215;}
	  Color = (TColor)ii;
      continue;
    }
	if (strcmp(str,"#font_color" ) == 0){
      int ii = SujiConvEx(w_str);
      if (ii < 0){ii = 0;}
	  label->Font->Color = (TColor)ii;
	  continue;
    }
	if (strcmp(str,"#font_name") == 0){
	  label->Font->Name = w_str;
      continue;
    }
	if (strcmp(str,"#stay"      ) == 0){
      if (strcmp(w_str,"0") == 0){stay->Checked = false; FormStyle = fsNormal;}
      else                       {stay->Checked = true;  FormStyle = fsStayOnTop;}
      continue;
      }
	if (strcmp(str,"#para"      ) == 0){
      if (strcmp(w_str,"0") == 0){para->Checked = false; sh->paraFlg = false;}
	  else                       {para->Checked = true;  sh->paraFlg = true;}
	  continue;
	}
	if (strcmp(str,"#shell"      ) == 0){
	  if (strcmp(w_str,"0") == 0){shell->Checked = false; sh->shellFlg = false;}
	  else                       {shell->Checked = true;  sh->shellFlg = true;}
	  continue;
	}
    if (strcmp(str,"#label_c"      ) == 0){
      if (strcmp(w_str,"0") == 0){label->Visible = false; label_c->Checked = false;}
	  else                       {label->Visible = true;  label_c->Checked = true;}
	  continue;
	}
  LOOP_END
  DragAcceptFiles(Handle, true);
  delete csvi;
  delete in_fp;

}

void Tpad_Form1::confFileWrite(){
  FILE *fp;
  if (para_file == NULL) {return;}

  fp = fopen(para_file->c_str(),"wt");

  fprintf(fp,"#exit,%d\n",g_exit);
  fprintf(fp,"#label,%s\n",label->Caption.c_str());
  fprintf(fp,"#height,%d\n",Height);
  fprintf(fp,"#width,%d\n",Width);
  fprintf(fp,"#top,%d\n",Top);
  fprintf(fp,"#left,%d\n",Left);
  fprintf(fp,"#shell,%d\n",shell->Checked );
  fprintf(fp,"#para,%d\n",para->Checked );
  fprintf(fp,"#back_color,%d\n",Color );
  fprintf(fp,"#font_color,%d\n",label->Font->Color );
  fprintf(fp,"#font_style,%d\n",label->Font->Style );
  fprintf(fp,"#font_size,%d\n",label->Font->Size );
  fprintf(fp,"#font_name,%s\n",label->Font->Name.c_str());
  if (stay->Checked){fprintf(fp,"#stay,1\n");}
  else              {fprintf(fp,"#stay,0\n");}
  if (para->Checked){fprintf(fp,"#para,1\n");}
  else              {fprintf(fp,"#para,0\n");}
  if (shell->Checked){fprintf(fp,"#shell,1\n");}
  else               {fprintf(fp,"#shell,0\n");}
  if (label_c->Checked){fprintf(fp,"#label_c,1\n");}
  else               {fprintf(fp,"#label_c,0\n");}
  fprintf(fp,"#list,");
  CsvWrite(fp,list_file->c_str());
  fprintf(fp,"\n");

  //  fprintf(fp,"#list,\"%s\"\n",list_file);
  fclose(fp);
}



void __fastcall Tpad_Form1::stayClick(TObject *Sender)
{
 if (stay->Checked){stay->Checked = false; FormStyle = fsNormal;}
 else              {stay->Checked = true;  FormStyle = fsStayOnTop;}
 DragAcceptFiles(Handle, true);	
}
//---------------------------------------------------------------------------

void __fastcall Tpad_Form1::paraClick(TObject *Sender)
{
if (sh->paraFlg){sh->paraFlg = false; para->Checked = false;}
else            {sh->paraFlg = true;  para->Checked = true;}
}
//---------------------------------------------------------------------------

void __fastcall Tpad_Form1::shellClick(TObject *Sender)
{
if (sh->shellFlg){sh->shellFlg = false; shell->Checked = false;}
else             {sh->shellFlg = true;  shell->Checked = true;}	
}
//---------------------------------------------------------------------------
// --------------- CSV WRITE ------------------
void Tpad_Form1::CsvWrite(FILE *out_fp,char *str) {
  csv_str->DblIns(str);
  fprintf(out_fp,"%s",csv_str->c_str());
}

void Tpad_Form1::fileReadToMemo(){

  Read64_C *in_fp = new  Read64_C(list_file->c_str(),"rt",1024);
  if (in_fp->okCheck() == false) {
    delete in_fp;
    return;
  }

  pad_Form2->memo->Visible = false;
  pad_Form2->memo->Lines->Clear();

  CsvAnl_C *csvi = new CsvAnl_C;
  TEXT_READ_LOOP(in_fp,buff)
    csvi->Exec(buff);
    char *str   = NULL;
    char *w_str = NULL;
    CSV_LOOP_(csvi,w,true)
      if      (str   == NULL) {str = w;}
      else if (w_str == NULL) {w_str = w;}
      break;
    LOOP_END
    if (w_str == NULL) {continue;}
	if (strcmp(str,"#context") == 0){
	  if (w_str == NULL){pad_Form2->memo->Lines->Add("");}
	  else              {pad_Form2->memo->Lines->Add(w_str);}
	  continue;
	}
  LOOP_END
  pad_Form2->memo->Visible = true;
  delete in_fp;
  delete csvi;
}


void __fastcall Tpad_Form1::view1Click(TObject *Sender)
{
fileReadToMemo();
pad_Form2->Visible = true;
}
//---------------------------------------------------------------------------


void __fastcall Tpad_Form1::FormDblClick(TObject *Sender)
{
  sh->listDelete();
  fileReadToMemo();
  sh->batFileGen(pad_Form2->memo->Lines);
  sh->batExec();
}
//---------------------------------------------------------------------------

void __fastcall Tpad_Form1::label_cClick(TObject *Sender)
{
if (label_c->Checked){label->Visible = false; label_c->Checked = false;}
else                 {label->Visible = true;  label_c->Checked = true;}

}
//---------------------------------------------------------------------------




void __fastcall Tpad_Form1::labelDblClick(TObject *Sender)
{
  sh->listDelete();
  fileReadToMemo();
  sh->batFileGen(pad_Form2->memo->Lines);
  sh->batExec();	
}
//---------------------------------------------------------------------------

void __fastcall Tpad_Form1::exec1Click(TObject *Sender)
{
  sh->listDelete();
  fileReadToMemo();
  sh->batFileGen(pad_Form2->memo->Lines);
  sh->batExec();	
}
//---------------------------------------------------------------------------

