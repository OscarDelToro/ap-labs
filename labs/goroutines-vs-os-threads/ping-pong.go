package main

import (
	"fmt"
	"time"
)

func ping(a chan uint, b chan uint) {
	for {
		a <- (<-b + 1)
	}
}

func main() {
	var commsPerSecond uint
	a := make(chan uint)
	b := make(chan uint)
	var duration uint
	duration = 7
	fmt.Printf("Starting test. Running for %d seconds\n", duration)
	go ping(a, b)
	go ping(b, a)
	b <- 0
	time.Sleep(time.Duration(duration) * time.Second)
	commsPerSecond = <-a / duration
	fmt.Println("Communications Per Second : ", commsPerSecond)

}
