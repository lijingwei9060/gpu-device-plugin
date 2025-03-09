package common

import "time"

const (
	ResourceName   string = "jiangyuan.com/gpu"
	DevicePath		 string = "/dev/"
	HostPathPrefix     string = "/dev/"
	ContainerPathPrefix string = "/dev/"
	EnvName						string = "ALLOCATED_JY_GPU_DEVICES"
	DeviceSocket   string = "jiangyuan.sock"
	DeviceName		 string = "gcu"
	ConnectTimeout        = time.Second * 5
)
