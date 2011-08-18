


::hg update
cd C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial

hg pull

hg update


@Echo Off
@echo ----------------------------------------------------
@echo hit Ctrl+C to NOT continue with the OPERATIONS BATCH
@echo ----------------------------------------------------

timeout 20

::start operations batch
cd C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\batchFiles
start OPERATIONS_BATCH.bat

::copy batch to update to desktop
copy "C:\Workspace\openFrameworks_0062\apps_arne\zinaMercurial\zina-app-mercurial\batchFiles\ZINA_APP_UPDATE.bat" "C:\Users\%username%\Desktop" /Y





