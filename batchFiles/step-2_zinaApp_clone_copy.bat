
::INFO
::first download code from google code, then add data from drive to project bin folder

::clone \zinaMercurial from google code
hg clone https://zina.application@code.google.com/p/zina-app-mercurial C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\

::copy \data\stations
mkdir C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\stations
xcopy E:\ZINA_TO_NETBOOK\COPY_TO_DATA_FOLDER\stations C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\stations /D /E /C /R /I /K /Y

::copy \data\sound
mkdir C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\sound
xcopy E:\ZINA_TO_NETBOOK\COPY_TO_DATA_FOLDER\sound C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\sound /D /E /C /R /I /K /Y

::copy link to workbench to desktop
copy "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\TortoiseHg\TortoiseHg Workbench.lnk" "C:\Users\%username%\Desktop"

::copy link to workbench to desktop
copy "C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\batchFiles\ZINA APP UPDATE.bat" "C:\Users\%username%\Desktop"