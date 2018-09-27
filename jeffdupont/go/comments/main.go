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
	flag.StringVar(&dataFile, "data", "../../../data.json", "path to data file")
}

type Handler struct {
	Store *Store
}

func (h *Handler) BuildDataStore(dataFile string) {
	if dataFile != "" {
		file, err := os.Open(dataFile)
		if err != nil {
			log.Error(err.Error())
		}

		b, err := ioutil.ReadAll(file)
		if err != nil {
			log.Error(err.Error())
		}
		defer file.Close()

		store := &Store{}
		if err = json.Unmarshal(b, store); err != nil {
			log.Error(err.Error())
		}
		store.SetDataFile(dataFile)

		h.Store = store
	}
}

func main() {
	flag.Parse()

	log.SetHandler(text.New(os.Stdout))

	h := Handler{}
	h.BuildDataStore(dataFile)

	r := mux.NewRouter()
	r.HandleFunc("/", h.rootHandler)
	r.HandleFunc("/comments", h.getComments).Methods("GET")
	r.HandleFunc("/comments", h.createComment).Methods("POST")
	r.HandleFunc("/comments/{id}", h.getComment).Methods("GET")
	r.HandleFunc("/comments/{id}", h.updateComment).Methods("PUT")
	r.HandleFunc("/comments/{id}", h.deleteComment).Methods("DELETE")

	log.WithFields(log.Fields{
		"port":          port,
		"comment_count": len(h.Store.Comments),
	}).Info(fmt.Sprintf("starting server at %s", port))
	if err := http.ListenAndServe(":"+port, r); err != nil {
		log.Error(err.Error())
	}
}

func (h Handler) getComments(w http.ResponseWriter, r *http.Request) {
	// ?size=10&from=1
}

func (h Handler) createComment(w http.ResponseWriter, r *http.Request) {
	log.Info("create comment")
	w.WriteHeader(http.StatusCreated)
}

func (h Handler) getComment(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	id := vars["id"]
	log.WithField("comment_id", id).Info("get comment")
}

func (h Handler) updateComment(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	id := vars["id"]
	log.WithField("comment_id", id).Info("update comment")
	w.WriteHeader(http.StatusAccepted)
}

func (h Handler) deleteComment(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	id := vars["id"]
	log.WithField("comment_id", id).Info("delete comment")
	w.WriteHeader(http.StatusAccepted)
}

func (h Handler) rootHandler(w http.ResponseWriter, r *http.Request) {
	log.Info("root handler")
	w.Header().Add("Content-type", "application/json")
	w.Write([]byte("{\"version\":\"0.1\",\"name\":\"API Challenge\",\"comment_count\":" + fmt.Sprintf("%d", len(h.Store.Comments)) + "}"))
}
