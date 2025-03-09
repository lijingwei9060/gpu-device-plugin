package main

import (
	"go_erml"
	"fmt"
)

func main() {
	erml.InitV2(false)
	cnt, _ := erml.GetDevCount()
	for idx := uint(0); idx < cnt; idx++ {
		handle, _ := erml.GetDeviceHandleByIndex(idx)
		pci_link, _ := handle.GetPcieLinkInfo()
		fmt.Println("device ", idx, "link speed = ", pci_link.Link_Speed)
		fmt.Println("device ", idx, "link width = ", pci_link.Link_Width)
	}

	erml.Shutdown()
}
