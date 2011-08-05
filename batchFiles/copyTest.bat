

::COPY ZINA DATA FROM DRIVE AND CLONE FROM INTERNET TO DISK

::copy \SOFTWARE
mkdir c:\TestFolder\Software
xcopy e:\ZINA_TO_NETBOOK\Software c:\TestFolder\Software /D /E /C /R /I /K /Y

::copy \WORKSPACE
mkdir c:\TestFolder\Workspace
xcopy e:\ZINA_TO_NETBOOK\Workspace c:\TestFolder\Workspace /D /E /C /R /I /K /Y

::clone \zinaMercurial from google code
hg clone https://arneboon@code.google.com/p/zina-app-mercurial C:\TestFolder\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\

::copy \data\stations
mkdir C:\TestFolder\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\stations
xcopy E:\ZINA_TO_NETBOOK\COPY_TO_DATA_FOLDER\stations C:\TestFolder\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\stations /D /E /C /R /I /K /Y

::copy \data\sound
mkdir C:\TestFolder\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\sound
xcopy E:\ZINA_TO_NETBOOK\COPY_TO_DATA_FOLDER\sound C:\TestFolder\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\bin\data\sound /D /E /C /R /I /K /Y




