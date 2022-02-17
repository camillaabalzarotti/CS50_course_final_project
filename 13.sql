SELECT DISTINCT people.name FROM people
JOIN stars ON stars.person_id = people.id
JOIN kevin ON kevin.id = stars.movie_id
WHERE stars.person_id !=
(SELECT id FROM people WHERE name = "Kevin Bacon");