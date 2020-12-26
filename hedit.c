#include "src/main/main.c"
#include "src/main/buffer.c"
#include "src/main/file/fileWriter.c"

#if defined(HEDIT_PLATFORM_LINUX)
#include "src/main/file/fileMapping.linux.c"
#elif defined(HEDIT_PLATFORM_WINDOWS)
#include "src/main/file/fileMapping.windows.c"
#elif defined(HEDIT_PLATFORM_OTHER)
#include "src/main/file/fileMapping.c"
#endif
