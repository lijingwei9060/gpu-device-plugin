// Copyright 2020-2024 Enrigin. All Rights Reserved.

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
        hwArchName, _ := handle.GetHwArchName()

        fmt.Println("device idx:", dev_idx)
        fmt.Println("hw arch name = ", hwArchName)
    }
    erml.Shutdown()
}