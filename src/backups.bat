@Echo off 
SETLOCAL
Set /a mycnt=1	
Set myrep=.\*
Set tosave=""
::choose the usb disk if it is connected, otherwise choose the directory D:\Backups
if exist F: (set tosave=F:) else (set tosave=D:\Backups)
::count the number of backups present in the directory 
for /f %%A in ('dir %tosave% /b /a-d ^| find "NS2_"') do (set /a mycnt+=1)
::save the files in a zip file with the appropriate name (except jpg, jpeg, png files)
7za a -r -tzip -y -x!*.png -x!*.jpg -x!*.jpeg -x!*.obj -x!*.dll %tosave%\NS2_%mycnt%.zip %myrep%

ENDLOCAL
