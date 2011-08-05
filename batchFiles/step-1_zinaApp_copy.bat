

::COPY ZINA DATA FROM DRIVE AND CLONE FROM INTERNET TO DISK

::copy \SOFTWARE
mkdir c:\Software
xcopy e:\ZINA_TO_NETBOOK\Software c:\Software /D /E /C /R /I /K /Y

::copy \WORKSPACE
mkdir c:\Workspace
xcopy e:\ZINA_TO_NETBOOK\Workspace c:\Workspace /D /E /C /R /I /K /Y



::copy \STARTUP LNK

copy "C:\Workspace\START ZINA APP.lnk" "C:\Users\%username%\Desktop"



copy "C:\Workspace\START ZINA APP.lnk" "C:\Users\%username%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup"