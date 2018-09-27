package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"

	"github.com/gorilla/mux"

	"github.com/apex/log"
	"github.com/apex/log/handlers/text"
)

var port string
var dataFile string

func init() {
	flag.StringVar(&port, "port", "8080", "port to run the server")
	flag.StringVar(&dataFile, "data", "", "path to data file")
}

type Handler struct {
	Store *Store
}

func main() {
	flag.Parse()

	log.SetHandler(text.New(os.Stdout))
	log.SetLevelFromString("debug")

	file, err := os.Open(dataFile)
	if err != nil {
		log.Error(err.Error())
	}

	var store *Store
	b, err := ioutil.ReadAll(file)
	if err != nil {
		log.Error(err.Error())
	}
	file.Close()

	json.Unmarshal(b, store)

	store.SetDataFile(dataFile)

	h := Handler{
		Store: store,
	}

	r := mux.NewRouter()
	r.HandleFunc("/", h.rootHandler)

	log.WithFields(log.Fields{
		"port":       port,
		"user count": len(h.Store.Users),
	}).Info(fmt.Sprintf("starting server at %s", port))
	if err := http.ListenAndServe(":"+port, r); err != nil {
		log.Error(err.Error())
	}
}

func (h Handler) rootHandler(w http.ResponseWriter, r *http.Request) {
	log.Info("root handler")

	for _, user := range h.Store.Users {
		log.WithField("user", user).Debug(user.Name)
	}
}
