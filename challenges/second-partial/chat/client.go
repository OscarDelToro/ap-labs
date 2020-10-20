// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"io"
	"log"
	"net"
	"os"
)

//!+
func main() {
	conn, err := net.Dial("tcp", os.Args[4])
	if err != nil {
		log.Fatal(err)
	}
	done := make(chan struct{})
	_, e := io.WriteString(conn, os.Args[2]+"\n")
	if e != nil {
		log.Fatal(err)
	}
	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors

		log.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn, os.Stdin)

	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

/* enviar esta secuencia como unicast a quien envia el mensaje para desaparecer la linea donde escribe el mensaje
fmt.Print("\033[A")
	fmt.Printf("\r")
*/

func mustCopy(dst io.Writer, src io.Reader) {

	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
