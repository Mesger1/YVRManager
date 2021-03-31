; The name of the installer
Name "YVR Manager Installer"

; The file to write
OutFile "YVR-Installer.exe"

; The default installation directory
InstallDir "C:\Program Files (x86)\YVRManager"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

;--------------------------------

; The stuff to install
Section "YVRMANAGER FILES"

SectionIn RO

; Set output path to the installation directory.
SetOutPath $INSTDIR

; Put file there
File "YVR-Manager.exe"
File "Qt5Widgets.dll"
File "Qt5Network.dll"
File "Qt5Gui.dll"
File "Qt5Core.dll"

SetOutPath $INSTDIR\platforms
File "platforms\qdirect2d.dll"
File "platforms\qminimal.dll"
File "platforms\qoffscreen.dll"
File "platforms\qwindows.dll"

SectionEnd

Section "Fix Firewall Settings"

ExecShell "runas" "netsh advfirewall firewall add allowedprogram $INSTDIR\YVR-Manager.exe YVR=Manager ENABLE"

SectionEnd

; Optional section (can be disabled by the user)
Section "Desktop Shortcut"

SetOutPath "$DESKTOP"
CreateShortcut "$DESKTOP\YVR Manager.lnk" "$INSTDIR\YVR-Manager.exe"

SectionEnd