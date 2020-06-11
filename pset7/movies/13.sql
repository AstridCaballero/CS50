--list the names of all people who starred in a movie in which Kevin Bacon also starred
SELECT name FROM people
WHERE
id IN
(SELECT DISTINCT(people.id) FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON movies.id = stars.movie_id
WHERE
movies.id IN
(SELECT movies.id FROM movies
JOIN stars ON stars.movie_id = movies.id
JOIN people ON people.id = stars.person_id
WHERE
people.name = 'Kevin Bacon' AND
people.birth = 1958)
AND people.id NOT IN
(SELECT people.id FROM people
WHERE
people.name = 'Kevin Bacon' AND
people.birth = 1958))