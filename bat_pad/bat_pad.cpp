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

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("_bat_pad.cpp", pad_Form1);
USEFORM("_edit.cpp", pad_Form2);
//---------------------------------------------------------------------------
HINSTANCE g_hinst;
HANDLE mx;

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE hInstance, LPSTR, int)
{
	int cnt = ParamCount();
 	if (cnt == 0) {return 1;}
	AnsiString cmd = ParamStr(1);
	char *w = cmd.c_str();

	int len = strlen(w);
	for(int i=0;i<len;i++){
      if ((*(w+i) == '\\') || (*(w+i) == ' ')){
		*(w+i) = '_';
	  }
	}
	mx = CreateMutex( NULL, true, w );
	if( GetLastError() ){return 1;}

	try
	{
		Application->Initialize();
		SetWindowLong(Application->Handle,GWL_EXSTYLE,GetWindowLong(Application->Handle, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		g_hinst = hInstance;
		Application->CreateForm(__classid(Tpad_Form1), &pad_Form1);
         Application->CreateForm(__classid(Tpad_Form2), &pad_Form2);
         Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	ReleaseMutex( mx );
	return 0;
}
//---------------------------------------------------------------------------
