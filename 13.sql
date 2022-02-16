   SELECT name FROM people
   ...> INNER JOIN stars ON people.id = stars.person_id
   ...> WHERE stars.movie_id =
   ...> (SELECT id FROM movies
   ...> INNER JOIN stars ON movies.id = stars.movie_id
   ...> WHERE stars.person_id =
   ...> (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = "1958"));
   ...> AND stars.person_id != (SELECT id FROM people WHERE name = "Kevin Bacon");