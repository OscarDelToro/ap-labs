package main

import (
	"fmt"
<<<<<<< HEAD
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

=======
)

func main() {
	var commsPerSecond int

	fmt.Println("Communications Per Second : ", commsPerSecond)
>>>>>>> d539e0a3d167b4cbf2b238ba00a8dd1c1285187d
}
