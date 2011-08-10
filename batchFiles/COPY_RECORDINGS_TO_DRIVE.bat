
::INFO
::copy recordings from disk to drive

::TODO
::prompt user to input drive letter

set /p drive_letter= INPUT DRIVE LETTER TO COPY TO [d, e, f, g]:  

set path_from=C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\bin\data\recordings\ 
set path_to=E:\ZINA_APP\voice_recordings\%username%\recordings\ 

echo copy files to: %path_to%
mkdir %path_to%
robocopy %path_from% %path_to% /E
pause