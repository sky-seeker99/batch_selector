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
#include <stdio.h>
#include <vcl.h>
#pragma hdrstop

#include "../csvpp_xbase/xarea.h"

//---------------------------------------------------------------------------

#pragma argsused

void send(HWND hwndTarget,char *str){
  COPYDATASTRUCT data;
  data.dwData = 1;
  data.cbData = strlen(str)+ 1;
  data.lpData = (void*)str;
  SendMessage(hwndTarget,WM_COPYDATA,(WPARAM)Application->Handle,(LPARAM)&data);
}

void msg(){
  printf("bat_sel_remote.exe exec job_name group_name bat_label sleep_time\n");
  printf("bat_sel_remote.exe kill job_name\n");
  printf("bat_sel_remote.exe kill_all\n");
  printf("bat_sel_remote.exe wait job_name\n");
  printf("bat_sel_remote.exe own_kill\n");
  printf("bat_sel_remote.exe job_list file_name\n");
  printf("bat_sel_remote.exe config file_name\n");
  printf("bat_sel_remote.exe config_write file_name\n");
  printf("ex)\n");
  printf("bat_sel_remote.exe own_kill\n");
  printf("bat_sel_remote.exe wait job0001\n");
  printf("bat_sel_remote.exe exec job0001 default csvmake 10\n");
  printf("bat_sel_remote.exe kill job0001\n");
  printf("bat_sel_remote.exe kill_all\n");
  printf("bat_sel_remote.exe job_list job_list.txt\n");
  printf("bat_sel_remote.exe config menu_data.csv\n");
  printf("bat_sel_remote.exe config_write menu_data.csv\n");
  exit(1);
}

void sendString(char *str){
  HWND hwndTarget = FindWindow(NULL,"bat_sel");
  if (hwndTarget != NULL) {
    send(hwndTarget,str);
  }
  else {
    printf("Window(bat_sel) not Found!\n");
  }
}


int main(int argc, char* argv[])
{
  char *kind       = NULL;
  char *job_name   = NULL;
  char *group_name = NULL;
  char *exec_name  = NULL;
  char *sleep_time = NULL;
  VariableArea_C *str = new VariableArea_C(256);
  VariableArea_C *prm = new VariableArea_C(256);


  printf("batch selector remote exec Ver 1.05\n");

  for(int i=1;i<argc;i++){
    if (kind       == NULL) {kind   = argv[i]; continue;}
    if (job_name   == NULL) {job_name   = argv[i]; continue;}
    if (group_name == NULL) {group_name = argv[i]; continue;}
    if (exec_name  == NULL) {exec_name  = argv[i]; continue;}
    if (sleep_time == NULL) {sleep_time = argv[i]; continue;}
    prm->cat_string(",");
    prm->cat_string(argv[i]);
  }
  if (kind == NULL) {
    msg();
  }
  if (strcmp(kind,"exec")==0) {
    if (sleep_time == NULL) {
      msg();
    }
    str->set_string("#exec,");
    str->cat_string(job_name);  str->cat_string(",");
    str->cat_string(group_name);  str->cat_string(",");
    str->cat_string(exec_name);  str->cat_string(",");
    str->cat_string(sleep_time);
    str->cat_string(prm->cc_str());
    sendString(str->cc_str());
  }
  if (strcmp(kind,"kill")==0) {
    if (job_name == NULL) {
      msg();
    }
    str->set_string("#kill,");
    str->cat_string(job_name);
    sendString(str->cc_str());
  }
  if (strcmp(kind,"kill_all")==0) {
    str->set_string("#kill_all");
    sendString(str->cc_str());
  }
  if (strcmp(kind,"own_kill")==0) {
    str->set_string("#own_kill");
    sendString(str->cc_str());
  }
  if (strcmp(kind,"wait")==0) {
    if (job_name == NULL) {
      msg();
    }
    str->set_string("#wait_name,");
    str->cat_string(job_name);
    sendString(str->cc_str());
  }
  if (strcmp(kind,"job_list")==0) {
    if (job_name == NULL) {
      msg();
    }
    str->set_string("#job_list,");
    str->cat_string(job_name);
    sendString(str->cc_str());
  }
  if (strcmp(kind,"config")==0) {
    if (job_name == NULL) {
      msg();
    }
    str->set_string("#config,");
    str->cat_string(job_name);
    sendString(str->cc_str());
  }
  if (strcmp(kind,"config_write")==0) {
    if (job_name == NULL) {
      msg();
    }
    str->set_string("#config_write,");
    str->cat_string(job_name);
    sendString(str->cc_str());
  }

	

delete str;
delete prm;
return 0;
}
//---------------------------------------------------------------------------
