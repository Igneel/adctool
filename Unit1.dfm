object Form1: TForm1
  Left = 208
  Top = 132
  Width = 687
  Height = 540
  Caption = 'Измерение фотопроводимости и фотомагнитного эффекта'
  Color = clBtnFace
  Constraints.MaxHeight = 540
  Constraints.MaxWidth = 687
  Constraints.MinHeight = 517
  Constraints.MinWidth = 667
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PC: TPageControl
    Left = 0
    Top = 0
    Width = 657
    Height = 441
    ActivePage = FirstTab
    MultiLine = True
    TabOrder = 0
    object FirstTab: TTabSheet
      Caption = 'FirstTab'
      object Label1: TLabel
        Left = 400
        Top = 8
        Width = 48
        Height = 13
        Caption = 'Драйвер:'
      end
      object Label2: TLabel
        Left = 472
        Top = 16
        Width = 45
        Height = 13
        Caption = 'Частота:'
      end
      object Label3: TLabel
        Left = 400
        Top = 88
        Width = 27
        Height = 13
        Caption = 'DMA:'
      end
      object Label4: TLabel
        Left = 472
        Top = 96
        Width = 75
        Height = 13
        Caption = 'Размер блока:'
      end
      object Label5: TLabel
        Left = 400
        Top = 128
        Width = 19
        Height = 13
        Caption = 'IRQ'
      end
      object Label6: TLabel
        Left = 400
        Top = 176
        Width = 43
        Height = 26
        Alignment = taCenter
        Caption = 'Каналы: Первый:'
        WordWrap = True
      end
      object Label7: TLabel
        Left = 400
        Top = 232
        Width = 39
        Height = 13
        Caption = 'Второй:'
      end
      object Label8: TLabel
        Left = 400
        Top = 48
        Width = 34
        Height = 13
        Caption = 'Адрес:'
      end
      object Chart1: TChart
        Left = 0
        Top = 0
        Width = 393
        Height = 329
        AnimatedZoom = True
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          'TChart')
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object Series1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series3: TLineSeries
          Marks.ArrowLength = 0
          Marks.Visible = False
          SeriesColor = clYellow
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series4: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object Series5: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlack
          LinePen.Width = 3
          Pointer.HorizSize = 5
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.VertSize = 5
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object Memo1: TMemo
        Left = 424
        Top = 272
        Width = 217
        Height = 129
        ScrollBars = ssVertical
        TabOrder = 1
      end
      object uiControl: TButton
        Left = 472
        Top = 176
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 2
        OnClick = uiControlClick
      end
      object uiDriverName: TComboBox
        Left = 400
        Top = 24
        Width = 65
        Height = 21
        ItemHeight = 13
        TabOrder = 3
        Text = 'LAVP'
        OnChange = uiDriverNameChange
      end
      object uiDRQ: TComboBox
        Left = 400
        Top = 104
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 4
        OnChange = uiDRQChange
      end
      object Chan1: TComboBox
        Left = 400
        Top = 208
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 5
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6')
      end
      object Chan2: TComboBox
        Left = 400
        Top = 248
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 6
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7')
      end
      object uiBase: TComboBox
        Left = 400
        Top = 64
        Width = 65
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 7
        OnChange = uiBaseChange
      end
      object Button1: TButton
        Left = 472
        Top = 208
        Width = 75
        Height = 25
        Caption = 'Матрицы'
        TabOrder = 8
        OnClick = Button1Click
      end
      object uiFreq: TTrackBar
        Left = 472
        Top = 32
        Width = 73
        Height = 25
        Orientation = trHorizontal
        Frequency = 1
        Position = 0
        SelEnd = 0
        SelStart = 0
        TabOrder = 9
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = uiFreqChange
      end
      object uiBlockSize: TTrackBar
        Left = 472
        Top = 112
        Width = 73
        Height = 25
        Orientation = trHorizontal
        Frequency = 1
        Position = 0
        SelEnd = 0
        SelStart = 0
        TabOrder = 10
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = uiBlockSizeChange
      end
      object uiBlockSize2: TEdit
        Left = 472
        Top = 144
        Width = 65
        Height = 21
        TabOrder = 11
        Text = '1024'
        OnChange = uiBlockSize2Change
        OnKeyPress = Edit1KeyPress
      end
      object uiFrenq: TEdit
        Left = 472
        Top = 64
        Width = 65
        Height = 21
        TabOrder = 12
        Text = '200000'
        OnChange = uiFrenqChange
        OnKeyPress = Edit1KeyPress
      end
      object uiIRQ: TComboBox
        Left = 400
        Top = 144
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 13
        OnChange = uiIRQChange
      end
      object Button3: TButton
        Left = 472
        Top = 240
        Width = 75
        Height = 25
        Caption = 'Clear'
        TabOrder = 14
        OnClick = Button3Click
      end
      object Button2: TButton
        Left = 560
        Top = 176
        Width = 75
        Height = 25
        Caption = 'пФурье'
        Enabled = False
        TabOrder = 15
      end
      object Memo6: TMemo
        Left = 0
        Top = 336
        Width = 289
        Height = 73
        ScrollBars = ssVertical
        TabOrder = 16
      end
      object Button4: TButton
        Left = 560
        Top = 208
        Width = 75
        Height = 25
        Caption = 'Не нажимать!'
        TabOrder = 17
        OnClick = Button4Click
      end
      object Button10: TButton
        Left = 312
        Top = 352
        Width = 75
        Height = 25
        Caption = 'Button10'
        TabOrder = 18
        OnClick = Button10Click
      end
      object Edit2: TEdit
        Left = 560
        Top = 136
        Width = 81
        Height = 21
        TabOrder = 19
        Text = '2'
      end
    end
    object Resistance: TTabSheet
      Caption = 'Сопротивление'
      ImageIndex = 1
      object Label9: TLabel
        Left = 384
        Top = 8
        Width = 95
        Height = 13
        Caption = 'Протекающий ток:'
      end
      object Label15: TLabel
        Left = 440
        Top = 96
        Width = 82
        Height = 13
        Alignment = taCenter
        Caption = 'Длина фильтра:'
      end
      object Label19: TLabel
        Left = 432
        Top = 120
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = 'Частота дискрет:'
      end
      object Label20: TLabel
        Left = 432
        Top = 144
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = 'Частота пропуск:'
      end
      object Label21: TLabel
        Left = 424
        Top = 168
        Width = 99
        Height = 13
        Alignment = taCenter
        Caption = 'Частота затухания:'
      end
      object ChartResist: TChart
        Left = 8
        Top = 8
        Width = 369
        Height = 313
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          'График магнитосопротивления')
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object SeriesRes1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesRes2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFFTRes: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object out1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clYellow
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object uiResControl: TButton
        Left = 448
        Top = 200
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 1
        OnClick = uiControlClick
      end
      object CurrentRes: TEdit
        Left = 384
        Top = 24
        Width = 97
        Height = 21
        TabOrder = 2
        Text = '1'
        OnKeyPress = Edit1KeyPress
      end
      object uiResFeat: TButton
        Left = 448
        Top = 232
        Width = 75
        Height = 25
        Caption = 'Подгонка'
        TabOrder = 3
        OnClick = uiResFeatClick
      end
      object Memo2: TMemo
        Left = 416
        Top = 264
        Width = 185
        Height = 89
        TabOrder = 4
      end
      object ResCurveIndex: TRadioGroup
        Left = 488
        Top = 24
        Width = 153
        Height = 57
        Caption = 'Номер кривой:'
        ItemIndex = 0
        Items.Strings = (
          'Первая (прямой ток)'
          'Вторая (обратный ток)')
        TabOrder = 5
      end
      object uiFFTRes: TButton
        Left = 528
        Top = 232
        Width = 75
        Height = 25
        Caption = 'Фильтр'
        TabOrder = 6
        OnClick = uiFFTResClick
      end
      object Lfilter1: TEdit
        Left = 528
        Top = 96
        Width = 73
        Height = 21
        TabOrder = 7
        Text = '20'
        OnKeyPress = Edit1KeyPress
      end
      object Fd1: TEdit
        Left = 528
        Top = 120
        Width = 73
        Height = 21
        TabOrder = 8
        Text = '5000'
        OnKeyPress = Edit1KeyPress
      end
      object Fp1: TEdit
        Left = 528
        Top = 144
        Width = 73
        Height = 21
        TabOrder = 9
        Text = '15'
        OnKeyPress = Edit1KeyPress
      end
      object Fz1: TEdit
        Left = 528
        Top = 168
        Width = 73
        Height = 21
        TabOrder = 10
        Text = '25'
        OnKeyPress = Edit1KeyPress
      end
      object Button5: TButton
        Left = 296
        Top = 328
        Width = 107
        Height = 25
        Caption = 'Очистить графики'
        TabOrder = 11
        OnClick = Button5Click
      end
      object Interval1: TEdit
        Left = 48
        Top = 344
        Width = 49
        Height = 21
        TabOrder = 12
        Text = '0,02'
        OnKeyPress = Edit1KeyPress
      end
      object Interval2: TEdit
        Left = 104
        Top = 344
        Width = 49
        Height = 21
        TabOrder = 13
        Text = '0,15'
        OnKeyPress = Edit1KeyPress
      end
      object ImpulsKiller: TButton
        Left = 48
        Top = 368
        Width = 105
        Height = 25
        Caption = 'Убрать всплеск'
        TabOrder = 14
        OnClick = ImpulsKillerClick
      end
      object Button9: TButton
        Left = 168
        Top = 368
        Width = 75
        Height = 25
        Caption = 'х10'
        TabOrder = 15
        OnClick = Button9Click
      end
      object Button11: TButton
        Left = 344
        Top = 368
        Width = 129
        Height = 25
        Caption = 'Оптимальный фильтр'
        Enabled = False
        TabOrder = 16
        OnClick = Button11Click
      end
      object Button12: TButton
        Left = 504
        Top = 368
        Width = 121
        Height = 25
        Caption = 'Увел кол-ва точек'
        Enabled = False
        TabOrder = 17
        OnClick = Button12Click
      end
      object Button13: TButton
        Left = 256
        Top = 368
        Width = 75
        Height = 25
        Caption = 'Оценить шум'
        TabOrder = 18
        OnClick = Button13Click
      end
    end
    object HallEffect: TTabSheet
      Caption = 'Эффект Холла'
      ImageIndex = 2
      object Label11: TLabel
        Left = 384
        Top = 8
        Width = 95
        Height = 13
        Caption = 'Протекающий ток:'
      end
      object Label16: TLabel
        Left = 440
        Top = 96
        Width = 82
        Height = 13
        Alignment = taCenter
        Caption = 'Длина фильтра:'
      end
      object Label22: TLabel
        Left = 432
        Top = 120
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = 'Частота дискрет:'
      end
      object Label23: TLabel
        Left = 432
        Top = 144
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = 'Частота пропуск:'
      end
      object Label24: TLabel
        Left = 424
        Top = 168
        Width = 99
        Height = 13
        Alignment = taCenter
        Caption = 'Частота затухания:'
      end
      object ChartHall: TChart
        Left = 8
        Top = 8
        Width = 369
        Height = 313
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          'TChart')
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object SeriesHall1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesHall2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFFTHall: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object out2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clYellow
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object HallCurveIndex: TRadioGroup
        Left = 488
        Top = 24
        Width = 153
        Height = 57
        Caption = 'Номер кривой:'
        ItemIndex = 0
        Items.Strings = (
          'Первая (прямой ток)'
          'Вторая (обратный ток)')
        TabOrder = 1
      end
      object uiHallFeat: TButton
        Left = 448
        Top = 232
        Width = 75
        Height = 25
        Caption = 'Подгонка'
        TabOrder = 2
        OnClick = uiHallFeatClick
      end
      object uiHallControl: TButton
        Left = 448
        Top = 200
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 3
        OnClick = uiControlClick
      end
      object Memo3: TMemo
        Left = 416
        Top = 264
        Width = 185
        Height = 89
        TabOrder = 4
      end
      object CurrentHall: TEdit
        Left = 384
        Top = 24
        Width = 97
        Height = 21
        TabOrder = 5
        Text = '1'
        OnKeyPress = Edit1KeyPress
      end
      object uiFFTHall: TButton
        Left = 528
        Top = 232
        Width = 75
        Height = 25
        Caption = 'Фильтр'
        TabOrder = 6
        OnClick = uiFFTHallClick
      end
      object Button6: TButton
        Left = 296
        Top = 328
        Width = 107
        Height = 25
        Caption = 'Очистить графики'
        TabOrder = 7
        OnClick = Button6Click
      end
      object Lfilter2: TEdit
        Left = 528
        Top = 96
        Width = 73
        Height = 21
        TabOrder = 8
        Text = '20'
        OnKeyPress = Edit1KeyPress
      end
      object Fd2: TEdit
        Left = 528
        Top = 120
        Width = 73
        Height = 21
        TabOrder = 9
        Text = '5000'
        OnKeyPress = Edit1KeyPress
      end
      object Fp2: TEdit
        Left = 528
        Top = 144
        Width = 73
        Height = 21
        TabOrder = 10
        Text = '15'
        OnKeyPress = Edit1KeyPress
      end
      object Fz2: TEdit
        Left = 528
        Top = 168
        Width = 73
        Height = 21
        TabOrder = 11
        Text = '25'
        OnKeyPress = Edit1KeyPress
      end
    end
    object Faradey: TTabSheet
      Caption = 'Фопроводимость (Фарадей) '
      ImageIndex = 3
      object Label12: TLabel
        Left = 384
        Top = 8
        Width = 95
        Height = 13
        Caption = 'Протекающий ток:'
      end
      object Label13: TLabel
        Left = 384
        Top = 48
        Width = 144
        Height = 13
        Caption = 'Коэффициент усиления(mV):'
      end
      object Label17: TLabel
        Left = 440
        Top = 96
        Width = 82
        Height = 13
        Alignment = taCenter
        Caption = 'Длина фильтра:'
      end
      object Label25: TLabel
        Left = 432
        Top = 120
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = 'Частота дискрет:'
      end
      object Label26: TLabel
        Left = 432
        Top = 144
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = 'Частота пропуск:'
      end
      object Label27: TLabel
        Left = 424
        Top = 168
        Width = 99
        Height = 13
        Alignment = taCenter
        Caption = 'Частота затухания:'
      end
      object ChartFaradey: TChart
        Left = 8
        Top = 8
        Width = 369
        Height = 313
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          'TChart')
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object SeriesFaradey1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFaradey2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFFTFaradey: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object out3: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clYellow
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object CurrentFaradey: TEdit
        Left = 384
        Top = 24
        Width = 97
        Height = 21
        TabOrder = 1
        Text = '1'
        OnKeyPress = Edit1KeyPress
      end
      object GainKoefFaradey: TComboBox
        Left = 384
        Top = 64
        Width = 97
        Height = 21
        ItemHeight = 13
        TabOrder = 2
        Items.Strings = (
          '30'
          '10'
          '3'
          '1'
          '0.3'
          '0.1')
      end
      object FaradeyCurveIndex: TRadioGroup
        Left = 488
        Top = 24
        Width = 153
        Height = 57
        Caption = 'Номер кривой:'
        ItemIndex = 0
        Items.Strings = (
          'Первая (прямой ток)'
          'Вторая (обратный ток)')
        TabOrder = 3
      end
      object uiFaradeyControl: TButton
        Left = 448
        Top = 200
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 4
        OnClick = uiControlClick
      end
      object uiFaradeyFeat: TButton
        Left = 448
        Top = 232
        Width = 75
        Height = 25
        Caption = 'Подгонка'
        TabOrder = 5
        OnClick = uiFaradeyFeatClick
      end
      object Memo4: TMemo
        Left = 416
        Top = 264
        Width = 185
        Height = 89
        TabOrder = 6
      end
      object uiFFTFaradey: TButton
        Left = 528
        Top = 232
        Width = 75
        Height = 25
        Caption = 'Фильтр'
        TabOrder = 7
        OnClick = uiFFTFaradeyClick
      end
      object Button7: TButton
        Left = 296
        Top = 328
        Width = 107
        Height = 25
        Caption = 'Очистить графики'
        TabOrder = 8
        OnClick = Button7Click
      end
      object Lfilter3: TEdit
        Left = 528
        Top = 96
        Width = 73
        Height = 21
        TabOrder = 9
        Text = '20'
        OnKeyPress = Edit1KeyPress
      end
      object Fd3: TEdit
        Left = 528
        Top = 120
        Width = 73
        Height = 21
        TabOrder = 10
        Text = '5000'
        OnKeyPress = Edit1KeyPress
      end
      object Fp3: TEdit
        Left = 528
        Top = 144
        Width = 73
        Height = 21
        TabOrder = 11
        Text = '15'
        OnKeyPress = Edit1KeyPress
      end
      object Fz3: TEdit
        Left = 528
        Top = 168
        Width = 73
        Height = 21
        TabOrder = 12
        Text = '25'
        OnKeyPress = Edit1KeyPress
      end
    end
    object Foygt: TTabSheet
      Caption = 'Фотопроводимость (Фойгт)'
      ImageIndex = 4
      object Label10: TLabel
        Left = 384
        Top = 48
        Width = 144
        Height = 13
        Caption = 'Коэффициент усиления(mV):'
      end
      object Label14: TLabel
        Left = 384
        Top = 8
        Width = 95
        Height = 13
        Caption = 'Протекающий ток:'
      end
      object Label18: TLabel
        Left = 440
        Top = 96
        Width = 82
        Height = 13
        Alignment = taCenter
        Caption = 'Длина фильтра:'
      end
      object Label28: TLabel
        Left = 432
        Top = 120
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = 'Частота дискрет:'
      end
      object Label29: TLabel
        Left = 432
        Top = 144
        Width = 89
        Height = 13
        Alignment = taCenter
        Caption = 'Частота пропуск:'
      end
      object Label30: TLabel
        Left = 424
        Top = 168
        Width = 99
        Height = 13
        Alignment = taCenter
        Caption = 'Частота затухания:'
      end
      object ChartFoygt: TChart
        Left = 8
        Top = 8
        Width = 369
        Height = 313
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Title.Text.Strings = (
          'TChart')
        Legend.Visible = False
        View3D = False
        TabOrder = 0
        object SeriesFoygt1: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFoygt2: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesFFTFoygt: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clYellow
          Title = 'SeriesFFTFoygt'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object out4: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object GainKoefFoygt: TComboBox
        Left = 384
        Top = 64
        Width = 97
        Height = 21
        ItemHeight = 13
        TabOrder = 1
        Items.Strings = (
          '30'
          '10'
          '3'
          '1'
          '0.3'
          '0.1')
      end
      object CurrentFoygt: TEdit
        Left = 384
        Top = 24
        Width = 97
        Height = 21
        TabOrder = 2
        Text = '1'
        OnKeyPress = Edit1KeyPress
      end
      object FoygtCurveIndex: TRadioGroup
        Left = 488
        Top = 24
        Width = 153
        Height = 57
        Caption = 'Номер кривой:'
        ItemIndex = 0
        Items.Strings = (
          'Первая (прямой ток)'
          'Вторая (обратный ток)')
        TabOrder = 3
      end
      object uiFoygtControl: TButton
        Left = 448
        Top = 200
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 4
        OnClick = uiControlClick
      end
      object uiFoygtFeat: TButton
        Left = 448
        Top = 232
        Width = 75
        Height = 25
        Caption = 'Подгонка'
        TabOrder = 5
        OnClick = uiFoygtFeatClick
      end
      object Memo5: TMemo
        Left = 416
        Top = 264
        Width = 185
        Height = 89
        TabOrder = 6
      end
      object uiFFTFoygt: TButton
        Left = 528
        Top = 232
        Width = 75
        Height = 25
        Caption = 'Фильтр'
        TabOrder = 7
        OnClick = uiFFTFoygtClick
      end
      object Button8: TButton
        Left = 296
        Top = 328
        Width = 107
        Height = 25
        Caption = 'Очистить графики'
        TabOrder = 8
        OnClick = Button8Click
      end
      object Fd4: TEdit
        Left = 528
        Top = 120
        Width = 73
        Height = 21
        TabOrder = 9
        Text = '5000'
        OnKeyPress = Edit1KeyPress
      end
      object Lfilter4: TEdit
        Left = 528
        Top = 96
        Width = 73
        Height = 21
        TabOrder = 10
        Text = '20'
        OnKeyPress = Edit1KeyPress
      end
      object Fp4: TEdit
        Left = 528
        Top = 144
        Width = 73
        Height = 21
        TabOrder = 11
        Text = '15'
        OnKeyPress = Edit1KeyPress
      end
      object Fz4: TEdit
        Left = 528
        Top = 168
        Width = 73
        Height = 21
        TabOrder = 12
        Text = '25'
        OnKeyPress = Edit1KeyPress
      end
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 463
    Width = 671
    Height = 19
    Panels = <
      item
        Text = 'Текущее состояние:'
        Width = 110
      end
      item
        Text = 'Готова к работе. Тестовая версия'
        Width = 182
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object MainMenu1: TMainMenu
    Left = 16
    Top = 16
    object N1: TMenuItem
      Caption = 'Файл'
      object N4: TMenuItem
        Caption = 'Открыть'
        ShortCut = 16463
        OnClick = N4Click
      end
      object N5: TMenuItem
        Caption = 'Сохранить'
        ShortCut = 16467
        OnClick = N5Click
      end
      object N3: TMenuItem
        Caption = 'Выход'
        OnClick = N3Click
      end
    end
    object N8: TMenuItem
      Caption = 'Инструменты'
      object N101: TMenuItem
        Caption = 'Умножить текущий график на 10'
        Enabled = False
      end
      object N9: TMenuItem
        Caption = 'Очистить графики'
        ShortCut = 8238
        OnClick = N9Click
      end
      object N10: TMenuItem
        Caption = 'Убрать всплеск'
        Enabled = False
      end
    end
    object N2: TMenuItem
      Caption = 'Настройки'
      Enabled = False
    end
    object N6: TMenuItem
      Caption = 'Помощь'
      Enabled = False
      object N7: TMenuItem
        Caption = 'О программе'
        ShortCut = 112
      end
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 56
    Top = 24
  end
  object OpenDialog1: TOpenDialog
    Left = 96
    Top = 24
  end
end
