; create setup for csvfix

[Setup]
AppName=CSVfix
AppVerName=CSVfix 1.0
AppPublisher=Neil Butterworth
AppPublisherURL=http://code.google.com/p/csvfix/
AppSupportURL=http://code.google.com/p/csvfix/
AppUpdatesURL=http://code.google.com/p/csvfix/
DefaultDirName={pf}\CSVfix
DefaultGroupName=CSVfix
AllowNoIcons=yes
LicenseFile=D:\Users\NeilB\devel\csvfix\installer\files\LICENSE
OutputBaseFilename=CSVFix_Setup
SetupIconFile=D:\Users\NeilB\devel\csvfix\installer\files\csvfix.ico
Compression=lzma
SolidCompression=yes
InfoAfterFile=D:\Users\NeilB\devel\csvfix\installer\files\readme.txt

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "D:\Users\NeilB\devel\csvfix\installer\files\csvfix.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Users\NeilB\devel\csvfix\installer\files\csvfix.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Users\NeilB\devel\csvfix\installer\files\LICENSE"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Users\NeilB\devel\csvfix\installer\files\readme.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\Users\NeilB\devel\csvfix\installer\files\data\*"; DestDir: "{app}\Data"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\CSVfix Help"; Filename: "{app}\csvfix.chm"
Name: "{group}\{cm:ProgramOnTheWeb,CSVfix}"; Filename: "http://code.google.com/p/csvfix/"
Name: "{group}\{cm:UninstallProgram,CSVfix}"; Filename: "{uninstallexe}"

[Run]

