--list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred
SELECT movies.title FROM movies
WHERE
movies.id IN
(SELECT movies.id FROM movies
JOIN stars ON stars.movie_id = movies.id
JOIN people ON people.id = stars.person_id
WHERE
name = 'Johnny Depp') AND movies.id IN
(SELECT movies.id FROM movies
JOIN stars ON stars.movie_id = movies.id
JOIN people ON people.id = stars.person_id
WHERE
name = 'Helena Bonham Carter')