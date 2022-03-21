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

//
// shell exec
//
#include "shell_exec.h"

ShellExec_T::ShellExec_T(){
  it      = new MemHeader_T;
  file_fp = new VariableArray_C(10);
  job_file_fp = new VariableArray_C(10);
  bat_fp  = new VariableArray_C(10);
  job_fp  = new MEM_FP(it);
  fileFlg = false;
  fn = new FileNameSep_T;
  paraFlg = false;
  shellFlg = true;
  temp = new tempFileInf_C;
  paraInhFlag = false;

  g_p2 = new VariableArea_C("");
  g_p3 = new VariableArea_C("");
  g_p4 = new VariableArea_C("");
  g_p5 = new VariableArea_C("");
  g_p6 = new VariableArea_C("");
  g_p7 = new VariableArea_C("");
  g_p8 = new VariableArea_C("");
  g_p9 = new VariableArea_C("");

  g_p2p9_cnt = 0;
  makeFile = new sChar("");
  state_makeFile = 0;
  jobActFlg = true;
  hideFlag = false;
  
}

ShellExec_T::~ShellExec_T(){
  sChar *w;
  jobQueue_T *j;

  ARY_LOOP(w,sChar,job_file_fp) delete w; LOOP_END

  jobActFlg = false;
  MEM_LOOP(j,jobQueue_T,job_fp) delete j; LOOP_END


  listDelete();
  delete file_fp;
  delete bat_fp;
  delete fn;
  delete job_fp;
  delete temp;

  delete g_p2;
  delete g_p3;
  delete g_p4;
  delete g_p5;
  delete g_p6;
  delete g_p7;
  delete g_p8;
  delete g_p9;
  delete makeFile;

}

// # ファイル名登録 #####
void ShellExec_T::fileStore(char *file){
  sChar *w = new sChar(file);
  file_fp->mem_alloc((unsigned char *)w);
  fileFlg = true;

  g_p2p9_cnt++;
  if      (g_p2p9_cnt == 2){g_p2->set_string(file);}
  else if (g_p2p9_cnt == 3){g_p3->set_string(file);}
  else if (g_p2p9_cnt == 4){g_p4->set_string(file);}
  else if (g_p2p9_cnt == 5){g_p5->set_string(file);}
  else if (g_p2p9_cnt == 6){g_p6->set_string(file);}
  else if (g_p2p9_cnt == 7){g_p7->set_string(file);}
  else if (g_p2p9_cnt == 8){g_p8->set_string(file);}
  else if (g_p2p9_cnt == 9){g_p9->set_string(file);}

}

// # リスト開放 #####
void ShellExec_T::listDelete(){
  sChar *w;

  ARY_LOOP(w,sChar,file_fp) delete w; LOOP_END
  ARY_LOOP(w,sChar,bat_fp )
//    DeleteFile(w->c_str());
    delete w;
  LOOP_END
  file_fp->clear();
  bat_fp->clear();
  fileFlg = false;
  
  g_p2->set_string("");
  g_p3->set_string("");
  g_p4->set_string("");
  g_p5->set_string("");
  g_p6->set_string("");
  g_p7->set_string("");
  g_p8->set_string("");
  g_p9->set_string("");
  g_p2p9_cnt = 0;
  
}


// # バッチ実行 #####
void ShellExec_T::batExec(){
  ARY_LOOP(currFile,sChar,bat_fp)
/*
FILE *fp=fopen("c:\\aaa.txt","at");
fprintf(fp,"currFile=%s\n",currFile->c_str());
fclose(fp);
*/

    if (shellFlg == false){system(currFile->c_str());}
    else {
      sprintf(cmdLine,"/C \"%s\"",currFile->c_str());
      if (hideFlag == false){
        ShellExecute(NULL,NULL,"cmd",cmdLine,NULL,SW_SHOWNORMAL);
      }
      else {
        ShellExecute(NULL,NULL,"cmd",cmdLine,NULL,SW_HIDE | SW_MINIMIZE);
      }
    }
  LOOP_END
  fileFlg = false;
}

// # 実行コア行生成 #####
void ShellExec_T::batFileGen(TStrings *bat_context){
  Write64_C *fp;
  sChar *ww;
  bool flg;

  bool notMulti = false;

  if (fileFlg == false){
    ww = new sChar("");
    file_fp->mem_alloc((unsigned char *)ww);
  }

  if ((fileFlg == false) || (paraFlg == false)){flg = false;} else {flg = true;}

  if (flg == false){fp = temp->openTempFile("bat","wt",10240);}

  ARY_LOOP(ww,sChar,file_fp)
    if (notMulti){break;}
    if (flg){
      fp = temp->openTempFile("bat","wt",10240);
    }
    fn->set(ww->c_str());
    currFile = ww;
    for(int i=0;i<bat_context->Count;i++){
      bool w_notMulti = batFileGenLine(fp,bat_context->Strings[i].c_str());
      if (w_notMulti){notMulti = true;}
    }
    if (flg){
      ww = new sChar(temp->curr_temp);
      bat_fp->mem_alloc((unsigned char *)ww);
      temp->closeTempFile();
    }
  LOOP_END

  if (flg == false){
    ww  = new sChar(temp->curr_temp);
    bat_fp->mem_alloc((unsigned char *)ww);
    temp->closeTempFile();
  }


}



// # バッチファイル行生成 #####
bool ShellExec_T::batFileGenLine(Write64_C *fp,char *lines){

  bool notMulti = false;

  char  *cmd = cmdLine;
  char  *w   = lines;
  int    len = strlen(w);
  
  
  for(int i=0;i<len;i++){
    int kbn=0;
    if (*w == '%'){
      if      (*(w+1) == '1'){kbn = 1;} // full path
      else if (*(w+1) == '2'){kbn = 12;} // full path
      else if (*(w+1) == '3'){kbn = 13;} // full path
      else if (*(w+1) == '4'){kbn = 14;} // full path
      else if (*(w+1) == '5'){kbn = 15;} // full path
      else if (*(w+1) == '6'){kbn = 16;} // full path
      else if (*(w+1) == '7'){kbn = 17;} // full path
      else if (*(w+1) == '8'){kbn = 18;} // full path
	  else if (*(w+1) == '9'){kbn = 19;} // full path

	  if (paraInhFlag == false) {
		if (*(w+1) == 'r'){kbn = 2;} // drive
		else if (*(w+1) == 'd'){kbn = 3;} // directory
		else if (*(w+1) == 'f'){kbn = 4;} // file name
		else if (*(w+1) == 'e'){kbn = 5;} // extension
	  }
	}
    if (kbn == 0){
      *cmd++ = *w++;
      continue;
    }
    w += 2;
    char *www;
    if      (kbn ==  1){www = currFile->c_str();}
    else if (kbn ==  2){www = fn->DrvGet ();}
    else if (kbn ==  3){www = fn->DirGet ();}
    else if (kbn ==  4){www = fn->FileGet();}
    else if (kbn ==  5){www = fn->ExtGet ();}
    else if (kbn == 12){www = g_p2->c_str(); notMulti = true;}
    else if (kbn == 13){www = g_p3->c_str(); notMulti = true;}
    else if (kbn == 14){www = g_p4->c_str(); notMulti = true;}
    else if (kbn == 15){www = g_p5->c_str(); notMulti = true;}
    else if (kbn == 16){www = g_p6->c_str(); notMulti = true;}
    else if (kbn == 17){www = g_p7->c_str(); notMulti = true;}
    else if (kbn == 18){www = g_p8->c_str(); notMulti = true;}
    else if (kbn == 19){www = g_p9->c_str(); notMulti = true;}

    for(;;){
      if (*www == 0x00){break;}
      *cmd++ = *www++;
    }
  LOOP_END
  *cmd = 0x00;

  sp_push(cmdLine);
  sp_push2(cmdLine);


  char *w_start = "";
  if (mem_cmp(cmdLine,"!#file_begin ",13) == 0){
	state_makeFile = 1;
	makeFile->set(&cmdLine[13]);
	makeFile->Push();
	return(notMulti);
  }
  else if (mem_cmp(cmdLine,"!#file_end",10) == 0){
	state_makeFile = 0;
	makeFile->set("");
	return(notMulti);
  }
  else if (mem_cmp(cmdLine,"!#parainh",9) == 0){
    paraInhFlag = true;
	return(notMulti);
  }
  else if (mem_cmp(cmdLine,"!#proc",6)==0) {
	for(int i=0;i<6;i++){
	  cmdLine[i] = ' ';
	}
	w_start = "start \"bat-sel\"";
  }


  char *w_echo = "";
  char *w_redi   = "";
  if (state_makeFile == 1){
	state_makeFile++;
	w_echo = "echo ";
	w_redi  = " > ";
  }
  else if (state_makeFile == 2){
	w_echo = "echo ";
	w_redi  = " >> ";
  }


  int len_cmdLine = strlen(cmdLine);
  bool flg=false;
  if (len > 0) {
    if (cmdLine[len_cmdLine - 1] == '\\') {
  	  cmdLine[len_cmdLine - 1] = ' ';
      flg = true;
    }
  }
  //fprintf(fp,"%s%s%s%s%s",w_start,w_echo,cmdLine,w_redi,makeFile->c_str());
  fp->write(w_start);
  fp->write(w_echo);
  fp->write(cmdLine);
  fp->write(w_redi);
  fp->write(makeFile->c_str());


  if (flg==false){
    //fprintf(fp,"\n");
    fp->writeln("");
  }


//  fprintf(fp,"%s\n",cmdLine);
  return(notMulti);
}



// JOB 待ちチェック
bool ShellExec_T::jobWaitCheck(char *inh_name){
  if (inh_name == NULL){return(true);}
  it->srch_key = inh_name;
  if (job_fp->mem_srch() == 0){return(false);}
  return(true);
}

// JOB 登録
bool ShellExec_T::jobStore(char *own_dir,TStrings *Lines,char *job_name,char *group_name,char *exec_name,char *sleep_time,char *inh_name,bool ownKillFlag){
  if (jobActFlg == false) {
    return(false);
  }
  jobQueue_T *j;
  bool flg=true;


#ifdef DEBUG_SH
  FILE *fp=fopen("c:\\output.txt","at");
  fprintf(fp,"A:%s\n",job_name);
  fclose(fp);
#endif



  it->srch_key = job_name;
  if (job_fp->mem_srch() == 0) {
    j = (jobQueue_T *)job_fp->mem_link_ptr_rd();
    delete j;
    job_fp->mem_del();
    flg = false;
  }

  j = new jobQueue_T(job_name,group_name,exec_name,sleep_time,inh_name,ownKillFlag);
  j->enableFlg = jobWaitCheck(inh_name);

  it->srch_key = job_name;
  it->alloc_ptr = (MM_PTR_T *)j;
  job_fp->mem_srch_alloc();

  sChar *w;
  ARY_LOOP(w,sChar,job_file_fp)
    j->job_file_fp->mem_alloc((unsigned char *)w);
  LOOP_END
  job_file_fp->clear();

#ifdef DEBUG_SH
  fp=fopen("c:\\output.txt","at");
  MEM_LOOP(j,jobQueue_T,job_fp)
    fprintf(fp,"B:%s\n",j->job_name->c_str());
  LOOP_END
  fclose(fp);
#endif

  return(flg);
}

// JOB 削除
bool ShellExec_T::jobKill(char *job_name){
  if (jobActFlg == false) {
    return(false);
  }
  it->srch_key = job_name;
  if (job_fp->mem_srch() != 0) {
    return(false);
  }
  jobQueue_T *j = (jobQueue_T *)job_fp->mem_link_ptr_rd();
  delete j;
  job_fp->mem_del();

  MEM_LOOP_EX(j,jobQueue_T,job_fp)
    if (j->inh_name == NULL) {
      continue;
    }
    j->enableFlg = jobWaitCheck(j->inh_name->c_str());
  LOOP_END
  return(true);
}

// JOB 実行
int ShellExec_T::jobExecLoop(char *own_dir,TStrings *Lines){
  jobQueue_T *j;
  int num=0;

  MEM_LOOP_EX(j,jobQueue_T,job_fp)
    if (j->enableFlg == false){continue;}

    j->sleep_time--;
    if (j->sleep_time <= 0) {
      jobExec(j,own_dir,Lines);
      if (j->ownKillFlag){
        j->sleep_time = 0;
        j->state = S_JOB_RUN;
        j->enableFlg = false;
      }
      else {
        num++;
        delete j;
        job_fp->mem_del();
      }
    }
  LOOP_END
  if (num > 0){
    MEM_LOOP_EX(j,jobQueue_T,job_fp)
      if (j->inh_name == NULL) {
          continue;
      }
      j->enableFlg = jobWaitCheck(j->inh_name->c_str());
    LOOP_END
  }

  return(num);
}

// JOB 実行
void ShellExec_T::jobExec(jobQueue_T *j,char *own_dir,TStrings *Lines){
  sChar *file = new sChar(own_dir);
  file->cat("#bat#");
  file->cat(j->group_name->c_str());
  file->cat("#");
  file->cat(j->exec_name->c_str());
  file->cat(".csv");

  Lines->Clear();
  char *w_str;
  char *str;
  CsvInf_T *csvinf = new CsvInf_T(file->c_str());
  if (csvinf->CsvError_Rd() == 0){
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
        if (w_str == MM_NULL){Lines->Add("");}
        else                 {Lines->Add(w_str);}
        continue;
      }
    }
  }

  listDelete();
  sChar *w;
  ARY_LOOP(w,sChar,j->job_file_fp)
    fileStore(w->c_str());
  LOOP_END
  batFileGen(Lines);
  batExec();
  delete csvinf;
}

// # ファイル名登録 #####
void ShellExec_T::jobFileStore(char *file){
  sChar *w = new sChar(file);
  job_file_fp->mem_alloc((unsigned char *)w);
}
