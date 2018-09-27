package main

type Store struct {
	file  string
	Users []*User `json:"data"`
}

func (s *Store) SetDataFile(file string) {
	s.file = file
}

func (s Store) FindByID(id int) *User {
	for _, user := range s.Users {
		if id == user.ID {
			return user
		}
	}
	return nil
}
