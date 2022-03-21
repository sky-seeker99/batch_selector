object Form4: TForm4
  Left = 0
  Top = 0
  Caption = 'Job Queue Menu'
  ClientHeight = 348
  ClientWidth = 535
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 142
    Width = 535
    Height = 7
    Cursor = crVSplit
    Align = alTop
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 535
    Height = 142
    Align = alTop
    TabOrder = 0
    object list: TListBox
      Left = 1
      Top = 1
      Width = 533
      Height = 140
      Align = alClient
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Style = []
      ItemHeight = 13
      MultiSelect = True
      ParentFont = False
      TabOrder = 0
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 149
    Width = 535
    Height = 199
    Align = alClient
    TabOrder = 1
    object dummy: TMemo
      Left = 1
      Top = 1
      Width = 533
      Height = 197
      Align = alClient
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Style = []
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
    end
  end
  object color_diag: TColorDialog
    Left = 358
    Top = 68
  end
  object font_diag: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Left = 418
    Top = 52
  end
  object PopupMenu1: TPopupMenu
    Left = 112
    Top = 54
    object jobtimerstart1: TMenuItem
      Caption = 'job timer start'
      OnClick = jobtimerstart1Click
    end
    object jobtimerstop1: TMenuItem
      Caption = 'job timer stop'
      OnClick = jobtimerstop1Click
    end
    object jobdelete2: TMenuItem
      Caption = 'job delete'
      OnClick = jobdelete2Click
    end
    object jobexec1: TMenuItem
      Caption = 'job exec'
      OnClick = jobexec1Click
    end
    object jobdelete1: TMenuItem
      Caption = 'job all delete'
      OnClick = jobdelete1Click
    end
    object color_top: TMenuItem
      Caption = 'color up'
      OnClick = color_topClick
    end
    object colordown1: TMenuItem
      Caption = 'color down'
      OnClick = colordown1Click
    end
    object fontup1: TMenuItem
      Caption = 'font up'
      OnClick = fontup1Click
    end
    object fontdown1: TMenuItem
      Caption = 'font down'
      OnClick = fontdown1Click
    end
  end
end
