

::COPY ZINA DATA FROM DRIVE AND CLONE FROM INTERNET TO DISK

::copy \SOFTWARE
mkdir c:\Software
xcopy e:\ZINA_TO_NETBOOK\Software c:\Software /D /E /C /R /I /K /Y

::copy \WORKSPACE
mkdir c:\Workspace
xcopy e:\ZINA_TO_NETBOOK\Workspace c:\Workspace /D /E /C /R /I /K /Y

::clone \zinaMercurial from google code
hg clone https://arneboon@code.google.com/p/zina-app-mercurial C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\

::copy \data\stations
mkdir C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\stations
xcopy E:\ZINA_TO_NETBOOK\COPY_TO_DATA_FOLDER\stations C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\stations /D /E /C /R /I /K /Y

::copy \data\sound
mkdir C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\sound
xcopy E:\ZINA_TO_NETBOOK\COPY_TO_DATA_FOLDER\sound C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\sound /D /E /C /R /I /K /Y




