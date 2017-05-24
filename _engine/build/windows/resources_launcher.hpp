#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_$OUTPUT_TYPE$ && PIXIE_IS_DEBUG_BUILD == $IS_DEBUG_BUILD$ && PIXIE_IS_PORTABLE_BUILD == $IS_PORTABLE_BUILD$

#include "winver.h"

1 ICON "$AUTOGEN_PATH_RELATIVE$icon_launcher.ico"

1 VERSIONINFO
FILEVERSION		$VERSION_MAJOR$,$VERSION_MINOR$,$VERSION_REVISION$,$VERSION_BUILD$
PRODUCTVERSION	$VERSION_MAJOR$,$VERSION_MINOR$,$VERSION_REVISION$,$VERSION_BUILD$
FILEFLAGSMASK	VS_FFI_FILEFLAGSMASK
FILEFLAGS		0
FILEOS			VOS__WINDOWS32
FILETYPE		VFT_APP
FILESUBTYPE		VFT2_UNKNOWN
BEGIN
	BLOCK "StringFileInfo"
	BEGIN
		BLOCK "04090000"
		BEGIN
			VALUE "OriginalFilename", "$LAUNCHER_FILENAME$"
			VALUE "CompanyName", "$RESOURCES_COMPANY$"
			VALUE "FileDescription", "$RESOURCES_PRODUCT$"
			VALUE "Fileversion", "$VERSION_MAJOR$.$VERSION_MINOR$.$VERSION_REVISION$ build $VERSION_BUILD$ ($PLATFORM_FULLNAME$)"
			VALUE "InternalName", "$RESOURCES_INTERNAL$"
			VALUE "LegalCopyright", "$RESOURCES_COPYRIGHTS$"
			VALUE "ProductName", "$RESOURCES_PRODUCT$"
			VALUE "Productversion", "$VERSION_MAJOR$.$VERSION_MINOR$.$VERSION_REVISION$ build $VERSION_BUILD$ ($PLATFORM_FULLNAME$)"
		END
	END

	BLOCK "VarFileInfo"
	BEGIN
		VALUE "Translation", 0x409, 0
	END
END

#endif
