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
//---------------------------------------------------------------------------
USEFORM("_bat_sel.cpp", Form1);
USEFORM("input.cpp", Form3);
USEFORM("bat_pad\_bat_pad.cpp", pad_Form1);
USEFORM("bat_pad\_edit.cpp", pad_Form2);
USEFORM("job_unit.cpp", Form4);
//---------------------------------------------------------------------------
HANDLE mx;
HINSTANCE g_hinst;

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE hInstance, LPSTR, int)
{
	mx = CreateMutex( NULL, true, "CCCCCCCCBAT_MENU" );
	if( GetLastError() ){return 1;}

	try
	{
		Application->Initialize();
		SetWindowLong(Application->Handle,GWL_EXSTYLE,GetWindowLong(Application->Handle, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		g_hinst = hInstance;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TForm3), &Form3);
		Application->CreateForm(__classid(TForm4), &Form4);
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
