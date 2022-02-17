SELECT title FROM movies
INNER JOIN ratings ON movies.id = ratings.movie_id
INNER JOIN stars ON stars.movie_id = ratings.movie_id
WHERE person_id = (SELECT id FROM people WHERE name = "Chadwick Boseman")
ORDER BY ratings.rating DESC
LIMIT 5;