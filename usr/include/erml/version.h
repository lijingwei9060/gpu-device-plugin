#define VERSION_VALUE(Tool) ((uint32_t)(Tool##_VERSION_MAJOR)<<24 | \
                             (uint32_t)(Tool##_VERSION_MINOR)<<16 | \
                             (uint32_t)(Tool##_VERSION_PATCH)<<8  | \
                             (uint32_t)(Tool##_VERSION_TWEAK))


#define EFSMI_VERSION       1.0.0.20250226
#define EFSMI_VERSION_MAJOR 1
#define EFSMI_VERSION_MINOR 0
#define EFSMI_VERSION_PATCH 0
#define EFSMI_VERSION_TWEAK 20250226
#define EFSMI_VERSION_VALUE VERSION_VALUE(EFSMI)


#define IsVerEqual(Tool, major, minor, patch, tweak) ({\
        uint32_t ver = (((uint32_t)major)<<24 | ((uint32_t)minor)<<16 | ((uint32_t)patch<<8) | (tweak)) ; \
        (ver == Tool##_VERSION_VALUE) ? true : false;})
#define IsVerGreater(Tool, major, minor, patch, tweak) ({\
        uint32_t ver = (((uint32_t)major)<<24 | ((uint32_t)minor)<<16 | ((uint32_t)patch<<8) | (tweak)) ; \
        (ver < Tool##_VERSION_VALUE) ? true : false;})
#define IsVerEqualGreater(Tool, major, minor, patch, tweak) ({\
        uint32_t ver = (((uint32_t)major)<<24 | ((uint32_t)minor)<<16 | ((uint32_t)patch<<8) | (tweak)) ; \
        (ver <= Tool##_VERSION_VALUE) ? true : false;})

#define EFSMI_VERSION_STR       "1.0.0.20250226"
#define EFSMI_VERSION_STR_MAJOR "1"
#define EFSMI_VERSION_STR_MINOR "0"
#define EFSMI_VERSION_STR_PATCH "0"
#define EFSMI_VERSION_STR_TWEAK "20250226"
#define EFSMI_GIT_VERSION_STR   ""

#define ERML_VERSION_STR       "1.0.0.20250226"
#define ERML_VERSION_STR_MAJOR "1"
#define ERML_VERSION_STR_MINOR "0"

#define PROJECT_TYPE_       0   // efsmt
#define PROJECT_TYPE_X      1   // efdt for cape
#define PROJECT_TYPE_EFDT   2   // efdt for common
#define PROJECT_TYPE        PROJECT_TYPE_
