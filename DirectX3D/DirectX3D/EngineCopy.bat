cd..
cd..

xcopy /s /y /d .\GPEngine\Include\*.h .\Engine\Include\
xcopy /s /y /d .\GPEngine\Include\*.inl .\Engine\Include\

xcopy /s /y /d /exclude:Out.txt .\GPEngine\Bin_Debug\*.* .\Engine\Bin_Debug\

xcopy /s /y /d /exclude:Out.txt .\GPEngine\Bin_Debug\Shader\*.* .\Engine\Bin\Shader\

xcopy /s /y /d /exclude:Out.txt .\GPEngine\Bin\*.* .\Engine\Bin\

xcopy /s /y /d .\Engine\Bin_Debug\*.* .\Client\Bin_Debug\
xcopy /s /y /d .\Engine\Bin\*.* .\Client\Bin\