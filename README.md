# GPU Device Plugin

This repository contains the GPU Device Plugin for Kubernetes.

## Prerequisites

- Docker
- Kubernetes cluster
- Go 1.24.1
- enrigin driver installed， check model `lsmod|grep enrigin`
- enrigin device: `ls /dev/gcu*`
- Header files and libraries for `erml` located in `/usr/include/erml` and `/usr/lib/liberml*`

## Build Instructions

1. Clone the repository:
    ```sh
    git clone https://github.com/your-repo/gpu-device-plugin.git
    cd gpu-device-plugin
    ```

2. Ensure you have the necessary header files and libraries for `erml`:
    - Header files should be in `/usr/include/erml`
    - Libraries should be in `/usr/lib`

3. Build the Docker image:
    ```sh
    docker build -t jy-gpu-plugin:latest .
    ```

## Deployment

1. Push the Docker image to your container registry:
    ```sh
    docker tag jy-gpu-plugin:latest your-registry/jy-gpu-plugin:latest
    docker push your-registry/jy-gpu-plugin:latest
    ```

2. Update the `deploy/daemonset.yaml` file to use your container registry:
    ```yaml
    containers:
      - name: jy-gpu-plugin
        image: your-registry/jy-gpu-plugin:latest
    ```

3. Apply the DaemonSet to your Kubernetes cluster:
    ```sh
    kubectl apply -f deploy/daemonset.yaml
    ```

## Notes

- The DaemonSet configuration grants the container read/write access to all devices under `/dev`.
- Ensure that the Kubernetes nodes have the necessary GPU drivers installed.