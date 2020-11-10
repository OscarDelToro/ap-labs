// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"flag"
	"fmt"
	"os"

	"gopl.io/ch5/links"
)

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
type Site struct {
	currentDepth int
	link         string
}

var tokens = make(chan struct{}, 20)

var depth = flag.Int("depth", 1, "crawler depth")
var fileName = flag.String("results", "default.txt", "result file")

func crawl(website Site) []Site {
	f, err := os.OpenFile(*fileName, os.O_APPEND|os.O_WRONLY, 0644)
	if err != nil {
		fmt.Println(err)
	}
	_, err = fmt.Fprintln(f, website.link)
	if err != nil {
		fmt.Println(err)
		f.Close()
	}
	err = f.Close()
	if err != nil {
		fmt.Println(err)
	}
	if website.currentDepth < *depth {
		tokens <- struct{}{} // acquire a token
		list, err := links.Extract(website.link)
		sites := make([]Site, 0)
		for _, link := range list {
			sites = append(sites, Site{link: link, currentDepth: website.currentDepth + 1})
		}
		<-tokens // release the token
		if err != nil {
			//log.Print(err)
		}
		return sites

	}
	return []Site{}
}

//!-sema

//!+
func main() {
	flag.Parse()
	if len(os.Args) < 4 {
		fmt.Println("Wrong arguments")
		return
	}
	worklist := make(chan []Site)
	var n int // number of pending sends to worklist

	// Start with the command-line arguments.
	n++
	fmt.Printf("crawling at %s\n", os.Args[3:])
	file, err := os.Create(*fileName)
	if err != nil {
		fmt.Println(err)
		return
	}
	err = file.Close()
	if err != nil {
		fmt.Println(err)
		return
	}

	go func() {
		urls := make([]Site, 0)
		urls = append(urls, Site{link: (os.Args[3:])[0], currentDepth: 0})
		worklist <- urls
	}()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list {
			if !seen[link.link] {
				seen[link.link] = true
				n++
				go func(link Site) {
					worklist <- crawl(link)
				}(link)
			}
		}
	}
	fmt.Printf("crawl finished, check results at %s \n", *fileName)

}

//!-
