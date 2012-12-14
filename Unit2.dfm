object SaveForm: TSaveForm
  Left = 227
  Top = 171
  BorderStyle = bsDialog
  Caption = '���������� �����������'
  ClientHeight = 445
  ClientWidth = 628
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 176
    Top = 88
    Width = 99
    Height = 13
    Caption = '�������� ��� �� �:'
  end
  object Label2: TLabel
    Left = 16
    Top = 88
    Width = 113
    Height = 13
    Caption = '����������� ������:'
  end
  object Label3: TLabel
    Left = 496
    Top = 32
    Width = 93
    Height = 26
    Alignment = taCenter
    Caption = '������ ����� �� ���������� ����:'
    WordWrap = True
  end
  object SaveBut: TButton
    Left = 96
    Top = 416
    Width = 75
    Height = 25
    Caption = '���������'
    TabOrder = 0
    OnClick = SaveButClick
  end
  object Prepare: TRadioGroup
    Left = 16
    Top = 16
    Width = 225
    Height = 65
    Caption = '������ ����������:'
    ItemIndex = 1
    Items.Strings = (
      '�������� � ������ � ���� ����'
      '�������� � ������ � ������ �����'
      '��� ��������')
    TabOrder = 1
  end
  object Results: TRadioGroup
    Left = 248
    Top = 16
    Width = 161
    Height = 65
    Caption = '����������� ����������:'
    ItemIndex = 1
    Items.Strings = (
      '��������� �������'
      '���� ������')
    TabOrder = 2
    OnClick = ResultsClick
  end
  object SaveMode: TRadioGroup
    Left = 16
    Top = 224
    Width = 273
    Height = 89
    Caption = '����� ����������:'
    ItemIndex = 3
    Items.Strings = (
      '�������� ��� �� �'
      '�������� ��� �� �'
      '�������� �����'
      '��� �����')
    TabOrder = 3
    OnClick = SaveModeClick
  end
  object Memo1: TMemo
    Left = 16
    Top = 320
    Width = 321
    Height = 89
    Lines.Strings = (
      '������� ���� �������� �����/���������� �����������.'
      '���������� �� ����� ������ � ������������ ���������� '
      '���� �������������')
    TabOrder = 5
  end
  object Edit1: TEdit
    Left = 176
    Top = 104
    Width = 121
    Height = 21
    TabOrder = 4
    Text = '0,02'
    OnKeyPress = Edit1KeyPress
  end
  object ComboBox1: TComboBox
    Left = 16
    Top = 104
    Width = 145
    Height = 21
    Hint = 
      '�������� ������ ��� ����������, ����� ����� ������� ��� ��������' +
      '. �������� - �������� ��� ������ �������.'
    Style = csDropDownList
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    TabOrder = 6
    OnChange = ComboBox1Change
    Items.Strings = (
      'Series1'
      'Series2'
      'Series3'
      'Series4'
      'Series5'
      'SeriesRes1'
      'SeriesRes2'
      'SeriesFFTRes'
      'out1'
      'SeriesHall1'
      'SeriesHall2'
      'SeriesFFTHal'
      'out2'
      'SeriesFaradey1'
      'SeriesFaradey2'
      'SeriesFFTFaradey'
      'out3'
      'SeriesFoygt1'
      'SeriesFoygt2'
      'SeriesFFTFoygt'
      'out4')
  end
  object Memo2: TMemo
    Left = 16
    Top = 152
    Width = 321
    Height = 57
    Hint = '�������� ���������� �������'
    Lines.Strings = (
      'Memo2')
    ParentShowHint = False
    ShowHint = True
    TabOrder = 7
  end
  object LB1: TListBox
    Left = 344
    Top = 96
    Width = 129
    Height = 305
    Hint = 
      '�������� ��������� �������� ������ �� ��� ����� ������� ����, ��' +
      '������������ ����� ������� Ctrl �� ����������'
    ItemHeight = 14
    Items.Strings = (
      'Series1'
      'Series2'
      'Series3'
      'Series4'
      'Series5'
      'SeriesRes1'
      'SeriesRes2'
      'SeriesFFTRes'
      'out1'
      'SeriesHall1'
      'SeriesHall2'
      'SeriesFFTHal'
      'out2'
      'SeriesFaradey1'
      'SeriesFaradey2'
      'SeriesFFTFaradey'
      'out3'
      'SeriesFoygt1'
      'SeriesFoygt2'
      'SeriesFFTFoygt'
      'out4')
    MultiSelect = True
    ParentShowHint = False
    ShowHint = True
    Style = lbOwnerDrawFixed
    TabOrder = 8
  end
  object Memo3: TMemo
    Left = 488
    Top = 64
    Width = 129
    Height = 305
    Lines.Strings = (
      '-2'
      '-1'
      '0'
      '1'
      '2')
    ScrollBars = ssVertical
    TabOrder = 9
  end
  object Button1: TButton
    Left = 488
    Top = 376
    Width = 129
    Height = 25
    Caption = '��������� ���������'
    TabOrder = 10
    OnClick = Button1Click
  end
  object PresetPoints: TComboBox
    Left = 472
    Top = 8
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 11
    OnChange = PresetPointsChange
  end
  object Button2: TButton
    Left = 488
    Top = 408
    Width = 129
    Height = 25
    Caption = '������� ������'
    TabOrder = 12
    OnClick = Button2Click
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = '.txt'
    FileName = 'data.'
    Filter = '��������� ����|*.txt'
  end
end
