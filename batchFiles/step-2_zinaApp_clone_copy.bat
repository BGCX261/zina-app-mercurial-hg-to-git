
::INFO
::first download code from google code, then add data from drive to project bin folder

::clone \zinaMercurial from google code
hg clone https://zina.application@code.google.com/p/zina-app-mercurial C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial

::copy \data\stations
mkdir C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\bin\data\stations
xcopy E:\ZINA_TO_NETBOOK\COPY_TO_DATA_FOLDER\stations C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\bin\data\stations /D /E /C /R /I /K /Y

::copy \data\sound
mkdir C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\bin\data\sound
xcopy E:\ZINA_TO_NETBOOK\COPY_TO_DATA_FOLDER\sound C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\bin\data\sound /D /E /C /R /I /K /Y

::make empty dir for recordings
mkdir c:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\bin\data\recordings

::copy link to workbench to desktop
copy "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\TortoiseHg\TortoiseHg Workbench.lnk" "C:\Users\%username%\Desktop" /Y

::copy batch to update to desktop
copy "C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\batchFiles\ZINA_APP_UPDATE.bat" "C:\Users\%username%\Desktop" /Y



::copy batch to copy recorings to desktop
copy "C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\batchFiles\COPY_RECORDINGS_TO_DRIVE.bat" "C:\Users\%username%\Desktop" /Y

::copy driver remover
copy "C:\Software\USBDiskEject_Beta4.exe" "C:\Users\%username%\Desktop" /Y
