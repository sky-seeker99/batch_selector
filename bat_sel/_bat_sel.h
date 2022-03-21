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

#ifndef _bat_selH
#define _bat_selH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>

// tray ---
#define MYWM_NOTIFY         (WM_APP+100)
#define IDC_MYICON                     1006
extern HINSTANCE g_hinst;
LRESULT IconDrawItem(LPDRAWITEMSTRUCT lpdi);
// tray ---

#define EXTPAD 1


//---------------------------------------------------------------------------

class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TPanel *Panel1;
	TPanel *Panel2;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TSpeedButton *SpeedButton3;
	TSpeedButton *SpeedButton4;
	TSpeedButton *SpeedButton5;
	TCheckBox *bat_gen;
	TSpeedButton *SpeedButton8;
	TComboBox *group_name;
	TSaveDialog *save_di;
	TOpenDialog *open_di;
	TColorDialog *color_diag;
	TFontDialog *font_diag;
	TPopupMenu *PopupMenu1;
	TPopupMenu *tray_pop;
	TMenuItem *execute1;
	TMenuItem *rename1;
	TMenuItem *insert1;
	TMenuItem *stay;
	TMenuItem *para;
	TMenuItem *shell;
	TMenuItem *font1;
	TMenuItem *backcolor1;
	TMenuItem *loadfile1;
	TMenuItem *savefile1;
	TMenuItem *version1;
	TMenuItem *Show1;
	TMenuItem *Exit1;
	TMenuItem *batchpad1;
	TTimer *timer;
    TTimer *job_timer;
    TMenuItem *jobMenu;
    TMenuItem *jobFlg;
    TPanel *list_panel;
    TImage *Image1;
    TListBox *list;
    TPanel *edit_panel;
    TMemo *memo;
    TSplitter *sp1;
    TSpeedButton *sb6;
    TMenuItem *hideFlag;
    TMenuItem *PositionReset1;
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall version1Click(TObject *Sender);
	void __fastcall stayClick(TObject *Sender);
	void __fastcall insert1Click(TObject *Sender);
	void __fastcall font1Click(TObject *Sender);
	void __fastcall backcolor1Click(TObject *Sender);
	void __fastcall SpeedButton4Click(TObject *Sender);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall SpeedButton3Click(TObject *Sender);
	void __fastcall SpeedButton8Click(TObject *Sender);
	void __fastcall rename1Click(TObject *Sender);
	void __fastcall listClick(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall execute1Click(TObject *Sender);
	void __fastcall listDblClick(TObject *Sender);
	void __fastcall paraClick(TObject *Sender);
	void __fastcall shellClick(TObject *Sender);
	void __fastcall group_nameChange(TObject *Sender);
	void __fastcall SpeedButton5Click(TObject *Sender);
	void __fastcall loadfile1Click(TObject *Sender);
	void __fastcall savefile1Click(TObject *Sender);
	void __fastcall Show1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall batchpad1Click(TObject *Sender);
	void __fastcall timerTimer(TObject *Sender);
    void __fastcall listKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall job_timerTimer(TObject *Sender);
    void __fastcall jobMenuClick(TObject *Sender);
    void __fastcall jobFlgClick(TObject *Sender);
    void __fastcall memoChange(TObject *Sender);
    void __fastcall sb6Click(TObject *Sender);
    void __fastcall hideFlagClick(TObject *Sender);
    void __fastcall PositionReset1Click(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
	__fastcall ~TForm1();
	char cmdLine[4096];
	sChar *config_file;
	sChar *own_dir;
	sChar *group;
	file_ctrl_C *f_ctl;
	void __fastcall WMDropFiles(TWMDropFiles & Msg);
    void __fastcall WMCopy(TMessage &Msg);
	void confFileRead();
	void confFileWrite();
	bool confFileRead_old_chk();
	void listRead();
	void listWrite();
	void edit_read();
    void pad_confFileWrite(char *label,char *conf_file,char *list_file);
	sChar *pad_file;
	MEM_FP *pad_fp;
	MEM_FP *pad_init_fp;
    MemHeader_T *it;
    CsvAnl_C *csvi;
    void pad_exec(char *str);
    bool pad_check(char *file);
    bool g_Visible;
    bool memo_dd_flg;
    bool ownKillFlag;

    sChar *inh_name;

    int job_num;
    void exec_rtn();
    void kill_rtn();
    void job_list_rtn();
    void kill_all_rtn();
    void wait_name_rtn();
    void config_rtn();
    void config_write_rtn();
    void jobConsole();
	// tray ---
	void __fastcall DrawItem(TMessage& Msg);
	void __fastcall MyNotify(TMessage& Msg);
	bool __fastcall TrayMessage(DWORD dwMessage);
	HICON __fastcall IconHandle(void);
	void __fastcall ToggleState(void);
	PSTR __fastcall TipText(void);
	// tray ---


    // editor ---
	void contextWrite();
	void contextRead(char *new_file);
	void contextRename(char *p_file);
	void CsvWrite(FILE *out_fp,char *str);
	void contextWrite_dd(char *p_file,char *in_file);
	sChar *sv_file;
	bool memoChg;
    int sv_width;
    int sv_width2;
    // editor ---

	// file save/load  ---
	void fileAllLoad(char *file);
	// file save/load  ---


	// Windows 終了時のルーチン
	void __fastcall	WMQueryEndSession(TWMQueryEndSession &WMQES);	//これがイベントハンドラになる

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WMDropFiles)
		MESSAGE_HANDLER(WM_DRAWITEM,TMessage,DrawItem)
		MESSAGE_HANDLER(MYWM_NOTIFY,TMessage,MyNotify)
		MESSAGE_HANDLER(WM_QUERYENDSESSION,TWMQueryEndSession,WMQueryEndSession)
        VCL_MESSAGE_HANDLER(WM_COPYDATA, TMessage, WMCopy)
	END_MESSAGE_MAP(TForm)

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
