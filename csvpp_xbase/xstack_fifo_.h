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
// Variable length Area stack&fifo class
//

#ifndef _XAREA_STACK_H_
#define _XAREA_STACK_H_

#include "t_area_instance.h"

class VariableStack_C : public VariableStack_T<unsigned char>{
  public:
    VariableStack_C(int size): VariableStack_T<unsigned char>(size){delFlag=false;}
};

class VariableFifo_C : public VariableFifo_T<unsigned char>{
  public:
    VariableFifo_C(int size): VariableFifo_T<unsigned char>(size){delFlag=false;}
};

#endif // _XAREA_H_

