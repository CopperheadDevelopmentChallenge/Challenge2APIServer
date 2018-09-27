package main

type Store struct {
	file     string
	Comments []*Comment `json:"data"`
}

func (s *Store) SetDataFile(file string) {
	s.file = file
}

func (s Store) FindByID(id int) *Comment {
	for _, comment := range s.Comments {
		if id == comment.ID {
			return comment
		}
	}
	return nil
}
