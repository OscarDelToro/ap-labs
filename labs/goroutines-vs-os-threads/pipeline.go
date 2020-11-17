package main

import (
	"fmt"
<<<<<<< HEAD
	"runtime"
	"time"
)

var output = make(chan int)
var cont bool

func pipeblock(a chan int, b chan int, counter int) {
	if cont {
		go pipeblock(b, a, counter+1)
		b <- (<-a + 1)
	} else {
		output <- <-a
	}

}

func main() {
	var maxStages, transitTime int
	maxmem := 68
	a := make(chan int)
	b := make(chan int)
	cont = true
	go pipeblock(a, b, 1)
	startTime := time.Now()
	a <- 0

	var mem runtime.MemStats
	for {
		runtime.ReadMemStats(&mem)
		if mem.HeapInuse/1024/1024 < uint64(maxmem) {
			//fmt.Println(mem.HeapInuse / 1024 / 1024)

		} else {
			cont = false
			break

		}

	}

	maxStages = <-output
	transitTime = int(time.Since(startTime).Milliseconds())
	fmt.Println("Maximum number of pipeline stages   : ", maxStages)
	fmt.Printf("Time to transit trough the pipeline :  %d milliseconds\n", transitTime)
=======
)

func main() {
	var maxStages, transitTime int

	fmt.Println("Maximum number of pipeline stages   : ", maxStages)
	fmt.Println("Time to transit trough the pipeline : ", transitTime)
>>>>>>> d539e0a3d167b4cbf2b238ba00a8dd1c1285187d
}
