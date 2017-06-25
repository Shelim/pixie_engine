!include MUI2.nsh
!include LogicLib.nsh
!include GetFolderPath.nsh

Name "$RESOURCES_PRODUCT$"
Outfile "$INSTALLER_FILENAME$"
InstallDir "$PROGRAMFILES\$INSTALL_DIR$"

InstallDirRegKey HKLM "Software\$INSTALL_REGISTRY$" "Installation"

RequestExecutionLevel admin

!define MUI_ICON "install_icon.ico"
!define MUI_UNICON "uninstall_icon.ico"

!define MUI_ABORTWARNING

Var StartMenuFolder

!define MUI_WELCOMEFINISHPAGE_BITMAP "install_sidebar.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "uninstall_sidebar.bmp"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "install_header.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "install_header.bmp"

!define MUI_FINISHPAGE_RUN "$INSTDIR\$INSTALL_RUN$"

Function finishpageaction
CreateShortcut "$DESKTOP\$INSTALL_SHORTCUT$" "$INSTDIR\$LAUNCHER_FILENAME$"
FunctionEnd

!define MUI_FINISHPAGE_SHOWREADME ""
!define MUI_FINISHPAGE_SHOWREADME_TEXT "Create Desktop shortcut"
!define MUI_FINISHPAGE_SHOWREADME_FUNCTION finishpageaction

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\$INSTALL_REGISTRY$" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenu"

!define MUI_COMPONENTSPAGE_NODESC

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "install_license.rtf"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_COMPONENTS
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

Function .onInit
  
  ReadRegStr $R0 HKLM \
  "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
  "UninstallString"
StrCmp $R0 "" done
  
  MessageBox MB_OK|MB_ICONINFORMATION \
  "Setup will remove current version and install update"
  
  ClearErrors
  ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
 
  IfErrors no_remove_uninstaller done2
no_remove_uninstaller:
	MessageBox MB_OK|MB_ICONSTOP \
	"There was an error during remove. The setup will now abort"
	Abort
done:
  SetOutPath $TEMP
  file /oname=spl_lg_tmp.bmp "install_logo.bmp"
  advsplash::show 3000 600 400 -1 $TEMP\spl_lg_tmp
  Pop $0
  Delete $TEMP\spl_lg_tmp.bmp
done2:
FunctionEnd

Var GetInstalledSize.total
Function GetInstalledSize
	Push $0
	Push $1
	StrCpy $GetInstalledSize.total 0
	${ForEach} $1 0 256 + 1
		${if} ${SectionIsSelected} $1
			SectionGetSize $1 $0
			IntOp $GetInstalledSize.total $GetInstalledSize.total + $0
		${Endif}
 
		; Error flag is set when an out-of-bound section is referenced
		${if} ${errors}
			${break}
		${Endif}
	${Next}
 
	ClearErrors
	Pop $1
	Pop $0
	IntFmt $GetInstalledSize.total "0x%08X" $GetInstalledSize.total
	Push $GetInstalledSize.total
FunctionEnd

Section

	Call GetInstalledSize

	; install everything
	
	SetOutPath "$INSTDIR"
	file /r "$PATH_TO_FILES$\*.*"
	
	; finish installation
	
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  
	CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
	CreateShortCut "$SMPROGRAMS\$StartMenuFolder\$INSTALL_SHORTCUT$" "$INSTDIR\$LAUNCHER_FILENAME$"
	CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\uninstall.exe"
	
  !insertmacro MUI_STARTMENU_WRITE_END
  				 
	WriteRegStr HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
                 "DisplayName" "$RESOURCES_PRODUCT$"
	WriteRegStr HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
                 "Displayversion" "$VERSION_MAJOR$,$VERSION_MINOR$,$VERSION_REVISION$,$VERSION_BUILD$"
	WriteRegStr HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
                 "Publisher" "$RESOURCES_COMPANY$"
	WriteRegStr HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
                 "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
                 "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
                 "InstallLocation " "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
                 "DisplayIcon" "$INSTDIR\$GAME_FILENAME$"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
                 "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
                 "NoRepair" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$" \
				"EstimatedSize" "$GetInstalledSize.total"
					 
	WriteUninstaller "$INSTDIR\uninstall.exe"
	
;	${gameExplorer_Addgame} all "$INSTDIR\Mod10th\Drakan 10th Anniversary.exe" "$INSTDIR" "$INSTDIR\Mod10th\Drakan 10th Anniversary.exe" "{E97652F4-C9E9-4ADD-B453-1C8FCC917D46}"

SectionEnd

Section /o "un.Remove custom mods"

	RMdir /r "$INSTDIR"

SectionEnd

;var folder

Section /o "un.Remove saved games"

;	${SHGetKnownFolderPath} "${FOLDERID_Savedgames}" 0 $folder

	RMdir /R "$PROFILE\saved_games\$APPDATAPATH$"

SectionEnd

Section /o "un.Remove keybinding"

	RMdir /R "$APPDATA\$APPDATAPATH$"

SectionEnd

Section /o "un.Remove config"

	RMdir /R "$LOCALAPPDATA\$APPDATAPATH$"

SectionEnd

Section "-un.Main"

;	${gameExplorer_Removegame} "{E97652F4-C9E9-4ADD-B453-1C8FCC917D46}"

	Delete "$INSTDIR\uninstall.exe"
	Delete "$DESKTOP\$INSTALL_SHORTCUT$"
	
	ClearErrors
	
	RMdir /R "$INSTDIR\modules\base"
	RMdir "$INSTDIR\modules"
	
	ClearErrors
	FindFirst $R0 $R1 "$INSTDIR\*.*"
	IfErrors Exit2
	Top2:
		StrCmp $R1 "." Next2
		StrCmp $R1 ".." Next2
		StrCmp $R1 "modules" Next2
		StrCmp $R1 "sub_modules" Next2
		
		RMdir /R "$INSTDIR\$R1"
		Delete "$INSTDIR\$R1"
			
		Next2:
		ClearErrors
		FindNext $R0 $R1
		IfErrors Exit2
		Goto Top2
	Exit2:
	FindClose $R0
	
	RMdir "$INSTDIR"

	DeleteRegKey HKLM "Software\Microsoft\Windows\Currentversion\Uninstall\$RESOURCES_INTERNAL$"
	
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
	RMdir /R "$SMPROGRAMS\$StartMenuFolder"
SectionEnd