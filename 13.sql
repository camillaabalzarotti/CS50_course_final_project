SELECT name FROM people
JOIN stars ON people.id = stars.person_id
WHERE stars.movie_id =
 (SELECT id FROM movies
INNER JOIN stars ON movies.id = stars.movie_id
WHERE stars.person_id =
(SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = "1958"));
AND stars.person_id != (SELECT id FROM people WHERE name = "Kevin Bacon");

FILM DOVE HA RECITATO KEVIN BACON
   SELECT title FROM movies
   ...> JOIN stars ON movies.id = stars.movie_id
   ...> JOIN people ON people.id = stars.person_id
   ...> WHERE people.name = "Kevin Bacon" AND people.birth = "1958";