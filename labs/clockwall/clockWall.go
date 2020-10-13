package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
)

func main() {

	if len(os.Args) < 2 {
		fmt.Println("wrong args")
		os.Exit(1)
	}

	can := make(chan int)
	for  _,arg := range os.Args[1:] {
		connection, err := net.Dial("tcp", arg[strings.LastIndex(arg, "=")+1:])
		if err != nil {
			log.Fatal(err)
		}
		defer connection.Close()
		go io.Copy(os.Stdout, connection)
	}
	a := 1
	a = <-can
	log.Println("Channel closed. code: ", a)
	close(can)

}
