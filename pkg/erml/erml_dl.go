/////////////////////////////////////////////////////////////////////////////
//  @brief API interface of Enrigin Managerment Library
//
//  Enrigin Tech, All Rights Reserved. 2024 Copyright (C)
/////////////////////////////////////////////////////////////////////////////

package erml

import (
	"unsafe"
)
// #cgo CFLAGS: -I/usr/include/erml
// #cgo LDFLAGS: -L/usr/lib -lerml
/*
#include <dlfcn.h>
#include <stdbool.h>
#include <string.h>
#include "erml.h"
*/
import "C"

type dl_handle_ptr struct{ handles []unsafe.Pointer }

var dl dl_handle_ptr

func (dl *dl_handle_ptr) InitV2(noDriver bool) C.ermlReturn_t {
	openFlags := C.RTLD_LAZY | C.RTLD_GLOBAL
	searchPaths := []string{"", "/usr/lib/", "/usr/lib64/", "/usr/lib/x86_64-linux-gnu/"}
	searchNames := []string{"liberml.so", "liberml.so.1", "liberml.so.2", "liberml.so.2.1", "liberml.so.2.0", "liberml.so.1.0.0"}
	var handle unsafe.Pointer
	for _, path := range searchPaths {
		for _, name := range searchNames {
			handle = C.dlopen(C.CString(path+name), C.int(openFlags))
			if handle != C.NULL {
				dl.handles = append(dl.handles, handle)
				return C.ErmlInit(C.bool(noDriver))
			}
		}
	}
	return C.ERML_ERROR_LIBRARY_NOT_FOUND
}

func (dl *dl_handle_ptr) Shutdown() C.ermlReturn_t {
	C.ErmlShutdown()

	for _, handle := range dl.handles {
		err := C.dlclose(handle)
		if err != 0 {
			return C.ERML_ERROR_FUNCTION_NOT_FOUND
		}
	}

	return C.ERML_SUCCESS
}

func (dl *dl_handle_ptr) lookupSymbol(symbol string) C.ermlReturn_t {
	for _, handle := range dl.handles {
		C.dlerror()
		C.dlsym(handle, C.CString(symbol))
		if unsafe.Pointer(C.dlerror()) == C.NULL {
			return C.ERML_SUCCESS
		}
	}

	return C.ERML_ERROR_FUNCTION_NOT_FOUND
}
