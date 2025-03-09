
#ifndef ERML_EXPORT_H
#define ERML_EXPORT_H

#ifdef ERML_STATIC_DEFINE
#  define ERML_EXPORT
#  define ERML_NO_EXPORT
#else
#  ifndef ERML_EXPORT
#    ifdef liberml_EXPORTS
        /* We are building this library */
#      define ERML_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define ERML_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef ERML_NO_EXPORT
#    define ERML_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef ERML_DEPRECATED
#  define ERML_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef ERML_DEPRECATED_EXPORT
#  define ERML_DEPRECATED_EXPORT ERML_EXPORT ERML_DEPRECATED
#endif

#ifndef ERML_DEPRECATED_NO_EXPORT
#  define ERML_DEPRECATED_NO_EXPORT ERML_NO_EXPORT ERML_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef ERML_NO_DEPRECATED
#    define ERML_NO_DEPRECATED
#  endif
#endif

#endif /* ERML_EXPORT_H */
