package main

import (
	"fmt"
	"os"

	bpf "github.com/iovisor/gobpf/elf"
)

func main() {
	// 加载eBPF程序
	prog := bpf.NewModule("xdp_prog.o")
	defer prog.Close()

	// 将eBPF程序附加到网络接口
	if err := prog.Load(nil); err != nil {
		fmt.Fprintf(os.Stderr, "Failed to load XDP program: %s\n", err)
		return
	}

	if err := prog.AttachXDP("ens35", "xdp_firewall"); err != nil {
		fmt.Fprintf(os.Stderr, "Failed to attach XDP to ens35: %s\n", err)
		return
	}

	fmt.Println("XDP firewall is running. Press Ctrl-C to exit.")
	select {} // 挂起程序，保持XDP程序运行
}
