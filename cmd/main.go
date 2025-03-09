package main

import (
	"gpu-device-plugin/pkg/plugin"
	"gpu-device-plugin/pkg/utils"

	"k8s.io/klog/v2"
)


func main() {
	klog.Infof("device plugin starting")
	dp := plugin.NewGpuDevicePlugin()
	go dp.Run()

	// register when device plugin start
	if err := dp.Register(); err != nil {
		klog.Fatalf("register to kubelet failed: %v", err)
	}

	// watch kubelet.sock,when kubelet restart,exit device plugin,then will restart by DaemonSet
	stop := make(chan struct{})
	err := utils.WatchKubelet(stop)
	if err != nil {
		klog.Fatalf("start to kubelet failed: %v", err)
	}

	<-stop
	klog.Infof("kubelet restart,exiting")
}
