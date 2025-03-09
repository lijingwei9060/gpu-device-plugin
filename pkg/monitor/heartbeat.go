package main

import (
	"go_erml"
	"fmt"
	"time"
)

func main() {
	erml.InitV2(false)
	cnt, _ := erml.GetDevCount()
	for dev_idx := uint(0); dev_idx < cnt; dev_idx++ {
		handle, _ := erml.GetDeviceHandleByIndex(dev_idx)
		count, _ := handle.GetSsmFwHeartBeat()
		fmt.Println("ssm fw before sleep heartbeat ", count)
		time.Sleep(1 * time.Second)
		fmt.Println("ssm fw after sleep heartbeat ", count)
	}
	erml.Shutdown()
}
