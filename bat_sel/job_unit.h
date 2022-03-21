//---------------------------------------------------------------------------

#ifndef job_unitH
#define job_unitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
#include "../csvpp_xbase/xcsvi_anl.h"
#include "../csvpp_xbase/zschar.h"

class TForm4 : public TForm
{
__published:	// IDE 管理のコンポーネント
    TPanel *Panel2;
    TPanel *Panel3;
    TMemo *dummy;
    TListBox *list;
    TSplitter *Splitter1;
    TColorDialog *color_diag;
    TFontDialog *font_diag;
    TPopupMenu *PopupMenu1;
    TMenuItem *color_top;
    TMenuItem *colordown1;
    TMenuItem *fontup1;
    TMenuItem *fontdown1;
    TMenuItem *jobdelete1;
    TMenuItem *jobtimerstop1;
    TMenuItem *jobtimerstart1;
    TMenuItem *jobexec1;
    TMenuItem *jobdelete2;
    void __fastcall color_topClick(TObject *Sender);
    void __fastcall colordown1Click(TObject *Sender);
    void __fastcall fontup1Click(TObject *Sender);
    void __fastcall fontdown1Click(TObject *Sender);
    void __fastcall jobdelete1Click(TObject *Sender);
    void __fastcall jobtimerstop1Click(TObject *Sender);
    void __fastcall jobtimerstart1Click(TObject *Sender);
    void __fastcall jobexec1Click(TObject *Sender);
    void __fastcall jobdelete2Click(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	char cmdLine[4096];
    __fastcall TForm4(TComponent* Owner);
    __fastcall ~TForm4();
    sChar *config_file;
	void confFileRead();
	void confFileWrite();


};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
