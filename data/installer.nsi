!define VERSION 0.1.0
!define NAME ycontrol
!define ENTRY Software\${NAME}
!define REPO "https://github.com/busoc/ycontrol"
!define DEV BUSOCGC
!define COMPANY BUSOC

Name "${NAME} installer v${VERSION}"
OutFile "${NAME}-setup.exe"
InstallDir $PROGRAMFILES\${NAME}

BrandingText "${NAME} version ${VERSION}"

SetCompressor /SOLID lzma

ShowInstDetails show
ShowUnInstDetails show

SetDateSave off
SetOverwrite on

Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "Installer"

SetOutPath $INSTDIR
WriteUninstaller $INSTDIR\uninstall.exe

FILE /r ..\..\deploy\ycontrol\

createDirectory $SMPROGRAMS\${COMPANY}
createShortCut $SMPROGRAMS\${COMPANY}\ycontrol.lnk $INSTDIR\ycontrol\ycontrol.exe
DetailPrint "adding shortcut $SMPROGRAMS\${COMPANY}\ycontrol.lnk"

WriteRegStr HKCU ${ENTRY} "DisplayName" ${NAME}
DetailPrint "adding to registryDisplayName(${NAME}) in ${ENTRY}"
WriteRegStr HKCU ${ENTRY} "DisplayVersion" ${VERSION}
DetailPrint "adding to registryDisplayVersion(${VERSION}) in ${ENTRY}"
WriteRegStr HKCU ${ENTRY} "Maintainer" ${DEV}
DetailPrint "adding to registryMaintainer(${DEV}) in ${ENTRY}"
WriteRegStr HKCU ${ENTRY} "Repository" ${REPO}
DetailPrint "adding to registryRepository(${REPO}) in ${ENTRY}"
WriteRegStr HKCU ${ENTRY} "UninstallString" $INSTDIR\uninstall.exe
DetailPrint "adding to registryUnsinstallString() in ${ENTRY}"

SectionEnd

Section "Uninstall"

DELETE $INSTDIR\*exe
RMDIR /r $INSTDIR\ycontrol\

DELETE $SMPROGRAMS\${COMPANY}\ycontrol.lnk
RMDIR /r $SMPROGRAMS\${COMPANY}\

DetailPrint "delete shortcut $SMPROGRAMS\${COMPANY}\ycontrol.lnk"

DeleteRegKey HKCU ${ENTRY}
DetailPrint "deleting from registry ${ENTRY}"

SectionEnd
