SELECT title FROM johnny
JOIN stars ON johnny.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE people.name = "Helena Bonham Carter";

