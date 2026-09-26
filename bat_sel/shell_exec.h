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

//
// shell exec class
//

#ifndef _SHELL_EXEC_H_
#define _SHELL_EXEC_H_

#include "../csvpp_xbase/xarea.h"
#include "../csvpp_xbase/zfilename_anl.h"
#include "../csvpp_xbase/ztemp.h"
#include "../csvpp_xbase/zcsvfile.h"

//#define DEBUG_SH

#define S_JOB_JWAIT 0
#define S_JOB_TWAIT 1
#define S_JOB_RUN  2

// # job class
class jobQueue_T{
  public:



    sChar *job_name;
    int state;
    int sleep_time;
    sChar *group_name;
    sChar *exec_name;
    VariableArray_C *job_file_fp; // cell:sChar
    bool ownKillFlag;

    bool enableFlg;
    sChar *inh_name;

    jobQueue_T(char *p_job_name,char *p_group_name,char *p_exec_name,char *p_sleep_time,char *p_inh_name,bool p_ownKillFlag){
      job_name = new sChar(p_job_name);
      group_name = new sChar(p_group_name);
      exec_name = new sChar(p_exec_name);
      sleep_time = SujiConvEx(p_sleep_time);
      job_file_fp = new VariableArray_C(10);
      ownKillFlag = p_ownKillFlag;
      if (sleep_time < 0) {
        sleep_time = 0;
      }
      if (p_inh_name == NULL){
        state = S_JOB_TWAIT;
        inh_name = NULL;
        enableFlg = true;
      }
      else {
        state = S_JOB_JWAIT;
        inh_name = new sChar(p_inh_name);
        enableFlg = false;
      }
    }
    ~jobQueue_T(){
      sChar *w;
      ARY_LOOP(w,sChar,job_file_fp) delete w; LOOP_END

      if (inh_name != NULL){delete inh_name;}
      delete job_file_fp;
      delete job_name;
      delete group_name;
      delete exec_name;
    }
};

// # shell interface class
class ShellExec_T{
  public:
  VariableArray_C *bat_fp;  // cell:sChar
  VariableArray_C *file_fp; // cell:sChar
  VariableArray_C *job_file_fp; // cell:sChar
  MemHeader_T *it;
  MEM_FP *job_fp;  // cell:jobQueue_T
  bool jobActFlg;

  FileNameSep_T *fn;
  sChar *currFile;
  char cmdLine[10240];
  bool shellFlg;
  bool paraFlg;
  bool fileFlg;

  bool hideFlag;

  bool paraInhFlag;

  // %2 - %9
  VariableArea_C *g_p2;
  VariableArea_C *g_p3;
  VariableArea_C *g_p4;
  VariableArea_C *g_p5;
  VariableArea_C *g_p6;
  VariableArea_C *g_p7;
  VariableArea_C *g_p8;
  VariableArea_C *g_p9;
  int    g_p2p9_cnt;

  // temp file
  tempFileInf_C *temp;

  // file‰»
  sChar *makeFile;
  int    state_makeFile;

  void fileStore(char *file);
  void listDelete();
  void batExec();
  void batFileGen(TStrings *bat_context);
  bool batFileGenLine(Write64_C *fp,char *lines);

  void jobExec(jobQueue_T *j,char *own_dir,TStrings *Lines);
  int  jobExecLoop(char *own_dir,TStrings *Lines);
  bool jobKill(char *job_name);
  bool jobStore(char *own_dir,TStrings *Lines,char *job_name,char *group_name,char *exec_name,char *sleep_time,char *inh_name,bool ownKillFlag);
  void jobFileStore(char *file);
  bool jobWaitCheck(char *inh_name);

  ShellExec_T();
  ~ShellExec_T();
  };

#endif // _SHELL_EXEC_H_
