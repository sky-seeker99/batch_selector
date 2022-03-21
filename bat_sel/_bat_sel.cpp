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
#include <dir.h>



#define VERSION "Bat selector Version 1.21"


#include "input.h"
#include "editor.h"
#include "job_unit.h"
#include "shell_exec.h"
#include "file_ctrl.h"

#include "_bat_sel.h"

#include "bat_pad\_bat_pad.h"

// batch pad list
class TpadList
{
public:
  sChar *pad_name;
#ifndef EXTPAD
  Tpad_Form1 *pad;
#endif
  bool execFlg;
  TpadList(char *p){
    pad_name = new sChar(p);
    execFlg = true;
  }
  ~TpadList(){delete pad_name;}
};


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
extern TForm3 *Form3;
extern TForm4 *Form4;
ShellExec_T *sh;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
DragAcceptFiles(Handle, true);
AnsiString cmd;
FileNameSep_T *fn;
g_Visible = true;
config_file = new sChar("");
own_dir     = new sChar("");
pad_file    = new sChar("");
inh_name    = NULL;
ownKillFlag = false;

group       = new sChar("default");
sh  = new ShellExec_T();
fn  = new FileNameSep_T();
cmd = ParamStr(0);
fn->Set(cmd.c_str());
config_file->cat(fn->DrvGet());
config_file->cat(fn->DirGet());
config_file->cat("bat_sel.csv");
pad_file->cat(fn->DrvGet());
pad_file->cat(fn->DirGet());
pad_file->cat("bat_pad.exe");

own_dir->cat(fn->DrvGet());
own_dir->cat(fn->DirGet());

f_ctl = new file_ctrl_C(own_dir->c_str());

csvi = new CsvAnl_C;
job_num = 0;
it = new MemHeader_T;
pad_fp = new MEM_FP(it);
pad_init_fp = new MEM_FP(it);

if (confFileRead_old_chk())
  {
  sChar *old_file = new sChar(own_dir->c_str());
  old_file->cat("bat_sel_bak.csv");
  sprintf(cmdLine,"旧バージョンのコンフィグファイル(%s)を%sにコピーします", config_file->c_str(),old_file->c_str());
  MessageDlg(cmdLine, mtInformation ,TMsgDlgButtons() << mbOK, 0);
  AnsiString w_old,w_new;
  w_old = config_file->c_str();
  w_new = old_file->c_str();

  RenameFile(w_old,w_new);
  delete old_file;

  }
else {
  confFileRead();
  }
TrayMessage(NIM_ADD);
TrayMessage(NIM_MODIFY);
// -- memo ----
sv_file = new sChar("");
memoChg = false;
memo_dd_flg = false;
edit_read();


delete fn;
}
//---------------------------------------------------------------------------
__fastcall TForm1::~TForm1()
{
contextWrite();
confFileWrite();

delete sv_file;

delete config_file;
delete group;
delete own_dir;
delete pad_file;
delete sh;
delete f_ctl;
delete csvi;

if (inh_name != NULL){delete inh_name;}

sChar *str;
MEM_LOOP(str,sChar,pad_init_fp) delete str; LOOP_END

TpadList *pad;
MEM_LOOP(pad,TpadList,pad_fp) delete pad; LOOP_END

delete pad_fp;
delete pad_init_fp;
delete it;

TrayMessage(NIM_DELETE);
}


void __fastcall TForm1::WMQueryEndSession(TWMQueryEndSession &WMQES)
{
//ここにＷindows終了直前にやりたいことを書く
//最後に、Ｗindowsが終了してもいいかを指定

confFileWrite();
sh->temp->removeAllFile();
/*
delete config_file;
delete group;
delete own_dir;
delete pad_file;
delete sh;
delete f_ctl;

sChar *str;
MEM_LOOP(str,sChar,pad_fp) delete str; LOOP_END
delete pad_fp;
delete it;
*/
//TrayMessage(NIM_DELETE);

    WMQES.Result = TRUE;    //Ｗindowsに「終了してもいいよ」と伝える
//  WMQES.Result = FALSE;   //Ｗindowsの終了を禁止する
}


//---------------------------------------------------------------------------
void __fastcall TForm1::WMDropFiles(TWMDropFiles & Msg)
{
  char Buff[MAX_PATH]; //ファイル名
  int Count; //Dropされたファイル数
  bool sv_para;
  Count = DragQueryFile((void*)Msg.Drop, -1, NULL, NULL ); //ファイルの数を得る

  POINT point;
  DragQueryPoint((HDROP)Msg.Drop, &point);

//  if (sp1->Visible) {
  if (point.x > list->Width) {
    for (int i = 0; i < Count; i++) {  //ファイルの数だけ繰り返す
      DragQueryFile((void*)Msg.Drop, i, Buff, sizeof(Buff)); //i番目のファイル名をBuffに格納
      memo->Lines->Add(Buff);
    }
    return;
  }

  sv_width = 250;
  sh->listDelete();
  for (int i = 0; i < Count; i++) {  //ファイルの数だけ繰り返す
    DragQueryFile((void*)Msg.Drop, i, Buff, sizeof(Buff)); //i番目のファイル名をBuffに格納
    sh->fileStore(Buff);
  }

  if (bat_gen->Checked){
    sv_para = sh->paraFlg;
    sh->paraFlg = true;
  }
  sh->batFileGen(memo->Lines);
  if (bat_gen->Checked){
    sh->paraFlg = sv_para;
  }
  if (bat_gen->Checked == false){
    sh->batExec();
  } else {
    sChar *currFile;
    sChar *batFileName;
//    sh->bat_fp->mem_mcb_top_set();
    int sv_ptr=0;
    ARY_LOOP(currFile,sChar,sh->file_fp)
      sh->bat_fp->setReadPtr(sv_ptr);
      batFileName = (sChar *)sh->bat_fp->get();
      sv_ptr = sh->bat_fp->getReadPtr();
      sh->fn->Set(currFile->c_str());
      sprintf(cmdLine,"%s",sh->fn->FileGet());
      list->Items->Add(cmdLine);
      contextWrite_dd(sh->fn->FileGet(),batFileName->c_str());
    LOOP_END
  }
  DragQueryFile((void*)Msg.Drop, 0, Buff, sizeof(Buff));
  DragFinish((void*)Msg.Drop); //終了処理
  TForm::Dispatch(&Msg);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawItem(TMessage& Msg)
{
     IconDrawItem((LPDRAWITEMSTRUCT)Msg.LParam);
     TForm::Dispatch(&Msg);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MyNotify(TMessage& Msg)
{
    POINT MousePos;

    switch(Msg.LParam)
    {

        case WM_RBUTTONUP:
            if (GetCursorPos(&MousePos))
            {
            tray_pop->PopupComponent = Form1;
            //SetForegroundWindow(Handle);
            tray_pop->Popup(MousePos.x, MousePos.y);
            }
            break;

        case WM_LBUTTONUP:
             if (Visible)
               {
               g_Visible = false;
               Hide();
             //  confFileWrite();
               }
             else
               {
               SetForegroundWindow(Handle);
               g_Visible = true;
               Show();
               }
           break;
        default:
           break;
    }
    TForm::Dispatch(&Msg);
}


//---------------------------------------------------------------------------
bool __fastcall TForm1::TrayMessage(DWORD dwMessage)
{
   NOTIFYICONDATA tnd;
   PSTR pszTip;

   pszTip = TipText();

   tnd.cbSize          = sizeof(NOTIFYICONDATA);
   tnd.hWnd            = Handle;
   tnd.uID             = IDC_MYICON;
   tnd.uFlags          = NIF_MESSAGE | NIF_ICON | NIF_TIP;
   tnd.uCallbackMessage = MYWM_NOTIFY;

   if (dwMessage == NIM_MODIFY)
    {
        tnd.hIcon       = (HICON)IconHandle();
        if (pszTip)
           lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
        else
        tnd.szTip[0] = '\0';
    }
   else
    {
        tnd.hIcon = NULL;
        tnd.szTip[0] = '\0';
    }

   return (Shell_NotifyIcon(dwMessage, &tnd));
}

//---------------------------------------------------------------------------
HICON __fastcall TForm1::IconHandle(void)
{
        return (Image1->Picture->Icon->Handle);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToggleState(void)
{
    TrayMessage(NIM_MODIFY);
}
//---------------------------------------------------------------------------
PSTR __fastcall TForm1::TipText(void)
{
       return(VERSION);
}
//---------------------------------------------------------------------------

LRESULT IconDrawItem(LPDRAWITEMSTRUCT lpdi)
{
    HICON hIcon;

    hIcon = (HICON)LoadImage(g_hinst, MAKEINTRESOURCE(lpdi->CtlID), IMAGE_ICON,
        16, 16, 0);
    if (!hIcon)
        return(FALSE);

    DrawIconEx(lpdi->hDC, lpdi->rcItem.left, lpdi->rcItem.top, hIcon,
        16, 16, 0, NULL, DI_NORMAL);

    return(TRUE);
}

//---------------------------------------------------------------------------
void TForm1::confFileRead(){
  CsvInf_T *csvinf = new CsvInf_T(config_file->c_str());
  char *str;

  if (csvinf->CsvError_Rd() == 0)
    {
    for(;;)
      {
      if (csvinf->CsvFgets() == -1){break;}
      if (csvinf->CsvSelSu_Rd() == 0) {continue;}
      if ((str = csvinf->CsvSel_Rd(0)) == NULL) {continue;}
      char *w_str = csvinf->CsvSel_Rd(1);
      sp_push(str);
      sp_push2(str);
      if (strcmp(str,"#group"      ) == 0){group->set(w_str);  group_name->Text = w_str; continue;}
      if (strcmp(str,"#group_hist" ) == 0){group_name->Items->Add(w_str); continue;}
      if (strcmp(str,"#height"     ) == 0){Height       = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#width"      ) == 0){Width        = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#top"        ) == 0){Top          = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#left"       ) == 0){Left         = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#sv_width"   ) == 0){sv_width     = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#sv_width2"  ) == 0){sv_width2     = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#font_size"  ) == 0)
        {
        list->Font->Size = SujiConvEx(w_str);
        memo->Font->Size = SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#font_style" ) == 0)
        {
        list->Font->Style = (TFontStyles)SujiConvEx(w_str);
        memo->Font->Style = (TFontStyles)SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#back_color" ) == 0)
        {
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 16777215;}
        list->Color = (TColor)ii;
        memo->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#font_color" ) == 0)
        {
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 0;}
        list->Font->Color = (TColor)ii;
        memo->Font->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#font_name") == 0)
        {
        list->Font->Name = w_str;
        memo->Font->Name = w_str;
        continue;
        }
      if (strcmp(str,"#stay"      ) == 0)
        {
        if (strcmp(w_str,"0") == 0){stay->Checked = false; FormStyle = fsNormal;}
        else                       {stay->Checked = true;  FormStyle = fsStayOnTop;}
        continue;
        }
      if (strcmp(str,"#para"      ) == 0)
        {
        if (strcmp(w_str,"0") == 0){para->Checked = false; sh->paraFlg = false;}
        else                       {para->Checked = true;  sh->paraFlg = true;}
        continue;
        }
      if (strcmp(str,"#shell"      ) == 0)
        {
        if (strcmp(w_str,"0") == 0){shell->Checked = false; sh->shellFlg = false;}
        else                       {shell->Checked = true;  sh->shellFlg = true;}
        continue;
        }
      if (strcmp(str,"#hide"      ) == 0)
        {
        if (strcmp(w_str,"0") == 0){hideFlag->Checked = false; sh->hideFlag = false;}
        else                       {hideFlag->Checked = true;  sh->hideFlag = true;}
        continue;
        }

      if (strcmp(str,"#visible"      ) == 0)
        {
        if (strcmp(w_str,"0") == 0){g_Visible = false; }
        else                       {g_Visible = true;  }
        continue;
        }

      if (strcmp(str,"#job"      ) == 0)
        {
        if (strcmp(w_str,"0") == 0){jobFlg->Checked = false;}
        else                       {jobFlg->Checked = true; }
        continue;
        }


      if (strcmp(str,"#pad"        ) == 0)
        {
        it->alloc_ptr = (unsigned char *)new sChar(w_str);
        pad_init_fp->mem_alloc();
        }

      if (strcmp(str,"#list_width"     ) == 0){list_panel->Width        = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#sb6"     ) == 0){
        sb6->Caption = w_str;
        if (sb6->Caption == "×") {sb6->Hint = "Editor close";}
        else                      {sb6->Hint = "Editor open";}
        continue;
      }

      if (strcmp(str,"#sp1"      ) == 0)
        {
        if (strcmp(w_str,"0") == 0){sp1->Visible = false;}
        else                       {sp1->Visible = true; }
        continue;
        }
/*
      if (strcmp(str,"#memo_font_size" ) == 0)
        {
        memo->Font->Size = SujiConvEx(w_str);
        continue;
        }

      if (strcmp(str,"#memo_font_style" ) == 0)
        {
        memo->Font->Style = (TFontStyles)SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#memo_back_color" ) == 0)
        {
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 16777215;}
        memo->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#memo_font_color" ) == 0)
        {
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 0;}
        memo->Font->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#memo_font_name") == 0)
        {
        memo->Font->Name = w_str;
        continue;
        }
*/
      }
    }


  listRead();
  //list->ItemIndex = 0;
  //edit_read();
  DragAcceptFiles(Handle, true);
  delete csvinf;
  }

void TForm1::listRead()
{
  char *str;
  list->Items->Clear();
  sChar *file = new sChar(own_dir);
  file->cat("#group#");
  file->cat(group->c_str());
  file->cat(".csv");

  CsvInf_T *csvinf = new CsvInf_T(file->c_str());
  if (csvinf->CsvError_Rd() == 0){
    for(;;){
      if (csvinf->CsvFgets() == -1){break;}
      if (csvinf->CsvSelSu_Rd() == 0) {continue;}
      if ((str = csvinf->CsvSel_Rd(0)) == NULL) {continue;}
      char *w_str = csvinf->CsvSel_Rd(1);
      sp_push(str);
      sp_push2(str);
      if (strcmp(str,"#items") == 0){
        list->Items->Add(w_str);
        continue;
      }
    }
  }

  if (list->Items->Count == 0){list->Items->Add("default");}
  list->ItemIndex = 0;
  delete csvinf;
  delete file;

}

//---------------------------------------------------------------------------
// # config file write #####
void TForm1::confFileWrite()
  {
  FILE *fp;
  COPYDATASTRUCT data;

  fp = fopen(config_file->c_str(),"wt");

  fprintf(fp,"#group,%s\n",group->c_str());
  for(int k=0;k<group_name->Items->Count;k++){
    fprintf(fp,"#group_hist,%s\n",group_name->Items->Strings[k].c_str());
  }
  fprintf(fp,"#height,%d\n",Height);
  fprintf(fp,"#width,%d\n",Width);
  fprintf(fp,"#top,%d\n",Top);
  fprintf(fp,"#left,%d\n",Left);
//  fprintf(fp,"#shell,%d\n",shell->Checked );
  fprintf(fp,"#para,%d\n",para->Checked );
  fprintf(fp,"#back_color,%d\n",list->Color);
  fprintf(fp,"#font_color,%d\n",list->Font->Color );
  fprintf(fp,"#font_style,%d\n",list->Font->Style );
  fprintf(fp,"#font_size,%d\n",list->Font->Size );
  fprintf(fp,"#font_name,%s\n",list->Font->Name.c_str());
  if (stay->Checked){fprintf(fp,"#stay,1\n");}
  else              {fprintf(fp,"#stay,0\n");}
  if (para->Checked){fprintf(fp,"#para,1\n");}
  else              {fprintf(fp,"#para,0\n");}
  if (shell->Checked){fprintf(fp,"#shell,1\n");}
  else               {fprintf(fp,"#shell,0\n");}
  if (hideFlag->Checked){fprintf(fp,"#hide,1\n");}
  else                  {fprintf(fp,"#hide,0\n");}
  if (jobFlg->Checked){fprintf(fp,"#job,1\n");}
  else                {fprintf(fp,"#job,0\n");}
  if (g_Visible     ){fprintf(fp,"#visible,1\n");}
  else               {fprintf(fp,"#visible,0\n");}

  TpadList *p;
  MEM_LOOP(p,TpadList,pad_fp)
    fprintf(fp,"#pad,%s\n",p->pad_name->c_str());

    // message
    HWND hwndTarget = FindWindow(NULL,p->pad_name->c_str());
    if (hwndTarget != NULL) {
      data.dwData = 1;
      data.cbData = strlen("end")+ 1;
      data.lpData = (void*)"end";
      SendMessage(hwndTarget,WM_COPYDATA,(WPARAM)this->Handle,(LPARAM)&data);
    }


  LOOP_END
  fprintf(fp,"#list_width,%d\n",list_panel->Width);
  fprintf(fp,"#sb6,%s\n",sb6->Caption.c_str());
  if (sp1->Visible  ){fprintf(fp,"#sp1,1\n");}
  else               {fprintf(fp,"#sp1,0\n");}
  fprintf(fp,"#sv_width,%d\n",sv_width );
  fprintf(fp,"#sv_width2,%d\n",sv_width2 );
  /*
  fprintf(fp,"#memo_back_color,%d\n",memo->Color );
  fprintf(fp,"#memo_font_color,%d\n",memo->Font->Color );
  fprintf(fp,"#memo_font_style,%d\n",memo->Font->Style );
  fprintf(fp,"#memo_font_size,%d\n",memo->Font->Size );
  fprintf(fp,"#memo_font_name,%s\n",memo->Font->Name.c_str());
  */


  fclose(fp);
  listWrite();
  }

void TForm1::listWrite()
{
  sChar *file = new sChar(own_dir);
  file->cat("#group#");
  file->cat(group->c_str());
  file->cat(".csv");

  FILE *fp = fopen(file->c_str(),"wt");
  if (fp == NULL){delete file; return;}
  for(int i=0;i<list->Items->Count;i++){
    fprintf(fp,"#items,%s\n",list->Items->Strings[i]);
  }
  fclose(fp);
  delete file;
}



//---------------------------------------------------------------------------//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
Visible = true;
g_Visible = true;

Visible = false;
g_Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::version1Click(TObject *Sender)
{
 MessageDlg(VERSION, mtInformation  ,TMsgDlgButtons() << mbOK, 0);
    
}
//---------------------------------------------------------------------------
void __fastcall TForm1::stayClick(TObject *Sender)
{
 if (stay->Checked){stay->Checked = false; FormStyle = fsNormal;}
 else              {stay->Checked = true;  FormStyle = fsStayOnTop;}
 DragAcceptFiles(Handle, true); 
}
//---------------------------------------------------------------------------
void __fastcall TForm1::insert1Click(TObject *Sender)
{
contextWrite();

Form3->Left = Left;
Form3->Top  = Top-Form3->Height;
Form3->commandKbn = 0;
Form3->in->Text = "";
//Form3->Width   = Width+90;
Form3->Visible = true;
list->Selected[list->ItemIndex]=1; 
}
//---------------------------------------------------------------------------
void __fastcall TForm1::font1Click(TObject *Sender)
{
font_diag->Font = list->Font;
if (font_diag->Execute() == false){return;}
list->Font = font_diag->Font;   
memo->Font = font_diag->Font;
Form1->Visible = false;
Form1->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::backcolor1Click(TObject *Sender)
{
color_diag->Color = list->Color;
if (color_diag->Execute() == false){return;}
list->Color = color_diag->Color;
memo->Color = color_diag->Color;
Form1->Visible = false;
Form1->Visible = true;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton4Click(TObject *Sender)
{
  int sv = list->ItemIndex;
  for(int k=list->Items->Count-1;k>=0;k--){
    if (list->Selected[k]){
      list->Items->Delete(k);
    }
  }
  if (list->Items->Count<=sv) {
    sv = list->Items->Count - 1;
  }
  list->Selected[sv]=1;
  edit_read();  
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
if (list->ItemIndex <= 0){return;}
String sv = list->Items->Strings[list->ItemIndex-1];
list->Items->Strings[list->ItemIndex-1] = list->Items->Strings[list->ItemIndex];
list->Items->Strings[list->ItemIndex]   = sv;
list->ItemIndex--;
list->Selected[list->ItemIndex]=1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{
if (list->ItemIndex >= list->Items->Count-1){return;}
String sv = list->Items->Strings[list->ItemIndex+1];
list->Items->Strings[list->ItemIndex+1] = list->Items->Strings[list->ItemIndex];
list->Items->Strings[list->ItemIndex]   = sv;
list->ItemIndex++;
list->Selected[list->ItemIndex]=1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton8Click(TObject *Sender)
{
  int hit = -1;
  for(int k=0;k<group_name->Items->Count;k++){
    if (strcmp(group_name->Items->Strings[k].c_str(),group->c_str()) == 0){
      hit = k;
      break;
    }
  }
  if (hit == -1){
    group_name->Items->Add(group->c_str());
  }

  listWrite();
  group->set(group_name->Text.c_str());
  listRead();   
}
//---------------------------------------------------------------------------
void __fastcall TForm1::rename1Click(TObject *Sender)
{
contextWrite();

Form3->Left = Left;
Form3->Top  = Top-Form3->Height;
Form3->commandKbn = 1;
Form3->in->Text = list->Items->Strings[list->ItemIndex];
//Form3->Width   = Width;
Form3->Visible = true;
list->Selected[list->ItemIndex]=1; 
}
//---------------------------------------------------------------------------
void __fastcall TForm1::listClick(TObject *Sender)
{
edit_read();
}
//---------------------------------------------------------------------------
void TForm1::edit_read()
{
  
  if ((list->Items->Count > 0) && (list->ItemIndex >= 0)) {
    contextRead(list->Items->Strings[list->ItemIndex].c_str());
  } else {
    contextRead(NULL);
  }

  if (strcmp(group_name->Text.c_str(),group->c_str()) != 0){
    group_name->Text = group->c_str();
//    group_name->Width = group_name->Text.Length() * 7+30;
  }

}

void __fastcall TForm1::Exit1Click(TObject *Sender)
{

exit(0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::execute1Click(TObject *Sender)
{
  sh->listDelete();
  sh->batFileGen(memo->Lines);
  sh->batExec();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::listDblClick(TObject *Sender)
{
  edit_read();
  sh->listDelete();
  sh->batFileGen(memo->Lines);
  sh->batExec();
  contextWrite();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::paraClick(TObject *Sender)
{
if (sh->paraFlg){sh->paraFlg = false; para->Checked = false;}
else            {sh->paraFlg = true;  para->Checked = true;}    
}
//---------------------------------------------------------------------------
void __fastcall TForm1::shellClick(TObject *Sender)
{
if (sh->shellFlg){sh->shellFlg = false; shell->Checked = false;}
else             {sh->shellFlg = true;  shell->Checked = true;}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::group_nameChange(TObject *Sender)
{
//group_name->Width = group_name->Text.Length() * 7+30;
}
//---------------------------------------------------------------------------
bool TForm1::confFileRead_old_chk()
  {
  CsvInf_T *csvinf = new CsvInf_T(config_file->c_str());
  char *str;
  bool chk = false;

  if (csvinf->CsvError_Rd() == 0)
    {
    for(;;)
      {
      if (csvinf->CsvFgets() == -1){break;}
      if (csvinf->CsvSelSu_Rd() == 0) {continue;}
      if ((str = csvinf->CsvSel_Rd(0)) == NULL) {continue;}
     // char *w_str = csvinf->CsvSel_Rd(1);
      sp_push(str);
      sp_push2(str);
      if (strcmp(str,"#line") == 0){chk = true; break;}
      }
    }

  delete csvinf;
  return(chk);
  }
void __fastcall TForm1::SpeedButton5Click(TObject *Sender)
{
  for(int k=0;k<list->Items->Count;k++){
    if (list->Selected[k]){
      list->ItemIndex = k;
      sh->listDelete();
      contextRead(list->Items->Strings[k].c_str());
      sh->batFileGen(memo->Lines);
      sh->batExec();
	}
  }

}
//---------------------------------------------------------------------------
void TForm1::fileAllLoad(char *file){
  group_name->Items->Clear();
  list->Items->Clear();

  f_ctl->read(file);

  for(int i=0;;i++){
	char *tag = f_ctl->getGroup(i);
	if (tag == NULL){break;}
	group_name->Items->Add(tag);
	group->set(tag);
	listRead();
	group_name->Text = tag;
  }
}



void __fastcall TForm1::loadfile1Click(TObject *Sender)
{
  if (open_di->Execute() == false){return;}
  fileAllLoad(open_di->FileName.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TForm1::savefile1Click(TObject *Sender)
{
  contextWrite();
  listWrite();
  if (save_di->Execute() == false){return;}
  f_ctl->Write_begin(save_di->FileName.c_str());

  bool flg=false;
  for(int i=0;i<group_name->Items->Count;i++){
	f_ctl->Write(group_name->Items->Strings[i].c_str());
	if (group_name->Items->Strings[i] == group_name->Text) {
	  flg=true;
	}
  }
  if (flg==false) {
	f_ctl->Write(group_name->Text.c_str());
  }
  f_ctl->Write_end();   
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Show1Click(TObject *Sender)
{
 Visible = true;
 g_Visible = true;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
  if (sb6->Caption == "○") {
//    Form1->Width = sv_width2;
    list_panel->Width += Form1->Width - sv_width2;
    sv_width2 = Form1->Width;
  }
  group_name->Width = Form1->Width - 70;
}
//---------------------------------------------------------------------------
void TForm1::pad_confFileWrite(char *label,char *conf_file,char *list_file){
  FILE *fp;

  fp = fopen(conf_file,"rt");
  if (fp != NULL) {
    fclose(fp);
    return;
  }

  fp = fopen(conf_file,"wt");

  fprintf(fp,"#exit,1\n");
  fprintf(fp,"#label,%s\n",label);
  fprintf(fp,"#height,%d\n",30);
  fprintf(fp,"#width,%d\n",Width);
  fprintf(fp,"#top,%d\n",Top-30);
  fprintf(fp,"#left,%d\n",Left);
  fprintf(fp,"#shell,%d\n",shell->Checked );
  fprintf(fp,"#hide,%d\n",hideFlag->Checked );
  fprintf(fp,"#para,%d\n",para->Checked );
  fprintf(fp,"#back_color,%d\n",list->Color );
  fprintf(fp,"#font_color,%d\n",list->Font->Color );
  fprintf(fp,"#font_style,%d\n",list->Font->Style );
  fprintf(fp,"#font_size,%d\n",list->Font->Size );
  fprintf(fp,"#font_name,%s\n",list->Font->Name.c_str());
  if (stay->Checked){fprintf(fp,"#stay,1\n");}
  else              {fprintf(fp,"#stay,0\n");}
  if (para->Checked){fprintf(fp,"#para,1\n");}
  else              {fprintf(fp,"#para,0\n");}
  if (shell->Checked){fprintf(fp,"#shell,1\n");}
  else               {fprintf(fp,"#shell,0\n");}
  fprintf(fp,"#label_c,1\n");
  fprintf(fp,"#list,");
  CsvWrite(fp,list_file);
  fprintf(fp,"\n");


  //  fprintf(fp,"#list,\"%s\"\n",list_file);
  fclose(fp);
}

void __fastcall TForm1::batchpad1Click(TObject *Sender)
{
  AnsiString label;
  sChar *conf_file = new sChar("");
  sChar *list_file = new sChar("");

  edit_read();

  for(int k=0;k<list->Items->Count;k++){
    if (list->Selected[k]){
      label = list->Items->Strings[k];

      conf_file->set(own_dir->c_str());
      conf_file->cat("#bat#");
      conf_file->cat(group->c_str());
      conf_file->cat("#");
      conf_file->cat(label.c_str());
      list_file->set(conf_file->c_str());
      conf_file->cat(".prm");
      list_file->cat(".csv");

      pad_confFileWrite(label.c_str(),conf_file->c_str(),list_file->c_str());
      pad_exec(conf_file->c_str());
    }
  }
  delete conf_file;
  delete list_file;
//  confFileWrite();
}
//---------------------------------------------------------------------------

bool TForm1::pad_check(char *file){
  CsvInf_T *csvinf = new CsvInf_T(file);
  char *str;

  if (csvinf->CsvError_Rd() != 0){delete csvinf; return(false);}
  bool ret = false;
  for(;;){
    if (csvinf->CsvFgets() == -1){break;}
    if (csvinf->CsvSelSu_Rd() == 0) {continue;}
    if ((str = csvinf->CsvSel_Rd(0)) == NULL) {continue;}
    char *w_str = csvinf->CsvSel_Rd(1);
    sp_push(str);
    sp_push2(str);
    if (strcmp(str,"#exit") != 0){continue;}
    if (w_str == MM_NULL) {break;}
    if (strcmp(w_str,"0")==0) {ret = true; break;}
    break;
  }
  delete csvinf;
  return(ret);
}

void TForm1::pad_exec(char *str){

      TpadList *p;
#ifndef EXTPAD
      it->srch_key = str;
      if (pad_fp->mem_srch() == 0) {
        p = (TpadList *)pad_fp->mem_link_ptr_rd();
      }
      else {
        p = new TpadList(str);
        p->pad = new Tpad_Form1(Owner,str);
        it->alloc_ptr = (MM_PTR_T *)p;
        pad_fp->mem_srch_alloc();
      }
      p->pad->Visible = true;
      p->pad->g_exit = 0;

#else
      p = new TpadList(str);
      it->alloc_ptr = (MM_PTR_T *)p;
      pad_fp->mem_alloc();

      sChar *cmd = new sChar("");
      sChar *prm = new sChar("");

      cmd->set("\"");
      cmd->cat(pad_file->c_str());
      cmd->cat("\"");
      prm->set("\"");
      prm->cat(str);
      prm->cat("\"");

      ShellExecute(NULL,NULL,cmd->c_str(),prm->c_str(),NULL,SW_SHOWNORMAL);
      delete cmd;
      delete prm;
#endif
}



void __fastcall TForm1::timerTimer(TObject *Sender)
{
  timer->Enabled = false;
  sChar *str;
  MEM_LOOP(str,sChar,pad_init_fp)
    if (pad_check(str->c_str())){
      pad_exec(str->c_str());
    }
  LOOP_END
  Visible = g_Visible;

  list->Selected[list->ItemIndex]=1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::listKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==VK_RETURN) {
    edit_read();
    sh->listDelete();
    sh->batFileGen(memo->Lines);
    sh->batExec();
    contextWrite();
    Key=0x00;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::WMCopy(TMessage &Msg)
{
    COPYDATASTRUCT *copydata;
    copydata = (COPYDATASTRUCT*)Msg.LParam;

    if (jobFlg->Checked == false){return;}

    AnsiString s = (char *)copydata->lpData;
    Form4->dummy->Lines->Add(s);
    csvi->Exec(s.c_str());
    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (strcmp(str,"#job_list") == 0){job_list_rtn(); continue;}
      if (strcmp(str,"#exec") == 0){exec_rtn(); continue;}
      if (strcmp(str,"#wait_name") == 0){wait_name_rtn(); continue;}
      if (strcmp(str,"#own_kill") == 0){ownKillFlag = true; continue;}
      if (strcmp(str,"#kill") == 0){kill_rtn(); continue;}
      if (strcmp(str,"#kill_all") == 0){kill_all_rtn(); continue;}
      if (strcmp(str,"#config") == 0){config_rtn(); continue;}
      if (strcmp(str,"#config_write") == 0){config_write_rtn(); continue;}
    LOOP_END
}

void TForm1::kill_all_rtn(){

  jobQueue_T *j;
  job_num = 0;
  job_timer->Enabled = false;
  MEM_LOOP(j,jobQueue_T,sh->job_fp)
    delete j;
  LOOP_END
  sh->job_fp->mem_del_all();
  jobConsole();
}

void TForm1::job_list_rtn(){
    char *file_name=NULL;

    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (file_name   == NULL) {file_name   = str; continue;}
    LOOP_END
    if (file_name == NULL) {return;}

    FILE *fp = fopen(file_name,"wt");
    if (fp == NULL) {
        return;
    }

    char *state;
    jobQueue_T *j;
    bool flag=false;
    MEM_LOOP(j,jobQueue_T,sh->job_fp)
      flag = true;
      if      (j->state == S_JOB_JWAIT){state = "JOB WAIT  ";}
      else if (j->state == S_JOB_TWAIT){state = "TIMER WAIT";}
      else                             {state = "RUN       ";}
      if (j->inh_name != NULL){
        fprintf(fp,"%s : %08d : %-10s : %-10s : %-10s : %-10s\n",state,j->sleep_time,j->job_name->c_str(),j->group_name->c_str(),j->exec_name->c_str(),j->inh_name->c_str());
      }
      else {
        fprintf(fp,"%s : %08d : %-10s : %-10s : %-10s : \n",state,j->sleep_time,j->job_name->c_str(),j->group_name->c_str(),j->exec_name->c_str());
      }
    LOOP_END
    if (flag==false) {
        fprintf(fp,"no jobs\n");
    }
    fclose(fp);
}

void TForm1::config_rtn(){
    char *file_name=NULL;

    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (file_name   == NULL) {file_name   = str; continue;}
    LOOP_END
    if (file_name == NULL) {return;}
	fileAllLoad(file_name);
}


void TForm1::config_write_rtn(){
    char *file_name=NULL;

    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (file_name   == NULL) {file_name   = str; continue;}
    LOOP_END
    if (file_name == NULL) {return;}

    contextWrite();
    listWrite();
    f_ctl->Write_begin(file_name);

    bool flg=false;
    for(int i=0;i<group_name->Items->Count;i++){
      f_ctl->Write(group_name->Items->Strings[i].c_str());
      if (group_name->Items->Strings[i] == group_name->Text) {
        flg=true;
      }
    }
    if (flg==false) {
      f_ctl->Write(group_name->Text.c_str());
    }
    f_ctl->Write_end();
}

void TForm1::exec_rtn(){
    char *job_name=NULL;
    char *group_name=NULL;
    char *exec_name=NULL;
    char *sleep_time=NULL;

    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (job_name   == NULL) {job_name   = str; continue;}
      if (group_name == NULL) {group_name = str; continue;}
      if (exec_name  == NULL) {exec_name  = str; continue;}
      if (sleep_time == NULL) {sleep_time = str; continue;}
      sh->jobFileStore(str);
    LOOP_END
    if (sleep_time == NULL) {return;}
    
    char *w_inh_name = NULL;
    if (inh_name != NULL){
      w_inh_name = inh_name->c_str();
    }

    if (sh->jobStore(own_dir->c_str(),Form4->dummy->Lines,job_name,group_name,exec_name,sleep_time,w_inh_name,ownKillFlag)){
      job_num++;
      if (job_num==1) {
        job_timer->Enabled = true;
      }
    }

    if (inh_name != NULL){
      delete inh_name;
      inh_name = NULL;
    }
    ownKillFlag = false;
  jobConsole();
}

void TForm1::kill_rtn(){
    char *job_name=NULL;

    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (job_name   == NULL) {job_name   = str; continue;}
    LOOP_END
    if (job_name==NULL) {
        return;
    }
    if (sh->jobKill(job_name)){
      job_num--;
      if (job_num==0) {
        job_timer->Enabled = false;
      }
    }
  jobConsole();
}

void TForm1::wait_name_rtn(){
    char *job_name=NULL;

    CSV_LOOP(csvi,ustr,SP_PUSH)
      char *str = (char *)ustr;
      if (job_name   == NULL) {job_name   = str; continue;}
    LOOP_END
    if (job_name==NULL) {
        return;
    }
    
    if (inh_name != NULL){
      delete inh_name;
    }
    inh_name = new sChar(job_name);
}


void  TForm1::jobConsole(){
  if (Form4->Visible == false) {
    return;
  }
  char *state;
  Form4->list->Items->Clear();
  jobQueue_T *j;
  MEM_LOOP(j,jobQueue_T,sh->job_fp)
    if      (j->state == S_JOB_JWAIT){state = "JOB WAIT  ";}
    else if (j->state == S_JOB_TWAIT){state = "TIMER WAIT";}
    else                             {state = "RUN       ";}
    if (j->inh_name != NULL){
      sprintf(cmdLine,"%s : %08d : %-10s : %-10s : %-10s : %-10s",state,j->sleep_time,j->job_name->c_str(),j->group_name->c_str(),j->exec_name->c_str(),j->inh_name->c_str());
    }
    else {
      sprintf(cmdLine,"%s : %08d : %-10s : %-10s : %-10s : ",state,j->sleep_time,j->job_name->c_str(),j->group_name->c_str(),j->exec_name->c_str());
    }
    Form4->list->Items->Add(cmdLine);

  LOOP_END
}

void __fastcall TForm1::job_timerTimer(TObject *Sender)
{
  contextWrite();
  int w = sh->jobExecLoop(own_dir->c_str(),Form4->dummy->Lines);
  job_num -= w;
  if (job_num<0) {job_num = 0;}
  if (job_num==0) {
    job_timer->Enabled = false;
  }
  jobConsole();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::jobMenuClick(TObject *Sender)
{
  if (Form4->Visible) {Form4->Visible = false;}
  else                {Form4->Visible = true;}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::jobFlgClick(TObject *Sender)
{
    if (jobFlg->Checked == false){jobFlg->Checked = true;}
    else                         {jobFlg->Checked = false;}

}
//---------------------------------------------------------------------------

//----  editor ----------------------
// --------------- CSV WRITE ------------------
void TForm1::CsvWrite(FILE *out_fp,char *str)
  {
  int j,k,len;
  bool dbl_flg;
  char *buff2;
  JisInf_T *sj;

  len = strlen(str);
  sj  = new JisInf_T;

  dbl_flg = false;
  buff2 = new char[len*2+1];
  *buff2 = 0x00;
  for(k=0,j=0;j<len;++j)
    {
    if (sj->SJisCheck(str,j) == true)
      {
      *(buff2+k) = *(str+j); ++k;
      *(buff2+k) = 0x00;
      continue;
      }
    if (*(str+j) == '"'){dbl_flg = true; *(buff2+k) = *(str+j); ++k;}
    if (*(str+j) == ','){dbl_flg = true;}
    *(buff2+k) = *(str+j); ++k;
    *(buff2+k) = 0x00;
    }
  if (dbl_flg == true){fprintf(out_fp,"\"%s\"",buff2);}
  else                {fprintf(out_fp,"%s",buff2);}
  delete [] buff2;
  delete sj;
  }

// -----------------------------------------------------------------
void TForm1::contextWrite_dd(char *p_file,char *in_file)
{
  sChar *file = new sChar(Form1->own_dir);
  file->cat("#bat#");
  file->cat(Form1->group->c_str());
  file->cat("#");
  file->cat(p_file);
  file->cat(".csv");

//  FILE *in_fp = fopen(in_file,"rt");
  Read64_C *in_fp = new Read64_C(in_file,"rt",1024);
  FILE *fp = fopen(file->c_str(),"wt");
//  if (fp == NULL){delete file; fclose(in_fp); return;}
  if (fp == NULL){delete file; delete in_fp; return;}
//  xFgets_T *xf = new xFgets_T(1024);
  char *str;
  for(;;){
//      str = xf->xFgets(in_fp);
      str = in_fp->read();
      if (str == NULL){break;}
      if (*(str+strlen(str)-1) < ' '){*(str+strlen(str)-1) = 0x00;}
      fprintf(fp,"#context,");
	  CsvWrite(fp,str);
      fprintf(fp,"\n");
  }
//  delete xf;
//  fclose(in_fp);
  delete in_fp;
  fclose(fp);
  delete file;
}
// -----------------------------------------------------------------
void TForm1::contextRename(char *p_file)
{
  contextWrite();
  sChar *old_file = new sChar(Form1->own_dir);
  old_file->cat("#bat#");
  old_file->cat(Form1->group->c_str());
  old_file->cat("#");
  old_file->cat(sv_file->c_str());
  old_file->cat(".csv");
  sChar *new_file = new sChar(Form1->own_dir);
  new_file->cat("#bat#");
  new_file->cat(Form1->group->c_str());
  new_file->cat("#");
  new_file->cat(p_file);
  new_file->cat(".csv");

  AnsiString w_old,w_new;
  w_old = old_file->c_str();
  w_new = new_file->c_str();

  RenameFile(w_old,w_new);
  sv_file->set(p_file);
  delete old_file;
  delete new_file;
}
// -----------------------------------------------------------------
void TForm1::contextRead(char *new_file)
{
  char *str;
  contextWrite();
  memo->Lines->Clear();
  if (new_file == NULL){return;}
  sChar *file = new sChar(Form1->own_dir);
  file->cat("#bat#");
  file->cat(Form1->group->c_str());
  file->cat("#");
  file->cat(new_file);
  file->cat(".csv");

  char *w_str;
  CsvInf_T *csvinf = new CsvInf_T(file->c_str());
  if (csvinf->CsvError_Rd() == 0){
    memo->Visible = false;
    for(;;){
      if (csvinf->CsvFgets() == -1){break;}
      if (csvinf->CsvSelSu_Rd() == 0) {continue;}
      if ((str = csvinf->CsvSel_Rd(0)) == NULL) {continue;}
      w_str = NULL;
      if (csvinf->CsvSelSu_Rd() > 1) {
        w_str = csvinf->CsvSel_Rd(1);
      }
      sp_push(str);
      sp_push2(str);
      if (strcmp(str,"#context") == 0){
        if (w_str == MM_NULL){memo->Lines->Add("");}
        else              {memo->Lines->Add(w_str);}
        continue;
      }
    }
    memo->Visible = true;
  }
  sv_file->set(new_file);
  delete csvinf;
  delete file;

}

//---------------------------------------------------------------------------
void TForm1::contextWrite()
{
  if (memoChg == false){return;}

  sChar *file = new sChar(Form1->own_dir);
  file->cat("#bat#");
  file->cat(Form1->group->c_str());
  file->cat("#");
  file->cat(sv_file->c_str());
  file->cat(".csv");

  FILE *fp = fopen(file->c_str(),"wt");
  if (fp == NULL){delete file; return;}
  for(int i=0;i<memo->Lines->Count;i++){
    String a = memo->Lines->Strings[i];
    char *str = a.c_str();
    if (str == NULL){fprintf(fp,"#context,\n");}
    else {
      fprintf(fp,"#context,");
	  CsvWrite(fp,str);
      fprintf(fp,"\n");
      }
  }
  fclose(fp);
  memoChg = false;
  delete file;
}


void __fastcall TForm1::memoChange(TObject *Sender)
{
memoChg = true;    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::sb6Click(TObject *Sender)
{

  if (sb6->Caption == "×") {   // 縮める
    if (edit_panel->Width > 10) {
      sv_width = edit_panel->Width;
    }
    else {
      sv_width = 250;
    }
    int l = edit_panel->Width;
    Form1->Width -= edit_panel->Width;
    sv_width2 = Form1->Width;
    sb6->Caption = "○";
    sb6->Hint = "Editor open";
    sp1->Visible = false;
    Left = Left + l;
  }
  else{                         // 伸ばす
    sb6->Caption = "×";
    sb6->Hint = "Editor close";
    Form1->Width += sv_width;
    sp1->Visible = true;
    Left = Left - sv_width;

  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::hideFlagClick(TObject *Sender)
{
if (sh->hideFlag){sh->hideFlag = false; hideFlag->Checked = false;}
else             {sh->hideFlag = true;  hideFlag->Checked = true;}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PositionReset1Click(TObject *Sender)
{
Form1->Left = 0;
Form1->Top = 0;
}
//---------------------------------------------------------------------------
















