SELECT DISTINCT name FROM people WHERE id =
   ...> (SELECT person_id FROM directors WHERE movie_id =
   ...> (SELECT id FROM movies WHERE id =
   ...> (SELECT movie_id FROM ratings WHERE rating >= 9.0)));