package main

import (
	"encoding/json"
	"errors"
	"io/ioutil"
	"sync"
)

type comment struct {
	ID    int    `json:"id"`
	Name  string `json:"name"`
	Email string `json:"email"`
	Body  string `json:"body"`
}

type database struct {
	mutex  sync.RWMutex
	Data   []comment `json:"data"`
	lastID uint64
	index  map[int]int
}

const (
	inFile = "data.json"
)

var (
	errCommentNotFound   = errors.New("comment not found")
	errSerializationFail = errors.New("failed to serialize comment")
	errInvalidInput      = errors.New("invalid input for comment")
)

// read the data file and create a database in memory.
func newDatabase() (*database, error) {
	dat, err := ioutil.ReadFile(inFile)
	if err != nil {
		return nil, err
	}

	db := &database{
		index: make(map[int]int),
		Data:  make([]comment, 0, 500),
	}

	if err := json.Unmarshal(dat, db); err != nil {
		return nil, err
	}
	// determine the last ID and index by ID
	var tmpID int
	for i := range db.Data {
		if db.Data[i].ID > tmpID {
			tmpID = db.Data[i].ID
		}

		db.index[db.Data[i].ID] = i
	}
	db.lastID = uint64(tmpID)

	return db, nil
}

func (d *database) deleteComment(commentID int) ([]byte, error) {
	// get comment before deleting
	comment, err := d.commentByID(commentID)
	if err != nil {
		return nil, err
	}
	d.remove(commentID)

	return comment, nil
}

// swaps the last element with index i, then returns a new slice
func (d *database) remove(commentID int) {
	d.mutex.Lock()
	defer d.mutex.Unlock()

	idx := d.index[commentID]
	d.Data[len(d.Data)-1], d.Data[idx] = d.Data[idx], d.Data[len(d.Data)-1]
	d.Data = d.Data[:len(d.Data)-1]

	delete(d.index, commentID)
}

func (d *database) insert(c comment) ([]byte, error) {
	d.mutex.Lock()
	defer d.mutex.Unlock()

	d.lastID++
	c.ID = int(d.lastID)

	d.Data = append(d.Data, c)
	d.index[c.ID] = len(d.Data) - 1
	return json.Marshal(&c)
}

func (d *database) commentByID(commentID int) ([]byte, error) {
	d.mutex.RLock()
	defer d.mutex.RUnlock()

	if idx, ok := d.index[commentID]; ok {
		comment, err := json.Marshal(&(d.Data[idx]))
		if err != nil {
			return nil, errSerializationFail
		}
		return comment, nil
	}
	return nil, errCommentNotFound
}

func (d *database) updateComment(commentID int, c comment) ([]byte, error) {

	// reset the ID if not in the payload (but in the URL variable)
	if c.ID == 0 {
		c.ID = commentID
	}

	d.mutex.Lock()
	defer d.mutex.Unlock()

	if idx, ok := d.index[commentID]; ok {
		d.Data[idx] = c
		return json.Marshal(&c)
	}
	return nil, errCommentNotFound
}

// if from OR size is -1, then all comments are returned.
// from must be > 0 and < len(d.Data)
// size must be > 0 and size+from must be <= len(d.Data)
func (d *database) commentsByRange(from, size int) ([]byte, error) {
	if from == -1 || size == -1 {
		d.mutex.RLock()
		defer d.mutex.RUnlock()

		return json.Marshal(d.Data)
	}
	if (from) > len(d.Data) {
		return nil, errInvalidInput
	}

	if (from-1)+size > len(d.Data) {
		d.mutex.RLock()
		defer d.mutex.RUnlock()
		return json.Marshal(d.Data[from-1:])
	}

	d.mutex.RLock()
	defer d.mutex.RUnlock()
	return json.Marshal(d.Data[from-1 : (from-1)+size])
}
