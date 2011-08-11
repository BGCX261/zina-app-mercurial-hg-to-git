
::INFO
::copy recordings from disk to drive

set path_from=C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\bin\data\recordings\ 
set path_to=E:\ZINA_FROM_APP\voice_recordings\%username%\recordings\ 

echo copy files to: %path_to%
mkdir %path_to%
robocopy %path_from% %path_to% /E
pause