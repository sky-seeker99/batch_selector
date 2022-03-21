/*

 * Copyright (c) 2009 Shigeru Kasuya (sky_seeker99@users.sourceforge.jp)
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
