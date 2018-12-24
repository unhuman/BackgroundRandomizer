; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgAdjustImage
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "backgroundrandomizer.h"
LastPage=0

ClassCount=9
Class1=CBackgroundRandomizerApp
Class2=CAboutDlg
Class3=CBackgroundRandomizerDlg
Class4=CBgRandPopupAbout
Class5=CColorStaticST
Class6=CDlgManageBlockedImages
Class7=CDlgRegister
Class8=CSystemTray

ResourceCount=7
Resource1=IDD_DIALOG_BG_RAND_PARENT
Resource2=IDD_DIALOG_MANAGE_BLOCKED_IMAGES
Resource3=IDD_ABOUTBOX
Resource4=IDD_DIALOG_ADJUST_IMAGE_PROPERTIES
Resource5=IDD_BACKGROUNDRANDOMIZER_DIALOG
Resource6=IDD_DIALOG_REGISTER
Class9=CDlgAdjustImage
Resource7=IDR_MENU_TRAY_POPUP

[CLS:CBackgroundRandomizerApp]
Type=0
BaseClass=CWinApp
HeaderFile=BackgroundRandomizer.h
ImplementationFile=BackgroundRandomizer.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=BackgroundRandomizerDlg.h
ImplementationFile=BackgroundRandomizerDlg.cpp
LastObject=ID_MENU_ADJUSTCURRENTIMAGE

[CLS:CBackgroundRandomizerDlg]
Type=0
BaseClass=CDialog
HeaderFile=BackgroundRandomizerDlg.h
ImplementationFile=BackgroundRandomizerDlg.cpp

[CLS:CBgRandPopupAbout]
Type=0
BaseClass=CDialog
HeaderFile=BgRandPopupAbout.h
ImplementationFile=BgRandPopupAbout.cpp
Filter=D
VirtualFilter=dWC
LastObject=CBgRandPopupAbout

[CLS:CColorStaticST]
Type=0
BaseClass=CStatic
HeaderFile=ColorStaticST.h
ImplementationFile=ColorStaticST.cpp

[CLS:CDlgManageBlockedImages]
Type=0
BaseClass=CDialog
HeaderFile=DlgManageBlockedImages.h
ImplementationFile=DlgManageBlockedImages.cpp
LastObject=IDC_STATIC_PREVIEW
Filter=D
VirtualFilter=dWC

[CLS:CDlgRegister]
Type=0
BaseClass=CDialog
HeaderFile=DlgRegister.h
ImplementationFile=DlgRegister.cpp

[CLS:CSystemTray]
Type=0
BaseClass=CWnd
HeaderFile=SystemTray.h
ImplementationFile=SystemTray.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=15
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC_WEB_SITE,static,1342308608
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC_EMAIL,static,1342308608
Control9=IDC_STATIC_REGISTER,static,1342308608
Control10=IDC_STATIC,static,1342308353
Control11=IDC_STATIC_CREDIT_CARDS,static,1342177550
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC_SCREEN_MODE,static,1342308352

[DLG:IDD_BACKGROUNDRANDOMIZER_DIALOG]
Type=1
Class=CBackgroundRandomizerDlg
ControlCount=18
Control1=IDC_CHECK_ENABLE_RANDOMIZATION,button,1342242819
Control2=IDC_LIST_DIRECTORIES,listbox,1353783555
Control3=IDC_BUTTON_ADD,button,1342242816
Control4=IDC_BUTTON_REMOVE,button,1342242816
Control5=IDC_BUTTON_MANAGE_BLOCKED_IMAGES,button,1342242816
Control6=IDC_CHECK_DONT_DUPLICATE_IMAGES,button,1342242819
Control7=IDC_CHECK_MATCH_SCREEN_ORIENTATION,button,1342242819
Control8=IDC_CHECK_RESIZE_TO_FILL_DESKTOP,button,1342242819
Control9=IDC_CHECK_MAINTAIN_ASPECT_RATIO,button,1342242819
Control10=IDC_COMBO_ASPECT_RATIO_MODE,combobox,1344339971
Control11=IDC_STATIC,static,1342308352
Control12=IDC_COMBO_BACKGROUND_INTERVAL,combobox,1344339971
Control13=IDOK,button,1342242817
Control14=ID_CANCEL,button,1342242817
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_COMBO_AUTO_ROTATE_TO_MATCH_SCREEN,combobox,1344339971

[DLG:IDD_DIALOG_BG_RAND_PARENT]
Type=1
Class=CBgRandPopupAbout
ControlCount=0

[DLG:IDD_DIALOG_MANAGE_BLOCKED_IMAGES]
Type=1
Class=CDlgManageBlockedImages
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_BLOCKED_IMAGES,listbox,1353777411
Control4=IDC_STATIC_PREVIEW,static,1342177806
Control5=IDC_BUTTON_ALLOW_IMAGE_AGAIN,button,1342242816

[DLG:IDD_DIALOG_REGISTER]
Type=1
Class=CDlgRegister
ControlCount=21
Control1=IDC_EDIT_USERNAME,edit,1350631552
Control2=IDC_EDIT_LICENSE,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDC_BUTTON_CANCEL,button,1342242816
Control5=IDC_STATIC_EXPIRATION,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308864
Control8=IDC_STATIC,static,1342308864
Control9=IDC_STATIC_NOTICE,static,1342308352
Control10=IDC_STATIC_REGCOST,static,1342308352
Control11=IDC_STATIC_REGISTER_CARDS,static,1342177550
Control12=IDC_STATIC_WEBSITE,static,1342308608
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC_REGISTER,static,1342308608
Control15=IDC_STATIC_EMAIL,static,1342308608
Control16=IDC_STATIC,static,1342181383
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352

[MNU:IDR_MENU_TRAY_POPUP]
Type=1
Class=CBgRandPopupAbout
Command1=ID_MENU_CHANGEIMAGENOW
Command2=ID_MENU_ADJUSTCURRENTIMAGE
Command3=ID_MENU_DONTSHOWTHISIMAGEAGAIN
Command4=ID_MENU_SLIDESHOW
Command5=ID_MENU_SETTINGS
Command6=ID_MENU_REGISTRATION
Command7=ID_MENU_ABOUT
Command8=ID_MENU_TERMINATE
CommandCount=8

[DLG:IDD_DIALOG_ADJUST_IMAGE_PROPERTIES]
Type=1
Class=CDlgAdjustImage
ControlCount=8
Control1=IDC_BUTTON_ROTATE,button,1342242827
Control2=IDC_BUTTON_MIRROR,button,1342242944
Control3=IDC_BUTTON_FLIP,button,1342242944
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_STATIC_PREVIEW,static,1342177806
Control7=IDC_EDIT_FILENAME,edit,1350633600
Control8=IDC_BUTTON_ROTATE_COUNTER,button,1342242827

[CLS:CDlgAdjustImage]
Type=0
HeaderFile=DlgAdjustImage.h
ImplementationFile=DlgAdjustImage.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_ROTATE_COUNTER
VirtualFilter=dWC

