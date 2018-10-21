package main

import (
	"context"
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"os/signal"
	"strconv"
	"time"

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
	Log   log.Interface
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

		comments := []*Comment{}
		if err = json.Unmarshal(b, &comments); err != nil {
			log.Error(err.Error())
		}
		store := &Store{Comments: comments}
		store.SetDataFile(dataFile)

		h.Store = store
	}
}

func main() {
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt)

	flag.Parse()

	log.SetHandler(text.New(os.Stdout))

	h := Handler{Log: log.Log}
	h.BuildDataStore(dataFile)

	r := mux.NewRouter()
	r.HandleFunc("/", h.rootHandler)
	r.HandleFunc("/comments", h.getComments).Methods("GET")
	r.HandleFunc("/comments", h.createComment).Methods("POST")
	r.HandleFunc("/comments/{id}", h.getComment).Methods("GET")
	r.HandleFunc("/comments/{id}", h.updateComment).Methods("PUT")
	r.HandleFunc("/comments/{id}", h.deleteComment).Methods("DELETE")

	s := &http.Server{Addr: ":" + port, Handler: r}
	go func() {
		log.WithFields(log.Fields{
			"port":          port,
			"comment_count": len(h.Store.Comments),
		}).Info(fmt.Sprintf("starting server at %s", port))
		if err := s.ListenAndServe(); err != nil {
			log.Error(err.Error())
		}
	}()

	<-stop

	log.Info("Shutting down the server...")
	h.Close()

	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()
	s.Shutdown(ctx)
	log.Info("Server gracefully stopped")
}

func (h Handler) Close() {
	h.Log.WithFields(log.Fields{
		"filename":      h.Store.Filename(),
		"comment_count": len(h.Store.Comments),
	}).Infof("closing datastore")
	err := h.Store.Close()
	if err != nil {

	} else {
		h.Log.Info("datastore closed successfully")
	}
}

func (h Handler) getComments(w http.ResponseWriter, r *http.Request) {
	// ?size=10&from=1
	q := r.URL.Query()
	var page int
	var err error
	if size, ok := q["size"]; ok {
		page, err = strconv.Atoi(size[0])
		if err != nil {
			h.Log.Error(err.Error())
		}
	}
	if page == 0 {
		page = 10
	}
	var offset int
	if from, ok := q["from"]; ok {
		offset, err = strconv.Atoi(from[0])
		if err != nil {
			h.Log.Error(err.Error())
		}
	}
	comments := h.Store.Comments[offset:page]
	w.Header().Set("Content-Type", "application/json")
	err = json.NewEncoder(w).Encode(comments)
	if err != nil {
		h.Log.Error(err.Error())
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func (h *Handler) createComment(w http.ResponseWriter, r *http.Request) {
	log.Info("create comment")
	comment := &Comment{}
	err := json.NewDecoder(r.Body).Decode(comment)
	if err != nil {
		h.Log.Error(err.Error())
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
	defer r.Body.Close()

	comment = h.Store.Create(comment)

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusCreated)
	err = json.NewEncoder(w).Encode(comment)
	if err != nil {
		h.Log.Error(err.Error())
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
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
