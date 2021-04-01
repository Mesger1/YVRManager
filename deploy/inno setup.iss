; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=YVR-Manager
AppVersion=0.01
WizardStyle=modern
DefaultDirName={autopf}\YVRManager
DefaultGroupName=YVR-Manager
UninstallDisplayIcon={app}\YVR-Manager.exe
Compression=lzma2
SolidCompression=yes

[Files]
Source: "YVR-Manager.exe"; DestDir: "{app}"
Source: "Qt6Core.dll"; DestDir: "{app}"
Source: "Qt6Gui.dll"; DestDir: "{app}"
Source: "Qt6Network.dll"; DestDir: "{app}"
Source: "Qt6Widgets.dll"; DestDir: "{app}"
Source: "platforms\qdirect2d.dll"; DestDir: "{app}\platforms"
Source: "platforms\qminimal.dll"; DestDir: "{app}\platforms"
Source: "platforms\qoffscreen.dll"; DestDir: "{app}\platforms"
Source: "platforms\qwindows.dll"; DestDir: "{app}\platforms"


[Icons]
Name: "{group}\YVR-Manager"; Filename: "{app}\YVR-Manager.exe"
Name: "{commondesktop}\YVR-Manager"; Filename: "{app}\YVR-Manager"; IconFilename: "{app}\YVR-Manager.exe"
