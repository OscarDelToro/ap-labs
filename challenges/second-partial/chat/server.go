// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	users = make(map[string]string) //[remote address] username
	//alias       = make(map[string]string)//[username] remote address
	alias         = make(map[string]net.Conn) //[username] net con
	connectionLog = make(map[string]string)   //[username] connection time
	entering      = make(chan client)
	leaving       = make(chan client)
	messages      = make(chan string) // all incoming client messages
	admin         = ""
	usrCount      = 0
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	remitent := ""
	message := ""
	normal := false
	for {
		normal = false
		select {
		case msg := <-messages:
			//To do: MAKE THIS CODE A FUNCTION
			values := []rune(msg)
			a := strings.Index(msg, ":")
			if a >= 0 {
				message = string(values[a+2:])
				remitent = string(values[:a])
				normal = true
			}

			messageInfo := strings.SplitN(message, " ", 2)
			if len(messageInfo[0]) > 0 {
				if messageInfo[0][0] == '/' {
					switch messageInfo[0] {
					case "/users":
						showUsers(remitent)
					case "/msg":
						if len(messageInfo) < 2 {
							//error
							break
						}
						messageData := strings.SplitN(messageInfo[1], " ", 2)
						if len(messageInfo) < 2 {
							//error
							break
						}
						sendPrivateMessage(remitent, messageData[0], messageData[1])
					case "/time":

						showTime(remitent)
					case "/user":
						if len(messageInfo) < 2 {
							//error
							break
						}
						showInfo(remitent, messageInfo[1])
					case "/kick":
						if len(messageInfo) < 2 {
							//error
							break
						}
						if alias[remitent].RemoteAddr().String() == admin {
							kick(messageInfo[1])
							for cli := range clients {

								cli <- "[" + messageInfo[1] + "] was kicked "

							}
						}
					}

				} else {
					for cli := range clients {

						if normal {
							cli <- remitent + " > " + message
						} else {
							cli <- msg
						}
					}

				}

			} else {
				// Broadcast incoming message to all
				// clients' outgoing message channels.
				for cli := range clients {

					if normal {
						cli <- remitent + " > " + message
					} else {
						cli <- msg
					}
				}

			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}
func showUsers(to string) { //shows users to the one requesting at to
	for usr := range alias {
		sendPrivateMessage("irc-server", to, usr+" - connected since "+connectionLog[usr])
	}

}
func showTime(to string) { //shows time to the one requesting at to
	time := time.Now()
	sendPrivateMessage("irc-server", to, "Local Time: "+time.Location().String()+time.Format(" 15:04"))

}

func showInfo(to, usr string) { //shows info to the one requesting at to
	sendPrivateMessage("irc-server", to, "username: "+usr+" IP: "+strings.Split(alias[usr].LocalAddr().String(), ":")[0]+" Connected since: "+connectionLog[usr])

}
func kick(usr string) { //shows info to the one requesting at to

	a := alias[usr]
	if a != nil {
		alias[usr].Close()
		fmt.Printf("[%s] was kicked\n", usr)
		delete(users, alias[usr].RemoteAddr().String())
		delete(alias, usr)

	}

}
func sendPrivateMessage(from, to, message string) { //send private messages
	//fmt.Print("hi")
	ch := make(chan string)
	go clientWriter(alias[to], ch)
	ch <- from + " > " + message
	return

}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)
	time := time.Now()

	who := conn.RemoteAddr().String()
	//userName := "user" + strconv.Itoa(usrCount+1)
	input := bufio.NewScanner(conn)
	input.Scan()
	userName := input.Text()
	usrCount++
	users[who] = userName
	alias[userName] = conn
	//connectionLog[userName] = strconv.Itoa(time.Year()) + "-" + time.Month().String + "-" + strconv.Itoa(time.Day()) + " " + time.Format("15:04:01")
	connectionLog[userName] = time.Format("2006-01-02 15:04:05")
	fmt.Printf("irc-server > New connected user [%s]\n", users[who])
	//ch <- "You are " + who
	ch <- "irc-server > Welcome to the Simple IRC Server"
	ch <- "irc-server > Your user [" + users[who] + "] is successfully logged"
	if admin == "" {
		admin = who
		ch <- "irc-server > Congrats, you were the first user"
		ch <- "irc-server > You're the new IRC Server ADMIN"
		fmt.Printf("irc-server > [%s] was promoted as the channel ADMIN\n", users[who])
	}
	//messages <- who + " has arrived"
	messages <- users[who] + " has arrived"
	entering <- ch

	//input := bufio.NewScanner(conn)
	for input.Scan() {
		messages <- users[who] + ": " + input.Text()

	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	//messages <- who + " has left"
	messages <- "irc-server > " + users[who] + " left the channel"
	fmt.Printf("irc-server > [%s] left\n", users[who])

	a := alias[users[who]]
	if a != nil {
		delete(alias, users[who])
		delete(users, who)

	}

	if who == admin {
		for nam := range alias {
			sendPrivateMessage("irc-server", nam, "You're the new IRC Server ADMIN")
			fmt.Printf("irc-server > [%s] was promoted as the channel ADMIN\n", users[alias[nam].RemoteAddr().String()])
			admin = alias[nam].RemoteAddr().String()

		}
		//resetar admin
	}

	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	listener, err := net.Listen("tcp", os.Args[2]+":"+os.Args[4])
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("irc-server > Simple IRC Server started at %s\n", listener.Addr())

	go broadcaster()
	fmt.Print("irc-server > Ready for receiving new clients\n")
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}

}

//!-main
