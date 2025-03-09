package plugin

import (
	"context"
	"gpu-device-plugin/pkg/common"
	"log"
	"net"
	"os"
	"path"
	"syscall"
	"time"

	"github.com/pkg/errors"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
	pluginapi "k8s.io/kubelet/pkg/apis/deviceplugin/v1beta1"
)

type GpuDevicePlugin struct {
	server *grpc.Server
	stop   chan struct{} // this channel signals to stop the device plugin
	dm     *DeviceMonitor
}

func NewGpuDevicePlugin() *GpuDevicePlugin {
	return &GpuDevicePlugin{
		server: grpc.NewServer(grpc.EmptyServerOption{}),
		stop:   make(chan struct{}),
		dm:     NewDeviceMonitor(common.DevicePath),
	}
}

// Run start gRPC server and watcher
func (c *GpuDevicePlugin) Run() error {
	err := c.dm.List()
	if err != nil {
		log.Fatalf("list device error: %v", err)
	}

	pluginapi.RegisterDevicePluginServer(c.server, c)
	// delete old unix socket before start
	socket := path.Join(pluginapi.DevicePluginPath, common.DeviceSocket)
	err = syscall.Unlink(socket)
	if err != nil && !os.IsNotExist(err) {
		return errors.WithMessagef(err, "delete socket %s failed", socket)
	}

	sock, err := net.Listen("unix", socket)
	if err != nil {
		return errors.WithMessagef(err, "listen unix %s failed", socket)
	}

	go c.server.Serve(sock)

	// Wait for server to start by launching a blocking connection
	conn, err := connect(common.DeviceSocket, 5*time.Second)
	if err != nil {
		return err
	}
	conn.Close()

	return nil
}


// dial establishes the gRPC communication with the registered device plugin.
func connect(socketPath string, timeout time.Duration) (*grpc.ClientConn, error) {
	ctx, cancel := context.WithTimeout(context.Background(), timeout)
	defer cancel()
	c, err := grpc.DialContext(ctx, socketPath,
		grpc.WithTransportCredentials(insecure.NewCredentials()),
		grpc.WithBlock(),
		grpc.WithContextDialer(func(ctx context.Context, addr string) (net.Conn, error) {
			if deadline, ok := ctx.Deadline(); ok {
				return net.DialTimeout("unix", addr, time.Until(deadline))
			}
			return net.DialTimeout("unix", addr, common.ConnectTimeout)
		}),
	)
	if err != nil {
		return nil, err
	}

	return c, nil
}
