// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import ( 
	"math" 
	"math/rand" 
    "fmt"
    "os" 
	"time"
	"strconv"
)

type Point struct{ x, y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}
func inter(lineA, lineB, rectA, rectB Point) bool {
	headingA := heading(lineA, lineB, rectA)
	headingB := heading(lineA, lineB, rectB)
	headingC := heading(rectA, rectB, lineA)
	headingD := heading(rectA, rectB, lineB)
	return headingA == 0 && inRange(lineA, rectA, lineB) ||
		   headingB == 0 && inRange(lineA, rectB, lineB) ||
		   headingC == 0 && inRange(rectA, lineA, rectB) ||
		   headingD == 0 && inRange(rectA, lineB, rectB) ||
		   headingA != headingB && headingC != headingD
}
func heading(a, b, c Point) int {
	val := (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y)
	if val == 0 {
		return 0
	} else {
		return -1
	}
}
func inRange(a, b, c Point) bool {
	return b.x <= math.Max(a.x, c.x) && b.x >= math.Min(a.x, c.x) && 
	b.y <= math.Max(a.y, c.y) && b.y >= math.Min(a.y, c.y)
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func main() {
<<<<<<< HEAD
	sides, a := strconv.Atoi(os.Args[1])
	if a != nil {
		fmt.Println(a)
	}
	if sides < 3 {
		fmt.Println("Wrong parameters")	
		return
	}
	vertex := make(Path, sides) 
	collide := false
	fmt.Printf("- Generating a [%d] sides figure\n", sides)
	for{
		rand.Seed(time.Now().UnixNano())
		collide = false
		for i := 0; i < sides; i++{
			vertex[i] = Point{rando(-100.0,100.0),rando(-100.0,100.0)}
		}
		for i := 0; i < sides; i++{
			if inter(vertex[i%sides], vertex[(1+i)%sides], vertex[(2+i)%sides],vertex[(3+i)%sides]) {
				collide = true
				break
			}
		}
		if !collide {
			fmt.Println("- Figure's vertices")
			for i := range vertex {
				fmt.Printf("  - (%.1f, %.1f)\n", vertex[i].X(), vertex[i].Y())
			}
			fmt.Println("- Figure's perimeter: ")
			printPerimeter(vertex)
			break
		}
	}
}
func printPerimeter(vert Path){
	side:=make(Path, 2)
	fmt.Printf("  -")
	for i:=range vert{
		side[0]=vert[i]
		side[1]=vert[(i+1)%len(vert)]
		fmt.Printf(" %.2f ", side.Distance()) 
		if(i<len(vert)-1){
			fmt.Printf("+") 
		}
	}
	fmt.Printf("= %.2f \n", vert.Distance()) 

}

func rando(min, max float64) float64 {
	return min + rand.Float64() * (max + 1 - min)
=======
>>>>>>> e54a17263289e7fa9d505a09cde1526ec84079a5
}

//!-path
