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
