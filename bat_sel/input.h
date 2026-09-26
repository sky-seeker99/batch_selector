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

#ifndef inputH
#define inputH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

#include "../csvpp_xbase/xcsvi_anl.h"

class TForm3 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TPanel *Panel1;
	TPanel *Panel2;
	TButton *Button1;
	TButton *Button2;
	TEdit *in;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
    void __fastcall inKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// ユーザー宣言
    void Store();
public:		// ユーザー宣言
	__fastcall TForm3(TComponent* Owner);
	int commandKbn;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
