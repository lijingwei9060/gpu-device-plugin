package main

import (
	"go_erml"
	"fmt"
)

func main() {
	erml.InitV2(false)
	ver, _ := erml.GetDriverVer()
	fmt.Println("Driver Ver:  ", ver)
	erml.Shutdown()
}
