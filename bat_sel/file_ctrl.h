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

#ifndef _FILE_CTRL_H_
#define _FILE_CTRL_H_

#include <stdio.h>
#include "../csvpp_xbase/xarea.h"
#include "../csvpp_xbase/xread64.h"
#include "../csvpp_xbase/xwrite64.h"
#include "../csvpp_xbase/xcsvi_anl.h"


class file_ctrl_C{
  public:
    sChar      *own_dir;
    sChar      *top_file;
    sChar      *bat_file;
    Write64_C  *wr_fp;
    Write64_C  *tag_wr_fp;
    sChar      *tag_name;
	CsvAnl_C   *csvi;
	Read64_C   *r_fp;
    VariableArray_C *group_fp;

    file_ctrl_C(char *p){
      own_dir = new sChar(p);
      top_file = new sChar("");
      bat_file = new sChar("");
      tag_name = new sChar("");
      csvi     = new CsvAnl_C;
      r_fp     = NULL; 
      wr_fp    = NULL;
      tag_wr_fp= NULL;
      group_fp = new VariableArray_C(10);
    }
    ~file_ctrl_C(){
      delete own_dir;
      delete top_file;
      delete bat_file;
      delete tag_name;
      delete csvi;
      if (r_fp      != NULL){delete r_fp;}
      if (wr_fp     != NULL){delete wr_fp;}
      if (tag_wr_fp != NULL){delete tag_wr_fp;}
      sChar *s_str;
      ARY_LOOP(s_str,sChar,group_fp)
        delete s_str;
      LOOP_END
      delete group_fp;
    }
	char *getGroup(int idx){
	  sChar *s = (sChar *)group_fp->get(idx);
	  if (s == NULL){return(NULL);}
      return(s->c_str());
    }


	void read(char *read_file);
	void Write_begin(char *save_file);
    void Write_end();
    void Write(char *tag_name);
    void write_sub(char *tag_name,char *bat_name);

};

#endif // _FILE_CTRL_H_
