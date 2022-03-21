//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <process.h>
#include <stdio.h>



#include "shell_exec.h"
#include "file_ctrl.h"
#include "_bat_sel.h"
#include "job_unit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
extern TForm1 *Form1;
extern ShellExec_T *sh;

//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
    : TForm(Owner)
{
config_file = new sChar(Form1->own_dir->c_str());
config_file->cat("bat_sel_job.csv");
confFileRead();
}

__fastcall TForm4::~TForm4(){
  confFileWrite();
  delete config_file;
}


//---------------------------------------------------------------------------

void __fastcall TForm4::color_topClick(TObject *Sender)
{
color_diag->Color = list->Color;
if (color_diag->Execute() == false){return;}
list->Color = color_diag->Color;
Form4->Visible = false;
Form4->Visible = true;

}
//---------------------------------------------------------------------------

void __fastcall TForm4::colordown1Click(TObject *Sender)
{
color_diag->Color = dummy->Color;
if (color_diag->Execute() == false){return;}
dummy->Color = color_diag->Color;
Form4->Visible = false;
Form4->Visible = true;

}
//---------------------------------------------------------------------------

void __fastcall TForm4::fontup1Click(TObject *Sender)
{
font_diag->Font = list->Font;
if (font_diag->Execute() == false){return;}
list->Font = font_diag->Font;
Form4->Visible = false;
Form4->Visible = true;

}
//---------------------------------------------------------------------------

void __fastcall TForm4::fontdown1Click(TObject *Sender)
{
font_diag->Font = dummy->Font;
if (font_diag->Execute() == false){return;}
dummy->Font = font_diag->Font;
Form4->Visible = false;
Form4->Visible = true;

}
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------
void TForm4::confFileRead()
  {
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
      if (strcmp(str,"#height"    ) == 0){Height       = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#width"     ) == 0){Width        = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#top"       ) == 0){Top          = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#left"      ) == 0){Left         = SujiConvEx(w_str); continue;}
      if (strcmp(str,"#font_size" ) == 0)
        {
        list->Font->Size = SujiConvEx(w_str);
        continue;
        }

      if (strcmp(str,"#font_style" ) == 0)
        {
        list->Font->Style = (TFontStyles)SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#back_color" ) == 0)
        {
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 16777215;}
        list->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#font_color" ) == 0)
        {
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 0;}
        list->Font->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#font_name") == 0)
        {
        list->Font->Name = w_str;
        continue;
        }
      if (strcmp(str,"#font_size_d" ) == 0)
        {
        dummy->Font->Size = SujiConvEx(w_str);
        continue;
        }

      if (strcmp(str,"#font_style_d" ) == 0)
        {
        dummy->Font->Style = (TFontStyles)SujiConvEx(w_str);
        continue;
        }
      if (strcmp(str,"#back_color_d" ) == 0)
        {
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 16777215;}
        dummy->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#font_color_d" ) == 0)
        {
        int ii = SujiConvEx(w_str);
        if (ii < 0){ii = 0;}
        dummy->Font->Color = (TColor)ii;
        continue;
        }
      if (strcmp(str,"#font_name_d") == 0)
        {
        dummy->Font->Name = w_str;
        continue;
        }
      }
    }
  delete csvinf;
  }

void TForm4::confFileWrite()
  {
  FILE *fp;
  fp = fopen(config_file->c_str(),"wt");

  fprintf(fp,"#height,%d\n",Height);
  fprintf(fp,"#width,%d\n",Width);
  fprintf(fp,"#top,%d\n",Top);
  fprintf(fp,"#left,%d\n",Left);
  fprintf(fp,"#back_color,%d\n",list->Color );
  fprintf(fp,"#font_color,%d\n",list->Font->Color );
  fprintf(fp,"#font_style,%d\n",list->Font->Style );
  fprintf(fp,"#font_size,%d\n",list->Font->Size );
  fprintf(fp,"#font_name,%s\n",list->Font->Name.c_str());

  fprintf(fp,"#back_color_d,%d\n",dummy->Color );
  fprintf(fp,"#font_color_d,%d\n",dummy->Font->Color );
  fprintf(fp,"#font_style_d,%d\n",dummy->Font->Style );
  fprintf(fp,"#font_size_d,%d\n",dummy->Font->Size );
  fprintf(fp,"#font_name_d,%s\n",dummy->Font->Name.c_str());
  fclose(fp);
  }


// -----------------------------------------------------------------------

void __fastcall TForm4::jobdelete1Click(TObject *Sender)
{
  jobQueue_T *j;
  Form1->job_num = 0;
  Form1->job_timer->Enabled = false;
  list->Items->Clear();
  MEM_LOOP(j,jobQueue_T,sh->job_fp)
    delete j;
  LOOP_END
  sh->job_fp->mem_del_all();
}
//---------------------------------------------------------------------------

void __fastcall TForm4::jobtimerstop1Click(TObject *Sender)
{
  Form1->job_timer->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::jobtimerstart1Click(TObject *Sender)
{
  if (Form1->job_num == 0) {return;}
  Form1->job_timer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::jobexec1Click(TObject *Sender)
{
  jobQueue_T *j;
  McbArea_C *ptr = sh->job_fp->mem_mcb_top_rd();

  for(int k=0;k<list->Items->Count;k++){
    sh->job_fp->mem_mcb_ptr_wr(ptr);
    ptr = sh->job_fp->mem_mcb_next_rd();
    if (list->Selected[k]){
      j = (jobQueue_T *)sh->job_fp->mem_link_ptr_rd();
      sh->jobExec(j,Form1->own_dir->c_str(),dummy->Lines);
      sh->job_fp->mem_del();
      delete j;
      Form1->job_num--;
      if (Form1->job_num == 0) {
        Form1->job_timer->Enabled = false;
      }
    }
  }
  Form1->jobConsole();
}
//---------------------------------------------------------------------------

void __fastcall TForm4::jobdelete2Click(TObject *Sender)
{
  jobQueue_T *j;
  McbArea_C *ptr = sh->job_fp->mem_mcb_top_rd();

  for(int k=0;k<list->Items->Count;k++){
    sh->job_fp->mem_mcb_ptr_wr(ptr);
    ptr = sh->job_fp->mem_mcb_next_rd();
    if (list->Selected[k]){
      j = (jobQueue_T *)sh->job_fp->mem_link_ptr_rd();
      sh->job_fp->mem_del();
      delete j;
      Form1->job_num--;
      if (Form1->job_num == 0) {
        Form1->job_timer->Enabled = false;
      }
    }
  }
  Form1->jobConsole();

}
//---------------------------------------------------------------------------

