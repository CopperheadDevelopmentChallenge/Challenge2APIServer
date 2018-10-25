package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"os"
	"strconv"

	"github.com/gorilla/mux"
)

const usage = `Usage twitserv [-p]
Options:
  -p           the port in which to listen (default 8080)
`

var (
	helpFlag     *bool
	helpLongFlag *bool
	portFlag     *string
)

func init() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, usage)
	}

	helpFlag = flag.Bool("h", false, usage)
	helpLongFlag = flag.Bool("help", false, usage)
	portFlag = flag.String("p", "8080", "Must provide a port if port flag is used.")

	flag.Parse()

	if *helpFlag == true || *helpLongFlag == true {
		log.Fatalln(usage)
	}
}

var db *database

func main() {
	router := mux.NewRouter()
	router.HandleFunc("/comments", allCommentsHandler).Methods(http.MethodGet, http.MethodPost)
	router.HandleFunc("/comments/{id}", commentHandler).Methods(http.MethodGet, http.MethodPut, http.MethodDelete)

	log.Printf("Database initializing...")

	var err error
	db, err = newDatabase()
	if err != nil {
		log.Fatalln(err)
	}

	log.Println("Database initialized!")
	log.Println("Newest comment ID:", db.lastID)
	log.Println("Server listening on port", *portFlag)

	log.Fatal(http.ListenAndServe(":"+*portFlag, router))
}

func commentHandler(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)

	if id, ok := vars["id"]; ok {
		commentID, err := strconv.Atoi(id)
		if err != nil {
			w.WriteHeader(http.StatusBadRequest)
			return
		}

		// TODO: clean this up and reuse some shit.
		switch r.Method {
		case http.MethodGet:
			comment, err := db.commentByID(commentID)
			if err != nil {
				w.WriteHeader(http.StatusBadRequest)
				return
			}
			w.WriteHeader(http.StatusOK)
			w.Write(comment)
		case http.MethodDelete:
			comment, err := db.deleteComment(commentID)
			if err != nil {
				w.WriteHeader(http.StatusBadRequest)
				return
			}
			w.WriteHeader(http.StatusAccepted)
			w.Write(comment)
		case http.MethodPut:
			toUpdate, err := ioutil.ReadAll(r.Body)
			if err != nil {
				w.WriteHeader(http.StatusInternalServerError)
				return
			}
			comment, err := db.updateComment(commentID, toUpdate)
			if err != nil {
				w.WriteHeader(http.StatusBadRequest)
				return
			}
			w.WriteHeader(http.StatusAccepted)
			w.Write(comment)
		}
	}
}

func allCommentsHandler(w http.ResponseWriter, r *http.Request) {
	from, size, err := rangeParams(r.URL.Query())
	if err != nil {
		w.WriteHeader(http.StatusBadRequest)
		return
	}

	switch r.Method {
	case http.MethodGet:
		data, err := db.commentsByRange(from, size)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}
		w.WriteHeader(http.StatusOK)
		w.Write(data)
		return
	case http.MethodPost:
		toInsert, err := ioutil.ReadAll(r.Body)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}

		comment, err := db.insert(toInsert)
		if err != nil {
			w.WriteHeader(http.StatusBadRequest)
			return
		}
		w.WriteHeader(http.StatusCreated)
		w.Write(comment)
	}
}

// validates query string params for range and converts to ints
func rangeParams(keys url.Values) (f, s int, err error) {
	// no point in having one or the other range params...need both.
	if len(keys) < 2 {
		return -1, -1, nil
	}

	var from, size string

	if s, ok := keys["size"]; ok {
		if f, ok := keys["start"]; ok {
			size = s[0]
			from = f[0]
		}
	}

	// TODO: this is slow as shit
	startPos, err := strconv.Atoi(from)
	if err != nil || startPos < 1 {
		return -1, -1, err
	}
	toPos, err := strconv.Atoi(size)
	if err != nil {
		return -1, -1, err
	}
	return startPos, toPos, nil
}
