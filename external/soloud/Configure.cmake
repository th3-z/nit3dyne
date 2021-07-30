include (OptionDependentOnPackage)
include (PrintOptionStatus)

option (SOLOUD_DYNAMIC "Set to ON to build dynamic SoLoud" OFF)
print_option_status (SOLOUD_DYNAMIC "Build dynamic library")

option (SOLOUD_STATIC "Set to ON to build static SoLoud" ON)
print_option_status (SOLOUD_STATIC "Build static library")

option (SOLOUD_BACKEND_MINIAUDIO "Set to ON for building Miniaudio backend" ON)
print_option_status (SOLOUD_BACKEND_MINIAUDIO "Miniaudio backend")

option (SOLOUD_BACKEND_COREAUDIO "Set to ON for building CoreAudio backend" OFF)
print_option_status (SOLOUD_BACKEND_COREAUDIO "CoreAudio backend")

option (SOLOUD_BACKEND_OPENSLES "Set to ON for building OpenSLES backend" OFF)
print_option_status (SOLOUD_BACKEND_OPENSLES "OpenSLES backend")

option (SOLOUD_BACKEND_WASAPI "Set to ON for building WASAPI backend" OFF)
print_option_status (SOLOUD_BACKEND_WASAPI "WASAPI backend")
