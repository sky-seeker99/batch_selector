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

#include "file_ctrl.h"

void file_ctrl_C::read(char *read_file){
  r_fp = new Read64_C(read_file,"rt",10240);
  if (r_fp->okCheck() == false){
	delete r_fp;
	r_fp = NULL;
	return;
  }

  if (wr_fp     != NULL){delete wr_fp;}
  if (tag_wr_fp != NULL){delete tag_wr_fp;}
  wr_fp = NULL;
  tag_wr_fp = NULL;

  sChar *s_str;
  ARY_LOOP(s_str,sChar,group_fp)
    delete s_str;
  LOOP_END
  group_fp->clear();

  tag_name->set("");
  TEXT_READ_LOOP(r_fp,str)

	csvi->Exec(str);

	char *tag  = NULL;
	char *body = NULL;
	CSV_LOOP_NOCHK(csvi,w_str,NON_SP_PUSH)
	  if      (tag  == NULL){tag  = (char *)w_str;}
	  else if (body == NULL){body = (char *)w_str;}
	LOOP_END

	if (tag == NULL){continue;}
	sp_push(tag);
	sp_push2(tag);

	if (strcmp(tag,"#tag_begin") == 0){
	  if (body != NULL){
		s_str = new sChar(body);
		group_fp->mem_alloc((unsigned char *)s_str);

		tag_name->set(body);
		top_file->set(own_dir);
		top_file->cat("#group#");
		top_file->cat(tag_name->c_str());
		top_file->cat(".csv");
		if (tag_wr_fp != NULL){delete tag_wr_fp;}
		tag_wr_fp = new Write64_C(top_file->c_str(),"wt",10240);
	  }
	}
	else if (strcmp(tag,"#tag_end") == 0){
	  if (tag_wr_fp != NULL){delete tag_wr_fp;}
	  tag_wr_fp = NULL;
	}
	else if (strcmp(tag,"#items_begin") == 0){
	  if ((body != NULL) && (tag_wr_fp != NULL)){
		tag_wr_fp->write("#items,");
		tag_wr_fp->writeln(body);
		bat_file->set(own_dir);
		bat_file->cat("#bat#");
		bat_file->cat(tag_name->c_str());
		bat_file->cat("#");
		bat_file->cat(body);
        bat_file->cat(".csv");
        if (wr_fp     != NULL){delete wr_fp;}
		wr_fp = new Write64_C(bat_file->c_str(),"wt",10240);
      }
      continue;
    }
    else if (strcmp(tag,"#items_end") == 0){
      if (wr_fp != NULL){delete wr_fp;}
      wr_fp = NULL;
    }
    else if (strcmp(tag,"#context") == 0){
      if (wr_fp != NULL){
        wr_fp->write("#context,");
        if (body != NULL){wr_fp->csvWrite(body);}
        wr_fp->writeln("");
      }
    }
  LOOP_END

  if (wr_fp     != NULL){delete wr_fp;}
  if (tag_wr_fp != NULL){delete tag_wr_fp;}
  if (r_fp      != NULL){delete r_fp;}
  wr_fp = NULL;
  tag_wr_fp = NULL;
  r_fp = NULL;
}


void file_ctrl_C::Write_begin(char *save_file){
  wr_fp = new Write64_C(save_file,"wt",10240);
  if (wr_fp->okCheck() == false){
    delete wr_fp;
    wr_fp = NULL;
    return;
  }
}

void file_ctrl_C::Write_end(){
  if (wr_fp     != NULL){delete wr_fp;}
  wr_fp = NULL;
}

void file_ctrl_C::Write(char *tag_name){
  if (wr_fp == NULL){return;}
  top_file->set(own_dir);
  top_file->cat("#group#");
  top_file->cat(tag_name);
  top_file->cat(".csv");

  if (r_fp      != NULL){delete r_fp;}
  r_fp = new Read64_C(top_file->c_str(),"rt",1024);
  if (r_fp->okCheck() == false){
    delete r_fp;
    r_fp = NULL;
    return;
  }

  VariableArray_C *ary_fp = new VariableArray_C(10);
  for(;;){
    char *str = r_fp->read();
    if (str == NULL){break;}
    if (mem_cmp(str,"#items,",7) != 0){continue;}
    sChar *s_str = new sChar(str+7);
    ary_fp->mem_alloc((unsigned char *)s_str);
  }
  delete r_fp;
  r_fp = NULL;


  wr_fp->write("#tag_begin,"); 
  wr_fp->csvWrite(tag_name); 
  wr_fp->writeln("");
  sChar *s_str;
  ARY_LOOP(s_str,sChar,ary_fp)
    write_sub(tag_name,s_str->c_str());
  LOOP_END
  wr_fp->writeln("#tag_end");


  ARY_LOOP(s_str,sChar,ary_fp)
    delete s_str;
  LOOP_END
  delete ary_fp;


}

void file_ctrl_C::write_sub(char *tag_name,char *bat_name){
  bat_file->set(own_dir);
  bat_file->cat("#bat#");
  bat_file->cat(tag_name);
  bat_file->cat("#");
  bat_file->cat(bat_name);
  bat_file->cat(".csv");
  
  if (r_fp      != NULL){delete r_fp;}
  r_fp = new Read64_C(bat_file->c_str(),"rt",1024);
  if (r_fp->okCheck() == false){
    delete r_fp;
    r_fp = NULL;
    return;
  }

  wr_fp->write("#items_begin,"); 
  wr_fp->csvWrite(bat_name); 
  wr_fp->writeln("");

  TEXT_READ_LOOP(r_fp,str)
    csvi->Exec(str);

    char *tag  = NULL;
    char *body = NULL;
    CSV_LOOP_NOCHK(csvi,w_str,NON_SP_PUSH) 
      if      (tag  == NULL){tag  = (char *)w_str;}
      else if (body == NULL){body = (char *)w_str;}
    LOOP_END

    if (tag == NULL){continue;}
    sp_push(tag);
    sp_push2(tag);

    if (strcmp(tag,"#context") != 0){continue;}

    wr_fp->write("#context,");
    if (body != NULL){wr_fp->csvWrite(body);}
    wr_fp->writeln("");
  LOOP_END

  wr_fp->writeln("#items_end\n");

  delete r_fp;
  r_fp = NULL;
}


