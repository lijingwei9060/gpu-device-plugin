package main

import (
	"go_erml"
	"fmt"
)

func main() {
	erml.InitV2(false)
	cnt, _ := erml.GetDevCount()
	for dev_idx := uint(0); dev_idx < cnt; dev_idx++ {
		handle, _ := erml.GetDeviceHandleByIndex(dev_idx)
		major, main, _ := handle.GetDevMajorMain()
		fmt.Println("device major ", major, "main ", main)
	}
	erml.Shutdown()
}
